#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14
#define VBE_MODE_INFO_SIZE 256

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
  
  struct reg86u reg86;
  mmap_t memory;
  
  lm_init();
  
  lm_alloc(VBE_MODE_INFO_SIZE, &memory);
  
  reg86.u.w.ax = 0x4F01;
  reg86.u.w.cx = 1 << 14 | mode;
  reg86.u.w.es = PB2BASE(memory.phys);
  reg86.u.w.di = PB2OFF(memory.phys);
  reg86.u.b.intno = 0x10;
  
  if(sys_int86(&reg86) != OK){
	  printf("vbe_get_mode_info: sys_int86() failed \n");
	  return 1;
  }
  
  vmi_p = memory.virtual;
  
  lm_free(&memory);
  
  return 0;
}


