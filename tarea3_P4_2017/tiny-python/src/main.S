#include "screen.h"
#include "system.h"
.global main

.data
enter: .asciz "\n"
quo_xyz: .word 0
rem_xyz: .word 0
str_0: .asciz "x[2] = "
str_1: .asciz "x[1] = "
str_2: .asciz "x[0] = "
str_3: .asciz "x[3] = "
x: .space 40

.text

main:
li $a0, BRIGHT_WHITE
li $a1, BLACK
jal set_color
jal clear_screen
jal keypad_init

li $t0, 2
move $a0, $t0
li $a1, 4
jal mult
li $t0, 1
la $t1, x
addu $t1, $t1, $v0
sw $t0, ($t1)
li $t0, 1
move $a0, $t0
li $a1, 4
jal mult
li $t0, 2
la $t1, x
addu $t1, $t1, $v0
sw $t0, ($t1)
la $t0, str_0
move $a0, $t0
jal puts
li $t0, 2
move $a0, $t0
li $a1, 4
jal mult
la $t0, x
addu $v0, $v0, $t0
lw $t0, 0($v0)
move $a0, $t0
jal put_udecimal
la $a0, enter
jal puts
la $t0, str_1
move $a0, $t0
jal puts
li $t0, 1
move $a0, $t0
li $a1, 4
jal mult
la $t0, x
addu $v0, $v0, $t0
lw $t0, 0($v0)
move $a0, $t0
jal put_udecimal
la $a0, enter
jal puts
la $t0, str_2
move $a0, $t0
jal puts
li $t0, 0
move $a0, $t0
li $a1, 4
jal mult
la $t0, x
addu $v0, $v0, $t0
lw $t0, 0($v0)
move $a0, $t0
jal put_udecimal
la $a0, enter
jal puts
la $t0, str_3
move $a0, $t0
jal puts
li $t0, 3
move $a0, $t0
li $a1, 4
jal mult
la $t0, x
addu $v0, $v0, $t0
lw $t0, 0($v0)
move $a0, $t0
jal put_udecimal
la $a0, enter
jal puts

jr $ra
