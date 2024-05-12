%include "stdlib.s"
global _start

section .text
_start:

		mov r14, var_buf
		call main
		mov eax, 1
		mov ebx, 0
		int 80h


x1:
		mov rcx, 0x4008000000000000 
		push rcx
		pop qword [r14 + 96]
		ret


x2:
		mov rcx, 0x4024000000000000 
		push rcx
		pop qword [r14 + 104]
		ret


main:
		pop r15						; get ret adress
		call x1 
		call x2 
		call Input
		push rcx					; get input value
		pop qword [r14 + 16]
		call Input
		push rcx					; get input value
		pop qword [r14 + 24]
		call Input
		push rcx					; get input value
		pop qword [r14 + 32]
		call Input
		push rcx					; get input value
		pop qword [r14 + 40]
		mov rcx, 0x4030000000000000 
		push rcx
		pop qword [r14 + 48]

start_while_1:
		mov r12, 0					; conditional reg
		push qword [r14 + 104]
		push qword [r14 + 96]
		pop r11						; get fisrt cmp reg
		pop r10						; get second cmp reg
		cmp r10, r11
		jg more_0
		jmp logic_end_0

more_0:
		mov r12, 1					; conditional reg

logic_end_0:
		cmp r12, 1
		je or_0
		push qword [r14 + 16]
		push qword [r14 + 24]
		pop r11						; get fisrt cmp reg
		pop r10						; get second cmp reg
		cmp r10, r11
		je equal_0
		jmp logic_end_1

equal_0:
		mov r12, 1					; conditional reg

logic_end_1:
		cmp r12, 1
		je or_0

or_0:
		cmp r12, 1
		jne and_0
		mov r12, 0					; conditional reg
		push qword [r14 + 32]
		mov rcx, 0x4030000000000000 
		push rcx
		pop r11						; get fisrt cmp reg
		pop r10						; get second cmp reg
		cmp r10, r11
		jl less_0
		jmp logic_end_2

less_0:
		mov r12, 1					; conditional reg

logic_end_2:
		cmp r12, 1
		jne and_0

and_0:
		cmp r12, 1
		jne end_while_1
		mov r12, 0					; conditional reg
		mov r12, 0					; conditional reg
		push qword [r14 + 16]
		push qword [r14 + 24]
		pop r11						; get fisrt cmp reg
		pop r10						; get second cmp reg
		cmp r10, r11
		jg more_1
		jmp logic_end_3

more_1:
		mov r12, 1					; conditional reg

logic_end_3:
		cmp r12, 1
		jne end_if_0
		mov r12, 0					; conditional reg
		jmp end_while_1

end_if_0:

start_while_3:
		mov r12, 0					; conditional reg
		push qword [r14 + 32]
		push qword [r14 + 40]
		pop r11						; get fisrt cmp reg
		pop r10						; get second cmp reg
		cmp r10, r11
		jle less_equal_0
		jmp logic_end_4

less_equal_0:
		mov r12, 1					; conditional reg

logic_end_4:
		cmp r12, 1
		jne end_while_3
		mov r12, 0					; conditional reg
		push r15					; save ret adress this func
		push qword [r14 + 32]
		add r14, 40
		call Func 
		sub r14, 40
		pop r15						; get ret adress this func
		push rax					; get return value
		mov rcx, 0x4000000000000000 
		push rcx
		movsd xmm0, qword [rsp + 8]
		movsd xmm1, qword [rsp]
		pop rcx
		mulsd xmm0, xmm1
		movsd qword [rsp], xmm0


		pop qword [r14 + 144]
		add r14, 144
		call Print
		sub r14, 144
		push qword [r14 + 32]
		mov rcx, 0x3ff0000000000000 
		push rcx
		movsd xmm0, qword [rsp + 8]
		movsd xmm1, qword [rsp]
		pop rcx
		addsd xmm0, xmm1
		movsd qword [rsp], xmm0


		pop qword [r14 + 32]
		mov r12, 0					; conditional reg
		push qword [r14 + 32]
		push qword [r14 + 40]
		pop r11						; get fisrt cmp reg
		pop r10						; get second cmp reg
		cmp r10, r11
		je equal_1
		jmp logic_end_5

equal_1:
		mov r12, 1					; conditional reg

logic_end_5:
		cmp r12, 1
		jne end_if_1
		mov r12, 0					; conditional reg
		jmp end_while_3

end_if_1:
		jmp start_while_3

end_while_3:
		mov r12, 0					; conditional reg
		push qword [r14 + 16]
		push qword [r14 + 48]
		pop r11						; get fisrt cmp reg
		pop r10						; get second cmp reg
		cmp r10, r11
		je equal_2
		jmp logic_end_6

equal_2:
		mov r12, 1					; conditional reg

logic_end_6:
		cmp r12, 1
		jne end_if_2
		mov r12, 0					; conditional reg
		jmp start_while_3

end_if_2:
		push qword [r14 + 104]
		pop qword [r14 + 144]
		add r14, 144
		call Print
		sub r14, 144
		push qword [r14 + 96]
		mov rcx, 0x3ff0000000000000 
		push rcx
		movsd xmm0, qword [rsp + 8]
		movsd xmm1, qword [rsp]
		pop rcx
		addsd xmm0, xmm1
		movsd qword [rsp], xmm0


		pop qword [r14 + 96]
		push qword [r14 + 16]
		mov rcx, 0x3ff0000000000000 
		push rcx
		movsd xmm0, qword [rsp + 8]
		movsd xmm1, qword [rsp]
		pop rcx
		addsd xmm0, xmm1
		movsd qword [rsp], xmm0


		pop qword [r14 + 16]
		jmp start_while_1

end_while_1:
		mov rcx, 0x0 
		push rcx
		pop rax						; save return value
		push r15					; get ret adress this func
		ret


Func:
		pop r15						; get ret adress
		pop qword [r14 + 16]
		push qword [r14 + 16]
		mov rcx, 0x4059000000000000 
		push rcx
		movsd xmm0, qword [rsp + 8]
		movsd xmm1, qword [rsp]
		pop rcx
		addsd xmm0, xmm1
		movsd qword [rsp], xmm0


		pop qword [r14 + 144]
		add r14, 144
		call Print
		sub r14, 144
		push qword [r14 + 16]
		mov rcx, 0x4024000000000000 
		push rcx
		movsd xmm0, qword [rsp + 8]
		movsd xmm1, qword [rsp]
		pop rcx
		mulsd xmm0, xmm1
		movsd qword [rsp], xmm0


		pop rax						; save return value
		push r15					; get ret adress this func
		ret


section .data
var_buf: times 144 db 0
