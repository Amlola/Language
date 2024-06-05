Print:
    movsd xmm0, [r14]

    mov rax, 1000
    cvtsi2sd xmm1, rax
    mulsd xmm0, xmm1
    cvtsd2si rax, xmm0 

    xor r8, r8
    cmp rax, 0
    jge .Pozitive
    neg rax
    mov r8, 1

.Pozitive:
    mov rbp, r14
    mov rsi, rbp

    sub rsi, 3

    mov rbx, 10
    mov rdi, rax

.loop1:
    xor rdx, rdx
    div rbx
    add dl, '0'
    mov byte [rsi], dl
    dec rsi
    cmp rax, 0
    jne .loop1

    mov al, byte [rbp - 3]
    mov ah, byte [rbp - 4]
    mov bl, byte [rbp - 5]

    cmp rdi, 1000
    jge .Get_After_dor
    mov byte [rbp - 6], '0'
    dec rsi

.Get_After_dor:
    mov byte [rbp - 5], '.'
    mov byte [rbp - 4], bl
    mov byte [rbp - 3], ah
    mov byte [rbp - 2], al
    mov byte [rbp - 1], 10 

    cmp r8, 1
    jne .NotMinus
    mov byte [rsi], '-'
    dec rsi

.NotMinus:
    inc rsi
    mov rdx, rbp
    sub rdx, rsi
    mov rax, 1
    mov rdi, 1
    syscall

    ret


Input:
    mov rsi, var_buf 
    xor rax, rax
    xor rdi, rdi
    mov rdx, 16
    syscall

    xor rax, rax   
    mov rbx, 10     
    xor rdx, rdx    
    xor r8, r8   
    xor r9, r9     
    xor rcx, rcx  

.While:
    cmp byte [rsi], '-'
    je .Minus

    cmp byte [rsi], '.'
    je .Dot

    cmp byte [rsi], '0'
    jl .Break

    cmp r8, 0
    je .BeforeDot
    inc rcx

.BeforeDot:
    push rcx
    mov rcx, rdx
    mov cl, byte [rsi]
    sub cl, '0'        
    mul rbx          
    mov rdx, rcx
    pop rcx
    add rax, rdx    
    inc rsi
    jmp .While

.Minus:
    inc r9
    inc rsi
    jmp .While

.Dot:
    inc r8
    inc rsi
    jmp .While

.Break:
    cmp r9, 1
    jne .Positive1
    neg rax

.Positive1:
    cvtsi2sd xmm1, rax

    cmp r8, 0
    je .return

    mov rax, 1

.Ranks:
    mul rbx
    loop .Ranks

.LastDigit:
    cvtsi2sd xmm0, rax
    divsd xmm1, xmm0

.return:
    sub rsp, 8
    movsd [rsp], xmm1
    pop rcx

    ret