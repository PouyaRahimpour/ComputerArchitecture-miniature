# program to test nand instruction
one     .fill       1
two     .fill       2
        lw 1, 0, one
        lw 2, 0, two
        nand 3, 1, 2
        halt

