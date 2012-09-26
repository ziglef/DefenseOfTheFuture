#include "vt_info.h"

/* For video RAM emulation only */

#define NO_LINES 25
#define NO_COLUMNS 80

static char video_mem[NO_LINES][NO_COLUMNS*2];

void vt_info_get(vt_info_t *vi_p) {

  vi_p->scr_width = NO_COLUMNS;
  vi_p->scr_lines = NO_LINES;

  vi_p->vram_base = video_mem;
  vi_p->vram_size = NO_COLUMNS * NO_LINES;

}

void vt_info_display(vt_info_t *vip) {
  printf("vram_base: 0x%p\n", (void *)vip->vram_base);
  printf("vram_size: 0x%X (%d)\n", (unsigned int) vip->vram_size, (int)vip->vram_size);
  printf("scr_lines: %d\n", vip->scr_lines);
  printf("scr_width: %d\n", vip->scr_width);
}


