#ifndef __DOTF_H
#define __DOTF_H

// System Libraries
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

// LCOM Libraries
#include "i8042.h"
#include "i8254.h"
#include "kbc.h"
#include "lmlib.h"
#include "mouse.h"
#include "rtc.h"
#include "speaker.h"
#include "timer.h"
#include "vbe.h"
#include "video_gr.h"

// Language Libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

// Pixmap Libraries
#include "pixmap.h"
#include "read_xpm.h"
#include "sprite.h"
#include "asprite.h"

// Sprites for the Project
#include "Resources/bad.h"
#include "Resources/boss.h"
#include "Resources/bossbig.h"
#include "Resources/missbad.h"
#include "Resources/missgood.h"
#include "Resources/ship.h"

// Defines
#define VIDEO_MODE	0x117

#endif
