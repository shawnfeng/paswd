#include "secret.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <conio.h>
#include <Windows.h>
#include <string>
using std::string;
enum {
	NREFRESH = 10,
};
int secret(FILE *fp_in, FILE *fp_out, char (*mask_code)[PASSWD_BUF_LEN],
           const int code_num, const int once_pro)
{
	int i, j;
	int mask_code_len = 0;
	char *buf = (char *)malloc(sizeof(char) * once_pro);
	int tmp_id = 0;
	unsigned long file_count, been_del;
	int rd_num = 0;
	int file_index = 0;
	int refresh_p = NREFRESH;

	fseek(fp_in, 0, SEEK_END);
	file_count = ftell(fp_in);
	printf("文件大小为：%lg MB\n", (double)file_count / (1024.0 * 1024.0));
	rewind(fp_in);

	been_del = ftell(fp_in);
	been_del = (unsigned long)((double)been_del / (double)file_count * 100.0);
	printf("\b\b\b%ld%%",been_del);

	while (0 != (rd_num = (int)fread(buf, sizeof(char), once_pro, fp_in))) {
		for (i = 0; i < code_num; i++) {
			mask_code_len = (int)strlen(mask_code[i]);
			tmp_id = file_index % mask_code_len;
			for (j = 0; j < once_pro; j++) { 
				buf[j] ^= mask_code[i][tmp_id];
				tmp_id = ++tmp_id % mask_code_len;
			}
		}
		file_index = ftell(fp_in);

		if (NULL == fp_out) {
			fseek(fp_in, -rd_num, SEEK_CUR);
			fwrite(buf, sizeof(char), rd_num, fp_in);
			fflush(fp_in);	
		} else {
			fwrite(buf, sizeof(char), rd_num, fp_out);
			fflush(fp_out);	
		}	

		been_del = ftell(fp_in);
		been_del = (unsigned long)((double)been_del / (double)file_count * 100.0);
		if (refresh_p == 0) {
			refresh_p = NREFRESH;
			printf("\b\b\b%ld%%",been_del);
		}
		refresh_p--;
	}
	printf("\b\b\b%ld%%",been_del);
	
	free(buf);	
	return 0;
}

int judge_pwd(char pwd_out[PASSWD_BUF_LEN])
{
	int i;
	char ch;
	int pwd_index = 0;
	char pwd[2][100] = {0};

	for (i = 0;i < 2;i++) {
		if (0 == i) {
			printf("Please input the passward:");
		} else {
			printf("\nPlease input the passward again:");
		}

		pwd_index = 0;
		while ((ch = _getch()) != 13) {
			if ((ch != 8) && !(ch >= ' ' && ch <= '~')) {
				return 1;
			}
			if (8 == ch) { //退格
				if (0 == pwd_index) {
					continue;
				}
				pwd[i][--pwd_index] = 0;
				printf("\b \b");
				continue;
			}
			printf("*");
			pwd[i][pwd_index++] = ch;
		}
	}


	printf("\n");
	if (0 == strcmp(pwd[0],pwd[1])){
		memcpy(pwd_out,pwd,sizeof(char)*100);
		return 0;
	} else {
		return 2;
	}
}

int recv_judge_pwd(int pwd_rev)
{
	if (2 == pwd_rev) {
		printf("两次输入的密码不同！\n");
		return 2;
	}
	if (1 == pwd_rev) {
		printf("密码为英文字母，数字或英文符号！\n");
		return 1;
	}
	return 0;
}


void edit_file(FILE *fp_in, char (*mask_code)[PASSWD_BUF_LEN], const int code_num)
{
#define ONE_PRO 200
	char ch;
	char prev_ch = '\n';
	int once_input_index;
	string str;
	int up_count = ONE_PRO;
	int flg = 0;//第一次处理,处理中0，最后一次处理1
	int mask_code_len ;
	int tmp_id;
	int file_index;

	fseek(fp_in, 0, SEEK_END);
	file_index = ftell(fp_in);

	printf(">  ");

	while (true) {
		once_input_index = 0;
		while ('\n' != (ch = getchar())) {
			once_input_index++;
			if ('\t' == ch && once_input_index == 1) {
				ch = getchar();
				if ('$' == ch) {
					up_count = (int)str.size();
					flg = 1;
					break;
				} else if ('\n' == ch) {
					str += '\t';
					prev_ch = '\t';
					break;
				} else {
					str += '\t';
					str += ch;
					prev_ch = ch;
					continue;
				}
				
			}
			str += ch;
			prev_ch = ch;
		}

		if ((int)str.size() >= up_count) {
			for (int i = 0;i < code_num;i++) {
				mask_code_len = (int)strlen(mask_code[i]);
				tmp_id = file_index % mask_code_len;
				for (int j = 0;j <  up_count ;j++) { 
					str[j] ^= mask_code[i][tmp_id];
					tmp_id = ++tmp_id % mask_code_len;
				}
			}

			fwrite(&str[0],sizeof(char),up_count,fp_in);
			file_index = ftell(fp_in);
			fflush(fp_in);
			if (1 == flg) break;
			str.erase(str.begin(),str.begin() + up_count);
			
		}

		if (prev_ch == '\n') {
			str += "\r\n      ";
			printf(">  ");
			continue;
		}

		system("cls");
		prev_ch = ch;
		printf(">");
	}
	
#undef  ONE_PRO
}

void see_file(FILE *fp_in, char (*mask_code)[PASSWD_BUF_LEN], const int code_num)
{
	const int ONCE_SHOW_NUM = 400;
	char buf[ONCE_SHOW_NUM+1] = {0};
	int rd_num;
	int tmp_id ;
	int file_index;
	int mask_code_len ;
	char ch;

	while (true) {
		file_index = ftell(fp_in);
		rd_num = (int)fread(buf,sizeof(char),ONCE_SHOW_NUM,fp_in);
		
		for (int i = 0;i < code_num;i++) {
			mask_code_len = (int)strlen(mask_code[i]);
			tmp_id = file_index % mask_code_len;
			for (int j = 0;j < ONCE_SHOW_NUM ;j++) { 
				buf[j] ^= mask_code[i][tmp_id];
				tmp_id = ++tmp_id % mask_code_len;
			}
		}

		if (rd_num < ONCE_SHOW_NUM ) { //结束文件读取
			buf[rd_num] = 0;
			printf("%s",buf);
			break;
		}

		int flg = 1;
		while (1 == flg) {
			ch = getchar();
			switch (ch) {
			case 'd':
				flg = 0;
				printf("%s",buf);
				break;
			case 'c':
				flg = 0;
				system("cls");
				break;
			case 'h':
				flg = 0;
				//fseek(fp_in,-ONCE_SHOW_NUM-ONCE_SHOW_NUM,SEEK_CUR);
				rewind(fp_in);
				fflush(stdout); //清空输出缓冲区,防止不正确的汉字输出
				break;
			default:
				break;
			}
		}
		
	}
}
