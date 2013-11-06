#ifndef SECRET_H_H
#define SECRET_H_H
#include <stdio.h>

enum {
	PROCE_UNIT = 1048576, /* 每次处理字节数 */
	PASSWD_BUF_LEN = 100, /* 密码输入缓冲区长度 */
};

int secret(FILE *fp_in, FILE *fp_out, char (*mask_code)[PASSWD_BUF_LEN],
           const int code_num, const int once_pro);
int judge_pwd(char pwd_out[PASSWD_BUF_LEN]);
int recv_judge_pwd(int pwd_rev);

void edit_file(FILE *fp_in, char (*mask_code)[PASSWD_BUF_LEN], const int code_num);
void see_file(FILE *fp_in, char (*mask_code)[PASSWD_BUF_LEN], const int code_num);

#endif
