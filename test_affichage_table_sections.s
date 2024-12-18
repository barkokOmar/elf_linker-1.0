.global _start

_start:
    ldr r0, =num1
    ldr r1, =num2
    add r0, r0, r1

    ldr r1, =resultat
    str r0, [r1]

    mov r0, #1
    ldr r1, =message
    ldr r2, =len
    mov r7, #4
    svc 0

    mov r0, #0
    mov r7, #1
    svc 0

.data
num1: .word 10
num2: .word 5
resultat: .word 0
message: .asciz "La somme est: 15 \n"
len = . - message

