# Tushar Malakar
# MIPS factorial
 
.data
msg1: .asciiz "Enter n: "
msg2: .asciiz "Factorial of n is: "
 
.text
    # ask user for input
    la        $a0, msg1
    li        $v0, 4
    syscall
 
    li        $v0, 5
    syscall
    move      $t0, $v0
 
    # call factorial function
    addi      $sp, $sp, -12  # alloc 12 bytes
    sw        $t0, 0($sp)    # arg1: number n
    sw        $ra, 8($sp)    # save program counter (PC)
    jal       factorial
    lw        $ra, 8($sp)    # restore program counter (PC)
    lw        $s0, 4($sp)    # load the final return value
    addi      $sp, $sp, 12   # dealloc 12 bytes
 
    # print result
    la        $a0, msg2
    li        $v0, 4
    syscall
 
    move      $a0, $s0
    li        $v0, 1
    syscall
 
    li        $v0, 10
    syscall
 
factorial:
    # base case
    lw        $t0, 0($sp)
    beq       $t0, 0, return1
 
    addi      $t0, $t0, -1
    # call factorial recursively
    addi      $sp, $sp, -12
    sw        $t0, 0($sp)
    sw        $ra, 8($sp)
    jal       factorial
    lw        $ra, 8($sp)
    lw        $t1, 4($sp)
 
    lw        $t0, 12($sp)
    mul       $t2, $t1, $t0
    # save the product to previous function's return value
    sw        $t2, 16($sp)
    addiu     $sp, $sp, 12
 
    jr        $ra
 
return1:
    li        $t0, 1
    sw        $t0, 4($sp)
    jr        $ra
