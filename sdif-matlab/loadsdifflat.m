% Load a complete SDIF file into one matrix.  The file must contain only one
% matrix per frame with the same number of columns throughout.
% name can include an SDIF selection
%
% Usage:   [ data, frames, signatures ] = loadsdifflat (name)
%
% where data(n), n = frames(i, 3), is the matrix at time frames(i, 1) in
% stream frames(i, 2) of height frames(i, 4) with frame type 
% signatures(i, 1:4) and matrix type signatures(i, 5:8).

% $Id: loadsdifflat.m,v 1.1 2000-05-04 13:24:07 schwarz Exp $
%
% loadsdifflat.m	31. January 2000	Diemo Schwarz
%
% $Log: not supported by cvs2svn $

function [ data, frames, signatures ] = loadsdifflat (name)

    sdifexists (name);

    di = 1;	% data index
    fi = 1;	% frame index

    NVTinfo = loadsdif (name);
    
    while (1)				% read frame by frame
	[ d, t, s, f, m ] = loadsdif;

	if isempty (d),  break;  end
    
	[ rows, cols ]	    = size(d);
	frames(fi, 1)       = t;
	frames(fi, 2)       = s;
	frames(fi, 3)       = di;
	frames(fi, 4)       = rows;
	signatures(fi, 1:4) = f;
	signatures(fi, 5:8) = m;
	data(di, 1:cols)    = d;
    
	fi = fi + 1;
	di = di + rows;
    end
    
    loadsdif ('close');		% close files
return
