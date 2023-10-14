a <-- x^2
     0       POW         a (1),    x (0),    2
b <-- (a + x)
     1       ADD         b (2),    a (1),    x (0)
c <-- 1/x
     2      COPY         c (3),    x (0)
result <-- (a*x^2 + b*x + c)
     3       POW       _t7 (7),    x (0),    2
     4       MUL       _t8 (8),    a (1),  _t7 (7)
     5       MUL       _t9 (9),    b (2),    x (0)
     6     ADDTO       _t8 (8),  _t9 (9)
     7       ADD    result (4),  _t8 (8),    c (3)
