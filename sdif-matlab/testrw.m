
function testrw (i)	% i = intrusion of other data

name = 'testrw.sdif';
sig  = '1FQ0';
isig = '1GAI';
data = [0:3];
stream = 1;

writesdif(name)
for x = data,
    disp(['write ' num2str(x)]);
    if i ~= 3,
	writesdif(x, stream, sig, sig, [x])
    else				   % same frame & stream, more matrices
	writesdif(x, stream, sig, sig, [x], isig, [-x])
    end
    
    if i == 1,
	writesdif(x, 2, sig, sig, [x+9])   % same frame, other stream
    end
    
    if i == 2,
	writesdif(x, stream, isig, sig, [x+9])  % same stream, other frames
    end
end
writesdif('close')

loadsdif([name '::#' num2str(stream) ':' sig '/' sig])
for x = data,
    [a, b, c, d, e] = loadsdif;
    if ~isempty(a)  &  (a == [x])  ...
       &  (b == x)  &  (c == stream)  &  (d == sig)  &  (e == sig),
	disp([num2str(a) ' ok:   ' s2str(a, b, c, d, e) ' == ' ...
				   s2str([x], x, stream, sig, sig)]);
    else
	disp([num2str(x) ' ERROR ' s2str(a, b, c, d, e) ' != ' ...
				   s2str([x], x, stream, sig, sig)]);
    end
end
loadsdif('close')


function [str] = s2str (a, b, c, d, e)
    if (nargin > 1)  &  ~isempty (a),
	str = sprintf ('data %s @%g #%d :%s/%s', ...
		       num2str(a), b, c, d, e);
    else
	str = 'EMPTY!';
    end
return
