/* $Id: sdif.h,v 1.9 2001-04-25 11:29:10 tisseran Exp $
 *
 * This file contains type declaration of variables used in SDIF library.
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.8  2000/12/07 13:01:39  roebel
 * Fixed wrong enum datatype declarations for backward compatibility
 *
 * Revision 1.7  2000/11/21 16:34:48  roebel
 * New SdifSignatureConst builds integer signature according to
 * endianess of machine. Multicharacter constants are no longer
 * supported by the library. Cleaned up sdif.h/SdifGlobals.h a bit.
 * Test for Multicharacter conversion is removed from configure.in.
 *
 * Revision 1.6  2000/11/21 14:51:34  schwarz
 * - sdif.h is now included by all sdif/Sdif*.c files.
 * - Removed all public typedefs, enums, structs, and defines from the
 *   individual sdif/Sdif*.h files, because they were duplicated in sdif.h.
 * - Todo: Do the same for the function prototypes, decide which types and
 *   prototypes really need to be exported.
 * - Preliminary new version of SdiffGetPos, SdiffSetPos.  They used the
 *   type fpos_t, which is no longer a long on RedHat 7 Linux.
 *
 * Revision 1.5  2000/11/16 12:20:17  lefevre
 * no message
 *
 * Revision 1.4  2000/11/16  12:02:22  lefevre
 * no message
 *
 * Revision 1.3  2000/11/15  14:53:22  lefevre
 * no message
 *
 * Revision 1.2  2000/10/27  20:03:18  roebel
 * autoconf merged back to main trunk
 *
 * Revision 1.1.2.2  2000/08/21  21:34:52  tisseran
 * *** empty log message ***
 *
 * Revision 1.1.2.1  2000/08/21  17:08:40  tisseran
 * *** empty log message ***
 *
 * Revision 1.1.2.1  2000/08/21  13:07:41  tisseran
 * *** empty log message ***
 *
 * $Date: 2001-04-25 11:29:10 $
 *
 */


#ifndef _SDIF_H
#define _SDIF_H 1

#ifdef __cplusplus
extern "C" {
#endif


static const char _sdif_h_cvs_revision_ [] = "$Id: sdif.h,v 1.9 2001-04-25 11:29:10 tisseran Exp $";


#include <stdio.h>
#include <float.h>


/* SdifHash.h */
typedef enum SdifHashIndexTypeE
{
  eHashChar,
  eHashInt4
} SdifHashIndexTypeET;


typedef union SdifHashIndexU SdifHashIndexUT;

union SdifHashIndexU
{
  char* Char[1]; /* tab of one pointer to fixe union size at 4 or 8 bytes */
  unsigned int  Int4;
} ;


typedef struct SdifHashNS SdifHashNT;
typedef struct SdifFileS SdifFileT;

struct SdifHashNS 
{
  SdifHashNT *Next;
  SdifHashIndexUT Index;
  void* Data;
};


typedef struct SdifHashTableS SdifHashTableT;

struct SdifHashTableS
{
  SdifHashNT* *Table;
  unsigned int HashSize;
  SdifHashIndexTypeET IndexType;
  void (*Killer)();  /* no verification of arguments */
  unsigned int NbOfData;
} ;

/* SdifError.h */
typedef enum SdifErrorE
{
  eFalse = 0,
  eTrue = 1,
  eFreeNull = 256,
  eAllocFail,
  eArrayPosition,
  eEof,
  eFileNotFound,
  eInvalidPreType,
  eAffectationOrder,
  eNoModifErr,
  eNotInDataTypeUnion,
  eNotFound,
  eExistYet,
  eWordCut,
  eTokenLength
} SdifErrorEnum;


/*DOC:
  Exit function type (See SdifSetExitFunc). */
typedef void (*SdifExitFuncT) (void);


/* SdifMemory.h */
#define _SdifMrNameSize 64
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




/* to do fpos_t compatible on MacinTosh */
#if defined(MACINTOSH) || defined(WIN32)
    /* on mac or windows, seeking on a stream is always considered
       successful (return 0)! */
#   define SdiffPosT		long
#   define SdiffIsFile(f)	((f)!=stdin && (f)!=stdout && (f)!=stderr)
#   define Sdiffftell(f)	(SdiffIsFile(f)  ?  ftell(f)  :  0)
#   define SdiffGetPos(f,p)	((*(p) = Sdiffftell(f)) == -1  ?  -1  :  0)
#   define SdiffSetPos(f,p)	SdiffIsFile(f)  \
				    ?  fseek(f, (long)(*(p)), SEEK_SET)  :  0
#else
/*
#   define SdiffPosT		fpos_t
#   define SdiffGetPos(f,p)     fgetpos((f),(p))
#   define SdiffSetPos(f,p)     fsetpos((f),(p))

#   define SdiffGetPos(f,p)	((*(p) = ftell(f)) == -1  ?  -1  :  0)
#   define SdiffSetPos(f,p)     (fseek(f, (long)(*(p)), SEEK_SET))
*/

/*DS: FORCE long fpos*/
/* ftell/fseek can be applied to stdin/out/err at least in a restricted manner
 * (same as fgetpos/fsetpos) so let's try */
#   define SdiffPosT		long
#   define SdiffGetPos(f,p)	((*(p) = ftell(f)) == -1  ?  -1  :  0)
#   define SdiffSetPos(f,p)	fseek(f, (long)(*(p)), SEEK_SET) 
#endif



/* SdifHard_OS.h */

typedef char           SdifChar;
typedef short          SdifInt2;
typedef unsigned short SdifUInt2;
typedef int            SdifInt4;
typedef unsigned int   SdifUInt4;
typedef float          SdifFloat4;
typedef double         SdifFloat8;
typedef unsigned int   SdifSignature;


typedef enum SdifMachineE
{
  eUndefinedMachine,
  eBigEndian,
  eLittleEndian,
  eBigEndian64,
  eLittleEndian64,
  ePDPEndian
} SdifMachineET;

typedef enum SdifBinaryMode
{
  eBinaryModeUnknown,
  eBinaryModeWrite,
  eBinaryModeRead,
  eBinaryModeStdInput,
  eBinaryModeStdOutput,
  eBinaryModeStdError
} SdifBinaryModeET ;

/* SdifGlobals.h */
/* DOC:

  Macro to generate an integer representation of the sequence of unsigned chars 
  for example :

  SdifSignature sig=SdifSignatureConst('A','B','C','D');

  Because integers are differently handled on little/big endian machines the
  signatures are swapped if read from a file to match internal format. */
  
#   define SdifSignatureConst(p1,p2,p3,p4) (((((unsigned int)(p1))&0xff)<<24)|((((unsigned int)(p2))&0xff)<<16)|((((unsigned int)(p3))&0xff)<<8)|(((unsigned int)(p4))&0xff))


typedef enum SdifSignatureE
{
  eSDIF = SdifSignatureConst('S','D','I','F'), /* SDIF header */
  e1NVT = SdifSignatureConst('1','N','V','T'), /* Name Value Table */
  e1TYP = SdifSignatureConst('1','T','Y','P'), /* TYPe declarations */
  e1MTD = SdifSignatureConst('1','M','T','D'), /* Matrix Type Declaration */
  e1FTD = SdifSignatureConst('1','F','T','D'), /* Frame Type Declaration */
  e1IDS = SdifSignatureConst('1','I','D','S'), /* ID Stream Table */
  eSDFC = SdifSignatureConst('S','D','F','C'), /* Start Data Frame Chunk (text files) */
  eENDC = SdifSignatureConst('E','N','D','C'), /* END Chunk (text files) */
  eENDF = SdifSignatureConst('E','N','D','F'), /* END File (text files) */
  eFORM = SdifSignatureConst('F','O','R','M'), /* FORM for IFF compatibility (obsolete ?) */
  eEmptySignature = SdifSignatureConst('\0','\0','\0','\0')
} SdifSignatureET;

typedef enum SdifModifModeE
{
  eNoModif,
  eCanModif
} SdifModifModeET;


/* DataTypeEnum

   On Matt Wright's visit at IRCAM June 1999, we defined a new
   encoding for the MatrixDataType field with the feature that the low
   order byte encodes the number of bytes taken by each matrix
   element.  

   Low order byte encodes the number of bytes 
   High order bytes come from this (extensible) enum:

        0 : Float
        1 : Signed integer
        2 : Unsigned integer
        3 : Text (UTF-8 when 1 byte)
        4 : arbitrary/void
*/
typedef enum SdifDataTypeE
{
  eText	    = 0x0301,
  eChar     = 0x0301,
  eFloat4   = 0x0004,
  eFloat8   = 0x0008,
  eInt2     = 0x0102,
  eInt4     = 0x0104,
  eInt8     = 0x0108,
  eUInt2    = 0x0202,
  eUInt4    = 0x0204,
  eUInt8    = 0x0208,
 	    
  eFloat4a  = 0x0001,	/* =  1 */    /* Backwards compatibility with old */
  eFloat4b  = 0x0020,	/* = 32 */    /* IRCAM versions < 3 of SDIF */
  eFloat8a  = 0x0002,	/* =  2 */    /* IN TEXT MODE ONLY! */
  eFloat8b  = 0x0040	/* = 64 */
} SdifDataTypeET;

/* SdifList.h */
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

/* SdifNameValue.h */
typedef struct SdifNameValueS SdifNameValueT;
struct SdifNameValueS
{
  char *Name;
  char *Value;
} ;

typedef struct SdifNameValueTableS SdifNameValueTableT;
struct SdifNameValueTableS
{
    SdifHashTableT* NVHT;
    SdifUInt4       NumTable;
    SdifUInt4       StreamID;	/* id of stream the table belongs to */
} ;

typedef struct SdifNameValuesLS SdifNameValuesLT;
struct SdifNameValuesLS
{
    SdifListT*              NVTList;
    SdifNameValueTableT*    CurrNVT;
    SdifUInt4               HashSize;
};


/* SdifStreamID.h */

/*
// DATA GROUP:		Stream ID Table and Entries for 1IDS ASCII chunk
*/


/*DOC:
  Stream ID Table Entry */
typedef struct SdifStreamIDS SdifStreamIDT;
struct SdifStreamIDS
{
  SdifUInt4     NumID;
  char *Source;
  char *TreeWay; /* for the moment or to be general*/
} ;

/*DOC:
  Stream ID Table, holds SdifStreamIDT stream ID table entries */
typedef struct SdifStreamIDTableS SdifStreamIDTableT;
struct SdifStreamIDTableS
{
    SdifHashTableT* SIDHT;
    SdifUInt4       StreamID;
    SdifFloat8      Time;	/* always _SdifNoTime */
} ;


typedef struct SdifColumnDefS SdifColumnDefT;

struct SdifColumnDefS
{
  char *Name;
  SdifUInt4 Num;
} ;


/* SdifMatrixType.h */
typedef struct SdifMatrixTypeS SdifMatrixTypeT;

struct SdifMatrixTypeS
{
  SdifSignature     Signature;

  SdifMatrixTypeT*  MatrixTypePre;

  SdifListT*        ColumnUserList; /* List of columns added by user: 
				       SdifMatrixTypeInsertTailColumn(MatrixTypeT *)
				    */

  SdifUInt4       NbColumnDef; /* Number of columns created by user:
				  SdifMatrixTypeInsertTailColumn(MatrixTypeT *)
			       */
  SdifModifModeET ModifMode;
};


/* SdifFrameType.h */
typedef struct SdifComponentS SdifComponentT;
struct SdifComponentS
{
  SdifSignature MtrxS;
  char *Name;
  SdifUInt4  Num;
} ;

typedef struct SdifFrameTypeS SdifFrameTypeT;
struct SdifFrameTypeS
{
  SdifSignature Signature;

  SdifFrameTypeT* FrameTypePre;

  SdifHashTableT *ComponentUseHT;
  SdifUInt4       NbComponentUse;

  SdifUInt4       NbComponent;
  SdifModifModeET ModifMode;
};


/* SdifMatrix.h */
typedef struct SdifMatrixHeaderS SdifMatrixHeaderT;

struct SdifMatrixHeaderS
{
  SdifSignature  Signature;
  SdifDataTypeET DataType; /* Low level data type */
  SdifUInt4      NbRow;
  SdifUInt4      NbCol;
} ;


typedef union DataTypeU DataTypeUT;

union DataTypeU
{
  SdifFloat4 *Float4;
  SdifFloat8 *Float8;
  SdifInt2   *Int2  ;
  SdifInt4   *Int4  ;
/*SdifInt8   *Int8  ;*/
  SdifUInt2  *UInt2 ;
  SdifUInt4  *UInt4 ;
/*SdifUInt8  *UInt8 ;*/
  SdifChar   *Char  ;
  void	     *Void  ;	/* generic pointer */
} ;


typedef struct SdifOneRowS SdifOneRowT;

struct SdifOneRowS
{
  SdifDataTypeET DataType;
  SdifUInt4      NbData;
  DataTypeUT     Data;
  SdifUInt4      NbGranuleAlloc;
} ;

typedef struct SdifMatrixDataS SdifMatrixDataT;
struct SdifMatrixDataS
{
  SdifMatrixHeaderT *Header;
  SdifOneRowT       **Rows;
  SdifUInt4         Size;
} ;

/* SdifFrame.h */
typedef struct SdifFrameHeaderS SdifFrameHeaderT;
struct SdifFrameHeaderS
{
  SdifSignature Signature;
  SdifUInt4  Size;
  SdifUInt4  NbMatrix;
  SdifUInt4  NumID;
  SdifFloat8 Time;
} ;


typedef struct SdifFrameDataS SdifFrameDataT;
struct SdifFrameDataS
{
  SdifFrameHeaderT *Header;
  SdifMatrixDataT* *Matrix_s;
} ;

/* SdifTimePosition.h */
typedef struct SdifTimePositionS SdifTimePositionT;

struct SdifTimePositionS
{
  SdifFloat8    Time;
  SdiffPosT     Position;
} ;

SdifTimePositionT* SdifCreateTimePosition(SdifFloat8 Time, SdiffPosT Position);
void               SdifKillTimePosition(void* TimePosition);


typedef struct SdifTimePositionLS SdifTimePositionLT;

struct SdifTimePositionLS
{
    SdifListT*          TimePosList;
} ;

/* SdifSignatureTab.h */
typedef struct SdifSignatureTabS SdifSignatureTabT;
struct SdifSignatureTabS
{
  SdifUInt4 NbSignMax;
  SdifUInt4 NbSign;
  SdifSignature* Tab;
};


/* SdifSelect.h */

/* 
// DATA GROUP:	SDIF Selection
*/

/* tokens (numerical ids) for sdifspec separators */
typedef enum { sst_specsep, sst_stream, sst_frame, sst_matrix, sst_column, 
	       sst_row,     sst_time,   sst_list,  sst_range,  sst_delta,
	       sst_num,	/* number of tokens */	   sst_norange = 0
} SdifSelectTokens;

/*DOC: 
  Selection element interface (returned by SdifGetNextSelection*):
  One basic data element value, with optional range.  
  The meaning of range is determined by rangetype: 

  [] 0		no range
  [] sst_range	range is value..range
  [] sst_delta	range is value-range..value+range
*/

typedef struct 
{
    int		       value, range;
    SdifSelectTokens   rangetype; /* 0 for not present, sst_range, sst_delta */
} SdifSelectElementIntT;

typedef struct 
{
    double	       value, range;
    SdifSelectTokens   rangetype; /* 0 for not present, sst_range, sst_delta */
} SdifSelectElementRealT;

/* no SdifSelectElementSignatureT or string range, since it makes no sense */



/*DOC:
  Internal: one value of different possible types in a selection
  element (the element list determines which type is actually used).  
*/
typedef union SdifSelectValueS 
{
    int            integer;
    double         real;
    char	   *string;
    SdifSignature  signature;
} SdifSelectValueT;

/*DOC: 
  Selection element internal data structure:
  One basic data element, with optional <ul>
  <li> range (value is lower, range is upper bound) or 
  <li> delta (value-range is lower, value+range is upper bound)
  </ul>
*/
typedef struct SdifSelectElementS
{
    SdifSelectValueT value;
    SdifSelectValueT range;
    SdifSelectTokens rangetype; /* 0 for not present, sst_range, sst_delta */
} SdifSelectElementT, *SdifSelectElementP;

/*DOC: 
  Holds a selection of what data to access in an SDIF file,
  parsed from a simple regular expression.  
*/
typedef struct
{
    char	*filename,	/* allocated / freed by 
				   SdifInitSelection / SdifFreeSelection */
		*basename;	/* points into filename */
    SdifListP	stream, frame, matrix, column, row, time;
} SdifSelectionT;

/* TODO: array of select elements
     struct { SdifListP list; SdifSelectElementT minmax; } elem [eSelNum];
   indexed by
     enum   { eTime, eStream, eFrame, eMatrix, eColumn, eRow, eSelNum }
   to use in all API functions instead of SdifListP.
*/



/* SdifErrMess.h */
typedef enum SdifErrorTagE
{
	eUnknow,
	eNoError,
	eTypeDataNotSupported,
	eNameLength,
	eReDefined,
	eUnDefined,
	eSyntax,
	eBadTypesFile,
	eBadType,
	eBadHeader,
	eRecursiveDetect,
	eUnInterpreted,
	eOnlyOneChunkOf,
	eUserDefInFileYet,
	eBadMode,
	eBadStdFile,
	eBadNbData,
	eReadWriteOnSameFile,
    eBadFormatVersion,
    eMtrxUsedYet,
    eMtrxNotInFrame
} SdifErrorTagET;


/*DOC:
  Level of Error */
typedef enum SdifErrorLevelE
{
	eFatal,
	eError,
	eWarning,
	eRemark,
	eNoLevel,
	eNumLevels	/* level count, must always be last */
} SdifErrorLevelET;


typedef struct SdifErrorS SdifErrorT;
struct SdifErrorS
{
	SdifErrorTagET		Tag;
	SdifErrorLevelET	Level;
	char*				UserMess;
};

typedef struct SdifErrorLS SdifErrorLT;
struct SdifErrorLS
{
  SdifListT*    ErrorList;
  SdifFileT*	SdifF; /* only a link */
};




/* SdifFileStruct.h */

/*
// DATA GROUP:	SDIF File Structure
*/

/*DOC:
  File mode argument for SdifFOpen. */
typedef enum SdifFileModeE
{
  eUnknownFileMode,	/* 0 */
  eWriteFile,
  eReadFile,
  ePredefinedTypes,	/* 3 */

  eModeMask = 7,	/* get rid of flags */

  /* from here on we have flags that can be or'ed with the previous modes */
  eParseSelection = 8
} SdifFileModeET ;



enum SdifPassE
{
  eNotPass,
  eReadPass,
  eWritePass
};


/*
// DATA GROUP:		SDIF File Structure
*/

#define	MaxUserData	10
/*DOC:
  THE SDIF File Structure! */
struct SdifFileS
{
  char		     *Name;		/* Name of the file, can be "stdin, stdout, stderr */
  SdifFileModeET     Mode;		/* eWriteFile or eReadFile or ePredefinedTypes */
  int		     isSeekable;	/* file is neither standard i/o nor pipe i/o */

  SdifUInt4	     FormatVersion;	/* version of the SDIF format itself */
  SdifUInt4	     TypesVersion;	/* version of the description type collection */

  SdifNameValuesLT   *NameValues;       /* DataBase of Names Values */
  SdifHashTableT     *MatrixTypesTable; /* DataBase of Matrix Types */
  SdifHashTableT     *FrameTypesTable;  /* DataBase of Frame Types */
/*  SdifHashTableT     *StreamIDsTable;    DataBase of Stream IDs */
  SdifStreamIDTableT *StreamIDsTable;   /* DataBase of Stream IDs */
  SdifTimePositionLT *TimePositions;    /* List of (Time, Position in file) */
  SdifSelectionT     *Selection;	/* default selection parsed from Name */

  FILE *Stream;                         /* Stream to read or to write */

  SdifSignature      CurrSignature;
  SdifFrameHeaderT   *CurrFramH;        /* Current Frame Header can be NULL */
  SdifMatrixHeaderT  *CurrMtrxH;        /* Current Matrix Header can be NULL */

  SdifFrameTypeT     *CurrFramT;
  SdifMatrixTypeT    *CurrMtrxT;
  SdifFloat8         PrevTime;
  SdifSignatureTabT  *MtrxUsed;

  SdifOneRowT        *CurrOneRow;
  /* Current OneRow allocated memory in function
   * of _SdifGranule, use SdifReInitOneRow(SdifOneRowT *OneRow, SdifDataTypeET DataType, SdifUInt4 NbData)
   * to assure NbData (=NbColumns) objects memory allocated
   */

  size_t  FileSize;
  size_t  ChunkSize;

  SdiffPosT  CurrFramPos;
  SdiffPosT  StartChunkPos;
  SdiffPosT  Pos;
  
  SdifUInt2  TypeDefPass;
  SdifUInt2  StreamIDPass;

  char *TextStreamName;                 /* Name of the text file corresponding to the sdif file */
  FILE *TextStream;                     /* Stream text */

  SdifUInt4     ErrorCount [eNumLevels];/* Error count per level of severity */
  SdifErrorLT  *Errors;			/* List of errors or warnings */

  int		NbUserData;		/* todo: hash table */
  void		*UserData [MaxUserData];
};	/* end struct SdifFileS */




/* SdifString.h */

typedef struct SdifStringS SdifStringT;
struct SdifStringS
{
  char   *str; 
  size_t TotalSize; /* Memory size allocated for str */
  size_t SizeW; /* Memory size actually used */
  int    NbCharRead; /* Number of char read */
};


/*DOC: 
  Test if file is an SDIF file.

  [] Returns:	0 if not an SDIF file (the first 4 chars are not "SDIF"),
		or file can not be opened, else 1.  

  Warning: This function doesn't work with stdio. */
int SdifCheckFileFormat (const char *name);


/*DOC: 
  Test if file contains frames of certain types.

  [in]  name	Filename + selection
	frames  Table of frame signatures to look for
  []	return	The first signature from frames found, or eEmptySignature if 
		no frames could be found (or if file is not SDIF).

  Warning: This function doesn't work with stdio. */
SdifSignature SdifCheckFileFramesTab   (const char		*name, 
					const SdifSignatureTabT *frames);

/*DOC: 
  Test if file contains frames of certain types.

  [in]  name	Filename + selection
	frames  Array of frame signatures to look for, terminated with 
		eEmptySignature.
  []	return	The index in frames of the first signature found, or -1
		if no frames could be found (or if file is not SDIF).

  Warning: This function doesn't work with stdio. */
int	      SdifCheckFileFramesIndex (const char	        *name, 
					const SdifSignature     *frames);

/*DOC: 
  Test if file contains frames of certain types.

  [in]  in	open SDIF file
	frames  Table of frame signatures to look for
  [out] index   If the int pointer index is not NULL, it will receive
		the index in frames of the first signature found, or -1
		if no frames could be found (or if file is not SDIF).
  []	return	The first signature from frames found, or eEmptySignature if 
		no frames could be found (or if file is not SDIF).

  Warning: This function doesn't work with stdio. */
SdifSignature SdifCheckNextFrame       (SdifFileT		*in, 
					const SdifSignatureTabT *frames,
					int			*index);

/*DOC: 
  TODO: Test if file is an SDIF file (only when opening for read or
  append) and open it.

  [Return] NULL if not an SDIF file (the first 4 chars are not "SDIF"),
  or file can not be opened.  */
SdifFileT*	   SdifFTryOpen			(const char *Name, SdifFileModeET Mode);


/*DOC: 
  Converti un fichier texte pseudo-SDIF de nom TextStreamName en un
  fichier SDIF binaire de non SdifF->Name. Le fichier doit avoir été
  ouvert en écriture (eWriteFile).  */
size_t SdifToText (SdifFileT *SdifF, char *TextStreamName);


/*#include "SdifFile.h"
 */


/* Table of error format for the fprintf */
extern const SdifErrorT gSdifErrMessFormat[];

/*DOC:
  Switch output of error messages on stderr by _SdifFError on. 
*/
void	SdifEnableErrorOutput  (void);

/*DOC:
  Switch output of error messages on stderr by _SdifFError off. 
*/
void	SdifDisableErrorOutput (void);


extern char	gSdifBufferError[4096];
extern int	gSdifErrorOutputEnabled;


extern SdifExitFuncT gSdifExitFunc;
extern char *SdifErrorFile;
extern int SdifErrorLine;
extern FILE* SdifStdErr;



/*DOC: 
  Lit 4 bytes, les considère comme une signature qui est placée dans
  SdifF->CurrSignature, incrémente NbCharRead du nombre de bytes lus
  et renvoie le dernier caractère lu convert en int (-1 si erreur).  */
int    SdifFGetSignature	(SdifFileT *SdifF, size_t *NbCharRead);


/*DOC: 
  Lit l'entête du fichier, c'est à dire 'SDIF' puis 4 bytes.  affiche
  un message en cas de non reconnaissance du format.  */
size_t SdifFReadGeneralHeader    (SdifFileT *SdifF);

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
size_t SdifFSkipMatrix	         (SdifFileT *SdifF);
/*DOC:
  obsolete, now called SdifFSkipMatrix */
size_t SdifSkipMatrix            (SdifFileT *SdifF);

/*DOC: 
  Cette fonction permet de passer une matrice mais après la lecture de
  l'entête. On s'en sert lorsque le type de matrice est mauvais,
  inconnu, non interprétable par le programme lecteur.

  Note:  The matrix padding is skipped also. */
size_t SdifFSkipMatrixData       (SdifFileT *SdifF);
/*DOC:
  obsolete, now called SdifFSkipMatrixData */
size_t SdifSkipMatrixData        (SdifFileT *SdifF);

/*DOC: 
  Cette fonction à le même sens que SdifSkipMatrixData mais pour les
  frames. Il faut donc pour l'utiliser avoir au préalable lu la
  signature et l'entête.  */
size_t SdifFSkipFrameData        (SdifFileT *SdifF);
/*DOC:
  obsolete, now called SdifFSkipFrameData */
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

/*DOC:
  Read and throw away <i>num</i> bytes from the file. */
size_t SdifFReadAndIgnore (SdifFileT *SdifF, size_t bytes);


/*DOC: 
  écrit sur le fichier 'SDIF' puis 4 bytes chunk size.  */
size_t  SdifFWriteGeneralHeader   (SdifFileT *SdifF);

/*DOC: 
  écrit tous les chunks ASCII. C'est à dire: les tables de names
  values, les types créés ou complétés, et les Stream ID. Il faut donc
  au préalable avoir rempli complétement les tables avant de la
  lancer. Cette fonction de peut donc pas être executer une 2nd fois
  durant une écriture.  */
size_t  SdifFWriteAllASCIIChunks  (SdifFileT *SdifF);


/*
//FUNCTION GROUP:	Writing Matrices
*/

/*DOC: 
  Après avoir donner une valeur à chaque champ de SdifF->CurrMtrxH
  gràce à la fonction SdifFSetCurrMatrixHeader, SdifFWriteMatrixHeader
  écrit toute l'entête de la matrice.  Cette fonction réalise aussi
  une mise à jour de SdifF->CurrOneRow, tant au niveau de l'allocation
  mémoire que du type de données.  */
size_t  SdifFWriteMatrixHeader    (SdifFileT *SdifF);

/*DOC: 
  Après avoir donner les valeurs à chaque case de SdifF->CurrOneRow à
  l'aide de SdifFSetCurrOneRow ou de SdifFSetCurrOneRowCol (suivant
  que l'on possède déjà un tableau flottant ou respectivement une
  méthode pour retrouver une valeur de colonne), SdifFWriteOneRow
  écrit 1 ligne de matrice suivant les paramètres de SdifF->CurrMtrxH.  */
size_t  SdifFWriteOneRow          (SdifFileT *SdifF);

/*DOC: 
  Write whole matrix data, (after having set the matrix header with 
  SdifFSetCurrMatrixHeader (file, matrixsig, datatype, nrow, ncol).
  Data points to nbrow * nbcol * SdifSizeofDataType (datatype) bytes in 
  row-major order.  Padding still has to be written.  */
size_t SdifFWriteMatrixData (SdifFileT *SdifF, void *Data);

/*DOC:
  Write whole matrix: header, data, and padding.
  Data points to NbRow * NbCol * SdifSizeofDataType (DataType) bytes in
  row-major order. */
size_t SdifFWriteMatrix (SdifFileT     *SdifF,
			 SdifSignature  Signature,
			 SdifDataTypeET DataType,
			 SdifUInt4      NbRow,
			 SdifUInt4      NbCol,
			 void	       *Data);

/*DOC:
  Write a matrix with datatype text (header, data, and padding).
  Data points to Length bytes(!) of UTF-8 encoded text.  Length
  includes the terminating '\0' character!!!  That is, to write a
  C-String, use SdifFWriteTextMatrix (f, sig, strlen (str) + 1, str);
  to include it. */
size_t SdifFWriteTextMatrix (SdifFileT     *SdifF,
			     SdifSignature  Signature,
			     SdifUInt4      Length,
			     char	   *Data);

/*DOC: 
  TBI: Convert ASCII C-String to UTF-8 encoded string, returning
  length (including terminating null character). */
size_t SdifAsciiToUTF8 (char *ascii_in, char *utf8_out);

/*DOC: 
  Cette fonction permet en fin d'écriture de matrice d'ajouter le
  Padding nécessaire. Il faut cependant avoir la taille de ce
  Padding. On utilise SdifFPaddingCalculate(SdifF->Stream,
  SizeSinceAlignement) où SizeSinceAllignement est un
  <code>size_t</code> désignant le nombre de bytes qui sépare la
  position actuelle d'écriture avec une position connue où le fichier
  est aligné sur 64 bits (en général, c'est la taille de la matrice en
  cours d'écriture: NbRow*NbCol*DatWitdh).  */
size_t  SdifFWritePadding         (SdifFileT *SdifF, size_t Padding);


/*
//FUNCTION GROUP:	Writing Frames
*/

/*DOC: 
  Après avoir donner une valueur à chaque champ de SdifF->CurrFramH
  gràce à la fonction SdifFSetCurrFrameHeader, SdifFWriteFrameHeader
  écrit toute l'entête de frame.  Lorsque la taille est inconnue au
  moment de l'écriture, donner la valeur _SdifUnknownSize. Ensuite,
  compter le nombre de bytes écrit dans le frame et réaliser un
  SdifUpdateChunkSize avec la taille calculée.  */
size_t  SdifFWriteFrameHeader     (SdifFileT *SdifF);

/*DOC: 
  Execute un retour fichier de ChunkSize bytes et l'écrit, donc on
  écrase la taille du chunk ou du frame.  Dans le cas où le fichier
  est stderr ou stdout, l'action n'est pas réalisée.  */
void    SdifUpdateChunkSize       (SdifFileT *SdifF, size_t ChunkSize);

/*DOC: 
  Rewrite given frame size and number of matrices in frame header.
  Return -1 on error or if file is not seekable (stdout or stderr). */
int     SdifUpdateFrameHeader	  (SdifFileT *SdifF, size_t ChunkSize, 
				   SdifInt4 NumMatrix);

/*DOC:
  Write a whole frame containing one matrix: 
  frame header, matrix header, matrix data, and padding.
  Data points to NbRow * NbCol * SdifSizeofDataType (DataType) bytes in
  row-major order. 

  This function has the big advantage that the frame size is known in
  advance, so there's no need to rewind and update after the matrix
  has been written.  */
size_t  SdifFWriteFrameAndOneMatrix (SdifFileT	    *SdifF,
				     SdifSignature  FrameSignature,
				     SdifUInt4      NumID,
				     SdifFloat8     Time,
				     SdifSignature  MatrixSignature,
				     SdifDataTypeET DataType,
				     SdifUInt4      NbRow,
				     SdifUInt4      NbCol,
				     void	    *Data);


/*DOC:
  Return (constant) size of frame header after signature and size field. 
  Use this to calculate the Size argument for SdifFSetCurrFrameHeader. */
size_t SdifSizeOfFrameHeader (void);

/*DOC:
  Return size of matrix (header, data, padding).
  Use this to calculate the Size argument for SdifFSetCurrFrameHeader. */
size_t SdifSizeOfMatrix (SdifDataTypeET DataType,
			 SdifUInt4      NbRow,
			 SdifUInt4      NbCol);

/*DOC:
  Write a text matrix using a string.
  Return number of bytes written.
*/
size_t SdifFWriteTextFrame(SdifFileT     *SdifF,
			   SdifSignature FrameSignature,
			   SdifUInt4     NumID,
			   SdifFloat8    Time,
			   SdifSignature MatrixSignature,
			   char          *str,
			   size_t        length);

/*DOC:
  Write a text matrix using a SdifString.
  Return number of bytes written.
*/
size_t SdifFWriteTextFrameSdifString(SdifFileT     *SdifF,
				     SdifSignature FrameSignature,
				     SdifUInt4     NumID,
				     SdifFloat8    Time,
				     SdifSignature MatrixSignature,
				     SdifStringT   *SdifString);


/*
 * obsolete
 */
size_t  SdifFWriteNameValueCurrHT (SdifFileT *SdifF);
size_t  SdifFWriteAllNameValueHT  (SdifFileT *SdifF);


/*
// FUNCTION GROUP:	Opening and Closing of Files
*/

/*DOC:
 */
SdifFileT*         SdifFOpen                    (const char *Name, SdifFileModeET Mode);
SdifFileT*         SdifOpenFile                 (const char *Name, SdifFileModeET Mode);
SdifFileT*         SdifFOpenText                (SdifFileT *SdifF, const char* Name, SdifFileModeET Mode);
/*DOC:
 */
void               SdifFClose                   (SdifFileT *SdifF);
void               SdifCloseFile                (SdifFileT *SdifF);
SdifFrameHeaderT*  SdifFCreateCurrFramH         (SdifFileT *SdifF, SdifSignature Signature);
SdifMatrixHeaderT* SdifFCreateCurrMtrxH         (SdifFileT *SdifF);
FILE*              SdifFGetFILE_SwitchVerbose   (SdifFileT *SdifF, int Verbose);
void               SdifTakeCodedPredefinedTypes (SdifFileT *SdifF);
void               SdifFLoadPredefinedTypes     (SdifFileT *SdifF, char *TypesFileName);

extern int	  gSdifInitialised;
extern SdifFileT *gSdifPredefinedTypes;



/*
// FUNCTION GROUP:	Init/Deinit of the Library
*/

/*DOC: 
  Initialise the SDIF library, providing a name for an optional additional
  file with type definitions or "".
  <b>This function has to be called once and only once per process 
  before any other call to the SDIF library.</b> */
void SdifGenInit (char *PredefinedTypesFile); 

/*DOC:
  Initialise the SDIF library if it has not been initialised before.
  This function has to be called at least once, but can be called as
  many times as desired.  Especially useful for dynamic libraries.

  [in] PredefinedTypesFile:
	name for an optional additional file with type definitions or "". */
void SdifGenInitCond (char *PredefinedTypesFile);

/*DOC:
  Deinitialise the SDIF library */
void SdifGenKill (void); 

/*DOC:
  Set function that will be called after a grave error has occurred.  
  Default is exit(). */
void SdifSetExitFunc (SdifExitFuncT func);


/*DOC:
  Print version information to standard error. */
void SdifPrintVersion(void);


/*
// FUNCTION GROUP:	Current Header Access Functions
*/

/*DOC: 
  Permet de donner des valeurs à chaque champ de l'entête de frame
  temporaire de SdifF.<p> 

  Exemple:
  <code>SdifSetCurrFrameHeader(SdifF, '1FOB', _SdifUnknownSize, 3, streamid, 1.0);</code> */
SdifFrameHeaderT* SdifFSetCurrFrameHeader (SdifFileT *SdifF, 
					   SdifSignature Signature, 
					   SdifUInt4 Size,
					   SdifUInt4 NbMatrix, 
					   SdifUInt4 NumID, 
					   SdifFloat8 Time);

/*DOC: 
  Permet de donner des valeurs à chaque champ de l'entête de matice
  temporaire de SdifF.<p>

  Exemple:
  <code>SdifSetCurrMatrixHeader(SdifF, '1FOF', eFloat4, NbFofs, 7);</code> */
SdifMatrixHeaderT* SdifFSetCurrMatrixHeader (SdifFileT *SdifF, 
					     SdifSignature Signature,
					     SdifDataTypeET DataType, 
					     SdifUInt4 NbRow, SdifUInt4 NbCol);


/*DOC: 
  Recopie la mémoire pointée par Values en fonction de l'entête de
  matrice courante.<p> 

  Exemple:<br>
<pre>
  #define NbCols = 10;<br>

  float t[NbCols] = { 1., 2., 3., 4., 5., 6., 7., 8., 9., 0.};<br>

  SdifFSetCurrMatrixHeader(SdifF, 'mtrx', eFloat4, 1, NbCols);<br>
  SdifFSetCurrOneRow      (SdifF, (void*) t);<br>
</pre>

  On connait la taille de la mémoire à recopier par le type de donnée
  (ici: eFloat4) et le nombre de colonnes (ici: NbCols). Il faut que
  le type de donnée de la matrice courante corresponde avec la taille
  d'un élément de t. Si t est composé de float sur 4 bytes, alors on
  doit avoir eFloat4. Si t est composé de double float sur 8 bytes,
  alors c'est eFloat8.<br>

  En général, les données d'un programme ne se présente pas sous cette
  forme et il faut réaliser une transposition lors des transfert de
  Sdif à un programme. Le programme Diphone Ircam a un bon exemple de
  lecture avec transposition automatique, généralisée pour tout type
  de matrice. */
SdifOneRowT*  SdifFSetCurrOneRow       (SdifFileT *SdifF, void *Values);


/*DOC: 
  Permet de donner la valeur Value dans la ligbe de matrice temporaire
  de SdifF à la colonne numCol (0<numCol<=SdifF->CurrMtrxH->NbCol).  */
SdifOneRowT* SdifFSetCurrOneRowCol (SdifFileT *SdifF, SdifUInt4
numCol, SdifFloat8 Value);


/*DOC: 
  Recupère la valeur stockée à la colonne numCol de la ligne
  temporaire.  C'est un SdifFloat8 donc un double!!  */ 
SdifFloat8 SdifFCurrOneRowCol (SdifFileT *SdifF, SdifUInt4 numCol);


/*DOC: 
  Idem que la fonction précédente mais en utilisant le type de la
  matrice et le nom de la colonne.  */
SdifFloat8    SdifFCurrOneRowColName   (SdifFileT *SdifF, 
					SdifMatrixTypeT *MatrixType, 
					char *NameCD);


/*DOC: 
  Renvoie la signature temporaire de Chunk ou de Frame.  */
SdifSignature SdifFCurrSignature       (SdifFileT *SdifF);


/*DOC: 
  Met à 0 tous les bits de la signature temporaire.  */
SdifSignature SdifFCleanCurrSignature  (SdifFileT *SdifF);

/*DOC: 
  Renvoie la signature temporaire du dernier Frame lu ou du prochain à
  écrire.  */
SdifSignature SdifFCurrFrameSignature  (SdifFileT *SdifF);

/*DOC: 
  Renvoie la signature temporaire de la dernier matrice lue ou de la
  prochaine à écrire.  */
SdifSignature SdifFCurrMatrixSignature (SdifFileT *SdifF);

/*DOC: 
  Renvoie la ligne temporaire de SdifF.  */
SdifOneRowT*  SdifFCurrOneRow          (SdifFileT *SdifF);

/*DOC:
  Returns a pointer to the data of the current matrix row.  According to the matrix data type, it can be a pointer to float or double. */
void*	     SdifFCurrOneRowData	  (SdifFileT *SdifF);

/*DOC: 
  Renvoie SdifF->CurrMtrx->NbCol, nombre de colonnes de la matrice en
  cours de traitement.  */
SdifUInt4     SdifFCurrNbCol           (SdifFileT *SdifF);

/*DOC: 
  Renvoie SdifF->CurrMtrx->NbRow, nombre de lignes de la matrice en
  cours de traitement.  */
SdifUInt4     SdifFCurrNbRow           (SdifFileT *SdifF);

/*DOC: 
  Returns the data type of the current matrix. */
SdifDataTypeET SdifFCurrDataType (SdifFileT *SdifF);

/*DOC: 
  Renvoie SdifF->CurrFramH->NbMatrix, mombre de matrices du frame
  courant.  */
SdifUInt4     SdifFCurrNbMatrix        (SdifFileT *SdifF);

/*DOC: 
  Renvoie SdifF->CurrFramH->NumID, index de l'objet du frame courant.  */
SdifUInt4     SdifFCurrID              (SdifFileT *SdifF);

/*DOC: 
  Renvoie SdifF->CurrFramH->Time.  */
SdifFloat8    SdifFCurrTime            (SdifFileT *SdifF);



/*
// FUNCTION GROUP:	File Data Access Functions
*/

/*DOC:
  Return list of NVTs for querying. 
  [] precondition NVTs have been read with SdifFReadAllASCIIChunks. */
SdifNameValuesLT *SdifFNameValueList (SdifFileT *file);

/*DOC:
  Return number of NVTs present.
  [] precondition NVTs have been read with SdifFReadAllASCIIChunks. */
int SdifFNameValueNum (SdifFileT *file);

/*DOC:
  Return the file's stream ID table, created automatically by SdifFOpen. */
SdifStreamIDTableT *SdifFStreamIDTable (SdifFileT *file);

/*DOC:
  Add user data, return index added */
int SdifFAddUserData (SdifFileT *file, void *data);

/*DOC:
  Get user data by index */
void *SdifFGetUserData (SdifFileT *file, int index);



SdifFileT*    SdifFReInitMtrxUsed (SdifFileT *SdifF);
SdifFileT*    SdifFPutInMtrxUsed  (SdifFileT *SdifF, SdifSignature Sign);
SdifSignature SdifFIsInMtrxUsed   (SdifFileT *SdifF, SdifSignature Sign);



/*
// FUNCTION GROUP:	Error flag for file
*/

/*DOC: 
  Return pointer to last error struct or NULL if no error present
  for this file. */
SdifErrorT*     SdifFLastError    (SdifFileT *SdifF);

/*DOC: 
  Return tag of last error or eNoError if no error present for this file. */
SdifErrorTagET  SdifFLastErrorTag (SdifFileT *SdifF);


#define	MaxUserData	10


/*
// DATA GROUP:	SDIF File Structure
*/

/*DOC:
  File mode argument for SdifFOpen.
*/



#define _SdifFrameHeaderSize 16  /* (ID=4)+(size=4)+(time=8) */



SdifFrameHeaderT* SdifCreateFrameHeader(SdifSignature Signature,
					       SdifUInt4 Size,
					       SdifUInt4 NbMatrix,
					       SdifUInt4 NumID,
					       SdifFloat8 Time);

SdifFrameHeaderT* SdifCreateFrameHeaderEmpty(SdifSignature Signature);

void              SdifKillFrameHeader  (SdifFrameHeaderT *FrameHeader);

SdifFrameDataT* SdifCreateFrameData(SdifHashTableT *FrameTypesTable,
					   SdifSignature FrameSignature,
					   SdifUInt4 NumID,
					   SdifFloat8 Time);

void            SdifKillFrameData   (SdifHashTableT *FrameTypesTable, SdifFrameDataT *FrameData);

SdifFrameDataT* SdifFrameDataPutNthMatrixData(SdifFrameDataT *FrameData, unsigned int NthMatrix,
						     SdifMatrixDataT *MatrixData);

SdifFrameDataT* SdifFrameDataPutComponentMatrixData(SdifHashTableT *FrameTypesTable,
							   SdifFrameDataT *FrameData,
							   char *CompoName, SdifMatrixDataT *MatrixData);

SdifMatrixDataT* SdifFrameDataGetNthMatrixData(SdifFrameDataT *FrameData, unsigned int NthMatrix);

SdifMatrixDataT* SdifFrameDataGetComponentMatrixData(SdifHashTableT *FrameTypesTable,
							    SdifFrameDataT *FrameData,
						     char *CompoName);



SdifComponentT* SdifCreateComponent (SdifSignature MtrxS, char *Name, SdifUInt4 Num);
void            SdifKillComponent   (SdifComponentT *Component);
SdifFrameTypeT* SdifCreateFrameType (SdifSignature FramS, SdifFrameTypeT *PredefinedFrameType);

void            SdifKillFrameType               (SdifFrameTypeT *FrameType);
SdifComponentT* SdifFrameTypeGetComponent_MtrxS (SdifFrameTypeT *FrameType, SdifSignature MtrxS);
SdifComponentT* SdifFrameTypeGetComponent       (SdifFrameTypeT *FrameType, char *NameC);
SdifComponentT* SdifFrameTypeGetNthComponent    (SdifFrameTypeT *FrameType, SdifUInt4 NumC);
SdifFrameTypeT* SdifFrameTypePutComponent       (SdifFrameTypeT *FrameType, SdifSignature MtrxS, char *NameC);

SdifFrameTypeT* SdifGetFrameType       (SdifHashTableT *FrameTypeHT, SdifSignature FramS);
void            SdifPutFrameType       (SdifHashTableT *FrameTypeHT, SdifFrameTypeT *FrameType);
SdifUInt2       SdifExistUserFrameType (SdifHashTableT *FrameTypeHT);



/* set default if not overridden from makefile */
#ifndef _SdifFormatVersion
#define	_SdifFormatVersion 3
#endif

#define _SdifTypesVersion  1


/* _SdifEnvVar : Environnement variable which contains the name
 * of the file which contains predefined types (the name contains the path).
 * _SdifEnvVar is used in SdifFile.c SdifGenInit, the user can
 * reference predefined types by this envvar name.
 */
#define _SdifEnvVar "SDIFTYPES"

/* Default predefined types : _SdifTypesFileName see SdifFile.c
 */

#define _SdifListNodeStockSize 0x400 /*1024*/
#define _SdifGenHashSize 127
#define _SdifUnknownSize 0xffffffff
#define _SdifPadding 8
#define _SdifGranule 1024 /* for OneRow allocation in bytes */

#define _SdifFloat8Error  0xffffffff
#define _SdifNoTime	  _Sdif_MIN_DOUBLE_	/* for header ASCII frames */
#define _SdifNoStreamID   0xfffffffe		/* used for 1TYP */
#define _SdifAllStreamID  0xffffffff		/* used for 1IDS */
#define _SdifUnknownUInt4 0xffffffff

/* CNMAT restriction: only one frame type per stream.  
   Therefore we have to use unique IDs for all 'header' frames. */
#define _SdifNVTStreamID  0xfffffffd		/* used for 1NVT */
#define _SdifIDSStreamID  0xfffffffc		/* unused */
#define _SdifTYPStreamID  0xfffffffb		/* unused */


#define _SdifFloatEps  FLT_EPSILON

/* DataTypeEnum

   On Matt Wright's visit at IRCAM June 1999, we defined a new
   encoding for the MatrixDataType field with the feature that the low
   order byte encodes the number of bytes taken by each matrix
   element.  

   Low order byte encodes the number of bytes 
   High order bytes come from this (extensible) enum:

        0 : Float
        1 : Signed integer
        2 : Unsigned integer
        3 : Text (UTF-8 when 1 byte)
        4 : arbitrary/void
*/


#ifdef STDC_HEADERS  /* Is the compiler ANSI? */

/* generate template for all types */
#define sdif__foralltypes(macro, post)	macro(Float4)post \
					macro(Float8)post \
					macro(Int2  )post \
					macro(Int4  )post \
					macro(UInt2 )post \
					macro(UInt4 )post \
					macro(Char  )post \
				     /* macro(Int8  )post \
					macro(UInt8 )post \
				      */

#define sdif_foralltypes_post_body    /* this is empty */
#define sdif_foralltypes_post_proto ; /* this is a semicolon */


/* generate template for all types */
//old: #define sdif_foralltypes(macro)          sdif__foralltypes(macro,)
#define sdif_foralltypes(macro)         sdif__foralltypes(macro,sdif_foralltypes_post_body)

/* generate prototype template for all types */
#define sdif_proto_foralltypes(macro)   sdif__foralltypes(macro,sdif_foralltypes_post_proto)

#endif /* STDC_HEADERS */


#define _SdifStringLen 1024

extern char gSdifString[_SdifStringLen];
extern char gSdifString2[_SdifStringLen];
extern char gSdifErrorMess[_SdifStringLen];

#define _SdifNbMaxPrintSignature 8
extern char gSdifStringSignature[_SdifNbMaxPrintSignature][5];
extern int  CurrStringPosSignature;


/*
// FUNCTION GROUP:	utility functions
*/

/*DOC:
*/
char*     SdifSignatureToString(SdifSignature Signature);

/*DOC: 
  Compare two signatures, ignoring the first character which
  encodes the type version.  Note that comparison of full signatures
  can be done simply with '=='. 
*/
int     SdifSignatureCmpNoVersion(SdifSignature Signature1, SdifSignature Signature2);

/*DOC: 
  Returns size of SDIF data type in bytes
  (which is always the low-order byte).  
*/
SdifUInt4 SdifSizeofDataType (SdifDataTypeET DataType);

/*DOC: 
  Returns true if DataType is in the list of known data types.
*/
int SdifDataTypeKnown (SdifDataTypeET DataType);

/*DOC:
*/
size_t    SdifPaddingCalculate  (size_t NbBytes);

/*DOC:
*/
size_t    SdifFPaddingCalculate (FILE *f, size_t NbBytes);

/* (double f1) == (double f2) with _SdifFloatEps for error */
int SdifFloat8Equ(SdifFloat8 f1, SdifFloat8 f2);


#ifndef MIN
#define MIN(a,b)	((a) < (b)  ?  (a)  :  (b))
#endif

#ifndef MAX
#define MAX(a,b)	((a) > (b)  ?  (a)  :  (b))
#endif




/* _Sdif_MIN_DOUBLE_ tested on SGI, DEC alpha, PCWin95 as 0xffefffffffffffff
 * include may be limits.h (float.h is sure with VisualC++5 Win 95 or NT)
 */
#define _Sdif_MIN_DOUBLE_ (- DBL_MAX)


int       SdifStrLen  (const char *s);

/* returns 0 if strings are equal */
int       SdifStrCmp  (const char *s1, const char *s2);

/* returns true if strings are equal */
int	  SdifStrEq(const char *s1, const char *s2);
int       SdifStrNCmp (const char *s1, const char *s2, unsigned int n);
char*     SdifStrNCpy (char *s1, const char *s2, unsigned int n);
char*     SdifCreateStrNCpy (const char* Source, size_t Size);
void      SdifKillStr (char* String);


void     SdifSetStdIOBinary (void);
FILE*    SdiffBinOpen       (const char * Name, SdifBinaryModeET Mode);
SdifInt4 SdiffBinClose      (FILE *f);



SdifHashTableT* SdifCreateHashTable(unsigned int HashSize, SdifHashIndexTypeET IndexType, void (*Killer)());

void SdifMakeEmptyHashTable (SdifHashTableT* HTable);
void SdifKillHashTable      (SdifHashTableT* HTable);



/******************  eHashChar ****************/

unsigned int SdifHashChar(const char* s, unsigned int nchar, unsigned int HashSize);

void*           SdifHashTableSearchChar(SdifHashTableT* HTable, const char *s, unsigned int nchar);
SdifHashTableT* SdifHashTablePutChar   (SdifHashTableT* HTable, const char *s, unsigned int nchar, void* Data);


/***************** eHashInt4 **********************/

unsigned int SdifHashInt4(unsigned int i, unsigned int HashSize);

void*           SdifHashTableSearchInt4(SdifHashTableT* HTable, unsigned int i);
SdifHashTableT* SdifHashTablePutInt4   (SdifHashTableT* HTable, const unsigned int i, void* Data);


/*************************** for all ***********************/

void*           SdifHashTableSearch (SdifHashTableT* HTable, void *ptr, unsigned int nobj);
SdifHashTableT* SdifHashTablePut    (SdifHashTableT* HTable, const void *ptr, unsigned int nobj, void* Data);


#if 0	/* TBI */

/*
//FUNCTION GROUP: to be implemented / TBI
*/


/*DOC: 
  Write whole matrix, given as separate columns in array "columns" of
  pointer to "DataType".  Each columns [i], i = 0..NbCol-1, points to 
  NbRow * SdifSizeofDataType (DataType) bytes.  
  TBI 
*/
SdifFWriteMatrixColumns (SdifFileT     *file,
			 SdifSignature  Signature,
			 SdifDataTypeET DataType,
			 SdifUInt4      NbRow,
			 SdifUInt4      NbCol,
			 void	       *columns []);


SdifFWriteTextMatrix (SdifFileT f, SdifSignature matrixsig, const char *str)
{
    /* convert to UTF-8 */
    SdifConvertToUTF8 (str, utfstr);
    nbytes = strlen (utfstr);
    SdifFSetCurrMatrixHeader (f, matrixsig, eText, nbytes, 1);
    SdifFWriteMatrixHeader (f);

    /* write text data and padding */
}


/*DOC:
  Definition of the matrix callback function type, used for SdifReadSimple. 
  TBI 
*/
typedef int (*SdifMatrixCallbackT) (SdifFileT *file, 
				    int nummatrix, 
				    void *userdata);

/*DOC: 
  Reads an entire SDIF file, calling matrixfunc for each matrix in the
  SDIF selection taken from the filename.  Matrixfunc is called with
  the SDIF file pointer, the matrix count within the current frame,
  and the userdata unchanged. 
  TBI 
*/
SdifReadSimple (char		    *filename, 
		SdifMatrixCallbackT matrixfunc,
		void		    *userdata);


/* see SdifFRead */

SdifFReadMatrixAs_TYPE_ ();

#if 0
/* Alternative: extend SdifDataTypeET by */
  eTranslate = 0x1000, /* flag to be or'ed with data type: Translate on read */
  eTypeMask  = 0x0fff  /* mask to eliminate flags */
#endif


/*DOC: 
  Reads matrix header and data into memory allocated by the library,
  accessible by SdifFCurrMatrixData (). */
int SdifFReadMatrix (SdifFileT *file);

/* with type conversion */
int SdifFReadMatrixAs (SdifFileT *file, SdifDataTypeET as_type);

/*? text special: return allocated, decoded c-string, to be free'd by caller */
char *SdifFReadTextMatrix (SdifFileT *file);

/*DOC: 
  Reads matrix data into memory pointed to by target, which must point
  to at least nbrow * nbcol * size of datatype bytes of memory.  If
  target is NULL, the library will allocate enough space for the data
  of one matrix, accessible by SdifFCurrMatrixData ().

  [Precondition:] 
  Matrix header must have been read with SdifFReadMatrixHeader.  */
int SdifFReadMatrixData   (SdifFileT *file, void *target);

/* with type conversion */
int SdifFReadMatrixDataAs (SdifFileT *file, void *target,
			   SdifDataTypeET as_type);


/* --> SdifMatrix.h: add to SdifMatrixHeaderS not void *MatrixData, but:

   DataTypeUT Data;

   --> SdifFile.c: add void *SdifFCurrMatrixData (SdifFileT *);
*/

void *SdifFCurrMatrixData (SdifFileT *file)
{
  return file->CurrMtrxH->Data.Void;
}

void *SdifGetColumn ();



// Error handling


int /*bool*/ SdifFCheckStatus (SdifFileT *file)
{
  return (SdifLastError (file->ErrorList)) == NULL);
}


int /*bool*/ SdifFCheckStatusPrint (SdifFileT *file)
{
  SdifError err = SdifLastError (file->ErrorList));
  if (err != eNoError)
     print (SdifFsPrintFirstError (..., file, ...);
  return err == NULL;
}


/* --> test in SdifFReadGeneralHeader  (file) + SdifFReadAllASCIIChunks (file)
   if (!SdifFCheckStatus (file))
      SdifWarningAdd ("Followup error");
*/





#endif /* TBI */



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



SdifMatrixHeaderT* SdifCreateMatrixHeader    (SdifSignature Signature, 
					      SdifDataTypeET DataType,
					      SdifUInt4 NbRow, 
					      SdifUInt4 NbCol);

SdifMatrixHeaderT* SdifCreateMatrixHeaderEmpty (void);
void               SdifKillMatrixHeader        (SdifMatrixHeaderT *MatrixHeader);

SdifOneRowT*       SdifCreateOneRow          (SdifDataTypeET DataType, SdifUInt4  NbGranuleAlloc);
SdifOneRowT*       SdifReInitOneRow          (SdifOneRowT *OneRow, SdifDataTypeET DataType, SdifUInt4 NbData);
void               SdifKillOneRow            (SdifOneRowT *OneRow);
SdifOneRowT*       SdifOneRowPutValue        (SdifOneRowT *OneRow, SdifUInt4 numCol, SdifFloat8 Value);
SdifFloat8         SdifOneRowGetValue        (SdifOneRowT *OneRow, SdifUInt4 numCol);
SdifFloat8         SdifOneRowGetValueColName (SdifOneRowT *OneRow, SdifMatrixTypeT *MatrixType, char * NameCD);

SdifMatrixDataT*   SdifCreateMatrixData      (SdifSignature Signature, SdifDataTypeET DataType,
						     SdifUInt4 NbRow, SdifUInt4 NbCol);

void               SdifKillMatrixData        (SdifMatrixDataT *MatrixData);
SdifMatrixDataT*   SdifMatrixDataPutValue    (SdifMatrixDataT *MatrixData,
						     SdifUInt4  numRow, SdifUInt4  numCol, SdifFloat8 Value);

SdifFloat8         SdifMatrixDataGetValue    (SdifMatrixDataT *MatrixData,
						     SdifUInt4  numRow, SdifUInt4  numCol);




SdifColumnDefT*  SdifCreateColumnDef (char *Name,  unsigned int Num);
void             SdifKillColumnDef   (void *ColumnDef);

/*DOC: 
  premet de créer un objet 'type de matrice'. Le premier argument
  est la signature de ce type. Le second est l'objet 'type de matrice'
  prédéfini dans SDIF.<p>
  
  <strong>Important: Tous les types de matrices ou de frames utilisés
  dans une instance de SdifFileT doivent être ajoutés aux tables de
  cette instance, de façon a créer le lien avec les types
  prédéfinis.</strong> L'hors de la lecture des entêtes avec les
  fonctions SdifFReadMatrixHeader et SdifFReadFrameHeader, cette mise
  à jour se fait automatiquement à l'aide des fonctions
  SdifTestMatrixType et SdifTestFrameType. */
SdifMatrixTypeT* SdifCreateMatrixType              (SdifSignature Signature,
                    							   SdifMatrixTypeT *PredefinedMatrixType);
void             SdifKillMatrixType                (SdifMatrixTypeT *MatrixType);

/*DOC: 
  permet d'ajouter une colonne à un type (toujours la dernière
  colonne).  */
SdifMatrixTypeT* SdifMatrixTypeInsertTailColumnDef (SdifMatrixTypeT *MatrixType, char *NameCD);

/*DOC: 
  renvoie la position de la colonne de nom NameCD.  (0 si elle
  n'existe pas) */
SdifUInt4        SdifMatrixTypeGetNumColumnDef     (SdifMatrixTypeT *MatrixType, char *NameCD);

/*DOC: 
  renvoie la définition de la colonne (numéro, nom) en fonction
  du nom.(NULL si introuvable) */
SdifColumnDefT*  SdifMatrixTypeGetColumnDef        (SdifMatrixTypeT *MatrixType, char *NameCD);

/*DOC: 
  renvoie la définition de la colonne (numéro, nom) en fonction
  du numero.(NULL si introuvable) */
SdifColumnDefT*  SdifMatrixTypeGetNthColumnDef     (SdifMatrixTypeT *MatrixType, SdifUInt4 NumCD);


/*DOC: 
  renvoie le type de matrice en fonction de la Signature. Renvoie
  NULL si le type est introuvable. Attention, si Signature est la
  signature d'un type prédéfini,
  SdifGetMatrixType(SdifF->MatrixTypeTable,Signature) renvoie NULL si
  le lien avec entre SdifF et gSdifPredefinedType n'a pas été mis à
  jour.  */
SdifMatrixTypeT* SdifGetMatrixType		   (SdifHashTableT *MatrixTypesTable, 
						    SdifSignature Signature);

/*DOC: 
  permet d'ajouter un type de matrice dans une table.  */
void             SdifPutMatrixType(SdifHashTableT *MatrixTypesTable, SdifMatrixTypeT* MatrixType);
SdifUInt2        SdifExistUserMatrixType(SdifHashTableT *MatrixTypesTable);



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

#define _SdifNameValueHashSize 31


/*
 * NameValue
 */


SdifNameValueT* SdifCreateNameValue(const char *Name,  const char *Value);
void            SdifKillNameValue(SdifNameValueT *NameValue);




/*
 * NameValueTable
 */

SdifNameValueTableT* SdifCreateNameValueTable(  SdifUInt4 StreamID, 
                                                SdifUInt4 HashSize, 
						SdifUInt4 NumTable);
void            SdifKillNameValueTable          (void* NVTable);
SdifNameValueT* SdifNameValueTableGetNV         (SdifNameValueTableT* NVTable, const char *Name);
SdifNameValueT* SdifNameValueTablePutNV         (SdifNameValueTableT* NVTable, const char *Name,  const char *Value);
SdifFloat8      SdifNameValueTableGetTime       (SdifNameValueTableT* NVTable);
SdifUInt4       SdifNameValueTableGetNumTable   (SdifNameValueTableT* NVTable);
SdifUInt4       SdifNameValueTableGetStreamID  (SdifNameValueTableT* NVTable);



/*
 * NameValueTableList
 */

SdifNameValuesLT*   SdifCreateNameValuesL       (SdifUInt4  HashSize);
void                SdifKillNameValuesL         (SdifNameValuesLT *NameValuesL);

/*DOC: 
  Cette fonction permet d'ajouter une nouvelle NVT dans la liste
  de tables passée par argument:
  <code>SdifNameValuesLNewHT(SdifF->NamefValues);</code><br>
  Attention, à l'ouverture de SdifF, il n'y a aucune table dans
  SdifF->NamefValues. Il faudra donc au moins en ajouter une pour
  pouvoir y mettre des NameValue.  */
SdifNameValuesLT*   SdifNameValuesLNewTable     (SdifNameValuesLT *NameValuesL, SdifUInt4 StreamID);

/*DOC: 
  Cette fonction permet de définir la nième NVT de la liste des
  tables comme NVT courante.  */
SdifNameValueTableT*SdifNameValuesLSetCurrNVT   (SdifNameValuesLT *NameValuesL, SdifUInt4 NumCurrNVT);

/*DOC: 
  Cette fonction permet de récupérer une Name-Value de la liste
  des NVTs en passant le Name en argument.  Dans le cas ou Name est
  référencé dans plusieurs NVT, alors c'est la première NVT le
  contenant qui sera prise en compte.  Le pointeur retourné est de
  type SdifNameValueT qui contient deux champs: Name et Value.  */
SdifNameValueT*     SdifNameValuesLGet          (SdifNameValuesLT *NameValuesL, char *Name);

/*DOC: 
  Cette fonction réalise aussi une requête en fonction de Name
  mais uniquement dans la NVT courante.  */
SdifNameValueT*     SdifNameValuesLGetCurrNVT   (SdifNameValuesLT *NameValuesL, const char *Name);

/*DOC: 
  Cette fonction permet d'ajouter une NameValue à table courante
  qui est la dernière table créée ou celle définie en tant que table
  courante. Name et Value doivent être des chaines caractères ASCII
  sans espacements.  */
SdifNameValueT*     SdifNameValuesLPutCurrNVT   (SdifNameValuesLT *NameValuesL, const char *Name,  const char *Value);
SdifUInt2           SdifNameValuesLIsNotEmpty   (SdifNameValuesLT *NameValuesL);


/*
 * Obsolete
 */
SdifNameValuesLT*   SdifNameValuesLNewHT    (SdifNameValuesLT *NameValuesL);
SdifHashTableT*     SdifNameValuesLSetCurrHT(SdifNameValuesLT *NameValuesL, SdifUInt4 NumCurrHT);
SdifNameValueT*     SdifNameValuesLGetCurrHT(SdifNameValuesLT *NameValuesL, char *Name);
SdifNameValueT*     SdifNameValuesLPutCurrHT(SdifNameValuesLT *NameValuesL, const char *Name,  const char *Value);





#define   M_1FQ0_Frequency  "Frequency"
#define   M_1FQ0_Mode       "Mode"
#define   M_1FQ0_Hit        "Hit"

#define   M_1FOF_Frequency  "Frequency"
#define   M_1FOF_Amplitude  "Amplitude"
#define   M_1FOF_BandWidth  "BandWidth"
#define   M_1FOF_Tex        "Tex"
#define   M_1FOF_DebAtt     "DebAtt"
#define   M_1FOF_Atten      "Atten"
#define   M_1FOF_Phase      "Phase"

#define   M_1CHA_Channel1   "Channel1"
#define   M_1CHA_Channel2   "Channel2"
#define   M_1CHA_Channel3   "Channel3"
#define   M_1CHA_Channel4   "Channel4"

#define   M_1RES_Frequency  "Frequency"
#define   M_1RES_Amplitude  "Amplitude"
#define   M_1RES_BandWidth  "BandWidth"
#define   M_1RES_Saliance   "Saliance"
#define   M_1RES_Correction "Correction"

#define   M_1DIS_Distribution    "Distribution"
#define   M_1DIS_Amplitude  "Amplitude"

SdifFrameTypeT* CreateF_1FOB(void);
SdifFrameTypeT* CreateF_1REB(void);
SdifFrameTypeT* CreateF_1NOI(void);
void SdifCreatePredefinedTypes(SdifHashTableT *MatrixTypesHT,
                                      SdifHashTableT *FrameTypesHT);





/*************** Matrix Type ***************/

void SdifPrintMatrixType(FILE *fw, SdifMatrixTypeT *MatrixType);
void SdifPrintAllMatrixType(FILE *fw, SdifFileT *SdifF);

/*************** Frame Type ***************/

void SdifPrintFrameType(FILE *fw, SdifFrameTypeT *FrameType);
void SdifPrintAllFrameType(FILE *fw, SdifFileT *SdifF);

/********** Matrix **********/

void SdifPrintMatrixHeader(FILE *f, SdifMatrixHeaderT *MatrixHeader);
void SdifPrintOneRow(FILE *f, SdifOneRowT *OneRow);
void SdifPrintMatrixRows(FILE* f, SdifMatrixDataT *MatrixData);

/********** Frame ***********/

void SdifPrintFrameHeader(FILE *f, SdifFrameHeaderT* FrameHeader);

/************ High ***********/

void SdifPrintAllType(FILE *fw, SdifFileT *SdifF);



#define _SdifBSLittleE 4096

#define _SdifPaddingChar  '\0'
#define _SdifReservedChars  ",;{}:"


size_t Sdiffread  (void *ptr, size_t size, size_t nobj, FILE *stream);
size_t Sdiffwrite (void *ptr, size_t size, size_t nobj, FILE *stream);

/* Read, return the number of objects */

size_t SdiffReadChar   (SdifChar   *ptr, size_t nobj, FILE *stream);
size_t SdiffReadInt2   (SdifInt2   *ptr, size_t nobj, FILE *stream);
size_t SdiffReadUInt2  (SdifUInt2  *ptr, size_t nobj, FILE *stream);
size_t SdiffReadInt4   (SdifInt4   *ptr, size_t nobj, FILE *stream);
size_t SdiffReadUInt4  (SdifUInt4  *ptr, size_t nobj, FILE *stream);
size_t SdiffReadFloat4 (SdifFloat4 *ptr, size_t nobj, FILE *stream);
size_t SdiffReadFloat8 (SdifFloat8 *ptr, size_t nobj, FILE *stream);

/*size_t SdiffReadSignature (SdifSignature *Signature, FILE *stream);*/



/* Write, return the number of objects */

size_t SdiffWriteChar   (SdifChar   *ptr, size_t nobj, FILE *stream);
size_t SdiffWriteInt2   (SdifInt2   *ptr, size_t nobj, FILE *stream);
size_t SdiffWriteUInt2  (SdifUInt2  *ptr, size_t nobj, FILE *stream);
size_t SdiffWriteInt4   (SdifInt4   *ptr, size_t nobj, FILE *stream);
size_t SdiffWriteUInt4  (SdifUInt4  *ptr, size_t nobj, FILE *stream);
size_t SdiffWriteFloat4 (SdifFloat4 *ptr, size_t nobj, FILE *stream);
size_t SdiffWriteFloat8 (SdifFloat8 *ptr, size_t nobj, FILE *stream);

size_t SdiffWriteSignature (SdifSignature *Signature, FILE *stream);
size_t SdiffWriteString (char* ptr, FILE *stream);

/*
 *size_t SdiffReadUInt8  (SdifUInt8  *ptr, size_t nobj, FILE *stream);
 *size_t SdiffWriteUInt8  (SdifUInt8  *ptr, size_t nobj, FILE *stream);
 */

/**Ascii**/
/* fGet --> return the last char
 */
size_t SdiffReadSpace   (FILE* fr);

size_t SdiffReadSpacefromSdifString(SdifStringT *SdifString);

/*DOC:
  Return c if it is a reserved char, -1 otherwise.
*/
int SdifIsAReservedChar (char c);

/*DOC: 
  Convert str <strong>in place</strong> so that it doesn't
  contain any reserved chars (these become '.') or spaces (these
  become '_').

  [] returns str
*/
char *SdifStringToNV (/*in out*/ char *str);

/* SdiffGetString lit un fichier jusqu'a un caractere reserve, ne
   rempli s que des caracteres non-espacement, renvoie le caractere
   reserve, saute les premiers caracteres espacement lus.  Il y a
   erreur si fin de fichier ou si un caractere non-espacement et
   non-reseve est lu apres un caractere espacement.  ncMax est
   typiquement strlen(s)+1.  
*/
int SdiffGetString      (FILE* fr, char* s, size_t ncMax, size_t *NbCharRead);

/* retourne le caractere d'erreur */
int SdiffGetSignature   (FILE* fr, SdifSignature *Signature, size_t *NbCharRead);
/*DOC:
  Function return the signature in a SdifStringT
*/
int SdiffGetSignaturefromSdifString(SdifStringT *SdifString, SdifSignature *Signature);

int SdiffGetWordUntil   (FILE* fr, char* s, size_t ncMax, size_t *NbCharRead, char *CharsEnd);
/*DOC:
  Function return the word until in a SdifStringT
*/
int SdiffGetWordUntilfromSdifString(SdifStringT *SdifString, char* s, size_t ncMax,char *CharsEnd);

int SdiffGetStringUntil (FILE* fr, char* s, size_t ncMax, size_t *NbCharRead, char *CharsEnd);
/*DOC:
  Function return the string until in a SdifStringT
 */
int SdiffGetStringUntilfromSdifString(SdifStringT *SdifString, char *s, size_t ncMax,
				      char *CharsEnd);

int SdiffGetStringWeakUntil(FILE* fr, char* s, size_t ncMax, size_t *NbCharRead, char *CharsEnd);
/*DOC:
  Return the weak string until in a SdifStringT
*/
int SdiffGetStringWeakUntilfromSdifString(SdifStringT *SdifString, char* s,
					  size_t ncMax, char *CharsEnd);

int SdifSkipASCIIUntil  (FILE* fr, size_t *NbCharRead, char *CharsEnd);


#if 0	/* for cocoon's eyes only */
/* scan nobj items of TYPE from stream, return number sucessfully read */
size_t SdiffScan_TYPE   (FILE *stream, Sdif_TYPE  *ptr, size_t nobj);
size_t SdiffScanFloat4  (FILE *stream, SdifFloat4 *ptr, size_t nobj);
size_t SdiffScanFloat8  (FILE *stream, SdifFloat8 *ptr, size_t nobj);
#endif


#ifdef STDC_HEADERS  /* Is the compiler ANSI? */

#define sdif_scanproto(type) \
size_t SdiffScan##type (FILE *stream, Sdif##type *ptr, size_t nobj)

sdif_proto_foralltypes (sdif_scanproto)

#endif /* STDC_HEADERS */


/* Unsafe but optimized version of SdifStringToSignature:
   Exactly 4 chars are considered, so make sure *str has at least that many! 
   The str pointer MUST be word (at least 4 byte or so) aligned.
*/
SdifSignature _SdifStringToSignature (char *str);

/*DOC:
  Convert a string to an SDIF signature (in proper endianness).
  str can point to any string position of any length.  
*/
SdifSignature SdifStringToSignature (char *str);






/*DOC:
  Return pointer to start of filename component in path inPathFileName.
 */
char *SdifBaseName (const char* inPathFileName);


/* 
// FUNCTION GROUP:	Init/Deinit
 */

/* init module, called by SdifGenInit */
int SdifInitSelect (void);

/*DOC: 
  Allocate space for an sdif selection.
*/
SdifSelectionT *SdifCreateSelection (void);

/*DOC: 
*/
int SdifInitSelection (SdifSelectionT *sel, const char *filename, int namelen);

/*DOC: 
*/
int SdifFreeSelection (SdifSelectionT *sel);

/*
// FUNCTION GROUP:	Parse and Set Selection
*/


/*DOC: 
  Returns pointer to first char of select spec (starting with ::), 
  or NULL if not found.
*/
char *SdifSelectFindSelection (const char *filename);


/*DOC: 
*/
char *SdifGetFilenameAndSelection (/*in*/  const char *filename, 
				   /*out*/ SdifSelectionT *sel);

/*DOC: 
*/
void SdifPrintSelection (FILE *out, SdifSelectionT *sel, int options);



/*
// FUNCTION GROUP:	Add Selections to Element Lists
*/

/* Give documentation and fake prototype for _add... macro generated functions.
   Cocoon ignores the #if 0.
*/
#if 0

/*DOC:
  Create and add one value to selection element list.  There are four 
  functions generated automatically, with the meta type-variables _type_ and 
  _datatype_:
  [] _type_ is one of:  <br> Int, Real,   Signature,     String, for
  [] _datatype_ of:	<br> int, double, SdifSignature, char *, respectively.
*/
void SdifSelectAdd_TYPE_ (SdifListT *list, _datatype_ value);

/*DOC:
  Create and add one range to selection element list.  There are four 
  functions generated automatically, with the meta type-variables _type_ and 
  _datatype_:
  [] _type_ is one of:  <br> Int, Real,   Signature,     String, for
  [] _datatype_ of:	<br> int, double, SdifSignature, char *, respectively.
*/
void SdifSelectAdd_TYPE_Range (SdifListT *list, 
			       _datatype_ value, 
			       SdifSelectTokens rt, 
			       _datatype_ range);

#endif	/* if 0 */


#ifdef STDC_HEADERS

#define _addrangeproto(name, type, field) \
void SdifSelectAdd##name##Range (SdifListT *list, \
				 type value, SdifSelectTokens rt, type range)

#define _addsimpleproto(name, type, field) \
void SdifSelectAdd##name (SdifListT *list, type value)

#define _addproto(name, type, field) \
_addsimpleproto (name, type, field); \
_addrangeproto  (name, type, field);

_addproto (Int,	      int,		integer)
_addproto (Real,      double,		real)
_addproto (Signature, SdifSignature,	signature)
_addproto (String,    char *,		string)

#else

void SdifSelectAddInt (SdifListT *list, int value) ; 
void SdifSelectAddIntRange (SdifListT *list, int value, SdifSelectTokens rt, int range) ; 
void SdifSelectAddReal (SdifListT *list, double value) ; 
void SdifSelectAddRealRange (SdifListT *list,                                   double value, SdifSelectTokens rt, double range) ; 
void SdifSelectAddSignature (SdifListT *list, SdifSignature value) ; 
void SdifSelectAddSignatureRange (SdifListT *list, SdifSignature value, SdifSelectTokens rt,   SdifSignature range) ; 
void SdifSelectAddString (SdifListT *list, char * value) ; 
void SdifSelectAddStringRange (SdifListT *list, char * value, SdifSelectTokens rt,      char * range) ; 

#endif /* STDC_HEADERS */


/*
// FUNCTION GROUP:	Query parsed ranges (list of ranges).
*/

/*DOC:
  Query parsed ranges (list of ranges) for a selection element (one of
  the SdifListP lists in SdifSelectionT).  Init list traversal with
  SdifListInitLoop, then call SdifSelectGetNext<type>(list) until it
  returns 0.

  The number of selections in the list is SdifListGetNbData(list), if
  it is 0, or SdifListIsEmpty(list) is true, then there was no
  selection for that element.

  If force_range is 1, the out value is converted to a range in any
  case, with value <= range guaranteed.  
*/
int SdifSelectGetNextIntRange  (/*in*/  SdifListP list, 
				/*out*/ SdifSelectElementIntT  *range, 
				/*in*/  int force_range);

/*DOC: 
  See SdifSelectGetNextInt.
*/
int SdifSelectGetNextRealRange (/*in*/  SdifListP list, 
				/*out*/ SdifSelectElementRealT *range, 
				/*in*/  int force_range);

/*DOC: 
  Query list of parsed selection elements (one of the SdifListP
  lists in SdifSelectionT).  Init list traversal with
  SdifListInitLoop, then call SdifSelectGetNext<type>(list) until it
  returns 0.

  See also SdifSelectGetNextInt.  
*/
SdifSignature  SdifSelectGetNextSignature (/*in*/  SdifListP list);

/*DOC: 
  See SdifSelectGetNextSignature.
*/
char	      *SdifSelectGetNextString    (/*in*/  SdifListP list);


/*DOC: 
  Return value of first selection (ignoring range).
*/
int	       SdifSelectGetFirstInt       (SdifListP l, int defval);
double	       SdifSelectGetFirstReal      (SdifListP l, double defval);
char	      *SdifSelectGetFirstString    (SdifListP l, char *defval);
SdifSignature  SdifSelectGetFirstSignature (SdifListP l, SdifSignature defval);





/*
// FUNCTION GROUP:	Selection Testing Functions
*/

int SdifSelectTestIntRange  (SdifSelectElementT *elem, int cand);
int SdifSelectTestRealRange (SdifSelectElementT *elem, double cand);

int SdifSelectTestInt (SdifListT *list, int cand);
int SdifSelectTestReal (SdifListT *list, double cand);
int SdifSelectTestSignature (SdifListT *list, const SdifSignature cand);
int SdifSelectTestString (SdifListT *list, const char *cand);



/*
// FUNCTION GROUP:	Using a Selection in File I/O.
*/




/*DOC: 
  Test the selection elements from sel applicable to frame FramH:
  time, stream, frame type. */
int SdifFrameIsSelected (SdifFrameHeaderT *FramH, SdifSelectionT *sel);

/*DOC:
  Test the selection elements from sel applicable to matrix MtrxH: 
  the matrix signature. */
int SdifMatrixIsSelected (SdifMatrixHeaderT *MtrxH, SdifSelectionT *sel);


/*DOC: 
  Test if the current frame header is in the file selection
  (automatically parsed from the filename).  
  Can be called after SdifFReadFrameHeader(). */
int SdifFCurrFrameIsSelected (SdifFileT *file);

/*DOC:
  Test if the current matrix header is in the file selection
  (automatically parsed from the filename).  
  Can be called after SdifFReadMatrixHeader(). */
int SdifFCurrMatrixIsSelected (SdifFileT *file);




/* Growth steps for reallocation */
#define _SdifSignatureTabGranule 16

/*
// FUNCTION GROUP:	Handling of a Table of Signatures
*/

/*DOC:
  Create table for initially NbSignMax signatures. */
SdifSignatureTabT* SdifCreateSignatureTab (const SdifUInt4 NbSignMax);

/*DOC:
  Free signature table. */
void		   SdifKillSignatureTab   (SdifSignatureTabT *SignTab);

/*DOC:
  Reallocate table to hold NewNbSignMax signatures. */
SdifSignatureTabT* SdifReAllocSignatureTab(SdifSignatureTabT *SignTab, 
					   const SdifUInt4 NewNbSignMax);

/*DOC:
  Reallocate table to hold NewNbSignMax signatures and clear signatures. */
SdifSignatureTabT* SdifReInitSignatureTab (SdifSignatureTabT *SignTab, 
					   const SdifUInt4 NewNbSignMax);

/*DOC:
  Add signature Sign, no overflow check. */
SdifSignatureTabT* SdifPutInSignatureTab  (SdifSignatureTabT *SignTab, 
					   const SdifSignature Sign);

/*DOC:
  Add signature Sign, reallocate table if necessary. */
SdifSignatureTabT* SdifAddToSignatureTab  (SdifSignatureTabT *SignTab, 
					   const SdifSignature Sign);

/*DOC:
  Get signature at position index.  
  Returns eEmptySignature if index out of bounds. */
SdifSignature      SdifGetFromSignatureTab(const SdifSignatureTabT* SignTab, 
					   const int index);

/*DOC:
  Test if signature Sign is in table SignTab. 
  [] Returns Sign if yes, 0 (== eEmptySignature) if no. */
SdifSignature      SdifIsInSignatureTab   (const SdifSignatureTabT *SignTab, 
					   const SdifSignature Sign);

/*DOC:
  Test if signature Sign is in table SignTab. 
  [] Returns index of Sign if yes, -1 if no. */
int		   SdifFindInSignatureTab (const SdifSignatureTabT* SignTab, 
					   const SdifSignature Sign);






/*
// DATA GROUP:		Stream ID Table and Entries for 1IDS ASCII chunk
*/


SdifStreamIDT* SdifCreateStreamID(SdifUInt4 NumID, char *Source, char *TreeWay);
void           SdifKillStreamID(SdifStreamIDT *StreamID);


/*DOC:
  Create a stream ID table.  <strong>The stream ID table of the SDIF
  file structure is created automatically by SdifFOpen().</strong> 
  It can be obtained by SdifFStreamIDTable(). */
SdifStreamIDTableT* SdifCreateStreamIDTable     (SdifUInt4 HashSize);

/*DOC:
  Deallocate a stream ID table.  <strong>The stream ID table of the SDIF
  file structure is killed automatically by SdifFClose.</strong>  
  It can be obtained by SdifFStreamIDTable. */
void                SdifKillStreamIDTable       (SdifStreamIDTableT *SIDTable);

/*DOC:
  Add an entry to a stream ID table.  The table will be written by
  SdifFWriteAllASCIIChunks.
  [in]  SIDTable pointer to stream ID table, e.g. obtained by SdifFStreamIDTable
  [in]	NumID	stream ID of the frames the stream ID table describes
  [in]	Source	Source identifier for the table (ex. "Chant")
  [in]	TreeWay	Routing and parameters, separated by slashes
  [return]
		The stream ID table entry just created and added */
SdifStreamIDT*      SdifStreamIDTablePutSID     (SdifStreamIDTableT *SIDTable,
						 SdifUInt4	     NumID, 
						 char 		    *Source, 
						 char 		    *TreeWay);

/*DOC:
  Retrieve an entry to a stream ID table.  The table has to have been
  read by SdifFReadAllASCIIChunks.

  [in]  SIDTable pointer to stream ID table, e.g. obtained by 
		 SdifFStreamIDTable
  [in]	NumID	 stream ID of the frames the stream ID table describes
  [return]
		 pointer to stream ID table entry, or NULL if no entry for 
		 stream ID NumID exists. */
SdifStreamIDT*      SdifStreamIDTableGetSID     (SdifStreamIDTableT *SIDTable, 
						 SdifUInt4	     NumID);

/*DOC:
  Return number of entries in stream ID table SIDTable */
SdifUInt4           SdifStreamIDTableGetNbData  (SdifStreamIDTableT *SIDTable);


/*DOC:
  Return stream ID field in stream ID table entry SID */
SdifUInt4	    SdifStreamIDEntryGetSID	(SdifStreamIDT *SID);

/*DOC:
  Return source field in stream ID table entry SID */
char		   *SdifStreamIDEntryGetSource	(SdifStreamIDT *SID);

/*DOC:
  Return "treeway" field in stream ID table entry SID */
char		   *SdifStreamIDEntryGetTreeWay	(SdifStreamIDT *SID);




#define _SdifStringGranule 128 /* Default memory size allocated for string */


/* Function declaration */

/*DOC:
  Make a memory allocation for a SdifStringT structure.
  The size for the string is defined in SdifString.h; define _SdifStringGranule 128.
*/
SdifStringT * SdifStringNew(void);


/*DOC:
  Free memory allocated for SdifString.
*/
void SdifStringFree(SdifStringT * SdifString);


/*DOC:
  Append a string to another one.
  Manage memory reallocation.
  Return a boolean for the succes of the function's call.
*/
int SdifStringAppend(SdifStringT * SdifString ,char *strToAppend);


/*DOC:
  Read the current char (= fgetc).
*/
int SdifStringGetC(SdifStringT * SdifString);


/*DOC:
  Equivalent of ungetc
*/
int SdifStringUngetC(SdifStringT * SdifString);


/*DOC:
  Test the end of the string (= feof)
*/
int SdifStringIsEOS(SdifStringT *SdifString);



/*
typedef enum SdifInterpretationErrorE
{
  eTypeDataNotSupported= 300,
  eNameLength,
  eReDefined,
  eUnDefined,
  eSyntax,
  eRecursiveDetect,
  eBadTypesFile,
  eBadType,
  eBadHeader,
  eOnlyOneChunkOf,
  eUnInterpreted,
  eUserDefInFileYet,
  eBadMode,
  eBadStdFile,
  eBadNbData,
  eReadWriteOnSameFile
} SdifInterpretationErrorET;



void
SdifInterpretationError(SdifInterpretationErrorET Error, SdifFileT* SdifF, const void *ErrorMess);

#define _SdifFileMess(sdiff, error, mess) \
(SdifErrorFile = __FILE__, SdifErrorLine = __LINE__, SdifInterpretationError((error), (sdiff),(mess)))

*/

#define _SdifFileMess(sdiff, error, mess) 

/*DOC: 
  Cette fonction vérifie si le type de matrice est répertorié
  dans SdifF.<br> S'il ne l'est pas, alors elle vérifie si c'est un
  type prédéfinis. S'il est prédéfini, elle crée le lien de SdifF vers
  le type prédéfini. Sinon, elle envoie un message sur l'erreur
  standart.  */
SdifMatrixTypeT* SdifTestMatrixType (SdifFileT *SdifF, SdifSignature Signature);
SdifFrameTypeT*  SdifTestFrameType  (SdifFileT *SdifF, SdifSignature Signature);



int SdifFTestMatrixWithFrameHeader (SdifFileT* SdifF);
int SdifFTestDataType              (SdifFileT* SdifF);
int SdifFTestNbColumns             (SdifFileT* SdifF);
int SdifFTestNotEmptyMatrix        (SdifFileT* SdifF);
int SdifFTestMatrixHeader          (SdifFileT* SdifF);



SdifColumnDefT*  SdifTestColumnDef (SdifFileT *SdifF, SdifMatrixTypeT *MtrxT, char *NameCD);
SdifComponentT*  SdifTestComponent (SdifFileT* SdifF, SdifFrameTypeT *FramT, char *NameCD);

int SdifTestSignature            (SdifFileT *SdifF, int CharEnd, SdifSignature Signature, char *Mess);
int SdifTestCharEnd              (SdifFileT *SdifF, int CharEnd, char MustBe,
					   char *StringRead, int ErrCondition, char *Mess);


int SdifTestMatrixTypeModifMode  (SdifFileT *SdifF, SdifMatrixTypeT *MatrixType);
int SdifTestFrameTypeModifMode   (SdifFileT *SdifF, SdifFrameTypeT *FrameType);



size_t SdifFTextConvMatrixData     (SdifFileT *SdifF);
size_t SdifFTextConvMatrix         (SdifFileT *SdifF);
size_t SdifFTextConvFrameData      (SdifFileT *SdifF);
size_t SdifFTextConvFrameHeader    (SdifFileT *SdifF);
size_t SdifFTextConvFrame          (SdifFileT *SdifF);
size_t SdifFTextConvAllFrame       (SdifFileT *SdifF);
size_t SdifFTextConvFramesChunk    (SdifFileT *SdifF);
size_t SdifFTextConv               (SdifFileT *SdifF);

/* upper level : open the text in read mode */

/*DOC: 
  Converti un fichier SDIF ouvert en lecture (eReadFile) en un fichier
  texte pseudo-SDIF de nom TextStreamName.  */
size_t SdifTextToSdif (SdifFileT *SdifF, char *TextStreamName);



SdifTimePositionT* SdifCreateTimePosition(SdifFloat8 Time, SdiffPosT Position);
void               SdifKillTimePosition(void* TimePosition);


SdifTimePositionLT* SdifCreateTimePositionL(void);
void                SdifKillTimePositionL  (SdifTimePositionLT *TimePositionL);

SdifTimePositionLT* SdifTimePositionLPutTail(SdifTimePositionLT* TimePositionL,
                                             SdifFloat8 Time, SdiffPosT Position);
SdifTimePositionT*  SdifTimePositionLGetTail(SdifTimePositionLT* TimePositionL);


/* SdifFPrint */

size_t SdifFPrintGeneralHeader      (SdifFileT *SdifF);
size_t SdifFPrintNameValueLCurrNVT  (SdifFileT *SdifF);
size_t SdifFPrintAllNameValueNVT    (SdifFileT *SdifF);
size_t SdifFPrintAllType            (SdifFileT *SdifF);
size_t SdifFPrintAllStreamID        (SdifFileT *SdifF);
size_t SdifFPrintAllASCIIChunks     (SdifFileT *SdifF);
size_t SdifFPrintMatrixHeader       (SdifFileT *SdifF);
size_t SdifFPrintFrameHeader        (SdifFileT *SdifF);
size_t SdifFPrintOneRow             (SdifFileT *SdifF);

size_t SdifFPrintMatrixType         (SdifFileT *SdifF, SdifMatrixTypeT *MatrixType);
size_t SdifFPrintFrameType          (SdifFileT *SdifF, SdifFrameTypeT  *FrameType);



#ifdef __cplusplus
}
#endif

#endif /* _SDIF_H */
