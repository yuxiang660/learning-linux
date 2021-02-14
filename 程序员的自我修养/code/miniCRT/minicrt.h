#pragma once

//malloc
#ifndef NULL
#define NULL (0)
#endif

void free(void *ptr);
void* malloc(unsigned size);
int mini_crt_heap_init();


// string
int strcmp(const char* src, const char* dst);
unsigned strlen(const char* str);
char* strcpy(char* dst, const char* src);
char* itoa(int n, char* str, int radix);


//stdio
#define EOF (-1)
#define stdin 0
#define stdout 1
#define stderr 2

typedef int FILE;
int mini_crt_io_init();
int fopen(const char* filename, const char* mode);
int fread(void* buffer, int size, int count, int stream);
int fwrite(const void* buffer, int size, int count, int stream);
int fclose(int fp);
int fseek(int fp, int offset, int set);


// printf
int fputc(char c, int stream);
int fputs(const char* str, int stream);
int printf(const char* format, ...);
int fprintf(int stream, const char* format, ...);
