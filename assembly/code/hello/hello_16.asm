org 100h
mov ah,9          ;xxxx:0126 Function 09 of Int 21h: write string to standard output
mov dx,msg        ;xxxx:0128 DS:DX -> $-terminated string, see 102 address
int 21h           ;xxxx:012B Write String to STD Output.
mov ah,0          ;xxxx:012D Function 00 of Int 21h: terminate program
int 21h           ;xxxx:012F Terminate Program.

msg: db 0x0d,0x0a,'HELLO WORLD',0x0d,0x0a,'$'   ; "0d,0a" is "\r\n", "$" means end of string