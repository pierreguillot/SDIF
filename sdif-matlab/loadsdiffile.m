% NAME
%     loadsdiffile  -  Load a complete SDIF file
%
% USAGE 
%     [ data, header, frame, matrix ] = loadsdiffile (name)
%
%     where data{i} is the matrix at time header(i, 1) in stream header(i, 2) 
%     with signatures char(frame(i)), char(matrix(i)).
%
% SEE ALSO
%     loadsdifflat, loadsdif, sdifexist
% 
% AUTHOR
%     Diemo Schwarz (schwarz@ircam.fr), 31. January 2000
%
% CVS REVISION
%     $Id: loadsdiffile.m,v 1.3 2000-07-27 18:22:33 schwarz Exp $

% $Log: not supported by cvs2svn $
% Revision 1.2  2000/05/11  12:37:17  schwarz
% Init of output variables to avoid 'output argument not assigned' warning.
% Optional types file argument.
%
% Revision 1.1  2000/05/04  13:24:06  schwarz
% Matlab mex extension and support functions to load SDIF files.

function [ data, header, frame, matrix ] = loadsdiffile (name, types)

    data   = [];			% init to avoid 'non assigned' warning
    header = [];
    frame  = [];
    matrix = [];

    sdifexist (name);			% quit with error if file's not there

    if nargin == 1,			% open file with/without types
	NVTinfo = loadsdif (name);
    else
	NVTinfo = loadsdif (name, types);
    end
	
    n = 1;

    while (1)				% read frame by frame
	[ d, t, s, f, m ] = loadsdif;

	if isempty (d),  break;  end
    
	data{n}		= d;
	header(n, 1)	= t;
	header(n, 2)	= s;
	frame (n, 1:4)  = f;
	matrix(n, 1:4)  = m;

	n = n + 1;
    end
    
    loadsdif ('close');			% close files
return

% Usage:   [ data, time, stream, frame, matrix ] = loadsdiffile (name)
% where data{i} is the matrix at time(i) in stream(i) with signatures 
% frame{i}, matrix{i}.
%
%	frame{n}  = f;
%	matrix{n} = m;

