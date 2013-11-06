#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "secret.h"
#include "ays_para.h"

#define HELP_COMMENT "\
-h          help\n\
-i input    input file name\n\
-o output   output file name\n\
-w          write file\n\
-r          read file\n\
-e          encrypt file\n\
-cNUM       change password and NUM\n\
-nNUM       input file password NUM"


/* sample: -i infile -w/r/e/cNUM -nNUM [-o outfile] */
int main(int argc, char** argv)
{
	FILE* fp_in = NULL;
	FILE* fp_out = NULL;
	//char (*mask_code)[PASSWD_BUF_LEN] = NULL;
	char mask_code[10][PASSWD_BUF_LEN];
	int  pro_u = PROCE_UNIT;
	int nallpasswd, npasswd, nch_passwd = 0;
	int i;
	char c;

	struct para_s options;
	const char *s;
	if (NULL != (s = ays_para(&options, argc, argv))) {
		printf("%s\n", s);
		return 1;
	}

	c = options.para[PARA_WREC_ID];
	if (c == CPARA_H) {
		printf("%s\n", HELP_COMMENT);
		return 0;
	}
	
	if (NULL == (fp_in = fopen(options.fname[INFILE_ID], "rb+"))) {
		printf("open input file error");
		return 1;
	}

	

	npasswd = options.npasswd;
	if (options.para[PARA_WREC_ID] == CPARA_C) {
		nch_passwd = options.nch_passwd;
	}
	nallpasswd = npasswd + nch_passwd;

	//mask_code = (char (*)[PASSWD_BUF_LEN])malloc(sizeof(char (*)[PASSWD_BUF_LEN]) * (nallpasswd));
	//char *pcc;
	//mask_code[0] = pcc;
	memset(mask_code, 0 , sizeof(char) * PASSWD_BUF_LEN * 10);


	printf("注意如果你输错了密码将造成文件不能恢复，所以请您谨慎输入，\
牢记密码！一旦处理过程开始，请不要停止！否则造成数据损毁！\n\n");

	printf("你需要输入 %d 个密码\n", npasswd);
	for (i = 0; i < npasswd; i++) { //录入密码，根据指定密码个数
		printf("请输入第 %d 个 密码！\n",i + 1);
		if (judge_pwd(mask_code[i]) != 0) return 1; 
	} /* 加入支持0密码支持，和空密码支持 ！最大密码长度限制，如何设置缓冲区大小*/


	switch (c) {

		case CPARA_C:
			printf("你需要输入 %d 个新密码\n", nch_passwd);
			for (i = npasswd; i < nallpasswd; i++) { //录入密码，根据指定密码个数
				printf("请输入第 %d 个 新密码！\n", i + 1);
				if (judge_pwd(mask_code[i]) != 0) return 1; 
			}
			/* falling through */

		case CPARA_E:
			if (CPARA_O == options.para[PARA_OUTPUT_ID]) {
				if (NULL == (fp_out = fopen(options.fname[OUTFILE_ID], "wb"))) {
					printf("open output file error");
					return 1;
				}
				secret(fp_in, fp_out, mask_code, nallpasswd, pro_u);
			} else {
				secret(fp_in, NULL, mask_code, nallpasswd, pro_u);
			}
			
			break;
		case CPARA_W:
			edit_file(fp_in, mask_code, npasswd);
			break;
		case CPARA_R:
			see_file(fp_in, mask_code, npasswd);	
			break;
			
		default:
			assert(0);
	}

	

	//if (NULL !=mask_code) {
	//	free(mask_code);
	//}
	if (NULL != fp_in) {
		fclose(fp_in);
		fp_in = NULL;
	}

	if (NULL != fp_out) {
		fclose(fp_out);
		fp_out = NULL;
	}


	return 0;
}
