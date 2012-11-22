#include "dotf.h"

#define		VIDEO_105		0x105

int main(){

	sef_startup();

	vg_init(VIDEO_105);

	sleep(5);

	if(vg_exit())
		return 1;
	else
		return 0;
}
