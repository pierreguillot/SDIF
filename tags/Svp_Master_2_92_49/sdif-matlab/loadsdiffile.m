% NAME
%     loadsdiffile  -  Load a complete SDIF file
%
% USAGE 
%     [ data, header, frame, matrix ] = loadsdiffile (name,types)
%
%     where data{i} is the matrix at time header(i, 1) in stream header(i, 2) 
%     with signatures char(frame(i)), char(matrix(i)).
%     types is an optional sdiftypes definition file
%
% SEE ALSO
%     loadsdifflat, loadsdif, sdifexist
% 
% AUTHOR
%     Diemo Schwarz (schwarz@ircam.fr), 31. January 2000
%
% CVS REVISION
%     $Id: loadsdiffile.m,v 1.8 2005-07-13 13:03:26 roebel Exp $

% $Log: not supported by cvs2svn $
% Revision 1.7  2005/07/12 13:29:00  roebel
% Changed to new loadsdif behavior which will return empty matrices and zero length
% matrix signature for empty frames.
%
% Revision 1.6  2003/09/15 15:58:54  schwarz
% Properly preallocate arrays and cell arrays, dynamic reallocation every 10000 frames
% --> 100 times faster for big files (no lie!)
%
% Revision 1.5  2001/04/19 19:06:52  roebel
% help comment changed, added function parameter types.
%
% Revision 1.4  2000/08/27 14:24:11  schwarz
% Clarified empty matrix issue:  The doc was wrong!
% Updated doc and loadsdiffile and loadsdifflat now use eof flag right
% and don't stop on empty matrices.
%
% Revision 1.3  2000/07/27  18:22:33  schwarz
% man-style comments, better makedefs.
%
% Revision 1.2  2000/05/11  12:37:17  schwarz
% Init of output variables to avoid 'output argument not assigned' warning.
% Optional types file argument.
%
% Revision 1.1  2000/05/04  13:24:06  schwarz
% Matlab mex extension and support functions to load SDIF files.

function [ data, header, frame, matrix ] = loadsdiffile (name, types) %  todo: nmatrix)

    % start with empty buffer, reallocated immediately to something reasonable
    data   = {};
    header = [];
    frame  = [];
    matrix = [];
    nalloc = 0;	
    nblock = 10000;			% reallocation block size

    sdifexist (name);			% quit with error if file's not there

    if nargin == 1,			% open file with/without types
	NVTinfo = loadsdif (name);
    else
	NVTinfo = loadsdif (name, types);
    end
	
    n = 0;

    while (1)				% read frame by frame
	[ d, t, s, f, m ] = loadsdif;

        if length(m)~= 0
          n = n + 1;
          
          if n > nalloc,			% make more space (blockwise)
                                                % prealloc with (1, 1) matrix inside --> most common case needs
                                                % no memory allocation --> 60% runtime reduction
                                                data   = [ data; num2cell(zeros(nblock, 1)) ];
                                                header = [ header; zeros(nblock, 2) ];
                                                frame  = [ frame;  zeros(nblock, 4) ];
                                                matrix = [ matrix; zeros(nblock, 4) ];
                                                nalloc = nalloc + nblock;
                                                if nalloc > nblock,
                                                  disp([ 'loadsdiffile realloc ' num2str(nalloc) ])
                                                end
          end
          
          data{n}		= d;
          header(n, 1)	= t;
          header(n, 2)	= s;
          frame (n, 1:4)  = f;
          matrix(n, 1:4)  = m;
        else
          if isempty (t),  break;  end
	end    
    end
    
    % cut data to true size
    data   = data  (1:n, :);
    header = header(1:n, :);
    frame  = frame (1:n, :);
    matrix = matrix(1:n, :);
    
    loadsdif ('close');			% close files
return

% Usage:   [ data, time, stream, frame, matrix ] = loadsdiffile (name)
% where data{i} is the matrix at time(i) in stream(i) with signatures 
% frame{i}, matrix{i}.
%
%	frame{n}  = f;
%	matrix{n} = m;

