/*
 *
 * IRCAM SDIF Library (http://www.ircam.fr/sdif)
 *
 * Copyright (C) 1998, 1999, 2000 by IRCAM-Centre Georges Pompidou, Paris, France.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * See file COPYING for further informations on licensing terms.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            sdif@ircam.fr
 *
 *
 * SdifMemory.c
 *
 *
 * author: Dominique Virolle 1999
 *
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.7  2001/05/02 09:34:46  tisseran
 * Change License from GNU Public License to GNU Lesser Public License.
 *
 * Revision 3.6  2000/11/15 14:53:32  lefevre
 * no message
 *
 * Revision 3.5  2000/10/27  20:03:38  roebel
 * autoconf merged back to main trunk
 *
 * Revision 3.4.2.1  2000/08/21  21:35:37  tisseran
 * *** empty log message ***
 *
 * Revision 3.4  2000/07/06  19:01:49  tisseran
 * Add function for frame and matrix type declaration
 * Remove string size limitation for NameValueTable
 * TODO: 1TYP and 1IDS frame must contain an 1NVT (text) matrix
 *       Actually, data are written with fprintf.
 *
 * Revision 3.3  2000/05/12  14:41:48  schwarz
 * On behalf of Adrien, synchronisation with Mac sources, with some slight
 * changes because of cross-platform issues:
 * - Mac only stuff: XpSetFileAttribute XpFileSize
 * - Cross platform wrapper: XpGetenv XpExit
 * - Dangerous: strings.h (and thus bzero, bcopy) is not ANSI and thus doesn't
 *   exist on Mac.  Use string.h and memset, memcpy.
 *
 * Revision 3.2  1999/09/28  13:09:06  schwarz
 * Included #include <preincluded.h> for cross-platform uniformisation,
 * which in turn includes host_architecture.h and SDIF's project_preinclude.h.
 *
 * Revision 3.1  1999/03/14  10:57:10  virolle
 * SdifStdErr add
 *
 * Revision 2.1  1999/02/28  12:16:51  virolle
 * memory report
 *
 *
 */



#include <preincluded.h>

#include "SdifMemory.h"

#ifdef _SdifMemoryReport
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

SdifBlockListT SdifMrReport = {NULL, 0, 0, 0};


SdifBlockNodeT*
SdifCreateBlockNode(SdifBlockNodeT* Next, char* file, int line, char* type, void* ptr, size_t size, size_t nobj)
{
    SdifBlockNodeT* NewBlockNode;
    char* filename;

    NewBlockNode = (SdifBlockNodeT*) malloc (sizeof(SdifBlockNodeT));
    filename = strrchr(file, '\\');
    if (filename)
	filename++;
    else
	filename=file;

    strcpy(NewBlockNode->file, filename);
    strcpy(NewBlockNode->type, type);
    NewBlockNode->line	= line;
    NewBlockNode->ptr	= ptr;
    NewBlockNode->size	= size;
    NewBlockNode->nbobj	= nobj;
    NewBlockNode->Next	= Next;

    return NewBlockNode;
}


SdifBlockNodeT*
SdifKillBlockNode(SdifBlockNodeT* BlockNode)
{
    SdifBlockNodeT* Next;

    Next = BlockNode->Next;
    free(BlockNode->ptr); /* free of user ptr */
    free(BlockNode);
    return Next;
}


void
SdifPrintBlockNode(int sizealloc, char* mess, SdifBlockNodeT* BlockNode)
{
    fprintf(SdifStdErr, "%s, %5d, 0x%8.8x, %s,\t%5d, %5d, %5d,\t%s\n",
		BlockNode->file,
		BlockNode->line,
		BlockNode->ptr,
		BlockNode->type,
		BlockNode->size,
		BlockNode->nbobj,
		sizealloc,
		mess);
}

void*
SdifMr_alloc(SdifBlockListT* L, size_t size, size_t nobj, int clear)
{
    void* ptr;

    if (clear)
	ptr = calloc(nobj, size);
    else
	ptr = malloc(size * nobj);

    L->Alloc = SdifCreateBlockNode(L->Alloc, SdifErrorFile, SdifErrorLine, SdifMrType, ptr, size, nobj);
    L->BytesAlloc	+= size*nobj;
    L->BytesTotalAlloc	+= size*nobj;

#if defined(_SdifMemoryBigReport) || defined(_SdifMemoryReportAlloc)
    SdifPrintBlockNode(L->BytesAlloc, "SDIF ALLOC", L->Alloc);
#endif

    return ptr;
}



size_t
SdifMr_free(SdifBlockListT* L, void* ptr)
{
    SdifBlockNodeT* Curr;
    SdifBlockNodeT* Prev = NULL;

    size_t DeAllocSize = 0;


    for (Curr = L->Alloc; Curr; Curr=Curr->Next)
    {
	if (Curr->ptr == ptr)
	{
	    if (! Prev)
		L->Alloc = Curr->Next;
	    else
		Prev->Next = Curr->Next;

	    DeAllocSize = Curr->size * Curr->nbobj;
	    L->BytesAlloc   -= DeAllocSize;
	    L->BytesDeAlloc += DeAllocSize;

#if defined(_SdifMemoryBigReport) || defined(_SdifMemoryReportFree)
	    SdifPrintBlockNode(L->BytesAlloc, "SIDF FREE", Curr);
#endif
	    SdifKillBlockNode(Curr);
	    return DeAllocSize;

	}
	Prev = Curr;
    }
    fprintf(SdifStdErr, "SDIF ptr not found 0x%8.8x\n", ptr);
    free(ptr);
    return DeAllocSize;
}



void*
SdifMr_realloc(SdifBlockListT* L, void* oldptr, size_t size, size_t nobj)
{
    SdifBlockNodeT* Curr;
    void* newptr;

    size_t DeAllocSize = 0;
    size_t ReAllocSize = 0;


    newptr = realloc(oldptr, size * nobj); /* realloc user block */

    for (Curr = L->Alloc; Curr; Curr=Curr->Next)
    {
	if (Curr->ptr == oldptr)
	{
	    DeAllocSize	     = Curr->size * Curr->nbobj;
	    ReAllocSize	     = size * nobj;

	    L->BytesAlloc   -= DeAllocSize;
	    Curr->ptr	     = newptr;
	    Curr->size	     = size;
	    Curr->nbobj	     = nobj;
	    L->BytesAlloc   += ReAllocSize;
	    L->BytesTotalAlloc   += ReAllocSize;


#if defined(_SdifMemoryBigReport) || defined(_SdifMemoryReportAlloc) || defined(_SdifMemoryReportReAlloc)
	    SdifPrintBlockNode(L->BytesAlloc, "SDIF REALLOC", Curr);
#endif
	    return newptr;
	}
    }
    return newptr;
}


void
SdifMrDrainBlockList(SdifBlockListT* L)
{
    fprintf(SdifStdErr, "\n***** SDIF LIBRARY, Memory Report *****\n\
Total Allocation : %10ld bytes (realloc counted as free and alloc)\n\
      Free       : %10ld bytes\n\
      Allocated  : %10ld bytes (must be 0)\n\n",
		L->BytesTotalAlloc,
		L->BytesDeAlloc,
		L->BytesAlloc);

    if (L->Alloc)
    {
        fprintf(SdifStdErr, "Pointer still allocated :\n");
	while (L->Alloc)
	{
	    SdifPrintBlockNode(L->BytesAlloc, "SDIF DRAIN", L->Alloc);
	    L->Alloc = SdifKillBlockNode(L->Alloc);
	}
	fprintf(SdifStdErr, "***** end of list *****\n");
    }
}

#endif /* ifdef _SdifMemoryReport */
