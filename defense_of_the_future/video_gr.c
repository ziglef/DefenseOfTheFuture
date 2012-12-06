#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"
#include "video_gr.h"

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

	// -----------------------
	// -Non-Hardcoded Version-
	// -----------------------

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

	// -------------------
	// -Hardcoded Version-
	// -------------------
	/*
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

		h_res = H_RES;
		v_res = V_RES;
		bits_per_pixel = BITS_PER_PIXEL;

		mr.mr_base = VRAM_PHYS_ADDR;
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
	}*/

}

int vg_fill(unsigned long color) {
	
	int i, j, k;
	char *write_address = video_mem;
	long colour;

	for (i=0; i<v_res; i++) {
		for (j=0; j<h_res; j++) {
			colour = color;
			for (k=0; k<bits_per_pixel/8; k++) {
				*write_address = (char) colour;
				write_address++;
				colour >> 8;
			}
		}
	}
	return 0;
}

int vg_set_pixel(unsigned long x, unsigned long y, unsigned long color) {

	int i, j, k;
	char *write_address = video_mem;
	long colour;

	if((x < 0) || (x > h_res) || (y < 0) || (y > v_res)){
		return 1;
	}

	write_address += (bits_per_pixel/8)*((y-1)*h_res+x);

	colour = color;
	for (k=0; k<bits_per_pixel/8; k++) {
		*write_address = colour & 0xFF;
		write_address++;
		colour >> 8;
	}

	return 0;
}

long vg_get_pixel(unsigned long x, unsigned long y) {

	char *write_address = video_mem;

	if((x < 0) || (x > h_res) || (y < 0) || (y > v_res)){
		vg_draw_line(1,1,200,200,0xF4C3);
		return 1;
	}

	write_address += (bits_per_pixel/8)*((y-1)*h_res+x);

	return (long)*write_address;
}

int vg_draw_line(unsigned long xi, unsigned long yi, 
		 unsigned long xf, unsigned long yf, unsigned long color) {

	/* Bresenham's line algorithm - source: http://http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	 plot(x0,y0, x1,y1)
	  dx=x1-x0
	  dy=y1-y0

	  D = 2*dy - dx
	  plot(x0,y0)
	  y=y0

	  for x from x0+1 to x1
		if D > 0
		  y = y+1
		  plot(x,y)
		  D = D + (2*dy-2*dx)
		else
		  plot(x,y)
		  D = D + (2*dy)
	 */

	int i;

	int deltax = xf - xi;
	int deltay = yf - yi;
	int y, d, x;

	if((xi < 0) || (xi > h_res) || (yi < 0) || (yi > v_res) || (xf < 0) || (xf > h_res) || (yf < 0) || (yf > v_res)){
		return 1;
	}

	if(deltay != 0){
		if(deltax != 0){
			if((deltax > 0) && (deltay > 0)){
				d = 2*deltay - deltax;
				vg_set_pixel(xi,yi,color);
				y = yi;
				for(i=xi+1; i<=xf; i++){
					if(d > 0){
						y++;
						vg_set_pixel(i,y,color);
						d += (2*deltay-2*deltax);
					}else{
						vg_set_pixel(i,y,color);
						d += (2*deltay);
					}
				}
			}else{
				if((deltax > 0) && (deltay < 0)){
					d = -2*deltay - deltax;
					vg_set_pixel(xf,yf,color);
					y = yf;
					for(i=xf-1; i>xi; i--){
						if(d > 0){
							y++;
							vg_set_pixel(i,y,color);
							d += (-2*deltay-2*deltax);
						}else{
							vg_set_pixel(i,y,color);
							d += (-2*deltay);
						}
					}
				}else{
					if((deltax < 0) && (deltay > 0)){
						x = xi;
						xi = xf;
						xf = x;
						deltax = xf - xi;
						d = 2*deltay - deltax;
						vg_set_pixel(xi,yi,color);
						y = yi;
						for(i=xf-1; i>xi; i--){
							if(d > 0){
								y++;
								vg_set_pixel(i,y,color);
								d += (2*deltay-2*deltax);
							}else{
								vg_set_pixel(i,y,color);
								d += (2*deltay);
							}
						}
					}else{
						if((deltax < 0) && (deltay < 0)){
							x = xi;
							xi = xf;
							xf = x;
							y = yi;
							yi = yf;
							yf = y;
							deltax = xf - xi;
							deltay = yf - yi;
							d = 2*deltay - deltax;
							vg_set_pixel(xi,yi,color);
							y = yi;
							for(i=xi+1; i<=xf; i++){
								if(d > 0){
									y++;
									vg_set_pixel(i,y,color);
									d += (2*deltay-2*deltax);
								}else{
									vg_set_pixel(i,y,color);
									d += (2*deltay);
								}
							}
						}
					}
				}
			}
		}else{
			if(deltay > 0){
				for(i=yi; i<=yf; i++){
					vg_set_pixel(xi,i,color);
				}
			}else{
				for(i=yi; i>=yf; i--){
					vg_set_pixel(xi,i,color);
				}
			}
		}
	}else{
		if(deltax > 0){
			for(i=xi; i<=xf; i++){
				vg_set_pixel(i,yi,color);
			}
		}else{
			for(i=xi; i>=xf; i--){
				vg_set_pixel(i,yi,color);
			}
		}
	}


	return 0;
}

void vg_draw_sprite(Sprite *spr){
	int i,j;

	for(i=0; i<spr->height; i++){
		for(j=0; j<spr->width; j++){
			vg_set_pixel(spr->x+j, spr->y+i, *(spr->true_color+((i*spr->width)+j)));
		}
	}
}

int vg_draw_rec(unsigned long xi, unsigned long yi,
		 unsigned long xf, unsigned long yf, unsigned long color){
	int i,j;

	for(i=yi; i<yf; i++){
		for(j=xi; j<xf; j++){
			vg_set_pixel(j,i, color);
		}
	}
	// TODO: Needs fixing
/*
 * 	char *write_address = video_mem;

	write_address += (bits_per_pixel/8)*((yi-1)*h_res+xi);

	for(i=yi; i<yf; i++){
		for(j=xi; j<xf; j++){
			write_address[j*4] = (color >> 8);
			write_address[j*4 +1] = (color && 0xFF);
		}
		write_address += h_res*(bits_per_pixel/8);
	}
 *
 */
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
