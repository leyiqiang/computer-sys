.data
prompt: .asciiz "Initial array is: "
result: .asciiz "\nInsertion sort is finished:"
comma:  .asciiz ","
	.align 5
array:  .asciiz "Joe"
	.align 5
        .asciiz "Jenny"
	.align 5
        .asciiz "Jill"
	.align  5
        .asciiz "John"
	.align  5
        .asciiz "Jeff"
	.align  5
        .asciiz "Joyce"
	.align  5
        .asciiz "Jerry"
	.align  5
        .asciiz "Janice"
	.align  5
        .asciiz "Jack"	
	.align  5
        .asciiz "Jonna"
	.align  5
        .asciiz "Jack"
	.align  5
        .asciiz "Jocelyn"
	.align  5
        .asciiz "Jessie"
	.align  5
        .asciiz "Jess"
	.align  5
        .asciiz "Janet"
	.align  5
        .asciiz "Jane"	
	.align 5
	.word 0
	
	
	.align 2
	
	
dataAddr:.space 64
endAddr: #the end of dataAddr

size:   .word 16


i: 	.word dataAddr
j: 	.word dataAddr

.text
# the main function of inserSort
main: 

	
	la	$t0,dataAddr
	la	$t1,endAddr
	la	$t2,array
init:
	sw	$t2,0($t0)
	addi	$t0,$t0,4	
	addi	$t2,$t2,32
	blt 	$t0,$t1,init
	
	# print "Initial array is: "
	li $v0, 4
	la $a0, prompt
	syscall
	# print the unsorted list
	la $a0, array
	jal printnames

	#insertion sort begin
	jal insertSort
	
	# print "Insertion sort is finished:"
	la $a0, result
	li $v0, 4
	syscall
	
	# print the sorted list
	la $a0, dataAddr
	jal printnames
	
	# exit
	li $v0, 10
	syscall

# print names in the array	
printnames:
	addi $sp, $sp, -12
	sw $ra, 8($sp)
	la $t0, dataAddr

	beq $t1, $t0, endprint

loopprint:
	la $t1, endAddr
	la $a0, ($t0)
	lw $a0, ($a0)
	li $v0, 4
	syscall
	addi $t0, $t0, 4
	beq $t1, $t0, endprint
	li $v0, 4
	la $a0, comma
	syscall
	bgt $t1, $t2, loopprint

	
endprint:
	lw $ra, 8($sp)
	addi $sp, $sp, 12
	jr $ra


# insertion sort
insertSort:
	addi $sp, $sp, -12
	sw $ra, 8($sp)
	
# the outer loop
insertLoop1:
	# set i
	la $t0, i
	lw $t0, ($t0) 
	la $t1, endAddr
	# reach the end
	beq $t0, $t1, endSort
	
	la $t0, i
	lw $t0, ($t0)
	sw $t0, j 

	
	# set j = i - 1
	la $t0, i
	lw $t0, ($t0)
	addi $t0, $t0, 4
	sw $t0, i
	
# the inner loop
insertLoop2:
	lw $t0, j
	la $t1, dataAddr
	# break inner loop
	ble $t0, $t1, insertLoop1
	
	lw $t0, j
	addi $t1, $t0, -4
	lw $t2, ($t0)
	lw $t3, ($t1)
	
	li $t7, 0

# compare two names
compare:
	lb $t4, ($t2)
	lb $t5, ($t3)
	# switch
	blt $t4, $t5, switch
	
	# stay
	bgt $t4, $t5, insertLoop1
	
	# equal
equal:

	addi $t2, $t2, 1
	addi $t3, $t3, 1
	#if same, stay
	beq $t7, 8, insertLoop1
	addi $t7, $t7, 1
	j compare

# switch the position of two names	
switch:
	sub $t2, $t2, $t7
	sub $t3, $t3, $t7
	sw $t2, ($t1)
	sw $t3, ($t0)
	
	# j = j - 1
	la $t0, j
	lw $t0, ($t0)
	addi $t0, $t0, -4
	sw $t0, j
	
	j insertLoop2
	
# end of insertion sort
endSort:
	lw $ra, 8($sp)
	addi $sp, $sp, 12
	jr $ra
	
	
