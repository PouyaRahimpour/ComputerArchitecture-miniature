# program to test slti instruction
one     .fill       1
two     .fill       2
        lw 1, 0, one
        lw 2, 0, two
        slti 3, 2, 100
        halt
