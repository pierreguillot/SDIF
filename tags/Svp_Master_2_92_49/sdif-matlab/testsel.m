% $Id: testsel.m,v 1.1 2003-09-15 17:08:13 schwarz Exp $
%
% $Log: not supported by cvs2svn $

function testsel()

    global filename;
  
    format compact
    filename = 'xpianoRes.m5.FIL.sdif::1REB'
    
    loadsdif(filename);
    [DD, t, s, f, m] = loadsdif;
    loadsdif('close')

    [ nrow, ncol ] = size(DD)
    D = zeros(nrow+2, ncol+2);	% pad to extra columns
    D(1:nrow, 1:ncol) = DD;
    
    colmap = repmat(1:ncol+2, 1, ncol+2);
    
    for r = 1:nrow + 2,
	choice = randperm((ncol + 2)^2); 
	cindex = colmap(choice(1:5));
	testselection('one row, random columns', ...
		      num2str(r), cindex, D(r, unique(cindex)));
    end

    testselection('row ranges',       '6,66-69', [], D([6 66:69], :));
    testselection('overlapping rows', '6,1-9,7', [], D([1:9], :));

    

function testselection(descr, sel, colvec, D)
    
    global filename;
    
    if ~isempty(colvec),
	colsel = [ '.' num2str(colvec(1)) ];
	for i = 2:length(colvec),
	    colsel = [ colsel ',' num2str(colvec(i)) ];
	end
    else
	colsel = '';
    end
    
    loadsdif([ filename '_' sel colsel]);
    [d, t, s, f, m] = loadsdif;
    loadsdif('close')
    
    if all(d == D),
	disp([ descr ' selection _' sel ' ' colsel ' ok...' ]);
    else
	error([ 'mismatch for ' descr ' selection _' sel ' ' colsel ]);
    end
end
