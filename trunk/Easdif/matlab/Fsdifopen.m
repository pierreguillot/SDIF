% 
%  [filehandle, head, dir] = Fsdifopen(Filename, OpenMode, HeadToWrite)
%
%       Open SDIF File for reading or writing
%
% INPUT :
% 
% filename   : a string determining the file to open including a sdif
%              selection, in case the file is opened for reading.
%
% OpenMode   : indicates file mode, a single char argument being 'r' or
%              'R' for reading and 'w' or 'W' for writing.
%              If not specified OpenMode defaults to 'r'
%
% HeadToWrite: In case the file is opened for writing this struct matrix
%              contains the ASCII headers as returned in the head output
%              argument.
%
% OUTPUT 
%
% filehandle : uint32 file handle used in subsequent calls for
%              reading/writing/closing the file
%
% head       : If present this matrix struct will hold the complete
%              information in the SDIF ASCII FileHeader (NameValue Tables
%              in field NVT, File-Local Type extensions in field TYP and
%              IDS string in field IDS)
%
%   head.NVT : contains a Nx2 cell array of Name and Value pairs of the
%     Name Value Table of the file. If different Name Value Tables are
%     stored in the file, these tables are separated by a single row of
%     empty matrices.
%
%   head.TYP : Contains the file extensions of the file.
%     The local type declarations will be stored in a type struct having MTD   
%     and FTD sub-structs :
% 
%     Matrix types are declared in the MTD struct
%  
%                MTD.sig  = 1x4 double Matrix holding the SdifSignature (4 char sequence)
%                           of the matrix type
%                MTD.name = nx1 or 1xn cell array of strings each of
%                           which determines a colmun name of the matrix
%  
%     Frame types are declared in the FTD struct
%  
%                FTD.sig       = 1x4 double Matrix holding the
%                                SdifSignature (4 char sequence) of the frame type
%                FTD.matrixsig = nx4 double matrix holding the 4 char sequence 
%                                of all matrix types allowed in the frame
%                FTD matrixname= nx1 or 1xn cell array of strings each of
%                                which determines a colmun name of the matrix

%              If the dir variable is present it will contain a directory 
%              of the SDIF file content holding all header frame/matrix header         
%                informations but no data.
%              dir is an array of structs describing frames. Each frame contains f
% 
%                frame.sig      = (int) SdifSignature 
%              frame.streamid = (int) indicating streamid of the frame
%                frame.time     = time position of the frame
%              frame.matrix   = array of selected matrix signatures (int)
%                                 present in the frame
% 
%       Example:  head = loadSDIF('xylo.sdif')
%  

function [file,head,dir]=Fsdifopen(filename,iomode,head)


if (nargin == 1 || iomode == 'r' || iomode == 'R')
  switch nargout 
   case {1}
    file = Fsdif_read_handler('open',filename);
   case {2}
    [file,head] = Fsdif_read_handler('open',filename);
   otherwise
    [file,head,dir] = Fsdif_read_handler('open',filename);
  end
else
  if(nargin > 1 && (iomode == 'w' || iomode == 'W'))
    if nargin == 2
      head = [];
    end
    file = Fsdif_write_handler('open',filename,head);
  else
    error('wrong number of inout arguments for Fsdifopen')
  end
end
  