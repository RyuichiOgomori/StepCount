/* header files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jstr.h"

/* macros */
#define N 256

int IsStartCommentLine(char *line);

/* main */
int main(int argc, char *argv[]) {
	FILE *fp;
	char *filename = argv[1];
	char readline[N] = { '\0' };
	int comment_count = 0;
	int all_count = 0;
	int space_count = 0;

	/* �t�@�C���̃I�[�v�� */
	if ((fp = fopen_bomcut(filename, "r")) == NULL) {
		fprintf(stderr, "%s�̃I�[�v���Ɏ��s���܂���.\n", filename);
		exit(EXIT_FAILURE);
	}

	/* �t�@�C���̏I�[�܂ŕ�����ǂݎ��\������ */
	while (fgets(readline, N, fp) != NULL) {
		all_count++;
		if (IsStartCommentLine(readline) == 1){
			comment_count++;
			while (IsEndCommentLine(readline) == 0){
				if (fgets(readline, N, fp) == NULL){
					break;
				}
				all_count++;
				comment_count++;
			}
		}else if (IsCommentLine(readline) == 1){
			comment_count++;
		}else if (IsSpaceLine(readline) == 1){
			space_count++;
		}
	}
	/* �t�@�C���̃N���[�Y */
	fclose(fp);
	printf("�S�̂̍s����%d�s�ł����B\n", all_count);
	printf("�R�����g�s�̍s����%d�s�ł����B\n", comment_count);
	printf("�X�e�b�v���͑S����%d�s�ł����B\n", (all_count - (comment_count + space_count)));

	return 0;
}

/* �R�����g�J�n�`�F�b�N */
int IsStartCommentLine(char *line) {
	char comment_pre[] = "//";
	int i;

	for (i = 0;line[i] != '\0'; i++) {
		if (' ' != line[i]) {
			if (('/' == line[i] && '*' == line[i + 1])
				|| ('<' == line[i] && '!' == line[i + 1] && '-' == line[i + 2] && '-' == line[i + 3])){
				return 1;
			}
		}
	}
	return 0;
}

/* �R�����g�I���`�F�b�N */
int IsEndCommentLine(char *line) {
	int i;
	for (i = strlen(line);i > 0; i--) {
		if (('/' == line[i] && '*' == line[i - 1])
			|| ('>' == line[i] && '-' == line[i - 1] && '-' == line[i - 2])) {
			return 1;
		}
	}
	return 0;
}

/* �R�����g�s�`�F�b�N */
int IsCommentLine(char *line) {
	int i;
	for (i = 0;line[i] != '\0'; i++) {
		if (' ' != line[i]) {
			if (('/' == line[i] && '/' == line[i + 1])) {
				return 1;
			}
			else {
				return 0;
			}
		}
	}
	return 0;
}

/* �󔒍s�`�F�b�N */
int IsSpaceLine(char *line) {
	for (int i = 0; line[i] != '\0';i++) {
		if (' ' != line[i] && '�@' != line[i] && '\t' != line[i] && '\n' != line[i]) {
			return 0;
		}
	}
	return 1;
}