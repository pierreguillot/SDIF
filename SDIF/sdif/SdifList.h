/*
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
 * SdifList.h
 * 
 *
 * SdifList is coded like a small independant library.
 * Every List have its own size and data type.
 * Every data type must have a function to free memory : 
 *     void <KillerName>(<type>* ptr).
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.4.2.1  2000/08/21  21:35:32  tisseran
 * *** empty log message ***
 *
 * Revision 3.4  2000/05/15  16:22:31  schwarz
 * Avoid warning about KillerFT function pointer type (ANSI prototype given).
 * Argument to kill func is now void *.
 *
 * Revision 3.3  2000/03/01  11:19:24  schwarz
 * SdifListInitLoop also returns whether list is full.
 * Cocoonable comments.
 *
 * Revision 3.2  1999/09/28  13:09:03  schwarz
 * Included #include <preincluded.h> for cross-platform uniformisation,
 * which in turn includes host_architecture.h and SDIF's project_preinclude.h.
 *
 * Revision 3.1  1999/03/14  10:57:05  virolle
 * SdifStdErr add
 *
 * Revision 2.2  1999/01/23  15:55:53  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.1  1999/01/23  13:57:38  virolle
 * General Lists, and special chunk preparation to become frames
 *
 *
 */

#ifndef _SdifList_
#define _SdifList_



typedef void (*KillerFT) (void *);


typedef struct SdifListNS SdifListNT;

struct SdifListNS 
{
  SdifListNT *Next;
  void* Data;
};






typedef struct SdifListNStockS SdifListNStockT;

struct SdifListNStockS
{
    SdifListNT*  StockList; /* list of arrays of nodes, the first node is used to chain arrays */
    unsigned int SizeOfOneStock; /* must be > 1 */
    unsigned int NbStock;

    unsigned int NbNodesUsedInCurrStock;

    SdifListNT* Trash; /* to recycle nodes */

};




/* stocks management */

void        SdifInitListNStock      (SdifListNStockT *Stock, unsigned int SizeOfOneStock);
void        SdifNewStock            (SdifListNStockT *Stock);
SdifListNT* SdifGetNewNodeFromTrash (SdifListNStockT *Stock);
SdifListNT* SdifGetNewNodeFromStock (SdifListNStockT *Stock);
SdifListNT* SdifGetNewNode          (SdifListNStockT *Stock);
void        SdifPutNodeInTrash      (SdifListNStockT *Stock, SdifListNT* OldNode);
SdifListNT* SdifKillListNStock      (SdifListNT* OldStock);
void        SdifListNStockMakeEmpty (SdifListNStockT *Stock);

/* global variable gSdifListNodeStock */



extern  SdifListNStockT gSdifListNodeStock;
SdifListNStockT* SdifListNodeStock  (void);
void    SdifInitListNodeStock       (unsigned int SizeOfOneStock);
void    SdifDrainListNodeStock      (void);


/* nodes management */

SdifListNT* SdifCreateListNode  (SdifListNT *Next, void *Data);
SdifListNT* SdifKillListNode    (SdifListNT *Node, KillerFT Killer);



/* lists management */

typedef struct SdifListS SdifListT;
typedef SdifListT	*SdifListP;

struct SdifListS
{
  /* fifo list */
  SdifListNT *Head;
  SdifListNT *Tail;
  SdifListNT *Curr;  /* pointer before the next */
  void (*Killer)();  /* no verification of arguments */
  unsigned int NbData;
} ;


SdifListT*  SdifCreateList      (KillerFT Killer);
SdifListT*  SdifKillListHead    (SdifListT* List);
SdifListT*  SdifMakeEmptyList   (SdifListT* List);
void        SdifKillList        (SdifListT* List);

/*DOC:
  Init the function SdifListGetNext. 
  [Return] head of List. */
void*       SdifListGetHead     (SdifListT* List); 

void*       SdifListGetTail     (SdifListT* List);
int         SdifListIsNext      (SdifListT* List);
int         SdifListIsEmpty     (SdifListT* List);

/*DOC:
  Init for function SdifListGetNext.
  [Returns] true if List has elements. */
int         SdifListInitLoop    (SdifListT* List);

/*DOC:
  Set Curr to Curr->Next and after return Curr->Data */
void*       SdifListGetNext     (SdifListT* List);

/*DOC:
  Only return Curr->Data. */
void*       SdifListGetCurr     (SdifListT* List);

SdifListT*  SdifListPutTail     (SdifListT* List, void *pData);
SdifListT*  SdifListPutHead     (SdifListT* List, void *pData);
unsigned int SdifListGetNbData  (SdifListT* List);


#endif /* _SdifList_ */
