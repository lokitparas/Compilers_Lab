# int| f width: 4 offset: 0
#   --int| a width: 4 offset: -12
#   --int| b width: 4 offset: -16
#   --int| c width: 4 offset: 4
# int| main width: 4 offset: 4
#   --int| a width: 4 offset: 4
.data 

String0:	 .asciiz        "\n func "


.text 


f:
addi $sp, $sp -8
sw $ra, 4($sp)
sw $fp, 0($sp)
move $fp , $sp
addi $sp, $sp, -4
lw $s0, 16($fp)
lw $s1, 12($fp)
add $s1, $s1, $s0
sw $s1, 20($fp)
j Ret0

Ret0 :
move $sp, $fp                         #sp=fp
lw $fp, 0($sp)                    #restore previous frame pointer
lw $ra, 4($sp)                #restore RA
addi $sp, $sp, 20                  #stack restore
jr $ra

main:
addi $sp, $sp -8
sw $ra, 4($sp)
sw $fp, 0($sp)
move $fp , $sp
addi $sp, $sp, -4
addi $sp, $sp, -4                    #space for return value
                                               #parameters 
li $s0, 27
addi $sp $sp -4
sw $s0, 0($sp)
li $s0, 56
addi $sp $sp -4
sw $s0, 0($sp)
addi $sp, $sp, -4        #space for SL
jal f           #call to f
lw $s0, 0($sp)
addi $sp $sp 4
addi $s1, $fp, -4
sw $s0, 0($s1)
li $v0, 4                    #print string
la $a0,String0
syscall
lw $s0, -4($fp)
li $v0, 1						# system call code for printing int 
move $a0, $s0
syscall

Ret1 :
move $sp, $fp                         #sp=fp
lw $fp, 0($sp)                    #restore previous frame pointer
lw $ra, 4($sp)                #restore RA
addi $sp, $sp, 12                  #stack restore
jr $ra
