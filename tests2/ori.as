# program to test ori instruction
one     .fill       1
two   .fill       2
        lw 1, 0, one
        lw 2, 0, two
        ori 3, 1, 22
        halt

