#out of bound offset err
	slt 	2,3,1
	add 	7,15,3
	or  	7,15,3
lbl	nand	4,2,1
	slti	5,2,1
	addi	2,6,4
	sw  	3,0,num1
	lw  	1,6,15
	beq 	3,2,lbl
	jalr	5,12
	beq   	2,4,78768	#this number doesnt fit
	halt
num1	.fill	1
num2	.fill	3
