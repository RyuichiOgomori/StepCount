/*
UTF‘Î‰‚Ì•¶š—ñˆ—ŠÖ”ƒ‰ƒCƒuƒ‰ƒŠ
Ver0.12
2014.1.31
*/
#ifndef _JSTR_H
#define _JSTR_H

#include <stdio.h>
#include <stdarg.h>

#ifdef FORCE_JPRINTF
#define printf jprintf
#define sprintf jsprintf
#define fprintf jfprintf
#endif

// w’è‚ª‚È‚¯‚ê‚ÎUTF-8‚Æ‚·‚é
#if !defined(JCODING_EUC) && !defined(JCODING_SJIS) &&!defined(JCODING_UTF8)
#define JCODING_UTF8
#endif

#if defined(JCODING_EUC)
#define CheckChrSize(p)                         \
    ((*((unsigned char *)p) & 0x80) ? 2 : 1)
#elif defined(JCODING_SJIS)
#define CheckChrSize(p)                         \
    (((0x81 <= *((unsigned char *)p) && *((unsigned char *)p) <= 0x9f) ||\
      (0xe0 <= *((unsigned char *)p) && *((unsigned char *)p) <= 0xfc)) ? 2 : 1)
#elif defined(JCODING_UTF8)
#define CheckChrSize(p)                         \
    (((*((unsigned char *)p) & 0xf0) == 0xe0) ? 3 : 1)
#endif

#ifdef JCODING_UTF8
#define JBufSize(l) ((l)*3/2)
#else
#define JBufSize(l)  (l)
#endif


int jstrwidth(char *str);
unsigned int jstrcode(char *str, int *byte);
void jstrappend(char *str, unsigned int code, int byte);
int jstrslice(char *buf, char **str, int widthmax);
int jstrsub(char *buf, char *str, int widthmax);
int jsprintf(char *str, char *fmt, ...);
int jvsprintf(char *str, char *fmt, va_list ap);
int jprintf(char *fmt, ...);
int jfprintf(FILE *fp, char *fmt, ...);

#ifdef JCODING_UTF8
FILE *fopen_bomcut(const char *path, const char *mode);
#else
#define fopen_bomcut fopen
#endif

#endif


