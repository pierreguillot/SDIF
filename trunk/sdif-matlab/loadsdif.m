% NAME
%     loadsdif - mex-function to load data from an SDIF file.
%
% USAGE 
%     (All output parameters except matrix are optional.)
%
%   [NVTs, ...] = loadsdif(filename, sdif-types-file)	
%     Open file, filename can include an SDIF selection.
%     The optional parameter sdif-types-file is the file name of an SDIF
%     description types file (.STYP), which defines the allowed frame and
%     matrix types.
%
%     Return:  This call will in future return Name-Value Tables and other
%     ASCII header information.
%
%     Example:  loadsdif('xylo.sdif', 'SdifTypes.STYP')
%
%   [matrix, time, stream, framesig, matrixsig] = loadsdif
%     Read one matrix: Each call returns the next selected matrix, and 
%     optionally the frame time, stream, and the frame and matrix 4-char 
%     signatures.
%
%     Returns an empty matrix on end-of-file.
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
%     $Id: loadsdif.m,v 1.3 2000-08-04 14:42:30 schwarz Exp $
%
% $Log: not supported by cvs2svn $
% Revision 1.2  2000/07/27  18:30:35  schwarz
% @$#%^^!&* comment leader!
%
% Revision 1.1  2000/07/27  18:22:32  schwarz
% man-style comments, better makedefs.

