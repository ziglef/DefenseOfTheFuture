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

// System Libraries
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
