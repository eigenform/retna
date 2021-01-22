#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ogcsys.h>
#include <ogc/pad.h>

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;
void video_init()
{
	VIDEO_Init();
	PAD_Init();
	rmode = VIDEO_GetPreferredMode(NULL);
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight, 
			rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) {
		VIDEO_WaitVSync();
	}
}

void retna_quit(int res) 
{
	printf("[*] Press START to return to the system menu\n");
	while(1) 
	{
		PAD_ScanPads();
		u32 pressed = PAD_ButtonsDown(0);
		if ( pressed & PAD_BUTTON_START ) {
			printf("[!] Returning to system menu ...\n");
			exit(res);
		}
		VIDEO_WaitVSync();
	}
}

