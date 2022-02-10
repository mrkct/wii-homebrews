#include "frelib/base/Math.h"
#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;
static ir_t ir;
struct orient_t orientation;

void DrawHLine(int x1, int x2, int y, int color) {
  int i;
  y = 320 * y;
  x1 >>= 1;
  x2 >>= 1;
  for (i = x1; i <= x2; i++) {
    u32 *tmpfb = (u32 *)xfb;
    tmpfb[y + i] = color;
  }
}

void DrawVLine(int x, int y1, int y2, int color) {
  int i;
  x >>= 1;
  for (i = y1; i <= y2; i++) {
    u32 *tmpfb = (u32 *)xfb;
    tmpfb[x + (640 * i) / 2] = color;
  }
}

void DrawBox(int x1, int y1, int x2, int y2, int color) {
  DrawHLine(x1, x2, y1, color);
  DrawHLine(x1, x2, y2, color);
  DrawVLine(x1, y1, y2, color);
  DrawVLine(x2, y1, y2, color);
}

void Init() {
  VIDEO_Init();
  WPAD_Init();
  rmode = VIDEO_GetPreferredMode(NULL);
  xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
  console_init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight,
               rmode->fbWidth * VI_DISPLAY_PIX_SZ);
  VIDEO_Configure(rmode);
  VIDEO_SetNextFramebuffer(xfb);
  VIDEO_SetBlack(FALSE);
  VIDEO_Flush();
  VIDEO_WaitVSync();
  if (rmode->viTVMode & VI_NON_INTERLACE) {
    VIDEO_WaitVSync();
  }

  WPAD_SetVRes(WPAD_CHAN_0, 640, 480);
  WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
}

int main(int argc, char **argv) {
  Init();
  u64 counter = 0;

  while (1) {
    WPAD_ScanPads();
    WPAD_IR(WPAD_CHAN_0, &ir);
    WPAD_Orientation(WPAD_CHAN_0, &orientation);

    u32 pressed = WPAD_ButtonsDown(0);
    u32 down = WPAD_ButtonsHeld(0);
    u32 released = WPAD_ButtonsUp(0);

    if (pressed & WPAD_BUTTON_HOME) {
      exit(0);
    }

    if (pressed & WPAD_BUTTON_A) {
      printf("Pressed button A...\n");
      counter = 0;
    }

    if (down & WPAD_BUTTON_A) {
      counter++;
    }

    if ((released & WPAD_BUTTON_A) && counter > 0) {
      printf("Released button A. Counter=%llu\n", counter);
      counter = 0;
    }

    VIDEO_ClearFrameBuffer(rmode, xfb, COLOR_BLACK);

    printf("\x1b[2;0H");
    printf("\n\npitch: %.4f\nroll: %.4f\nyaw: %.4f\n", orientation.pitch,
           orientation.roll, orientation.yaw);
    printf("test: %d\n", clamp(0, 100, 70));

    DrawBox(ir.x - 32, ir.y - 32, ir.x + 32, ir.y + 32, COLOR_RED);

    VIDEO_WaitVSync();
  }

  return 0;
}
