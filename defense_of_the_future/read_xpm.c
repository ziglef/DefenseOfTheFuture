/**
 *  @author Joao Cardoso (jcard@fe.up.pt) ????
 *  Added by pfs@fe.up.pt
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tabdispersao.h"

#define HRES 1024
#define VRES 768
#define SIXTYKCOLORS 0

typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

/**************************************************************************/
/*!
    @brief  Converts a 24-bit RGB color to an equivalent 16-bit RGB565 value

    @param[in]  r
                8-bit red
    @param[in]  g
                8-bit green
    @param[in]  b
                8-bit blue

    @section Example

    @code

    // Get 16-bit equivalent of 24-bit color
    uint16_t gray = drawRGB24toRGB565(0x33, 0x33, 0x33);

    @endcode
*/
/**************************************************************************/
uint16_t drawRGB24toRGB565(uint8_t r, uint8_t g, uint8_t b)
{
#ifdef DEBUG
		printf("R:%d G:%d B:%d\n", r,g,b);
		printf("%d\n", ( ((r / 8) << 11) | ((g / 4) << 5) | (b / 8)) );
		sleep(1);
#endif
		return ( ((r / 8) << 11) | ((g / 4) << 5) | (b / 8) );
}

unsigned long *read_xpm(char *map[], unsigned long *cmap[], int *wd, int *ht)
{
  int width, height, colors;
  unsigned int col;
  int i, j;
  int *tmp;
  char *line;
  char symbol;
  unsigned long *pixtmp, *pix;
  unsigned long col_aux;

  /* read width, height, colors */
  if (sscanf(map[0],"%d %d %d", &width, &height, &colors) != 3) {
    printf("read_xpm: incorrect width, height, colors\n");
    return NULL;
  }
#ifdef DEBUG
  printf("%d %d %d\n", width, height, colors);
  sleep(1);
#endif
  if (width > HRES || height > VRES || colors > 255) {
    printf("read_xpm: incorrect width, height, colors\n");
    return NULL;
  }

  *wd = width;
  *ht = height;

  int sym[255];

  for (i=0; i<colors; i++)
    sym[i] = 0;

  /* read symbols <-> colors */
  for (i=0; i<colors; i++) {
    if (sscanf(map[i+1], "%c %x", &symbol, &col) != 2) {
      printf("read_xpm: incorrect symbol, color at line %d\n", i+1);
      return NULL;
    }
#ifdef DEBUG
    printf("%c %d\n", symbol, col);
    sleep(1);
#endif
    if (col > 16800000) {
      printf("read_xpm: incorrect color at line %d\n", i+1);
      return NULL;
    }
    if(SIXTYKCOLORS)
        col = drawRGB24toRGB565((col>>16)&0xFF,(col>>8)&0xFF,col&0xFF);

    sym[(int)symbol] = col;

#ifdef DEBUG
    printf("%c %d\n", symbol, col);
#endif
  }
  
  /* allocate pixmap memory */
  pix = pixtmp = malloc((width*height)*sizeof(int));

  /* parse each pixmap symbol line */
  for (i=0; i<height; i++) {
    line = map[colors+1+i];
#ifdef DEBUG
    printf("\nparsing %s\n", line);
    sleep(1);
#endif
    for (j=0; j<width; j++) {

      tmp = &(sym[(int)line[j]]);  /* find color of each symbol */
      if (tmp == NULL) {
    	  printf("read_xpm: incorrect symbol at line %d, col %d\n", colors+i+1, j);
    	  return NULL;
      }
      *pixtmp++ = *tmp; /* pointer arithmetic! back to books :-) */
#ifdef DEBUG
      printf("%c:%d ", line[j], *tmp);
#endif
    }
  }
  return pix;
}
