#ifdef EMUL /* In emulation mode */

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>

#else	/* Running on Minix*/

#include <minix/drivers.h>

#endif

#include "test7.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv) {

/* Initialize service */
sef_startup();

if (argc == 1) {
	print_usage(argv);
    return 0;
}
proc_args(argc, argv);

return 0;
}


#ifdef EMUL
static void print_usage(char *argv[]) {
  printf("Usage: one of the following:\n"
	 "\t %s config 1|2\n"
	 "\t %s set 1|2 <short base_addr> <long bits> <long stop> <long parity> <long rate>\n"
	 "\t %s poll \n"
	 "\t %s int \n"
	 "\t %s fifo \n",
	 argv[0], argv[0],argv[0],argv[0],argv[0]);
}
#else
static void print_usage(char *argv[]) {
  printf("Usage: one of the following:\n"
	 "\t service run %s -args \"config 1|2\"  \n"
	 "\t service run %s -args \"set 1|2 <short base_addr> <long bits> <long stop> <long parity> <long rate>\" \n"
	 "\t service run %s -args \"poll\" \n"
	 "\t service run %s -args \"int\" \n"
	 "\t service run %s -args \"fifo\" \n",
	argv[0], argv[0], argv[0], argv[0], argv[0]);
}

#endif

static int proc_args(int argc, char *argv[]) {

		unsigned short BASE;

        if (strncmp(argv[1], "config", strlen("config")) == 0) {
                if (argc != 3) {
                        printf("test_conf: wrong number of arguments for test of test_conf() \n");
                        return 1;
                }

                if ((BASE = parse_ulong(argv[2], 10)) == ULONG_MAX) return 1;

                if(BASE == 1)
                	test_conf(0x3F8);
                else
                	test_conf(0x2F8);
                printf("\n");
                return 0;

        } else if (strncmp(argv[1], "set", strlen("set")) == 0) {
                if (argc != 2) {
                        printf("test_set: wrong number of arguments for test of test_set() \n");
                        return 1;
                }


                printf("\n");
                return 0;
        } else if (strncmp(argv[1], "int", strlen("int")) == 0) {
				if (argc != 3) {
						printf("test_int: wrong number of arguments for test of test_int() \n");
						return 1;
				}




				printf("\n");
				return 0;
        } else {
                printf("test: non valid function \"%s\" to test\n", argv[1]);
                return 1;
        }
}

static unsigned long parse_ulong(char *str, int base) {
        char *endptr;
        unsigned long val;

        val = strtoul(str, &endptr, base);

        if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
                perror("strtol");
                return ULONG_MAX;
        }

        if (endptr == str) {
                printf("video_txt: parse_ulong: no digits were found in %s \n", str);
                return ULONG_MAX;
        }

        /* Successful conversion */
        return val;
}

static long parse_long(char *str, int base) {
        char *endptr;
        unsigned long val;

        val = strtol(str, &endptr, base);

        if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
                        || (errno != 0 && val == 0)) {
                perror("strtol");
                return LONG_MAX;
        }

        if (endptr == str) {
                printf("video_txt: parse_long: no digits were found in %s \n", str);
                return LONG_MAX;
        }

        /* Successful conversion */
        return val;
}



