#include "minicrt.h"


int fputc(char c, int stream)
{
   if (fwrite(&c, 1, 1, stream) != 1) return EOF;
   return c;
}

int fputs(const char* str, int stream)
{
   int len = strlen(str);
   if (fwrite(str, 1, len, stream) != len) return EOF;
   return len;
}

typedef char* va_list;
#define va_start(ap,arg) (ap=(va_list)&arg+sizeof(arg))
#define va_arg(ap,t) (*(t*)((ap+=sizeof(t))-sizeof(t)))
#define va_end(ap) (ap=(va_list)0)

static int vfprintf(int stream, const char* format, va_list arglist)
{
   int translating = 0;
   int ret = 0;
   const char* p = format;
   for(; *p != '\0'; ++p)
   {
      switch (*p)
      {
      case '%':
         if (!translating) {
            translating = 1;
         }
         else {
            if (fputc('%', stream) < 0) return EOF;
            ++ret;
            translating = 0;
         }
         break;
      case 'd':
         if (translating) {
            // %d
            char buf[16];
            translating = 0;
            itoa(va_arg(arglist, int), buf, 10);
            if (fputs(buf, stream) < 0) return EOF;
            ret += strlen(buf);
         }
         else {
            if (fputc('d', stream) < 0) return EOF;
            ++ret;
         }
         break;
      case 's':
         if (translating) {
            // %s
            const char* str = va_arg(arglist, const char*);
            translating = 0;
            if (fputs(str, stream) < 0) return EOF;
            ret += strlen(str);
         }
         else {
            if (fputc('s', stream) < 0) return EOF;
            ++ret;
         }
      default:
         if (translating) translating = 0;
         if (fputc(*p, stream) < 0) return EOF;
         ++ret;
         break;
      }
   }
   return ret;
}

int printf(const char* format, ...)
{
   va_list arglist;
   va_start(arglist, format);
   return vfprintf(stdout, format, arglist);
}

int fprintf(int stream, const char* format, ...)
{
   va_list arglist;
   va_start(arglist, format);
   return vfprintf(stream, format, arglist);
}
