#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"

int speaker_ctrl(unsigned char on) {
 if(on == 0){
  if(sys_outb(SPEAKER_CTRL,0x00) != OK)
   return 1;
 } else {
  if(sys_outb(SPEAKER_CTRL,0x03) != OK)
   return 1;
  }
 return 0;
}

int speaker_test(unsigned long freq, unsigned long time) {

 if(timer_set_square(2,freq))
 {
  printf("Timer_set_square Failed!\n");
  return 1;
 }

 if(speaker_ctrl(1))
 {
  printf("Speaker_ctrl Failed!\n");
  return 1;
 }

 if(timer_test_int(time))
 {
  printf("Timer_Test_Int Failed!\n");
  return 1;
 }

 if(speaker_ctrl(0))
 {
  printf("Speaker_ctrl Failed!\n");
  return 1;
 }
 
 return 0;
}
