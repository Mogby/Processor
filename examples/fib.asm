
read        ; n

popr 0      ; ; v[0] = n
push fib    ; fib ; v[0] = n
call        ; ; v[0] = fib(n)

pushr 0     ; fib(n)
write       ; print fib(n)

push exit   ; exit
jmp

fib:        ; ; v[0] = n
push 1      ; 1 ; v[0] = n
pushr 0     ; 1 n ; v[0] = n
push ret_1  ; 1 n ret_1 ; v[0] = n
jle         ; ; v[0] = n

pushr 0     ; n ; v[0] = n
dup         ; n n ; v[0] = n
push -1     ; n n -1 ; v[0] = n
add         ; n n-1 ; v[0] = n
popr 0      ; n ; v[0] = n-1

push fib    ; n fib ; v[0] = n-1
call        ; n ; v[0] = fib(n - 1)

popr 1      ; ; v[0] = fib(n - 1) v[1] = n
pushr 0     ; fib(n - 1) ; v[0] = fib(n - 1) v[1] = n

pushr 1     ; fib(n - 1) n ; v[0] = fib(n - 1) v[1] = n
push -2     ; fib(n - 1) n -2 ; v[0] = fib(n - 1) v[1] = n
add         ; fib(n - 1) n-2 ; v[0] = fib(n - 1) v[1] = n
popr 0      ; fib(n - 1) ; v[0] = n-2 v[1] = n

push fib    ; fib(n - 1) ; v[0] = n - 2 v[1] = n
call        ; fib(n - 1) ; v[0] = fib(n - 2)
pushr 0     ; fib(n - 1) fib(n - 2); v[0] = fib(n - 2)
add         ; fib(n) ; v[0] = fib(n - 2)
popr 0      ; ; v[0] = fib(n)
ret

ret_1:      ; ; v[0] = n
push 1      ; 1 ; v[0] = n
popr 0      ; ; v[0] = 1
ret

exit: