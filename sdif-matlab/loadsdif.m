% NAME
%     loadsdif - mex-function to load data from an SDIF file.
%
% USAGE 
%     (All output parameters except matrix are optional.)
%
%   [head] = loadsdif(filename, sdif-types-file)	
%     Open file, filename can include an SDIF selection.
%     The optional parameter sdif-types-file is the file name of an SDIF
%     description types file (.STYP), which defines the allowed frame and
%     matrix types.
%
%     Return:  If output matrix is specified this call will return 
%              Name-Value Tables and IDS from the
%              ASCII header information as one single string.
%
%     Example:  head = loadsdif('xylo.sdif', 'SdifTypes.STYP')
%
%   [matrix, time, stream, framesig, matrixsig] = loadsdif
%     Read one matrix: Each call returns the next selected matrix, and 
%     optionally the frame time, stream, and the frame and matrix 4-char 
%     signatures.
%
%     Returns empty values on end-of-file.  (matrix can be empty anytime,
%     the other values are empty only on eof.)
%
%     Example:  while(1),  
%			[d, t, s, f, m] = loadsdif;
%			if isempty (t),  break;  end
%		end
%
%   loadsdif('close')
%	Close file.
%
% SEE ALSO
%     loadsdiffile, loadsdifflat, sdifexist
% 
% AUTHOR
%     Diemo Schwarz (schwarz@ircam.fr), 25. January 2000

% This file contains only the help message!
%
% No CVS REVISION in help, since it won't change very much, but will be
% misleading.  (We'd really like to see the current version of loadsdif.c.)
%     $Id: loadsdif.m,v 1.5 2001-05-28 16:33:18 roebel Exp $
%
% $Log: not supported by cvs2svn $
% Revision 1.4  2000/08/27 14:24:12  schwarz
% Clarified empty matrix issue:  The doc was wrong!
% Updated doc and loadsdiffile and loadsdifflat now use eof flag right
% and don't stop on empty matrices.
%
% Revision 1.3  2000/08/04  14:42:30  schwarz
% Added reset of file variable, prevents crash on double-close.
% Version number is written in NVTs, and is used for distribution,
% defined in makefile (major.minor.release).
% Types file now included in distribution and documentation.
%
% Revision 1.2  2000/07/27  18:30:35  schwarz
% @$#%^^!&* comment leader!
%
% Revision 1.1  2000/07/27  18:22:32  schwarz
% man-style comments, better makedefs.

