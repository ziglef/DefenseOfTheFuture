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
#include "Resources/explosions.h"

// Sounds for the Project
#include "Resources/piano.h"
#include "Resources/sounds.h"

// Defines
#define VIDEO_MODE				0x117

#define SHIP_START_X			484
#define SHIP_START_Y			618

#define ENEMIES_START_X			140
#define ENEMIES_START_X2		190
#define ENEMIES_START_Y			130
#define ENEMIES_X_INCREMENT		100
#define ENEMIES_Y_INCREMENT		94
#define NO_ENEMIES				15
#define NO_PSHOTS				4
#define NO_EXPLOSIONS			5

// Makecodes
#define WMAKE					0x11
#define SMAKE					0x1F
#define AMAKE					0x1E
#define DMAKE					0x20
#define SPACEMAKE				0x39

#endif
