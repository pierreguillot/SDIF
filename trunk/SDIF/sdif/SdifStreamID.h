/* $Id: SdifStreamID.h,v 2.3 1999-01-23 15:56:01 virolle Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * SdifStreamID.h
 *
 * StreamID structures management
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 2.2  1999/01/23  13:57:47  virolle
 * General Lists, and special chunk preparation to become frames
 *
 * Revision 2.1  1998/12/21  18:27:40  schwarz
 * Inserted copyright message.
 *
 * Revision 2.0  1998/11/29  11:42:07  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.4  1998/11/10  15:31:55  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 */


#ifndef _SdifStreamID_
#define _SdifStreamID_

#include "SdifGlobals.h"
#include "SdifHash.h"

typedef struct SdifStreamIDS SdifStreamIDT;
struct SdifStreamIDS
{
  SdifUInt4     NumID;
  char *Source;
  char *TreeWay; /* for the moment or to be general*/
} ;

SdifStreamIDT* SdifCreateStreamID(SdifUInt4 NumID, char *Source, char *TreeWay);
void           SdifKillStreamID(SdifStreamIDT *StreamID);



typedef struct SdifStreamIDTableS SdifStreamIDTableT;
struct SdifStreamIDTableS
{
    SdifHashTableT* SIDHT;
    SdifUInt4       NumIDLink;
    SdifFloat8      Time;
} ;


SdifStreamIDTableT* SdifCreateStreamIDTable     (SdifUInt4 HashSize);
void                SdifKillStreamIDTable       (SdifStreamIDTableT* SIDTable);
SdifStreamIDT*      SdifStreamIDTablePutSID     (SdifStreamIDTableT* SIDTable, SdifUInt4 NumID, char *Source, char *TreeWay);
SdifStreamIDT*      SdifStreamIDTableGetSID     (SdifStreamIDTableT* SIDTable, SdifUInt4 NumID);
SdifUInt4           SdifStreamIDTableGetNbData  (SdifStreamIDTableT* SIDTable);


#endif /* _SdifStreamID_ */
