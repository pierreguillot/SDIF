% Load a complete SDIF file into one matrix.  The file must contain only one
% matrix per frame with the same number of columns throughout.
% name can include an SDIF selection
%
% Usage:   [ data, frames, signatures ] = loadsdifflat (name)
%
% where data(n), n = frames(i, 3), is the matrix at time frames(i, 1) in
% stream frames(i, 2) having frames(i, 4) rows (can be 0!) with frame type 
% signatures(i, 1:4) and matrix type signatures(i, 5:8).

% $Id: loadsdifflat.m,v 1.7 2003-10-31 19:37:26 schwarz Exp $
%
% loadsdifflat.m	31. January 2000	Diemo Schwarz
%
% $Log: not supported by cvs2svn $
% Revision 1.6  2003/10/29 18:12:38  schwarz
% fix frame/matrix correspondence bug
%
% Revision 1.5  2003/09/15 15:59:01  schwarz
% Properly preallocate arrays and cell arrays, dynamic reallocation every 10000 frames
% --> 100 times faster for big files (no lie!)
%
% Revision 1.4  2000/08/27  14:24:11  schwarz
% Clarified empty matrix issue:  The doc was wrong!
% Updated doc and loadsdiffile and loadsdifflat now use eof flag right
% and don't stop on empty matrices.
%
% Revision 1.3  2000/05/12  14:03:54  schwarz
% Oops-style errors.
%
% Revision 1.2  2000/05/11  12:37:19  schwarz
% Init of output variables to avoid 'output argument not assigned' warning.
% Optional types file argument.
%
% Revision 1.1  2000/05/04  13:24:07  schwarz
% Matlab mex extension and support functions to load SDIF files.

function [ data, frames, signatures ] = loadsdifflat (name, types)

    data       = [];			% init to avoid 'non assigned' warning
    frames     = [];
    signatures = [];
    nalloc     = 0;
    nblock     = 10000;			% reallocation block size

    sdifexist (name);			% quit with error if file's not there

    if nargin == 1,			% open file with/without types
	NVTinfo = loadsdif (name);
    else
	NVTinfo = loadsdif (name, types);
    end

    di = 0;	% data index
    fi = 0;	% frame index
    
    while (1)				% read frame by frame
	[ d, t, s, f, m ] = loadsdif;

	if isempty (t),  break;  end
    
	[ rows, cols ] = size(d);
	fi = fi + 1;
	di = di + 1;			% advance to next free index

	if di > nalloc,			% make more space (blockwise)
	    frames     = [ frames;     zeros(nblock, 4) ]; % too much here,
	    signatures = [ signatures; zeros(nblock, 8) ]; % but who cares
	    data       = [ data;       zeros(nblock, cols) ];
	    nalloc     = nalloc + nblock;
	    if nalloc > nblock,
		disp([ 'loadsdifflat realloc ' num2str(nalloc) ])
	    end
	end
	
	frames(fi, 1)       = t;
	frames(fi, 2)       = s;
	frames(fi, 3)       = di;
	frames(fi, 4)       = rows;
	signatures(fi, 1:4) = f;
	signatures(fi, 5:8) = m;
	data(di:di+rows-1, 1:cols) = d;
	di = di + rows - 1;
    end
    
    % cut data to true size
    frames     = frames    (1:fi, :);
    signatures = signatures(1:fi, :);
    data       = data      (1:di, :);
    
    loadsdif ('close');			% close files
return
