#ifndef __UI_H
#define __UI_H

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
extern lv_font_t * font;
int UI_Init(void);
void FT_font_Set(uint8_t size);

#endif