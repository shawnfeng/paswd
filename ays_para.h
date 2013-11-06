#ifndef MIO_H_H
#define MIO_H_H
#include <stdio.h>
#include <assert.h>

#define PARA_INPUT "-i"       /* 输入文件参数 */
#define PARA_OUTPUT "-o"      /* 输出文件参数 */
#define PARA_NPASSWD "-n"

#define PARA_WRITE "-w"
#define PARA_READ  "-r"
#define PARA_ECODE "-e"
#define PARA_CHANGE_PASSWD "-c"
#define PARA_HELP "-h"

#define CPARA_I 'i'      /* 输入文件参数，内部标识 */
#define CPARA_O 'o'      /* 输出文件参数，内部标识 */ 
#define CPARA_N 'n'      /* 需要密码的个数 */

#define CPARA_R 'r'      /* 读文件内容 */
#define CPARA_W 'w'      /* 写文件 */
#define CPARA_E 'e'      /* 加密文件 */
#define CPARA_C 'c'      /* 更改密码 */
#define CPARA_H 'h'      /* help */

enum {
	PARA_MIN_NUM = 5, /* -i infile -w/-r/-e/-c -nNUM */
	PARA_NUM = 9,
	PARA_INPUT_ID = 0,
	PARA_OUTPUT_ID = 1,
	PARA_WREC_ID = 2, 
	PARA_NPASSWD_ID = 3,

	PARA_NPASSWD_PREFIX_LEN = 2, /* 密码输入个数参数前缀长度 */
	PARA_NCH_PASSWD_PREFIX_LEN = 2,

	FILENAME_LEN = 50,
	FILE_NUM = 2,
	INFILE_ID = 0,
	OUTFILE_ID = 1,
};

struct para_s {
	int  npasswd;
	int  nch_passwd; /* 修改后的密码个数 */
	char fname[FILE_NUM][FILENAME_LEN]; 
	char para[PARA_NUM];
};

const char *ays_para(struct para_s *options, const int argc, char * const *argv);

#endif
