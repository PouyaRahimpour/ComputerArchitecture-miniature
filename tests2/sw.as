# program to test sw instruction
one     .fill       1
four    .fill       4
        lw 1, 0, one
        lw 2, 0, four
        add 3, 2, 1 # so it's 5 now
        sw 3, 0, one # one is 5 now
        lw 4, 0, one
        halt
