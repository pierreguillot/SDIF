/*
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * SdifMemory.c
 *
 *
 * author: Dominique Virolle 1999
 *
 *
 * $Log: not supported by cvs2svn $
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
    free(BlockNode->ptr); // free of user ptr
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
SdifMr_alloc(SdifBlockListT* L, size_t size, size_t nobj)
{
    void* ptr;

    ptr = malloc(size*nobj);
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


    newptr = realloc(oldptr, size * nobj); // realloc user block

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
