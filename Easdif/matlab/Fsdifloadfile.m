% NAME
%     Fsdifloadfile  -  Load a complete SDIF file
%
% USAGE
%     [ data, header, frame, matrix, dims ] = Fsdifloadfile (name)
%
%     where data{i} is the matrix at time header(i, 1) in stream header(i, 2)
%     with signatures char(frame(i)), char(matrix(i)) and of size dims{i}.
%  
%
% SEE ALSO
%
%     Fsdifopen, Fsdifclose, Fsdifread, Fsdif_read_handler, Fsdifexist
%
% AUTHOR
%     Axel Roebel (roebel(at)ircam.fr) 21. January 2008
% 
% CVS REVISION
%     $Id: Fsdifloadfile.m,v 1.2 2008-01-22 01:10:44 roebel Exp $
%
%                                         Copyright (c) 2008 by IRCAM
%
function [ data, header, frame, matrix, dims ] = Fsdifloadfile (name, types) 

  if( ~Fsdifexist(name))
    error(sprintf('%s is no sdif file \n',name))
  end

  file =  Fsdif_read_handler('open',name);
  switch nargout
   case {1}
    data = Fsdif_read_handler('loadfile',file);
   case {2}
    [data header] = Fsdif_read_handler('loadfile',file);
   case {3}
    [data header frame] = Fsdif_read_handler('loadfile',file);
   case {4}
    [data header frame matrix] = Fsdif_read_handler('loadfile',file);
   case {5}
    [data header frame matrix dims] = Fsdif_read_handler('loadfile',file);
  end
  Fsdif_read_handler('close',file);

return

