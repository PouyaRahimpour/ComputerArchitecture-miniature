# program to test slt instruction
one     .fill       1
three   .fill       3
        lw 1, 0, one
        lw 2, 0, three
        slt 3, 1, 2
        halt

