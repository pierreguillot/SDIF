
function testrw (i)	% i = intrusion of other data
    nerr   = 0;
    passed = 0;

    if nargin == 0,
	i = [0:4];
    end
    
    for x = i,
	err    = testrwi (x);
	nerr   = nerr   + err;
	passed = passed + (err == 0);
    end
    
    fprintf (1, '\nfinished, %d of %d tests passed\n\n', passed, length(i));
return



function err = testrwi (i)	% i = intrusion of other data
    err  = 0;
    name = ['gen/testrw' num2str(i) '.sdif'];
    sig  = '1FQ0';
    isig = '1GAI';
    data = [0:3];
    stream = 1;
    
    fprintf (1, '\nwriting test %d: %s\n', i, name);
    
    writesdif(name)
    for x = data,
	disp(['write ' num2str(x)]);
	if i ~= 3,	% i = 0: normal, no intrusion, 3 see below
	    writesdif(x, stream, sig, sig, [x])
	end
	
	switch i,
	    case 3,	% same frame & stream, more matrices
		writesdif(x, stream, sig, sig, [x], isig, [-x])
	
	    case 1,	% same frame, other stream
		writesdif(x, 2, sig, sig, [x+9])
	
	    case 2,	% same stream, other frames
		writesdif(x, stream, isig, sig, [x+9])
	    
	    case 4,	% frame with empty matrix
		writesdif(x, 2, isig, isig, [])
		
	end
    end
    writesdif('close')
    
    namesel = [name '::#' num2str(stream) ':' sig '/' sig];
    fprintf (1, '\nloading test %d: %s\n', i, namesel);
    
    loadsdif(namesel)
    for x = data,
	[a, b, c, d, e] = loadsdif;
	if ~isempty(a)  &  (a == [x])  ...
	   &  (b == x)  &  (c == stream)  &  (d == sig)  &  (e == sig),
	    disp([num2str(a) ' ok:   ' s2str(a, b, c, d, e) ' == ' ...
				       s2str([x], x, stream, sig, sig)]);
	else
	    disp([num2str(x) ' ERROR ' s2str(a, b, c, d, e) ' != ' ...
				       s2str([x], x, stream, sig, sig)]);
	    err = err + 1;
	end
    end
    loadsdif('close')
return



function [str] = s2str (a, b, c, d, e)
    if (nargin > 1)  &  ~isempty (a),
	str = sprintf ('data %s @%g #%d :%s/%s', ...
		       num2str(a), b, c, d, e);
    else
	str = 'EMPTY!';
    end
return
