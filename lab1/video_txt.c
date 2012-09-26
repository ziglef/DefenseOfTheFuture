#ifdef EMUL

#include <stdio.h>
#include <stdlib.h>

#else

#include <minix/drivers.h>
#include <sys/video.h>
#include <sys/mman.h>

#endif

#include <assert.h>

#include "vt_info.h"

#include "video_txt.h"

/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */

static unsigned scr_width;	/* Width of screen in columns */
static unsigned scr_lines;	/* Height of screen in lines */

void vt_fill(char ch, char attr) {
  
  char *ptr;
  ptr = video_mem;
  
  int width;
  int height;
  
  for(height=0; height<scr_lines; height++)
  {
	  for(width=0; width<scr_width; width++)
	  {
		  *ptr = ch;
		  ptr++;
		  *ptr = attr;
		  ptr++;
	  }
  }
}

void vt_blank(void) {
	vt_fill(0x00, 0x00);
}

int vt_print_char(char ch, int r, int c, char attr) {
  
	  char *ptr;
	  ptr = video_mem;

	  int width;
	  int height;

	  if((r > scr_lines) || (c > scr_width))
	  {
		  return 1;
	  }

	  for(height=1; height<r; height++)
	  {
		  for(width=1; width<=scr_width; width++)
		  {
			  ptr++;
			  ptr++;
		  }
	  }

	  for(width=1; width<c; width++)
	  {
		  ptr++;
		  ptr++;
	  }

	  *ptr = ch;
	  ptr++;
	  *ptr = attr;
	  ptr++;

	  return 0;
}

int vt_print_string(char *str, int r, int c, char attr) {

	  char *ptr;
	  ptr = video_mem;

	  int width;
	  int height;

	  int i;

	  int row = r;
	  int column = c;

	  if((r > scr_lines) ||
		 (c > scr_width) ||
		 ((scr_lines*scr_width) < ((r-1)*scr_width + c + (strlen(str) -1))))
	  {
		  return 1;
	  }

	  for(i=0; i<strlen(str); i++)
	  {
		  vt_print_char(str[i], row, column, attr);
		  if(column == scr_width)
		  {
			  column = 1;
			  row++;
		  }
		  else
			  column++;
	  }
	  return 0;
}

int vt_print_int(int num, int r, int c, char attr) {

	  char *ptr;
	  ptr = video_mem;

	  int i = 1;

	  int row = r;
	  int column = c;

	  int grandeza = 0;
	  int _grandeza = 0;
	  int _num = num;


	  while(_num>0)
	  {
		  _num = _num/10;
		  grandeza++;
	  }


	  if((r > scr_lines) ||
		 (c > scr_width) ||
		 ((scr_lines*scr_width) < ((r-1)*scr_width + c + (grandeza -1))))
	  {
		  return 1;
	  }


	  _num = num;
	  _grandeza = grandeza;

	  while(_grandeza > 1)
	  {
		  i = i * 10;
		  _grandeza--;
	  }


	  for(i=i; i>0; i=i/10)
	  {
		  _num = _num / i;

		  switch(_num)
		  {
			  case 0: vt_print_char(0x30,row,column,attr); break;
			  case 1: vt_print_char(0x31,row,column,attr); break;
			  case 2: vt_print_char(0x32,row,column,attr); break;
			  case 3: vt_print_char(0x33,row,column,attr); break;
			  case 4: vt_print_char(0x34,row,column,attr); break;
			  case 5: vt_print_char(0x35,row,column,attr); break;
			  case 6: vt_print_char(0x36,row,column,attr); break;
			  case 7: vt_print_char(0x37,row,column,attr); break;
			  case 8: vt_print_char(0x38,row,column,attr); break;
			  case 9: vt_print_char(0x39,row,column,attr); break;
		  }

		  if(column == scr_width)
		  {
			  column = 1;
			  row++;
		  }
		  else
			  column++;

		  _num = num % i;
	  }


	  return 0;
}


int vt_draw_frame(int width, int height, int r, int c, char attr) {

	  char *ptr;
	  ptr = video_mem;

	  int i;

	  int row = r;
	  int column = c;

	  int size = width*height;

	  if((r + height -1 > scr_lines) || (c + width -1 > scr_width) || ((width || height) <= 0))
	  {
		  return 1;
	  }

	  for(i=1; i<=size; i++)
	  {
		  if(i==1)
			  vt_print_char(0xC9, row, column, attr);
		  else
			  if(i==width)
				  vt_print_char(0xBB, row, column, attr);
			  else
				  if(i==size-width+1)
					  vt_print_char(0xC8, row, column, attr);
				  else
					  if(i==size)
						  vt_print_char(0xBC, row, column, attr);
					  else
						  if((row==r) || (row==(r+height-1)))
							  vt_print_char(0xCD, row, column, attr);
						  else
							  if((column==c) || (column==(c+width-1)))
								  vt_print_char(0xBA, row, column, attr);

		  if(column == (c+width-1))
		  		  {
		  			  column = c;
		  			  row++;
		  		  }
		  		  else
		  			  column++;
	  }

}

/* Needed only for emulation: does not emulate attributes */

void vt_update_display(void) {
  int i, j;
  for(i = 0; i < scr_lines; i++) {
      for(j = 0; j <  scr_width; j++)
	  printf("%c", (*(video_mem + i*scr_width*2 + j*2)) & 0xFF);
      printf("\n");
  }
}

#ifdef EMUL
char *vt_init(vt_info_t *vi_p) {

  /* No need for memory mapping */

  /* Save text mode resolution */
  
  video_mem = vi_p->vram_base;

  scr_lines = vi_p->scr_lines;
  scr_width = vi_p->scr_width;

  return video_mem;
}

#else

char *vt_init(vt_info_t *vi_p) {

  int r;
  struct mem_range mr;

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes)(vi_p->vram_base);
  mr.mr_limit = mr.mr_base + vi_p->vram_size;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
	  panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vi_p->vram_size);

  if(video_mem == MAP_FAILED)
	  panic("video_txt couldn't map video memory");

  /* Save text mode resolution */

  scr_lines = vi_p->scr_lines;
  scr_width = vi_p->scr_width;

  return video_mem;
}

#endif

