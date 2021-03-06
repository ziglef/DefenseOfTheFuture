#include <minix/drivers.h>

#include "test4.h"
#include "i8042.h"

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

static void print_usage(char *argv[]) {
	printf(
			"Usage: one of the following:\n"
					"\t service run %s -args \"test_scan\" \n"
					"\t service run %s -args \"test_leds <size> <leds>\" \n",
			argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {

	unsigned long returnVal;
	unsigned long n;

	if (strncmp(argv[1], "test_scan", strlen("test_scan")) == 0) {
		if (argc != 2) {
			printf("keyboard: wrong no of arguments for test of test_scan() \n");
			return 1;
		}

		test_scan();
		printf("\n");
		return returnVal;

	} else if (strncmp(argv[1], "test_leds", strlen("test_leds")) == 0) {
        if (argc < 3) {
                printf("test_leds: wrong no of arguments for test of test_leds() \n");
                return 1;
        }

        unsigned short vec[argc-2];
        unsigned int i;

       for(i=2; i<argc;i++)
       {
    	   if ((vec[i-2] = parse_ulong(argv[i], 10)) == ULONG_MAX)
    		   return 1;
       }

        test_leds((argc-2), vec);
        printf("\n");
        return returnVal;

	} else {
		printf("keyboard: non valid function \"%s\" to test\n", argv[1]);
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
