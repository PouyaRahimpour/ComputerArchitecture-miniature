# program to test add instruction
one     .fill       1
three   .fill       3
        lw 1, 0, one
        lw 2, 0, three
        add 3, 1, 2
        halt
