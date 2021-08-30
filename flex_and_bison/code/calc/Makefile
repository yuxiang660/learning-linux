fb1-3:
	flex fb1-3.l
	cc lex.yy.c -lfl
	./a.out

fb1-4:
	flex fb1-4.l
	cc lex.yy.c -lfl
	./a.out

fb1-5: fb1-5.l fb1-5.y
	bison -d fb1-5.y
	flex fb1-5.l
	cc fb1-5.tab.c lex.yy.c -lfl

clean:
	@rm -rf a.out lex.yy.c fb1-5.tab.c fb1-5.tab.h
