# program to test sub instruction
one     .fill       1
three   .fill       3
        lw 1, 0, one
        lw 2, 0, three
        sub 3, 2, 1
        halt

