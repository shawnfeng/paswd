#include "ays_para.h"
#include <stdlib.h>
#include <string.h>

/* 命令行参数输入错误 */
#define STR_ER_PARA_LESS	"参数数量输入不足最少的输入为： -i inputfile [-o outputfile]"
#define STR_ER_PARA_N_IF	"-i 后无输入文件"
#define STR_ER_PARA_N_OF	"-o 后无输出文件"
#define STR_ER_PARA_ER_PARA	"出现不可解析的参数"
#define STR_ER_PARA_N_I	    "没有输入文件 -i filename 参数"
#define STR_ER_PARA_N_WREC    "没有程序任务参数"
#define STR_ER_PARA_N_NPASSWD "没有密码个数参数"
#define STR_ER_PARA_FILENAME_LONGER "文件名过长"
#define STR_ER_PARA_REDUND  "文件输入参数冗余错误"
#define STR_ER_PARA_NPASSWD "密码个数输入出错"
#define STR_ER_PARA_NCH_PASSWD "修改密码后的个数出错"

const char *ays_para(struct para_s *options, const int argc, char * const *argv)
{
	int i;
	int npasswd;
	char *para = options->para;
	char (*fname)[FILENAME_LEN] = options->fname;

	/* if (argc < PARA_MIN_NUM) {
		return STR_ER_PARA_LESS;
	} */

	memset(para, 0, sizeof(char) * PARA_NUM);
	for (i = 1; i < argc; i++) {
		if (!strcmp(PARA_INPUT, argv[i])) {
			if (++i == argc) {
				return STR_ER_PARA_N_IF;
			}
			if (strlen(argv[i]) >= FILENAME_LEN) {
				return STR_ER_PARA_FILENAME_LONGER;
			}

			para[PARA_INPUT_ID] = CPARA_I;
			strcpy(fname[INFILE_ID], argv[i]);

		} else if (!strcmp(PARA_OUTPUT, argv[i])) {
			if (++i == argc) {
				return STR_ER_PARA_N_OF;
			}
			if (strlen(argv[i]) >= FILENAME_LEN) {
				return STR_ER_PARA_FILENAME_LONGER;
			}

			para[PARA_OUTPUT_ID] = CPARA_O;
			strcpy(fname[OUTFILE_ID], argv[i]);

		} else if (!strcmp(PARA_WRITE, argv[i])) {
			if (para[PARA_WREC_ID] != '\0') {
				return STR_ER_PARA_REDUND;
			}
			para[PARA_WREC_ID] = CPARA_W;

		} else if (!strcmp(PARA_READ, argv[i])) {
			if (para[PARA_WREC_ID] != '\0') {
				return STR_ER_PARA_REDUND;
			}
			para[PARA_WREC_ID] = CPARA_R;
			
		} else if (!strcmp(PARA_ECODE, argv[i])) {
			if (para[PARA_WREC_ID] != '\0') {
				return STR_ER_PARA_REDUND;
			}
			para[PARA_WREC_ID] = CPARA_E;

		} else if (!strcmp(PARA_HELP, argv[i])) {
			if (para[PARA_WREC_ID] != '\0') {
				return STR_ER_PARA_REDUND;
			}
			para[PARA_WREC_ID] = CPARA_H;

		} else if (!strncmp(PARA_CHANGE_PASSWD, argv[i], PARA_NCH_PASSWD_PREFIX_LEN)) {
			if (para[PARA_WREC_ID] != '\0') {
				return STR_ER_PARA_REDUND;
			}
	
			if (0 == (npasswd = atoi(&argv[i][PARA_NCH_PASSWD_PREFIX_LEN]))) {
				return STR_ER_PARA_NCH_PASSWD;
			}
			para[PARA_WREC_ID] = CPARA_C;
			options->nch_passwd = npasswd;

		} else if (!strncmp(PARA_NPASSWD, argv[i], PARA_NPASSWD_PREFIX_LEN)) {
			if (0 == (npasswd = atoi(&argv[i][PARA_NPASSWD_PREFIX_LEN]))) {
				return STR_ER_PARA_NPASSWD;
			}
			para[PARA_NPASSWD_ID] = CPARA_N;
			options->npasswd = npasswd;
		}

		else {
			return  STR_ER_PARA_ER_PARA;
		}
	}

	
	if (para[PARA_WREC_ID] == '\0') {
		return STR_ER_PARA_N_WREC;
	}

	if(para[PARA_WREC_ID] != CPARA_H) {

		if (para[PARA_INPUT_ID] != CPARA_I) {
			return STR_ER_PARA_N_I;
		}

		if (para[PARA_NPASSWD_ID] != CPARA_N) {
			return STR_ER_PARA_N_NPASSWD;
		}
	}

	return NULL;
}
