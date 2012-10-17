#ifdef EMUL /* In emulation mode */

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>

#else	/* Running on Minix*/

#include <minix/drivers.h>


#endif

#include "test5.h"

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

//test_leds(2, 10);

return 0;
}


#ifdef EMUL
static void print_usage(char *argv[]) {
  printf("Usage: one of the following:\n"
	 "\t %s test_packet \n"
	 "\t %s test_asynch <unsigned short duration \n"
	 "\t %s test_config <void> \n",
	 argv[0], argv[0]);
}
#else
static void print_usage(char *argv[]) {
  printf("Usage: one of the following:\n"
	 "\t service run %s -args \"packet\"  \n"
	 "\t service run %s -args \"asynch\" <short duration> \n"
	 "\t service run %s -args \"config\" <void> \n",
	argv[0], argv[0], argv[0]);
}

#endif

static int proc_args(int argc, char *argv[]) {


	unsigned long returnVal;
    unsigned short duration;

        if (strncmp(argv[1], "packet", strlen("packet")) == 0) {
                if (argc != 2) {
                        printf("test_packet: wrong n¼ of arguments for test of test_packet() \n");
                        return 1;
                }

                test_packet();
                printf("\n");
                return returnVal;

        } else if (strncmp(argv[1], "asynch", strlen("asynch")) == 0) {
                if (argc != 3) {
                        printf("test_asynch: wrong n¼ of arguments for test of test_asynch() \n");
                        return 1;
                }
                if ((duration = parse_ulong(argv[2], 10)) == ULONG_MAX)
                                        return 1;

                test_asynch(duration);
                printf("\n");
                return returnVal;
        }
            else if (strncmp(argv[1], "config", strlen("config")) == 0) {
                    if (argc != 2) {
                            printf("test_config: wrong n¼ of arguments for test of test_config() \n");
                            return 1;
                    }

                    test_config();
                    printf("\n");
                    return returnVal;

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



