/* $Id: sdiftypes.h,v 1.1.2.1 2000-08-21 13:07:41 tisseran Exp $
 *
 * This file contains type declaration of variables used in SDIF library.
 *
 * $Log: not supported by cvs2svn $
 * $Date: 2000-08-21 13:07:41 $
 *
 */


#ifndef _SDIFTYPES_H
#define _SDIFTYPES_H 1

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

/* SdifHard_OS.h */

typedef char           SdifChar;
typedef short          SdifInt2;
typedef unsigned short SdifUInt2;
typedef int            SdifInt4;
typedef unsigned int   SdifUInt4;
typedef float          SdifFloat4;
typedef double         SdifFloat8;
typedef unsigned int   SdifSignature;

#   define SdiffPosT		fpos_t
typedef enum SdifMachineE
{
  eUndefinedMachine,
  eBigEndian,
  eLittleEndian,
  eLittleEndianLittleConst,
  eBigEndian64,
  eLittleEndian64,
  eLittleEndianLittleConst64,
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
#   define SdifSignatureConst(s) (s)
typedef enum SdifSignatureE
{
  eSDIF = SdifSignatureConst('SDIF'), /* SDIF header */
  e1NVT = SdifSignatureConst('1NVT'), /* Name Value Table */
  e1TYP = SdifSignatureConst('1TYP'), /* TYPe declarations */
  e1MTD = SdifSignatureConst('1MTD'), /* Matrix Type Declaration */
  e1FTD = SdifSignatureConst('1FTD'), /* Frame Type Declaration */
  e1IDS = SdifSignatureConst('1IDS'), /* ID Stream Table */
  eSDFC = SdifSignatureConst('SDFC'), /* Start Data Frame Chunk (text files) */
  eENDC = SdifSignatureConst('ENDC'), /* END Chunk (text files) */
  eENDF = SdifSignatureConst('ENDF'), /* END File (text files) */
  eFORM = SdifSignatureConst('FORM'), /* FORM for IFF compatibility (obsolete ?) */
  eEmptySignature = SdifSignatureConst('\0\0\0\0')
} SdifSignatureET;

typedef enum SdifModifModeE
{
  eNoModif,
  eCanModif
} SdifModifModeET;


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
  eFloat4b  = 0x0010,	/* = 32 */    /* IRCAM versions < 3 of SDIF */
  eFloat8a  = 0x0002,	/* =  2 */    /* IN TEXT MODE ONLY! */
  eFloat8b  = 0x0020	/* = 64 */
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
typedef enum { sst_specsep, sst_stream, sst_frame, sst_matrix, sst_column, 
	       sst_row,     sst_time,   sst_list,  sst_range,  sst_delta,
	       sst_num,	/* number of tokens */	   sst_norange = 0
} SdifSelectTokens;

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

typedef union SdifSelectValueS 
{
    int            integer;
    double         real;
    char	   *string;
    SdifSignature  signature;
} SdifSelectValueT;

typedef struct SdifSelectElementS
{
    SdifSelectValueT value;
    SdifSelectValueT range;
    SdifSelectTokens rangetype; /* 0 for not present, sst_range, sst_delta */
} SdifSelectElementT, *SdifSelectElementP;

typedef struct
{
    char	*filename,	/* allocated / freed by 
				   SdifInitSelection / SdifFreeSelection */
		*basename;	/* points into filename */
    SdifListP	stream, frame, matrix, column, row, time;
} SdifSelectionT;

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


typedef enum SdifErrorLevelE
{
	eFatal,
	eError,
	eWarning,
	eRemark,
	eNoLevel
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
  

#define	MaxUserData	10
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

  unsigned int  NbOfWarning;
  SdifErrorLT  *Errors;

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


#endif /* _SDIFTYPES_H */
