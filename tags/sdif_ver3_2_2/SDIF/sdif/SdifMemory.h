/* $Id: SdifMemory.h,v 3.2 2000-10-27 20:03:39 roebel Exp $
 *
 * IRCAM SDIF Library (http://www.ircam.fr/sdif)
 *
 * Copyright (C) 1998, 1999, 2000 by IRCAM-Centre Georges Pompidou, Paris, France.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * See file COPYING for further informations on licensing terms.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            sdif@ircam.fr
 *
 *
 * SdifMemory.h
 *
 *
 * author: Dominique Virolle 1999
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.1.2.1  2000/08/21  21:35:38  tisseran
 * *** empty log message ***
 *
 * Revision 3.1  1999/03/14  10:57:11  virolle
 * SdifStdErr add
 *
 * Revision 2.1  1999/02/28  12:16:53  virolle
 * memory report
 *
 *
 */

#ifndef _SdifMemory_
#define _SdifMemory_

#include "SdifError.h"
#include <stdlib.h>

/*
 * make #define or token adding :
 * _SdifMemoryReport		to have a memory report at the end of execution (with the other token too).
 * _SdifMemoryReportAlloc	to have a report only about allocation and re-allocation
 * _SdifMemoryReportReAlloc	to have a report only about re-allocation
 * _SdifMemoryReportFree	to have a report only about memory released to the system
 * _SdifMemoryBigReport		to have a full report
 */

#if !defined(_SdifMemoryReport) && (defined(_SdifMemoryBigReport) || defined(_SdifMemoryReportAlloc) || defined(_SdifMemoryReportReAlloc) || defined(_SdifMemoryReportFree))
#define _SdifMemoryReport
#endif /* condition for small report */



#ifndef _SdifMemoryReport
/*
 * do not define _SdifMemoryReport to not have memory report
 */

#define SdifMalloc(_type) \
(SdifErrorFile = __FILE__, SdifErrorLine = __LINE__, (_type*) malloc(sizeof(_type)))

#define SdifCalloc(_type, _nbobj) \
(SdifErrorFile = __FILE__, SdifErrorLine = __LINE__, (_type*) malloc(sizeof(_type) * _nbobj))

#define SdifRealloc(_ptr, _type, _nbobj) \
(SdifErrorFile = __FILE__, SdifErrorLine = __LINE__, (_type*) realloc(_ptr, sizeof(_type) * _nbobj))

#define SdifFree(_ptr) \
(SdifErrorFile = __FILE__, SdifErrorLine = __LINE__, free(_ptr))


#else

#define _SdifMrNameSize 64
#define _SdifTypeToStr(_type) #_type


typedef struct SdifBlockNodeS SdifBlockNodeT;
struct SdifBlockNodeS
{
    SdifBlockNodeT*   Next;
    char	    file[_SdifMrNameSize];
    int		    line;
    char	    type[_SdifMrNameSize];
    void*	    ptr;
    size_t	    size;
    size_t	    nbobj;
};


typedef struct SdifBlockListS SdifBlockListT;
struct SdifBlockListS
{
    SdifBlockNodeT* Alloc;
    size_t	    BytesAlloc;
    size_t	    BytesTotalAlloc;
    size_t	    BytesDeAlloc;
};



char *SdifMrType;

SdifBlockNodeT*	SdifCreateBlockNode	(SdifBlockNodeT* Next, char *file, int line, char* type, void* ptr, size_t size, size_t nobj);
SdifBlockNodeT*	SdifKillBlockNode	(SdifBlockNodeT* BlockNode);
void		SdifPrintBlockNode	(int sizealloc, char* mess, SdifBlockNodeT* BlockNode);
void*		SdifMr_alloc		(SdifBlockListT* L, size_t size, size_t nobj);
size_t		SdifMr_free		(SdifBlockListT* L, void* ptr);
void*		SdifMr_realloc		(SdifBlockListT* L, void* oldptr, size_t size, size_t nobj);
void		SdifMrDrainBlockList	(SdifBlockListT* L);

extern SdifBlockListT SdifMrReport;

#define SdifMalloc(_type) \
(SdifErrorFile = __FILE__, SdifErrorLine = __LINE__, SdifMrType=_SdifTypeToStr(_type), \
(_type*) SdifMr_alloc(&SdifMrReport, sizeof(_type), 1))

#define SdifCalloc(_type, _nbobj) \
(SdifErrorFile = __FILE__, SdifErrorLine = __LINE__, SdifMrType=_SdifTypeToStr(_type), \
(_type*) SdifMr_alloc(&SdifMrReport, sizeof(_type), _nbobj))

#define SdifRealloc(_ptr, _type, _nbobj) \
(SdifErrorFile = __FILE__, SdifErrorLine = __LINE__, SdifMrType=_SdifTypeToStr(_type), \
(_type*) SdifMr_realloc(&SdifMrReport, _ptr, sizeof(_type), _nbobj))


#define SdifFree(_ptr) \
(SdifErrorFile = __FILE__, SdifErrorLine = __LINE__, \
SdifMr_free(&SdifMrReport, (void*) _ptr))


#endif /* _SdifMemoryReport */

#endif /* _SdifMemory_ */
