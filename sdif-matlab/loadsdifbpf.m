% loadsdifbpf -- Load a complete break-point function from an SDIF file
%
% USAGE 
%     [ bpf ] = loadsdifbpf(filename, types)
%
%     The filename includes an sdif selection that selects exactly one
%     matrix type with one row and one column.
%     types is an optional sdiftypes definition file
%
% SEE ALSO
%     loadsdiffile, loadsdifflat, loadsdif, sdifexist
% 
% AUTHOR
%     Diemo Schwarz (schwarz@ircam.fr), 29. August 2003
%
% CVS REVISION
%     $Id: loadsdifbpf.m,v 1.1 2004-02-19 10:55:01 schwarz Exp $

% $Log: not supported by cvs2svn $

function bpf = loadsdifbpf (name, types) %  todo: nmatrix)

    % start with empty buffer, reallocated immediately to something reasonable
    bpf = [];
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
	[ d, t ] = loadsdif;

	if isempty (t),  break;  end
    
	n = n + 1;

	if n > nalloc,			% make more space (blockwise)
	    bpf    = [ bpf; zeros(nblock, 2) ];
	    nalloc = nalloc + nblock;
	    if nalloc > nblock,
		disp([ 'loadsdifbpf realloc ' num2str(nalloc) ])
	    end
	end
	
	bpf(n, 1) = t;
	bpf(n, 2) = d(1);
    end

    % cut data to true size
    bpf = bpf(1:n, :);
    
    loadsdif ('close');			% close files
return
