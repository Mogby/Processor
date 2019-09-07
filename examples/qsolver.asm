; start of program
main:
    read
    popr 0

    read
    popr 1

    read
    popr 2          ; ; v[0] = a, v[1] = b, v[2] = c

    push qsolver
    call            ; ; v[0] = root1, v[1] = root2, v[2] = num_roots

    pushr 2
    push 0
    push no_roots
    je

    pushr 2
    push -1
    push inf_roots
    je

    pushr 2
    push 1
    push finish
    jg              ; exit if num_roots = 0

    pushr 1
    pushr 2         ; root2 num_roots ; v[0] = root1
    push write_real
    call

    push 2
    push finish     ; root2 num_roots 2 finish
    jg              ; exit if num_roots = 1

    popr 0
    push write_real
    call

    push finish
    jmp

; qsolver function
qsolver:            ; ; v[0] = a, v[1] = b, v[2] = c
    pushr 0
    push 0
    push linsolver  ; a 0 linsolver
    je

    push 4
    pushr 0
    mul
    pushr 2
    mul             ; 4ac
    pushr 1
    pushr 1
    mul             ; 4ac b^2
    sub             ; b^2-4ac
    dup
    popr 3          ; b^2-4ac ; v[0] = a, v[1] = b, v[2] = c, v[3] = b^2-4ac

    push 0
    push has_roots  ; b^2-4ac 0 one_root
    jle

    push 0
    popr 2
    ret             ; no roots

has_roots:          ; ; v[0] = a, v[1] = b, v[2] = c, v[3] = b^2-4ac
    pushr 3
    push 100
    mul
    sqrt
    popr 3          ; ; v[0] = a, v[1] = b, v[2] = c, v[3] = 10*sqrt(b^2-4ac)

    pushr 3
    push 0
    push one_root
    je

    pushr 3
    push 0
    push two_roots
    jl

    push 0
    popr 2          ; ; v[2] = 0
    ret

one_root:           ; ; v[0] = a, v[1] = b, v[2] = c, v[3] = 10*sqrt(b^2-4ac)
    pushr 0
    push -5
    pushr 1
    mul
    div
    popr 0
    push 1
    popr 2           ; ; v[0] = -10b/(2a) = -5b/a, v[2] = 1
    ret

two_roots:          ; ; v[0] = a, v[1] = b, v[2] = c, v[3] = 10*sqrt(b^2-4ac)
    pushr 0
    dup

    push 2
    pushr 3
    div

    push -5
    pushr 1         ; a a 5*sqrt(b^2-4ac) -5b
    mul

    sub
    div
    popr 0          ; a ; v[0] = root1, v[1] = b, v[2] = c, v[3] = 10*sqrt(b^2-4ac)

    push -2
    pushr 3
    div

    push -5
    pushr 1
    mul

    sub
    div
    popr 1          ; ; v[0] = root1, v[1] = root2

    push 2
    popr 2

    ret

linsolver:          ; ; v[0] = 0, v[1] = b, v[2] = c
    pushr 1
    push 0
    push linsolver_b0
    je

    pushr 1
    pushr 2
    push 10
    mul             ; b 10*c
    div             ; 10*c/b

    push -1
    mul             ; -10*c/b
    popr 0          ; ; v[0] = -10*c/b
    push 1
    popr 2
    ret

linsolver_b0:       ; ; v[0] = 0, v[1] = 0, v[2] = c
    pushr 2
    push 0
    push linsolver_b0_c0
    je
    push 0
    popr 2
    ret

linsolver_b0_c0:    ; ; v[0] = 0, v[1] = 0, v[2] = 0
    push -1
    popr 2
    ret

; write_real function
write_real:         ; ; v[0] = num
    push 0
    pushr 0
    push write_real_ge0
    jge

    push 45
    writech         ; '-' sign

    pushr 0
    push -1
    mul
    popr 0          ; ; v[0] = -num

write_real_ge0:     ; ; v[0] = num, num >= 0
    push 10
    pushr 0
    div
    write           ; integer part

    push 46
    writech         ; decimal point

    push 10
    pushr 0
    mod
    write           ; digit after point

    push 10
    writech         ; break line

    ret

no_roots:
    push 101        ; 'e'
    push 110        ; 'n'
    push 111        ; 'o'
    push 110        ; 'n'
    writech
    writech
    writech
    writech         ; 'none'
    push 10
    writech         ; break line
    push finish
    jmp

inf_roots:
    push 102        ; 'f'
    push 110        ; 'n'
    push 105        ; 'i'
    writech
    writech
    writech         ; 'inf'
    push 10
    writech         ; break line
    push finish
    jmp

; end of program
finish: