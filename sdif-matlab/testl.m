
function testl(name)
    format short g
    format loose  
    %format compact

    if nargin > 0,
	testfile (name)
    else
	testfile ('test2.sdif')
	testfile ('test2.sdif::@0+0.01#0')
	testfile ('test2.sdif::@0+0.01#1')
	testfile ...
	   ('/net/kethuk/snd/dhaes/ADDtromp1/tromp1_S1.f0.sdif::@0.1-1', ... 
	    '/net/litote/u.litote/atiam99/dhaes/src/matlab/NewTypes.STYP')
    end
return


function testfile (name, types)
    if nargin == 1,
	disp (['Testing SDIF file "' name '":']);
	NVTinfo = loadsdif (name)
    else
	disp (['Testing SDIF file "' name '" with types ' types ':']);
	NVTinfo = loadsdif (name, types)
    end

    while (1)
	[ data, time, stream, frame, matrix ] = loadsdif;
	if isempty (data),  
	    disp ('eouf!');
	    disp (' ');
	    break;
	else
	    disp (['time ' num2str(time) '  stream ' num2str(stream) ...
		   '  frame ' frame '  matrix ' matrix '  data [' ...
		   num2str(size(data)) '] =']);
	    disp (data);
	end
    end
    
    loadsdif ('close')
return


% nvtinfo			        = loadsdif ('open', name[::selection])
% [       time, stream, frame ]         = loadsdif ('nextframe',  [selection])
% [ data, time, stream, frame, matrix ] = loadsdif ('nextmatrix', [selection])
%					  loadsdif ('close')
% 
% later:  loadsdif ([file-id], ...)  for multiple files
