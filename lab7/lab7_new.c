#ifdef EMUL /* In emulation mode */

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>

#else /* Running on Minix*/

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
   "\t %s test_conf <short base_addr> \n"
   "\t %s test_set <short base_addr> <long bits> <long stop> <long parity> <long rate> \n"
   "\t %s test_poll <to be defined> \n"
   "\t %s test_int <to be defined > \n"
   "\t %s test_fifo <to be defined > \n",
   argv[0], argv[0],argv[0], argv[0],argv[0]);
}
#else
static void print_usage(char *argv[]) {
 printf("Usage: one of the following:\n"
   "\t service run %s -args \"conf <short base_addr>\"\n"
   "\t service run %s -args \"test_set <short base_addr> <long bits> <long stop> <long parity> <long rate>\"\n"
   "\t service run %s -args \"poll <to be defined>\"\n"
   "\t service run %s -args \"int <to be defined>\"\n"
   "\t service run %s -args \"fifo <to be defined>\"\n",
   argv[0], argv[0], argv[0], argv[0], argv[0]);
}

#endif

static int proc_args(int argc, char *argv[]) {

 unsigned short base_addr;
 unsigned long returnVal, bits, stop,parity, rate;

 if (strncmp(argv[1], "conf", strlen("conf")) == 0) {
  if (argc != 3) {
   printf("test_conf: wrong number of arguments for test of test_conf() \n");
   return 1;
  }
  if ((base_addr = parse_ulong(argv[2], 10)) == ULONG_MAX)
   return 1;

  returnVal = test_conf((unsigned short)base_addr);
  printf("\n");
  return returnVal;

 } else if (strncmp(argv[1], "set", strlen("set")) == 0) {
  if (argc != 7) {
   printf("test_set: wrong number of arguments for test of test_set() \n");
   return 1;
  }
  if ((base_addr = parse_ulong(argv[2], 10)) == ULONG_MAX)
   return 1;
  if ((bits = parse_ulong(argv[3], 10)) == ULONG_MAX)
   return 1;
  if ((stop = parse_ulong(argv[4], 10)) == ULONG_MAX)
   return 1;
  if ((parity = parse_ulong(argv[5], 10)) == ULONG_MAX)
   return 1;
  if ((rate = parse_ulong(argv[6], 10)) == ULONG_MAX)
   return 1;


  returnVal = test_set(base_addr, bits,stop,parity,rate);
  printf("\n");
  return returnVal;
 }
 else if (strncmp(argv[1], "poll", strlen("poll")) == 0) {

  return 0;
 }
 else if (strncmp(argv[1], "int", strlen("int")) == 0) {

  return 0;
 }
 else if (strncmp(argv[1], "fifo", strlen("fifo")) == 0) {

  return 0;
 }
 else {
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
