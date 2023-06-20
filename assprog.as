        lw      1,0,five    # load reg1 wth 5 (symbolic address)
        lw      2,1,2       # load reg2 with -1 (numeric address)
start   add     1,1,2       # decrement reg1
        beq     0,1,done    # goto end of program when reg1==0
        j       start       # go back to the beginning of the loop
done    halt                # end of the program
five    .fill 5
neg1    .fill -1
stAddr  .fill start         # will contain the address of start (2)




