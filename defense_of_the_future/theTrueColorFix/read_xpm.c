/**
 *  @author Joao Cardoso (jcard@fe.up.pt) ????
 *  Added by pfs@fe.up.pt
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HRES 768
#define VRES 1024

unsigned long *read_xpm(char *map[], unsigned long *cmap[], int *wd, int *ht)
{
  int width, height, colors;
  unsigned long col;
  int i, j;
  char *tmp, *line;
  char symbol;
  unsigned long *pixtmp, *pix;

  /* read width, height, colors */
  if (sscanf(map[0],"%d %d %d", &width, &height, &colors) != 3) {
    printf("read_xpm: incorrect width, height, colors\n");
    return NULL;
  }
#ifdef DEBUG
  printf("%d %d %d\n", width, height, colors);
#endif
  if (width > HRES || height > VRES || colors > 16800000) {
    printf("read_xpm: incorrect width, height, colors\n");
    return NULL;
  }

  *wd = width;
  *ht = height;

  char sym[16800000];

  for (i=0; i<colors; i++)
    sym[i] = 0;

  /* read symbols <-> colors */
  for (i=0; i<colors; i++) {
    if (sscanf(map[i+1], "%c %d", &symbol, &col) != 2) {
      printf("read_xpm: incorrect symbol, color at line %d\n", i+1);
      return NULL;
    }
#ifdef DEBUG
    printf("%c %d\n", symbol, col);
#endif
    if (col > 16800000) {
      printf("read_xpm: incorrect color at line %d\n", i+1);
      return NULL;
    }
    sym[col] = symbol;
  }
  
  /* allocate pixmap memory */
  pix = pixtmp = malloc((width*height)*sizeof(unsigned long));

  /* parse each pixmap symbol line */
  for (i=0; i<height; i++) {
    line = map[colors+1+i];
#ifdef DEBUG
    printf("\nparsing %s\n", line);
#endif
    for (j=0; j<width; j++) {
      tmp = memchr(sym, line[j], 16800000);  /* find color of each symbol */
      if (tmp == NULL) {
    	  printf("read_xpm: incorrect symbol at line %d, col %d\n", colors+i+1, j);
    	  return NULL;
      }
      *pixtmp++ = tmp - sym; /* pointer arithmetic! back to books :-) */
#ifdef DEBUG
      printf("%c:%d ", line[j], tmp-sym);
#endif
    }
  }
  return pix;
}
