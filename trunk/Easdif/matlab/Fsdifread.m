% function frames=Fsdifread(file,select)
%
% read 1-d array of structured sdif frames into file related to
% filehandle file
%
% INPUT :
%
% file   : filehandle opened for reading returned by Fsdifopen 
% select : selection controling the read
%
% Frame selection :
% frames=Fsdifread(file) read the next individual frame
%                        empty return indicates end of file
%
% otherwise selection can be a single struct holding the fields of the frame
% directory (see Fsdifopen).
%
% All fields of the selection that are present are used to filter the elements of the
% matrix to be read. Accordingly, a empty selection matrix does not
% filter anything and accordingly reads the whole file.
%
%
%
% OUTPUT :
%
% 1d array of data frames read
%
%
% frame format :
%    frames.sig      = 1x4 double array indicating the matrix signature
%    frames.streamid = real scalar of arbitrary type holding the
%                      streamid
%    frames.time     = real double indicating frame time
%    frames.data     = struct with fields named MD_XXXX
%                      where XXXX is representing the 4-char matrix
%                      signature and each field contains exactly one real
%                      valued matrix of any but 64-bit integer types.
%  example :
%
% frames(1)
%
%         sig: [73 71 66 71] == > double('IGBG')
%    streamid: 0
%        time: 1.3
%        msig: [73 71 66 71] == > double('IGBG')
%        data: [1x1 struct]
%
% frames(1).data
% 
%  MD_IGBG: [1 44100 1 1024 185]
%
%
%
% SEE also : Fsdifopen, Fsdifclose, Fsdifread, and the low level handlers
%     Fsdif_read_handler and Fsdif_write_handler
%
%
% AUTHOR : Axel Roebel
% DATE   : 21.01.2008
%
% $Revision: 1.1 $    last changed $Date: 2008-01-22 00:52:56 $
%
%                                                       Copyright (c) 2008 by  IRCAM 

function frames=Fsdifread(file,select)

  if(nargin == 1)
    frames = Fsdif_read_handler('read',file);
  else
    frames = Fsdif_read_handler('read',file,select);
  end