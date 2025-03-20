#include "pebble.h"

#if PBL_PLATFORM_EMERY
#define DISPLAY_CENTER_X 100
#define DISPLAY_CENTER_Y 114
#define DISPLAY_SCALE_X 1.39
#define DISPLAY_SCALE_Y 1.36
#elif PBL_PLATFORM_CHALK
#define DISPLAY_CENTER_X 90
#define DISPLAY_CENTER_Y 90
#define DISPLAY_SCALE_X 1.25
#define DISPLAY_SCALE_Y 1.07
#else
#define DISPLAY_CENTER_X 72
#define DISPLAY_CENTER_Y 84
#define DISPLAY_SCALE_X 1
#define DISPLAY_SCALE_Y 1
#endif