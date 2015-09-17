/*
UTF�Ή��̕����񏈗��֐����C�u����
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

// FORCE_JPRINTF�����̃t�@�C���̃R���p�C�����Ɏw�肳��Ă��Ă�
// �u���������I�ɉ�������
#ifdef FORCE_JPRINTF
#undef printf
#undef sprintf
#undef fprintf
#endif


#define BUF_SIZE 4096

// ������str�̌�������̕���Ԃ�
// ��������̕��́A1�o�C�g����(���p����)��1�A
// ���o�C�g����(�S�p����)��2�Ƃ��Čv�Z�����l
int jstrwidth(char *str){
	int size, width = 0;
	while (*str != '\0'){
		size = CheckChrSize(str);
		width += (size > 1) ? 2 : 1;
		str += size;
	}
	return width;
}

// str���w��������̐擪1�����̕����R�[�h��Ԃ�
// byte�ɂ͐擪1�����̃o�C�g��������
// �Ăяo������str+=*byte���Ȃ���J��Ԃ��ĂԂ��ƂŁA�����R�[�h�Ƃ���
// �ꕶ�����؂�o�����Ƃ��ł���
unsigned int jstrcode(char *str, int *byte){
	unsigned int c;

	if (*str == '\0') return 0;

	*byte = CheckChrSize(str);
	c = (unsigned char)*str; // 1�o�C�g�ڂ�ۑ�
	str++;
	if (*byte == 1) return c;
	c = (c << 8) + (unsigned char)*str; // 2�o�C�g�ڂƍ��킹��
	str++;
	if (*byte == 2) return c;
	c = (c << 8) + (unsigned char)*str; // 3�o�C�g�ڂƍ��킹��
	str++;
	return c;
}

// str���w���ʒu�ɁA�����R�[�hcode���\���������i�[����
// ���̕����R�[�h�����o�C�g�����ł��邩��byte�ŗ^����
// �Ăяo������str+=byte���Ă���ČĂяo�����邱�ƂŁA�A�����ĕ������i�[�ł���
// ���i�[���������̌��'\0'�Ń^�[�~�l�[�g���Ȃ��B���������ĕ���������ꍇ��
//   �ʓr'\0'�̊i�[���s���K�v������
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


// ������*str�̐擪����A�������̕����Ȃ�ׂ�widthmax�ɋ߂������ɂȂ�悤�؂�o���A
// buf�Ɋi�[����
// ���ۂɐ؂�o�����������̕���Ԃ�
// *str�͐؂�o������������̕������w���悤�ɐi�߂���
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

// �Ăяo������str�|�C���^�l��j�󂵂Ȃ�strslice
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
					// �����񂪍ő啶�������Z���ꍇ
					else if (width < wmax){
						strcpy(sp, argstr);
						sp += len;
						*sp = '\0';
					}
					// �����񂪍ő啶������蒷���ꍇ�͑ł��؂�
					// ���{�ꕶ�������E���ׂ��Ƃ��͒Z�߂ɐ؂�
					else {
						width = jstrsub(buf, argstr, wmax);
						if (flushleft){
							strcpy(sp, buf);
							sp += strlen(buf);
						}
						// ���E�̓��{�ꕶ����ł��������Ƃ��͋󔒂Œ���
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
			if (memcmp(buf, "\xef\xbb\xbf", 3) == 0){ // BOM��
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
