% Test if SDIF file exists.  Takes care of SDIF selection.
%
%          sdifexist(name)	Exits with error if file's not there.
% result = sdifexist(name)	Returns flag.

% $Id: sdifexist.m,v 1.2 2000-05-12 14:03:52 schwarz Exp $
%
% sdifexist.m	4. May 2000	Diemo Schwarz
%
% $Log: not supported by cvs2svn $
% Revision 1.1  2000/05/04  13:24:08  schwarz
% Matlab mex extension and support functions to load SDIF files.

function result = sdifexist (name)
    result = 1;
    
    % remove selection from filename
    colon = find (name == ':');
    if (length (colon) >= 2  &  colon(end-1) == colon(end)-1),
	testname = name (1:colon(end-1)-1);	
    else
	testname = name;
    end
    
    % are you there?
    if ~exist(testname),  
	if nargout == 0,
	    error (['SDIF file ' testname ' does not exist']);
	end
	result = 0;
    end
return
