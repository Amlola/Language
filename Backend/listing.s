%include "stdlib.s"
global _start

section .text
_start:

		mov r14, var_buf
		call main
		mov eax, 1
		mov ebx, 0
		int 80h


main:
		pop r15						; get ret adress
		call Input
		push rcx					; get input value
		pop qword [r14 + 16]
		push r15					; save ret adress this func
		push qword [r14 + 16]
		add r14, 8
		call Factorial 
		sub r14, 8
		pop r15						; get ret adress this func
		push rax					; get return value
		pop qword [r14 + 48]
		add r14, 48
		call Print
		sub r14, 48
		mov rcx, 0x0 
		push rcx
		pop rax						; save return value
		push r15					; get ret adress this func
		ret


Factorial:
		pop r15						; get ret adress
		pop qword [r14 + 16]
		mov r12, 0					; conditional reg
		push qword [r14 + 16]
		mov rcx, 0x3ff0000000000000 
		push rcx
		pop r11						; get fisrt cmp reg
		pop r10						; get second cmp reg
		cmp r10, r11
		je equal_0
		jmp logic_end_0

equal_0:
		mov r12, 1					; conditional reg

logic_end_0:
		cmp r12, 1
		jne end_if_0
		mov r12, 0					; conditional reg
		mov rcx, 0x3ff0000000000000 
		push rcx
		pop rax						; save return value
		push r15					; get ret adress this func
		ret


end_if_0:
		push r15					; save ret adress this func
		push qword [r14 + 16]
		mov rcx, 0x3ff0000000000000 
		push rcx
		movsd xmm0, qword [rsp + 8]
		movsd xmm1, qword [rsp]
		pop rcx
		subsd xmm0, xmm1
		movsd qword [rsp], xmm0


		add r14, 8
		call Factorial 
		sub r14, 8
		pop r15						; get ret adress this func
		push rax					; get return value
		push qword [r14 + 16]
		movsd xmm0, qword [rsp + 8]
		movsd xmm1, qword [rsp]
		pop rcx
		mulsd xmm0, xmm1
		movsd qword [rsp], xmm0


		pop rax						; save return value
		push r15					; get ret adress this func
		ret


section .data
var_buf: times 48 db 0
