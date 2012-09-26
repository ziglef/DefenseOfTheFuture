#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"

/* Constants for VBE 0x105 mode */

#define VRAM_PHYS_ADDR    0xD0000000
#define H_RES             1024
#define V_RES		  	  768
#define BITS_PER_PIXEL	  8

/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

void * vg_init(unsigned long mode) {

	struct reg86u reg86;
	vbe_mode_info_t vmi;
	
	int r;
	struct mem_range mr;
	
	reg86.u.w.ax = 0x4F02;
	reg86.u.w.bx = 1<<14|mode;
	reg86.u.b.intno = 0x10;
	
	if(sys_int86(&reg86) != OK) {
		printf("set_vbe_mode: sys_int86() failed\n");
		return NULL;
	}
	else {
		vbe_get_mode_info(mode, &vmi);
		
		h_res = vmi.XResolution;
		v_res = vmi.YResolution;
		bits_per_pixel = vmi.BitsPerPixel;
		
		mr.mr_base = vmi.PhysBasePtr;
		mr.mr_limit = mr.mr_base + (h_res * v_res * (bits_per_pixel / 8));
		
		if((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) != OK) {
			panic("vg_init: sys_privctl (ADD_MEM) failed: %d\n", r);
			return NULL;
		}
		else {
			video_mem = vm_map_phys(SELF, (void *)mr.mr_base, (h_res * v_res * (bits_per_pixel / 8)));
			
			if(video_mem == MAP_FAILED){
				panic("vg_init: Couldn't map video memory!\n");
				return NULL;
			} else
				return video_mem;
		}
	}
}

int vg_fill(unsigned long color) {
	return 0;
}

int vg_set_pixel(unsigned long x, unsigned long y, unsigned long color) {
	return 0;
}

long vg_get_pixel(unsigned long x, unsigned long y) {
	return 0;
}

int vg_draw_line(unsigned long xi, unsigned long yi, 
		 unsigned long xf, unsigned long yf, unsigned long color) {
	return 0;
}

int vg_exit() {
  struct reg86u reg86;

  reg86.u.b.intno = 0x10; /* BIOS video services */

  reg86.u.b.ah = 0x00;    /* Set Video Mode function */
  reg86.u.b.al = 0x03;    /* 80x25 text mode*/

  if( sys_int86(&reg86) != OK ) {
      printf("\tvg_exit(): sys_int86() failed \n");
      return 1;
  } else
      return 0;
}
