all: s_t m_t

s_t:
	gcc pthread.c -o main
	./main

m_t:
	gcc pthread.c -lpthread -o main
	./main

clean:
	@rm -rf main

