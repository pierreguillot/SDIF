
name = 'testrw.sdif'
sig  = '1FQ0'
data = [0:3]

writesdif(name)
for x = data,
    disp(['write ' num2str(x)]);
    writesdif(x, 1, sig, sig, [x])
end
writesdif('close')

loadsdif(name)
for x = data,
    [a, b, c, d, e] = loadsdif;
    if (a == [x])  &  (b == x)  &  (c == 1)  &  (d == sig)  &  (e == sig),
	disp([num2str(a) ' ok']);
    else
	disp([num2str([a, b, c, d, e]) ' != ' num2str([x]), num2str(x), ' 1 ', sig, sig]);
    end
end
loadsdif('close')
