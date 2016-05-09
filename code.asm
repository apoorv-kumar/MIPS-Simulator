#define variables in memory
	.word var1 , 3
	.word var2 , 5

#load word(integer) from memory 
lw $0 , var1

#load immediate integer into register
li $1 , 9
li $2 , 10

#save word in reg1 into variable var1
sw $1 , var1

#if values in reg1 and reg2 are same then branch to end 
beq $1 , $2 , end
#else continue

#add the values in $1 and $2 into $3
add $3 , $2 , $1

#sub #1 from $2 into $3
sub $3 , $3 , $1

#set on less than
slt $4 , $1 , $2

end:

exit
