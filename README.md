#MIPS SIMULATOR :: 
For course: Computer Organization and Architecture
            Feb 2011
			
designed by::	 apoorv kumar 
	 rollno::     09010111

usage::		
	provide the code file's address to the simulator (relative to the executable ... just the name incase both are in same folder)
	press return
	the simulator runs the code and ends when it encounters 'exit' instruction

	
##instructions supported::

	###0 operand
	----
	exit	#exit the execution and terminate normally

	###1 operand
	----
	j str_location   #jump to the location given by string str_location


	###2 operand
	----
	
	.word var_name , var_val  #define variable var_name in memory with value var_val
	li $reg_id , constant  	#load immediate constant into register given by $reg_id
	
	lw $reg_id , var_name	#load word(int) in memory given by name var_name to register $reg_id
	sw $reg_id , var_name	#store word(int) in memory given by name var_name from register $reg_id

	###3 operand
	----
	
	beq $reg_id1 , $reg_id2 , code_location  #branch to code_location if values are equal
	
	add $reg_id1 , $reg_id2 , $reg_id3  #sum the values $reg_id2 and $reg_id3 into $reg_id1
	sub $reg_id1 , $reg_id2 , $reg_id3  #diff of the values $reg_id2 and $reg_id3 into $reg_id1
	and $reg_id1 , $reg_id2 , $reg_id3  #bitwise and of the values $reg_id2 and $reg_id3 into $reg_id1
	or $reg_id1 , $reg_id2 , $reg_id3   #bitwise or of values $reg_id2 and $reg_id3 into $reg_id1
	slt $reg_id1 , $reg_id2 , $reg_id3  #put the lesser of values $reg_id2 and $reg_id3 into $reg_id1
	
	
	
###other features::

	1. commenting - separate line commenting with prefix '#' is supported 
			ie , comments cannot be on the same line as code , but on a new line
	
	2. indentation - using tabs , spaces and new lines for indenting is allowed , they are neglected
			while execution
			
	3. inline definitions - the variables can be allocated in the memory (defined) within the code , 
				and from any location; no distinction between data and text region exists.
		

###drawbacks::
	1. fault tolerance is weak
		-- segmentation fault occurs if a non existent file is opened.
	
	2. the variables have to be assigned a value while they are defined ... this is in order to show memory state.
	
	3. source code is monolithic and needs to be distributed into modules 

	
	
	
