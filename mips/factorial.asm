# name: Leyi Qiang
# CCIS account: Drunkbug
.data
	prompt:  .asciiz "Positive integer :"
	result:  .asciiz "The factorial is "
	numb: .word 0 
	answer:  .word 0
.text 
	#the main function
	#int main()
	main:
		# printf("Positive integer: ");
		li $v0, 4        # print_string
		la $a0, prompt   # load prompt value into $a0
		syscall          # print value of prompt to screen
		
		# scanf("%d", &number);
		li $v0, 5	 # read_int
		syscall		 # run the read_int syscall
		
		sw $v0, numb	 # store input in numb
		
		#  factorial(number);
		lw $a0, numb
		jal factorial
		sw $v0, answer
		
		# print the result
		li $v0, 4 	 # print_string
		la $a0, result   # load value of result into $a0
		syscall 	 # print value of result to screen
		
		li $v0, 1 	 # print_int
		lw $a0, answer   # print the answer
		syscall
		
		# exit
		li $v0, 10       # exit
		syscall
		
	
	# the factorial function	
	# int factorial(int x)
	factorial:
		#if (x == 0) return 1;
		bne $a0, $zero, notEqual 	# if numb not euqal 0, go to label notEqual
		addi $v0, $zero, 1		# set the base case $v0 = 1
		jr $ra				# return to the caller

	notEqual:
		addi $sp $sp, -8 		# make room for 2 registers on stack
		sw $ra, 0($sp) 			# save return address
		sw $a0, 4($sp) 			# save the numb
		addi $a0, $a0, -1 		# $a0 = numb - 1
		jal factorial 			# $v0 = fac(n-1)
		
		
		lw $a0, 4($sp) 			# restore $a0
		lw $ra, 0($sp) 			# restore $ra
		addi $sp, $sp, 8 		# adjust stack pointer to pop 2 items
		# ???
		mul $v0, $a0, $v0 		# $v0 = x * factorial(x-1)
		jr $ra 				# return 
		
		
		
		
		
		
