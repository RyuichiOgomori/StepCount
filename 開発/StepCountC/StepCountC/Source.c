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

	/* ファイルのオープン */
	if ((fp = fopen_bomcut(filename, "r")) == NULL) {
		fprintf(stderr, "%sのオープンに失敗しました.\n", filename);
		exit(EXIT_FAILURE);
	}

	/* ファイルの終端まで文字を読み取り表示する */
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
	/* ファイルのクローズ */
	fclose(fp);
	printf("全体の行数は%d行でした。\n", all_count);
	printf("コメント行の行数は%d行でした。\n", comment_count);
	printf("ステップ数は全部で%d行でした。\n", (all_count - (comment_count + space_count)));

	return 0;
}

/* コメント開始チェック */
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

/* コメント終了チェック */
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

/* コメント行チェック */
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

/* 空白行チェック */
int IsSpaceLine(char *line) {
	for (int i = 0; line[i] != '\0';i++) {
		if (' ' != line[i] && '　' != line[i] && '\t' != line[i] && '\n' != line[i]) {
			return 0;
		}
	}
	return 1;
}