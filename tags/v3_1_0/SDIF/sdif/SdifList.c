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
 * SdifList.c
 *
 * SdifList is coded like a small independant library.
 * Every List have its own size and data type.
 * Every data type must have a function to free memory : 
 *     void <KillerName>(<type>* ptr).
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.2  1999/09/28  13:09:02  schwarz
 * Included #include <preincluded.h> for cross-platform uniformisation,
 * which in turn includes host_architecture.h and SDIF's project_preinclude.h.
 *
 * Revision 3.1  1999/03/14  10:57:03  virolle
 * SdifStdErr add
 *
 * Revision 2.3  1999/02/28  12:16:49  virolle
 * memory report
 *
 * Revision 2.2  1999/01/23  15:55:52  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.1  1999/01/23  13:57:37  virolle
 * General Lists, and special chunk preparation to become frames
 *
 *
 */

#include <preincluded.h>
#include "SdifList.h"

#include <stdio.h>
#include <stdlib.h>

#include "SdifMemory.h"
#include "SdifError.h"


/* stocks management */


void
SdifInitListNStock(SdifListNStockT *Stock, unsigned int SizeOfOneStock)
{
    Stock->StockList                = NULL;    
    Stock->SizeOfOneStock           = SizeOfOneStock;    
    Stock->NbStock                  = 0;
    Stock->NbNodesUsedInCurrStock   = 0;    
    Stock->Trash                    = NULL;    
}



void
SdifNewStock(SdifListNStockT *Stock)
{
    SdifListNT* NewStock;

    NewStock = SdifCalloc(SdifListNT, Stock->SizeOfOneStock);
    if (NewStock)
    {
        (NewStock[0]).Next              = Stock->StockList;
        Stock->StockList                = NewStock;
        Stock->NbStock++;
        Stock->NbNodesUsedInCurrStock   = 1;
        /* first is used yet (cf. (NewStock[0]).Next in this function) */
    }
}


SdifListNT*
SdifGetNewNodeFromTrash(SdifListNStockT *Stock)
{
    SdifListNT* RecycledNode;

    RecycledNode = Stock->Trash;
    Stock->Trash = Stock->Trash->Next;
    return RecycledNode;
}



SdifListNT*
SdifGetNewNodeFromStock(SdifListNStockT *Stock)
{
    if (    (Stock->NbStock ==0)
         || (Stock->NbNodesUsedInCurrStock == Stock->SizeOfOneStock) )
    {
        SdifNewStock(Stock);
    }

    return &(Stock->StockList[(Stock->NbNodesUsedInCurrStock)++]);
}


SdifListNT*
SdifGetNewNode(SdifListNStockT *Stock)
{
    if (Stock->Trash)
        return SdifGetNewNodeFromTrash(Stock);
    else
        return SdifGetNewNodeFromStock(Stock);
}


void
SdifPutNodeInTrash(SdifListNStockT *Stock, SdifListNT* OldNode)
{
    OldNode->Next = Stock->Trash;
    Stock->Trash = OldNode;
}



SdifListNT*
SdifKillListNStock(SdifListNT* OldStock)
{
    SdifListNT* NextStock;

    NextStock = (OldStock[0]).Next;
    SdifFree(OldStock);
    return NextStock;
}



void
SdifListNStockMakeEmpty(SdifListNStockT *Stock)
{
    while (Stock->StockList)
    {
       Stock->StockList = SdifKillListNStock(Stock->StockList);
    }
    SdifInitListNStock(Stock, Stock->SizeOfOneStock);
}



/* global variable gSdifListNodeStock */



SdifListNStockT gSdifListNodeStock;

SdifListNStockT*
SdifListNodeStock(void)
{
    return & gSdifListNodeStock;
}

void
SdifInitListNodeStock(unsigned int SizeOfOneStock)
{
    SdifInitListNStock(SdifListNodeStock(), SizeOfOneStock);
}


void SdifDrainListNodeStock(void)
{
    SdifListNStockMakeEmpty(SdifListNodeStock());
}


/* nodes management */


SdifListNT*
SdifCreateListNode (SdifListNT *Next, void *Data)
{
    SdifListNT *NewNode;
    NewNode = SdifGetNewNode(SdifListNodeStock());
    NewNode->Next = Next;
    NewNode->Data = Data;
    return NewNode;
}




SdifListNT*
SdifKillListNode (SdifListNT *Node, KillerFT Killer)
{
    SdifListNT* Next;

    Next = Node->Next;
    if (Killer != NULL)
        (*(Killer))(Node->Data); /* this line can occasion a warning */

    SdifPutNodeInTrash (SdifListNodeStock(), Node);
    return Next;
}



/* user lists management */

SdifListT*
SdifCreateList(KillerFT Killer)
{
    SdifListT* NewList;

    NewList = SdifMalloc(SdifListT);
    if (NewList)
    {
        NewList->Head = NULL;
        NewList->Tail = NULL;
        NewList->Curr = NULL;
        NewList->Killer = Killer;
        NewList->NbData = 0;
        return NewList;
    }
    else
    {
      _SdifError(eAllocFail, "List allocation");
      return NULL;
    }
}


SdifListT*
SdifKillListHead(SdifListT* List)
{
    if (List->Head)
    {
        if (List->Head == List->Tail)
            List->Tail = NULL;

        List->Head = SdifKillListNode(List->Head, List->Killer);
        List->NbData--;
        return List;
    }
    else
    {
      _SdifError(eFreeNull, "KillListHead");
      return NULL;
    }
}


SdifListT*
SdifMakeEmptyList (SdifListT* List)
{
    while(List->Head)
        SdifKillListHead(List);

    return List;
}



void
SdifKillList (SdifListT* List)
{
    if (List)
    {
        SdifMakeEmptyList(List);
        SdifFree(List);
    }
    else
    {
      _SdifError(eFreeNull, "KillList");
    }
}





void*
SdifListGetHead (SdifListT* List)
{
    if (!List->Head)
    {
        return NULL;
    }
    else
    {
        List->Curr = List->Head;
        return List->Head->Data;
    }
}



void*
SdifListGetTail (SdifListT* List)
{
    if (List->Tail)
        return List->Tail->Data;
    else
        return NULL;
}




int
SdifListIsNext(SdifListT* List)
{
    return ( (List->Head != NULL) && (List->Curr != List->Tail) );
}



int
SdifListIsEmpty(SdifListT* List)
{
    return (List->Head == NULL);
}



int
SdifListInitLoop(SdifListT* List)
{
    List->Curr = NULL;
    return (List->Head != NULL);	/* true if List has elements */
}


void*
SdifListGetNext (SdifListT* List)
{
    if (List->Curr == NULL)
    {
        return SdifListGetHead(List);
    }
    else
    {
        if (List->Curr != List->Tail)
        {
            List->Curr= List->Curr->Next;
            return List->Curr->Data;
        }
        else
        {
          _SdifError(eArrayPosition, "ListGetNext error, current node is the tail");
          return NULL;
        }
    }
}



void*
SdifListGetCurr(SdifListT* List)
{
    if (List->Curr == NULL)
        return NULL;
    else
        return List->Curr->Data;
}



SdifListT*
SdifListPutTail (SdifListT* List, void *pData)
{   
    if (List->Head == NULL) /* first node */
    {
        List->Head = SdifCreateListNode(NULL, pData);
        List->Tail = List->Head;
    }
    else
    {
        List->Tail->Next = SdifCreateListNode(NULL, pData);
        List->Tail = List->Tail->Next;
    }
    List->NbData++;

    return List;
}




SdifListT*
SdifListPutHead (SdifListT* List, void *pData)
{
    if (List->Head == NULL) /* first node */
    {
        List->Head = SdifCreateListNode(NULL, pData);
        List->Tail = List->Head;
    }
    else
    {
        List->Head = SdifCreateListNode(List->Head->Next, pData);
    }
    List->NbData++;

    return List;
}


unsigned int
SdifListGetNbData(SdifListT* List)
{
    return List->NbData;
}
