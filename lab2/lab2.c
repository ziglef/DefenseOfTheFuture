#include <minix/drivers.h>

#include "vbe.h"
#include "lmlib.h"
#include "video_gr.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv) {

	char *video_mem;

	/* Initialize service */

	sef_startup();

	// Testing Purposes
	/*
	vg_init(0x105);
	sleep(5);
	vg_exit();
	*/
	
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
					"\t service run %s -args \"fill <hex-mode> <hex-color> <wait secs>\" \n"
					"\t service run %s -args \"setPixel <hex-mode> <hex-color> <x coord> <y coord> <wait secs>\" \n"
					"\t service run %s -args \"getPixel <hex-mode> <x coord> <y coord> <wait secs>\" \n"
					"\t service run %s -args \"line <hex-mode> <hex-color> <x1> <y1> <x2> <y2> <wait secs>\" \n",
			argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {

	unsigned long color, x, y, xDest, yDest, mode, secs, returnVal;

	/* check the function to test: if the first characters match, accept it */
	if (strncmp(argv[1], "fill", strlen("fill")) == 0) {
		if (argc != 5) {
			printf("video_txt: wrong no of arguments for test of vg_fill() \n");
			return 1;
		}
		if ((mode = parse_ulong(argv[2], 16)) == ULONG_MAX)
			return 1;
		if ((color = parse_ulong(argv[3], 16)) == ULONG_MAX)
			return 1;
		if ((secs = parse_ulong(argv[4], 10)) == ULONG_MAX)
			return 1;
		vg_init(mode);
		returnVal = vg_fill(color);
		sleep(secs);
		vg_exit();
		printf("\n");
		return returnVal;
	} else if (strncmp(argv[1], "setPixel", strlen("setPixel")) == 0) {
		if (argc != 7) {
			printf(
					"video_txt: wrong no of arguments for test of vg_set_pixel() \n");
			return 1;
		}
		if ((mode = parse_ulong(argv[2], 16)) == ULONG_MAX)
			return 1;
		if ((color = parse_ulong(argv[3], 16)) == ULONG_MAX)
			return 1;
		if ((x = parse_ulong(argv[4], 10)) == ULONG_MAX)
			return 1;
		if ((y = parse_ulong(argv[5], 10)) == ULONG_MAX)
			return 1;
		if ((secs = parse_ulong(argv[6], 10)) == ULONG_MAX)
			return 1;
		vg_init(mode);
		returnVal = vg_set_pixel(x, y, color);
		sleep(secs);
		vg_exit();
		printf("\n");
		return returnVal;
	} else if (strncmp(argv[1], "getPixel", strlen("getPixel")) == 0) {
		if (argc != 6) {
			printf(
					"video_txt: wrong no of arguments for test of vg_get_pixel() \n");
			return 1;
		}
		if ((mode = parse_ulong(argv[2], 16)) == ULONG_MAX)
			return 1;
		if ((x = parse_ulong(argv[3], 10)) == ULONG_MAX)
			return 1;
		if ((y = parse_ulong(argv[4], 10)) == ULONG_MAX)
			return 1;
		if ((secs = parse_ulong(argv[5], 10)) == ULONG_MAX)
			return 1;
		vg_init(mode);
		returnVal = vg_get_pixel(x, y);
		sleep(secs);
		vg_exit();
		printf("\n");
		return returnVal;
	} else if (strncmp(argv[1], "line", strlen("line")) == 0) {
		if (argc != 9) {
			printf(
					"video_txt: wrong no of arguments for test of vt_print_int() \n");
			return 1;
		}
		if ((mode = parse_ulong(argv[2], 16)) == ULONG_MAX)
			return 1;
		if ((color = parse_ulong(argv[3], 16)) == ULONG_MAX)
			return 1;
		if ((x = parse_ulong(argv[4], 10)) == ULONG_MAX)
			return 1;
		if ((y = parse_ulong(argv[5], 10)) == ULONG_MAX)
			return 1;
		if ((xDest = parse_ulong(argv[6], 10)) == ULONG_MAX)
			return 1;
		if ((yDest = parse_ulong(argv[7], 10)) == ULONG_MAX)
			return 1;
		if ((secs = parse_ulong(argv[8], 10)) == ULONG_MAX)
			return 1;
		vg_init(mode);
		returnVal = vg_draw_line(x, y, xDest, yDest, color);
		sleep(secs);
		vg_exit();
		printf("\n");
		return returnVal;
	} else {
		printf("video_txt: non valid function \"%s\" to test\n", argv[1]);
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
