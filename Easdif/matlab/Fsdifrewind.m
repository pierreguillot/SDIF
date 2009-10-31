% function Fsdifrewind(fh)
%
% rewind sdif file to the first frame
%
% AUTHOR : Axel Roebel
% DATE   : 31.10.2009
%
% $Revision: 1.1 $    last changed $Date: 2009-10-31 17:25:09 $
%
%                                                       Copyright (c) 2008 by  IRCAM 

function Fsdifrewind(fh)

  Fsdif_read_handler('rewind',fh);
