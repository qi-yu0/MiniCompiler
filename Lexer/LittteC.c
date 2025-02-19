#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXFILENAME 100 // 文件名的最大长�?
#define MAXSIZE 50
#define MAXTABLE 20 // 符号表的最大长�?
#define MAX_BUFFER_SIZE 200 //缓冲区的最大�?
typedef struct Node {
	char morpheme[MAXSIZE];
	int token;
} TOKEN;

static int i =
    1;       // i为源语言字符数组str的下标。不知道是什么原因，str[0]是乱码，所以从1开�?
char *keyword[MAXTABLE] = {"while", "do", "if", "then", "else", "int", "bool", "true", "false", "read", "write"}; // 该语言定义的关键字
int get_file_len(char *file_path);
int lex_analysis(char *str, char *out_file_name);
int isKeyword(char *str, char **symbol);
TOKEN *getInt(char *str);
TOKEN *getIdOrKeyword(char *str);



int lex_analysis(char *str, char *out_file_name) {
	//int i = 0;
	char ch;

	TOKEN *t = (TOKEN *)malloc(sizeof(TOKEN));
	TOKEN signtable[MAXTABLE];
	// init(keyword,signtable); //符号表初始化

	FILE *out = fopen(out_file_name, "w");
	if (out == NULL) {
		perror("File opening failure!");
		return -1;
	}
	int s = 0; //str指向的字符串的长�?
	while (str[s] != '\0')
		s++;


	while (i < s) {

		ch = str[i];

		while (ch == ' ' || ch == '\n' || ch == '\t') {
			++i;
			ch = str[i];
		}

		if (isdigit(ch) || ch == '-') {
			t = getInt(str);
		} else if (isalpha(ch)) {
			t = getIdOrKeyword(str);
		} else {
			switch (ch) {
				case '+':
					/* code */
				{
					strcpy(t->morpheme, "+");
					t->token = 14;
					break;
				}
				case '*': {
					strcpy(t->morpheme, "*");
					t->token = 16;

					break;
				}
				case '/': {

					if (str[i + 1] == '/') {
						++i;
						strcpy(t->morpheme, "//");
						t->token = 35;
					} else {
						strcpy(t->morpheme, "/");
						t->token = 17;
					}
					break;
				}
				case '|': {
					++i;
					if (str[i] == '|') {

						strcpy(t->morpheme, "||");
						t->token = 18;
					} else
						perror("85match failure!");
					break;
				}
				case '&': {
					++i;
					if (str[i] == '&') {

						strcpy(t->morpheme, "&&");
						t->token = 19;
					} else
						perror("95match failure!");
					break;
				}
				case '!': {
					if (str[i + 1] == '=') {
						++i;

						strcpy(t->morpheme, "!=");
						t->token = 26;
					} else {

						strcpy(t->morpheme, "!");
						t->token = 20;
					}
					break;
				}
				case '<': {
					if (str[i + 1] == '=') {
						++i;

						strcpy(t->morpheme, "<=");
						t->token = 23;
					} else {

						strcpy(t->morpheme, "<");
						t->token = 21;
					}
					break;
				}
				case '>': {
					if (str[i + 1] == '=') {
						++i;

						strcpy(t->morpheme, ">=");
						t->token = 24;
					} else {

						strcpy(t->morpheme, ">");
						t->token = 22;
					}
					break;
				}
				case '=': {
					if (str[i + 1] == '=') {
						++i;

						strcpy(t->morpheme, "==");
						t->token = 25;
					} else {

						strcpy(t->morpheme, "=");
						t->token = 28;
					}
					break;
				}
				case ':': {

					if (str[i + 1] == '=') {
						++i;
						strcpy(t->morpheme, ":=");
						t->token = 27;
					} else {
						strcpy(t->morpheme, ":");
						t->token = 36;
					}

					break;
				}
				case '(': {

					strcpy(t->morpheme, "(");
					t->token = 29;
					break;
				}
				case ')': {

					strcpy(t->morpheme, ")");
					t->token = 30;
					break;
				}
				case '{': {

					strcpy(t->morpheme, "{");
					t->token = 31;

					break;
				}
				case '}': {
					strcpy(t->morpheme, "}");
					t->token = 32;
					break;
				}
				case ',': {
					strcpy(t->morpheme, ",");
					t->token = 33;
					break;
				}
				case ';': {
					strcpy(t->morpheme, ";");
					t->token = 34;
					break;
				}
				case '.': {
					strcpy(t->morpheme, ".");
					t->token = 37;
					break;
				}

				default:

					perror("195match failure!\n");
					break;
			}
		}
		i++;
		//printf("%s  %d\n", t->morpheme, t->token);
		fprintf(out, "[%-20s %10d]\n", t->morpheme, t->token);



	}
	if (fclose(out) != 0) {
		perror("fclose");
		exit(EXIT_FAILURE);
	}

}

TOKEN *getInt(char *str) {
	int j = i;
	char integer[MAXSIZE];
	int count = 0;
	char pre;
	TOKEN *t = (TOKEN *)malloc(sizeof(TOKEN));
	if (str[j] == '-') { //�?-' 前的第一个非空字�?
		int previous = j;
		while (str[previous] == ' ')
			previous++;
		pre = str[previous];
	}

	if (str[j] == '-' && (!isdigit(str[j + 1]) || isdigit(pre) || pre == ')'
	                      || isalpha(pre))) { //此时'-'为减�?
		++j;
		strcpy(t->morpheme, "-");
		t->token = 15;
	} else {
		if (str[j] == '-') {
			integer[count++] = '-';
			++j;
		}

		if (str[j] == '0') {

			integer[count++] = '0';

		} else if (str[j] <= '9' && str[j] >= '1') {
			do {
				/* code */
				integer[count++] = str[j++];
			} while (isdigit(str[j]));
		} else {

			perror("246match failure!");
		}
		integer[count] = '\0';

		strcpy(t->morpheme, integer);

		t->token = 2;
	}
	i = j;
	return t;
}

TOKEN *getIdOrKeyword(char *str) {
	int j, c = 0;
	j = i;
	int id_code;
	char string[MAXSIZE];
	TOKEN *t = (TOKEN *)malloc(sizeof(TOKEN));


	if (isalpha(str[j])) {

		string[c++] = str[j++];
		while (isalpha(str[j]) || isdigit(str[j])) {
			string[c++] = str[j++];
		}
		string[c] = '\0';
		strcpy(t->morpheme, string);

		if ((id_code = isKeyword(string, keyword)) > 0) {

			t->token = id_code;

		} else
			t->token = 1;

	}

	i = j;

	return t;
}

int isKeyword(char *str, char **symbol) {
	int j = 0;



	while (isalpha(symbol[j][0])) {

		if (strcmp(symbol[j], str))
			return j + 3;
		else
			++j;
	}

	return -1;
}

int get_file_len(char *file_path) {
	int f_len = 0;
	if (file_path == NULL) {
		perror("NO Such File");
		return -1;
	}

	FILE *fp;
	fp = fopen(file_path, "rb");
	if (fp == NULL)
		return f_len;

	fseek(fp, 0, SEEK_END); // 先将指针偏移到文件尾
	f_len = ftell(fp);

	fclose(fp);

	return f_len;
}

int main() {
	int file_len;
	char *str;
	char buffer[MAX_BUFFER_SIZE];
	char source_file[MAXFILENAME] =
	    "D:\\CoderLife\\src_c\\Compilation_principle\\chapter_two\\sourceProgram4.txt"; // 源文件名
	char out_file_name[MAXFILENAME] =
	    "D:\\CoderLife\\src_c\\Compilation_principle\\chapter_two\\out.txt";          // 待写入文件名

	file_len = get_file_len(source_file);        // 获取文件长度
	FILE *file_stream = fopen(source_file, "r"); // 打开文件，获取文件流
	if (file_len > 0)
		str = (char *)malloc(file_len * sizeof(char)); // 为字符串分配长度为file_len的空�?

	while (fgets(buffer, MAX_BUFFER_SIZE, file_stream) != NULL) { // 将文件流中的数据读入str数组中去
		strcat(str, buffer);

	}



	if (fclose(file_stream) != 0) { 	 // 操作结束，关闭文�?
		perror("fclose");
		exit(EXIT_FAILURE);
	}

	lex_analysis(str, out_file_name);  // 对str数组进行词法分析，结果写入文件名为out的文件中�?

	return 0;
}