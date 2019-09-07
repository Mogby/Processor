push 1 ; this is a commment
push 1 ; you can use ';' in comments
add
write ; should output '2'

push 10 ; ascii code for '\n'
writech ; should break line

read
read
add
write

push 10 ; ascii code for '\n'
writech ; should break line

push bar ; jumping to bar
jmp

foo: ; writing `foo` label offset
push foo
write

push 10 ; ascii code for '\n'
writech ; should break line

bar: ; writing `bar` label offset
push bar
write

push 10 ; ascii code for '\n'
writech ; should break line

exit: