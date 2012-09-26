#ifdef EMUL /* In emulation mode */

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>

#else	/* Running on Minix*/

#include <minix/drivers.h>

#endif

#include "vt_info.h"
#include "video_txt.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv) {

  char *video_mem;
  vt_info_t vt_info;

#ifndef EMUL
  /* Initialize service */
  
  sef_startup();
#endif

  /* Get VDU info */

  vt_info_get(&vt_info);

  /* Print display info */
  vt_info_display(&vt_info);

  video_mem = vt_init(&vt_info);

  printf("lab1: VRAM mapped on virtual address %p\n", video_mem);

  if ( argc == 1 ) {
      print_usage(argv);
      return 0;
  } else {   
      if( proc_args(argc, argv) == 0 ) {
#ifdef EMUL
	  vt_update_display();
#endif
      }
  }
  return 0;

}

#ifdef EMUL
static void print_usage(char *argv[]) {
  printf("Usage: one of the following:\n"
	 "\t %s fill <hex-code> <hex-attr> \n"
	 "\t %s blank - to blank screen \n"
	 "\t %s char <hex-code> <line no> <col no> <hex-attr> \n"
	 "\t %s string <string> <line no> <col no> <hex-attr> \n"
	 "\t %s int <decimal number> <line no> <col no> <hex-attr> \n"
	 "\t %s frame <width> <height> <line no> <col no> <hex-attr> \n",
	 argv[0], argv[0], argv[0], argv[0], argv[0], argv[0]);
}
#else
static void print_usage(char *argv[]) {
  printf("Usage: one of the following:\n"
	 "\t service run %s -args \"fill <hex-code> <hex-attr>\" \n"
	 "\t service run %s -args \"blank\" - to blank screen \n"
	 "\t service run %s -args \"char <hex-code> <line no> <col no> <hex-attr>\" \n"
	 "\t service run %s -args \"string <string> <line no> <col no> <hex-attr>\" \n"
	 "\t service run %s -args \"int <decimal number> <line no> <col no> <hex-attr>\" \n"
	 "\t service run %s -args \"frame <width> <height> <line no> <col no> <hex-attr>\" \n",
	 argv[0], argv[0], argv[0], argv[0], argv[0], argv[0]);
}

#endif

static int proc_args(int argc, char *argv[]) {

  unsigned long ch, row, col, attr, width, height;
  char *str;
  long num;

  /* always blank the screen: in emulation mode, we need to use a printable char */
#ifdef EMUL
  vt_fill(' ', 0x00);
#else
  vt_blank();
#endif
  /* check the function to test: if the first characters match, accept it */
  if (strncmp(argv[1], "blank", strlen("blank")) == 0) {
	  if( argc != 2 ) {
		  printf("video_txt: wrong no of arguments for test of vt_blank() \n");
		  return 1;
	  }
	  printf("video_txt:: vt_blank()\n"); /* Actually, it was already invoked */
	  return 0;
  }
  if (strncmp(argv[1], "fill", strlen("fill")) == 0) {
	  if( argc != 4 ) {
		  printf("video_txt: wrong no of arguments for test of vt_fill_char() \n");
		  return 1;
	  }
	  if( (ch = parse_ulong(argv[2], 16)) == ULONG_MAX )
		  return 1;
	  if( (attr = parse_ulong(argv[3], 16)) == ULONG_MAX )
		  return 1;
	  printf("video_txt:: vt_fill(0x%X, 0x%X)\n",
			  (unsigned)ch, (unsigned)attr);
	  vt_fill(ch, attr);
	  return 0;
  } else if (strncmp(argv[1], "char", strlen("char")) == 0) {
	  if( argc != 6 ) {
		  printf("video_txt: wrong no of arguments for test of vt_print_char() \n");
		  return 1;
	  }
	  if( (ch = parse_ulong(argv[2], 16)) == ULONG_MAX )
		  return 1;
	  if( (row = parse_ulong(argv[3], 10)) == ULONG_MAX )
		  return 1;
	  if( (col = parse_ulong(argv[4], 10)) == ULONG_MAX )
		  return 1;
	  if( (attr = parse_ulong(argv[5], 16)) == ULONG_MAX )
		  return 1;
	  printf("video_txt:: vt_print_char(0x%X, %lu, %lu, 0x%X)\n",
			  (unsigned)ch, row, col, (unsigned)attr);
	  return vt_print_char(ch, row, col, attr);
  } else if (strncmp(argv[1], "string", strlen("string")) == 0) {
	  if( argc != 6 ) {
		  printf("video_txt: wrong no of arguments for test of vt_print_string() \n");
		  return 1;
	  }
	  str = argv[2];
	  if( (row = parse_ulong(argv[3], 10)) == ULONG_MAX )
		  return 1;
	  if( (col = parse_ulong(argv[4], 10)) == ULONG_MAX )
		  return 1;
	  if( (attr = parse_ulong(argv[5], 16)) == ULONG_MAX )
		  return 1;
	  printf("video_txt:: vt_print_string(%s, %lu, %lu, 0x%X)\n",
			  str, row, col, (unsigned)attr);
	  return vt_print_string(str, row, col, attr);
  } else if (strncmp(argv[1], "int", strlen("int")) == 0) {
	  if( argc != 6 ) {
		  printf("video_txt: wrong no of arguments for test of vt_print_int() \n");
		  return 1;
	  }
	  if( (num = parse_long(argv[2], 10)) == LONG_MAX )
		  return 1;
	  if( (row = parse_ulong(argv[3], 10)) == ULONG_MAX )
		  return 1;
	  if( (col = parse_ulong(argv[4], 10)) == ULONG_MAX )
		  return 1;
	  if( (attr = parse_ulong(argv[5], 16)) == ULONG_MAX )
		  return 1;
	  printf("video_txt:: vt_print_int(%lu, %lu, %lu, 0x%X)\n",
		 num, row, col, (unsigned)attr);
	  return vt_print_int(num, row, col, attr);
  } else if (strncmp(argv[1], "frame", strlen("frame")) == 0) {
	  if( argc != 7 ) {
		  printf("video_txt: wrong no of arguments for test of vt_draw_frame() \n");
		  return 1;
	  }
	  if( (width = parse_ulong(argv[2], 10)) == LONG_MAX )
		  return 1;
	  if( (height = parse_ulong(argv[3], 10)) == LONG_MAX )
		  return 1;
	  if( (row = parse_ulong(argv[4], 10)) == ULONG_MAX )
		  return 1;
	  if( (col = parse_ulong(argv[5], 10)) == ULONG_MAX )
		  return 1;
	  if( (attr = parse_ulong(argv[6], 16)) == ULONG_MAX )
		  return 1;
	  printf("video_txt:: draw_frame(%lu, %lu, %lu, %lu, 0x%X)\n",
		 width, height, row, col, (unsigned)attr);
	  return vt_draw_frame(width, height, row, col, attr);
  } else {
	  printf("video_txt: non valid function \"%s\" to test\n", argv[1]);
	  return 1;
  }
}

static unsigned long parse_ulong(char *str, int base) {
  char *endptr;
  unsigned long val;

  val = strtoul(str, &endptr, base);

  if ((errno == ERANGE && val == ULONG_MAX )
	  || (errno != 0 && val == 0)) {
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
