% NAME
%     loadsdif - mex-function to load data from an SDIF file.
%
% USAGE 
%     (all output parameters except matrix are optional)
%
%   [NVTs, ...] = loadsdif(filename)	
%     Open file, filename can include an SDIF selection.
%     Will in future return Name-Value Table and other ASCII header information
%
%   [matrix, time, stream, framesig, matrixsig] = loadsdif
%     Read one matrix: Each call returns the next selected matrix, and 
%     optionally the frame time, stream, and the frame and matrix 4-char 
%     signatures.
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
%
% CVS REVISION
%     $Id: loadsdif.m,v 1.1 2000-07-27 18:22:32 schwarz Exp $

% This file contains only the help message!
%
% $Log: not supported by cvs2svn $
