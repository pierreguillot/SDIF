% $Id: testreadwrite.m,v 1.2 2003-09-15 17:09:23 schwarz Exp $
%
% $Log: not supported by cvs2svn $


function testreadwrite(name)
    format short g
    format loose  
    %format compact

    testcase (1, '1HRM', [1 2 3 4; 2 4 6 8]);
return

function dataout = testcase (n, sig, datain, dataval)
    if nargout == 3,  dataval = datain;  end

    writesdif(['testrw' num2str(n) '.sdif']);
    writesdif(n, n, sig, sig, datain);
    writesdif('close');
    
    loadsdif(['testrw' num2str(n) '.sdif']);
    [matrix, time, stream, frametype, matrixtype] = loadsdif
    loadsdif('close');

    if dataout == dataval,
	disp(['test case ' num2str(n) ' ok']);
    else
	disp(['test case ' num2str(n) ' FAIL']);
    end	
	
return
