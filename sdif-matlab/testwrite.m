function testwrite (name)
    writesdif(name)
%   writesdif(0, 1, '1NVT', '1NVT', 'name\tvalue\n')
    writesdif(1, 1, '1ABC', 'IABC', [1], '1ABC', [1 2; 5 6])
    writesdif('close')
