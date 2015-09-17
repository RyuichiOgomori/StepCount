/*
UTF対応の文字列処理関数ライブラリ
Ver0.12
2014.1.31
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <assert.h>
#include "jstr.h"

// FORCE_JPRINTFがこのファイルのコンパイル時に指定されていても
// 置換を強制的に解除する
#ifdef FORCE_JPRINTF
#undef printf
#undef sprintf
#undef fprintf
#endif


#define BUF_SIZE 4096

// 文字列strの見かけ上の幅を返す
// 見かけ上の幅は、1バイト文字(半角文字)を1、
// 多バイト文字(全角文字)を2として計算した値
int jstrwidth(char *str){
	int size, width = 0;
	while (*str != '\0'){
		size = CheckChrSize(str);
		width += (size > 1) ? 2 : 1;
		str += size;
	}
	return width;
}

// strが指す文字列の先頭1文字の文字コードを返す
// byteには先頭1文字のバイト数が入る
// 呼び出し側でstr+=*byteしながら繰り返し呼ぶことで、文字コードとして
// 一文字ずつ切り出すことができる
unsigned int jstrcode(char *str, int *byte){
	unsigned int c;

	if (*str == '\0') return 0;

	*byte = CheckChrSize(str);
	c = (unsigned char)*str; // 1バイト目を保存
	str++;
	if (*byte == 1) return c;
	c = (c << 8) + (unsigned char)*str; // 2バイト目と合わせる
	str++;
	if (*byte == 2) return c;
	c = (c << 8) + (unsigned char)*str; // 3バイト目と合わせる
	str++;
	return c;
}

// strが指す位置に、文字コードcodeが表す文字を格納する
// その文字コードが何バイト文字であるかをbyteで与える
// 呼び出し側でstr+=byteしてから再呼び出しすることで、連続して文字を格納できる
// ※格納した文字の後は'\0'でターミネートしない。したがって文字列を作る場合は
//   別途'\0'の格納を行う必要がある
void jstrappend(char *str, unsigned int code, int byte){
	if (byte == 1){
		assert(code < 0x100);
		*str = code;
	}
	else if (byte == 2){
		assert(code < 0x10000);
		*str = (code >> 8) & 0xff;
		str++;
		*str = code & 0xff;
	}
	else if (byte == 3){
		assert(code < 0x1000000);
		*str = (code >> 16) & 0xff;
		str++;
		*str = (code >> 8) & 0xff;
		str++;
		*str = code & 0xff;
	}
}


// 文字列*strの先頭から、見かけの幅がなるべくwidthmaxに近い長さになるよう切り出し、
// bufに格納する
// 実際に切り出した見かけの幅を返す
// *strは切り出した部分直後の文字を指すように進められる
int jstrslice(char *buf, char **str, int widthmax){
	int size = 0, width = 0, cwidth, csize;
	char *p = *str;

	while (*p != '\0'){
		csize = CheckChrSize(p);
		cwidth = (csize > 1) ? 2 : 1;
		width += cwidth;
		if (width > widthmax){
			width -= cwidth;
			break;
		}
		else {
			size += csize;
			p += csize;
		}
	}
	memcpy(buf, *str, size);
	*(buf + size) = '\0';
	*str += size;
	return width;
}

// 呼び出し側のstrポインタ値を破壊しないstrslice
int jstrsub(char *buf, char *str, int widthmax){
	return jstrslice(buf, &str, widthmax);
}

int jsprintf(char *str, char *fmt, ...){
	va_list ap;
	int bytes;

	va_start(ap, fmt);
	bytes = jvsprintf(str, fmt, ap);
	va_end(ap);
	return bytes;
}

int jvsprintf(char *str, char *fmt, va_list ap){
	char *hp, *tp, *pp = NULL, *sp, *argstr;
	char buf[BUF_SIZE];
	int in_conv = 0, i;
	int flushleft = 0, wmin, wmax, len, width;

	hp = tp = fmt;
	sp = str;

	while (*tp != '\0'){
		if (in_conv){
			if (isalpha(*tp)){
				memcpy(buf, hp, tp - hp + 1);
				buf[tp - hp + 1] = '\0';
				switch (*tp){
				case 'd':
				case 'i':
				case 'o':
				case 'x':
				case 'X':
				case 'u':
				case 'c':
					sp += sprintf(sp, buf, va_arg(ap, int));
					break;
				case 'f':
				case 'e':
				case 'E':
				case 'g':
				case 'G':
					sp += sprintf(sp, buf, va_arg(ap, double));
					break;
				case 'p':
					sp += sprintf(sp, buf, va_arg(ap, void *));
					break;
				case 's':
					wmin = 0;
					wmax = 99999;
					buf[pp - hp] = '\0';
					sp += sprintf(sp, buf);
					pp++;
					if (*pp == '-'){
						flushleft = 1;
						pp++;
					}
					if (isdigit(*pp)){
						wmin = atoi(pp);
						while (isdigit(*pp)){ pp++; }
					}
					if (*pp == '.'){
						pp++;
						if (isdigit(*pp)){
							wmax = atoi(pp);
						}
					}
					argstr = va_arg(ap, char *);
					len = strlen(argstr);
					width = jstrwidth(argstr);
					if (width < wmin){
						if (flushleft){
							strcpy(sp, argstr);
							sp += len;
						}
						for (i = 0; i < wmin - width; i++){
							*sp = ' ';
							sp++;
						}
						if (!flushleft){
							strcpy(sp, argstr);
							sp += len;
						}
					}
					// 文字列が最大文字数より短い場合
					else if (width < wmax){
						strcpy(sp, argstr);
						sp += len;
						*sp = '\0';
					}
					// 文字列が最大文字数より長い場合は打ち切り
					// 日本語文字が境界を跨ぐときは短めに切る
					else {
						width = jstrsub(buf, argstr, wmax);
						if (flushleft){
							strcpy(sp, buf);
							sp += strlen(buf);
						}
						// 境界の日本語文字を打ちきったときは空白で調整
						if (width < wmax){
							*sp = ' ';
							sp++;
						}
						if (!flushleft){
							strcpy(sp, buf);
							sp += strlen(buf);
						}
					}
					break;
				}
				hp = tp = tp + 1;
				in_conv = 0;
			}
			else if (*tp == '%'){
				*sp = '%';
				sp++;
				hp = tp = tp + 1;
				in_conv = 0;
			}
			else {
				tp++;
			}
		}
		else {
			if (*tp == '%'){
				pp = tp;
				tp++;
				in_conv = 1;
			}
			else {
				tp++;
			}
		}
	}
	sp += sprintf(sp, hp);
	return sp - str;
}


int jprintf(char *fmt, ...){
	char buf[BUF_SIZE];
	va_list ap;
	int bytes;

	va_start(ap, fmt);
	bytes = jvsprintf(buf, fmt, ap);
	va_end(ap);
	fputs(buf, stdout);
	return bytes;
}

int jfprintf(FILE *fp, char *fmt, ...){
	char buf[BUF_SIZE];
	va_list ap;
	int bytes;

	va_start(ap, fmt);
	bytes = jvsprintf(buf, fmt, ap);
	va_end(ap);
	fputs(buf, fp);
	return bytes;
}

#ifdef JCODING_UTF8
FILE *fopen_bomcut(const char *path, const char *mode){
	FILE *fp;
	char buf[4];

	fp = fopen(path, mode);
	if (fp != NULL){
		if (fread(buf, 1, 3, fp) == 3){
			//printf("%x%x%x\n", buf[0], buf[1], buf[2]);
			if (memcmp(buf, "\xef\xbb\xbf", 3) == 0){ // BOMつき
				//printf("Has BOM\n");
			}
			else {
				rewind(fp);
			}
		}
	}
	return fp;
}
#endif
