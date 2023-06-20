#twice define of a lable err
	slt	    2,3,1
	add	    7,15,3
	nand	4,2,1
	slti	5,2,1
end	addi	2,6,4 	#first define
	sw	    3,0,num
	lw 	    1,6,15
	beq	    3,2,end
	jalr	5,12
	j	    end
end	halt	    	#second define
num	.fill	123


