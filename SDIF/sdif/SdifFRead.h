/* $Id: SdifFRead.h,v 3.3 1999-09-28 13:08:54 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr

LIBRARY
 * SdifFRead.h
 *
 * F : SdifFileT* SdifF, Read : binary read (SdifF->Stream)
 *
 * author: Dominique Virolle 1997

LOG
 * $Log: not supported by cvs2svn $
 * Revision 3.2  1999/08/25  18:32:34  schwarz
 * Added cocoon-able comments with sentinel "DOC:" (on a single line).
 *
 * Revision 3.1  1999/03/14  10:56:44  virolle
 * SdifStdErr add
 *
 * Revision 2.3  1999/01/23  15:55:43  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.2  1999/01/23  13:57:28  virolle
 * General Lists, and special chunk preparation to become frames
 *
 * Revision 2.1  1998/12/21  18:27:11  schwarz
 * Inserted copyright message.
 *
 * Revision 2.0  1998/11/29  11:41:34  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.2  1998/11/10  15:31:42  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 */


#ifndef _SdifFRead_
#define _SdifFRead_

#include "SdifGlobals.h"
#include "SdifFileStruct.h"
#include <stdio.h>
#include "SdifMatrix.h"
#include "SdifFrame.h"
#include "SdifFGet.h"


/*DOC: 
  Lit l'entête du fichier, c'est à dire 'SDIF' puis 4 bytes.  affiche
  un message en cas de non reconnaissance du format.  */
size_t SdifFReadGeneralHeader    (SdifFileT *SdifF);

size_t SdifFReadChunkSize        (SdifFileT *SdifF);
size_t SdifFReadNameValueLCurrNVT(SdifFileT *SdifF);
size_t SdifFReadAllType          (SdifFileT *SdifF);
size_t SdifFReadAllStreamID      (SdifFileT *SdifF);

/*DOC: 
  Cette fonction permet de lire tous les Chunk ASCII qui se
  trouveraient en début de fichier juste après l'entête générale. Elle
  s'arrête lorsqu'elle ne reconnaît pas la signature de chunk comme un
  ASCII Chunk. Cette signature est donc normalement celle d'un
  frame. Elle est stockée dans SdifF->CurrSignature. <sttrong>Il n'est
  donc pas nécessaire de la relire</strong>.  */
size_t SdifFReadAllASCIIChunks   (SdifFileT *SdifF);

/*DOC: 
  Cette fonction lit une entête de matrice <strong>signature
  incluse</strong>.  Elle vérifie le type de matrice, le champ
  DataType. Toute les données se trouvent stockées dans
  SdifF->CurrMtrxH. La plupart de ses champs sont directement
  accessible par les fonctions indépendantes du mode d'ouverture du
  fichier.  <strong>Elle effectue une mise à jour de l'allocation
  mémoire de SdifF->CurrOneRow en fonction des paramètres de l'entête
  de matrice.</strong> Ainsi, on est normalement près pour lire chaque
  ligne de la matrice courrante.  */
size_t SdifFReadMatrixHeader     (SdifFileT *SdifF);

/*DOC: 
  Cette fonction permet de lire 1 ligne de matrice. Les données lues
  sont stockées dans SdifF->CurrOneRow (jusqu'à une prochaine lecture
  d'entête de matrice qui réinitialise ses paramètres).  */
size_t SdifFReadOneRow           (SdifFileT *SdifF);

/*DOC: 
  Cette fonction lit l'entête d'un frame à partir de la taille et
  jusqu'au temps. Donc <strong>elle ne lit pas la signature</strong>
  mais donne à SdifF->CurrFramH->Signature la valeur de
  SdifF->CurrSignature.  La lecture doit se faire avant, avec
  SdifFGetSignature.  */
size_t SdifFReadFrameHeader      (SdifFileT *SdifF);

/*DOC: 
  Cette fonction permet de passer une matrice toute entière entête
  incluse. Elle est utile lorsque qu'un frame contient plus de
  matrices que le programme lecteur n'en connaît. Il peut ainsi les
  passer pour retomber sur un autre frame.  */
size_t SdifSkipMatrix            (SdifFileT *SdifF);

/*DOC: 
  Cette fonction permet de passer une matrice mais après la lecture de
  l'entête. On s'en sert lorsque le type de matrice est mauvais,
  inconnu, non interprétable par le programme lecteur.  */
size_t SdifSkipMatrixData        (SdifFileT *SdifF);

/*DOC: 
  Cette fonction à le même sens que SdifSkipMatrixData mais pour les
  frames. Il faut donc pour l'utiliser avoir au préalable lu la
  signature et l'entête.  */
size_t SdifSkipFrameData         (SdifFileT *SdifF);

/*DOC: 
  Cette fonction permet de lire le Padding en fin de matrice.
  l'utilisation classique de cette fonctin est:<br> 
  <code> SizeR =  SdifFReadPadding(SdifF, SdifFPaddingCalculate(SdifF->Stream, SizeR));</code><br> 
  où SizeR est la taille en bytes lue depuis le
  début de la matrice, c'est à dire NbRow*NbCol*DataWith. En réalité,
  pour que SdifFPaddingCalculate fonctionne, il est seulement
  nécessaire que SizeR soit le nombre de bytes qui s'épare la position
  actuelle dans le fichier et un byte, repère d'allignement sur 64
  bits.  */
size_t SdifFReadPadding          (SdifFileT *SdifF, size_t Padding);
size_t SdifFReadUndeterminatedPadding (SdifFileT *SdifF);

size_t SdifFReadOneMatrixType    (SdifFileT *SdifF);
size_t SdifFReadOneFrameType     (SdifFileT *SdifF);

/*
 * obsolete
 */
size_t SdifFReadNameValueCurrHT  (SdifFileT *SdifF);

#endif /* _SdifFRead_ */
