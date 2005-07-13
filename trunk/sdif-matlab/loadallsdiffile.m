% NAME
%     loadallsdiffile  -  Load a complete SDIF file
%
% USAGE 
%     [ data, header, frame, matrix ] = loadallsdiffile (name,types)
%
%     where data{i} is the matrix at time header(i, 1) in stream header(i, 2) 
%     with signatures char(frame(i)), char(matrix(i)).
%     types is an optional sdiftypes definition file
%
%     In contrast to loadsdiffile loadallsdiffile will return empty
%     frames, such that complete time information is available.
%
% SEE ALSO
%     loadsdifflat, loadsdif, sdifexist
% 
% AUTHOR
%     Axel Roebel (roebel@ircam.fr), 12. July 2005
%
% CVS REVISION
%     $Id: loadallsdiffile.m,v 1.2 2005-07-13 13:03:03 roebel Exp $

% $Log: not supported by cvs2svn $
% Revision 1.1  2005/07/12 13:26:47  roebel
% Added new version of loadsdiffile that will include empty frames
% in the list of returned matrices, such that a complete list
% of frmae times can be established.
%


function [ data, header, frame, matrix ] = loadallsdiffile (name, types) %  todo: nmatrix)

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

        if isempty (t),  break;  end
    
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
        if length(m) ~=0
          matrix(n, 1:4)  = m;
        else
          matrix(n, 1:4)  = 'none';
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

