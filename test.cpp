#include "ays_para.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	struct para_s options;
	const char *s;
	s = ays_para(&options, argc, argv);

	if (s != NULL) {
		printf("%s\n", s);
	}
	return 0;
}
