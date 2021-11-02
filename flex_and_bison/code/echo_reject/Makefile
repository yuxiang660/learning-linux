all: clean
	flex echo_reject.l
	gcc lex.yy.c
	./a.out

clean:
	rm -rf lex.yy.c a.out
