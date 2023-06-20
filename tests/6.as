#invalid register number

	slt	2,3,1
    add	7,16,3  #invalid register number should be in [0, 15]
	and	7,16,3
	nand	4,2,1
	slti	5,2,1
	addi	2,6,4
	sw	3,0,num
	lw 	1,6,16
	beq	3,2,end
	jlar	5,12
	j	end
end	halt
num	.fill	123
