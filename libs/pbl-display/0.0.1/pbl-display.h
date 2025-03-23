#include "pebble.h"

#if PBL_PLATFORM_EMERY
#define DISPLAY_WIDTH 200
#define DISPLAY_HEIGHT 228
#define DISPLAY_CENTER_X DISPLAY_WIDTH / 2
#define DISPLAY_CENTER_Y DISPLAY_HEIGHT / 2
#define DISPLAY_SCALE_X 1.39
#define DISPLAY_SCALE_Y 1.36
#elif PBL_PLATFORM_CHALK
#define DISPLAY_WIDTH 180
#define DISPLAY_HEIGHT 180
#define DISPLAY_CENTER_X DISPLAY_WIDTH / 2
#define DISPLAY_CENTER_Y DISPLAY_HEIGHT / 2
#define DISPLAY_SCALE_X 1.25
#define DISPLAY_SCALE_Y 1.07
#else
#define DISPLAY_WIDTH 144
#define DISPLAY_HEIGHT 168
#define DISPLAY_CENTER_X DISPLAY_WIDTH / 2
#define DISPLAY_CENTER_Y DISPLAY_HEIGHT / 2
#define DISPLAY_SCALE_X 1
#define DISPLAY_SCALE_Y 1
#endif

void round_face_positions(uint8_t less_screen_size, uint8_t x_inset, uint8_t y_inset, uint8_t buffer[][2]);
void square_face_positions(uint8_t less_screen_size, uint8_t x_inset, uint8_t y_inset, uint8_t buffer[][2]);