#include "pbl-display.h"

/**
 * Adds to a buffer the position of numbers on a square face
 * @param less_screen_size The amount of pixels to subtract from the bottom and right of the screen when calculating positions. Useful when drawing elements that draw from top-left like Fonts
 * @param x_inset The amount of inset to apply to x coordinates
 * @param y_inset The amount of inset to apply to y coordinates
 */
void square_face_positions(uint8_t less_screen_size, uint8_t x_inset, uint8_t y_inset, uint8_t buffer[][2])
{
    uint8_t pebble_width_minus_less_screen_size = DISPLAY_WIDTH - less_screen_size;
    uint8_t pebble_height_minus_less_screen_size = DISPLAY_HEIGHT - less_screen_size;
    uint8_t pebble_width_minus_inset = pebble_width_minus_less_screen_size - (x_inset * 2);
    uint8_t pebble_height_minus_inset = pebble_height_minus_less_screen_size - (y_inset * 2);

    uint8_t x_multiple = pebble_width_minus_inset / 4;
    uint8_t y_multiple = pebble_height_minus_inset / 4;
    uint8_t x_pos[5] = {
        x_inset,
        x_multiple + x_inset,
        (x_multiple * 2) + x_inset,
        (x_multiple * 3) + x_inset,
        pebble_width_minus_less_screen_size - x_inset};
    uint8_t y_pos[5] = {
        y_inset,
        y_multiple + y_inset,
        (y_multiple * 2) + y_inset,
        (y_multiple * 3) + y_inset,
        pebble_height_minus_less_screen_size - y_inset};

    // 1
    buffer[0][0] = x_pos[3];
    buffer[0][1] = y_pos[0];

    // 2
    buffer[1][0] = x_pos[4];
    buffer[1][1] = y_pos[1];

    // 3
    buffer[2][0] = x_pos[4];
    buffer[2][1] = y_pos[2];

    // 4
    buffer[3][0] = x_pos[4];
    buffer[3][1] = y_pos[3];

    // 5
    buffer[4][0] = x_pos[3];
    buffer[4][1] = y_pos[4];

    // 6
    buffer[5][0] = x_pos[2];
    buffer[5][1] = y_pos[4];

    // 7
    buffer[6][0] = x_pos[1];
    buffer[6][1] = y_pos[4];

    // 8
    buffer[7][0] = x_pos[0];
    buffer[7][1] = y_pos[3];

    // 9
    buffer[8][0] = x_pos[0];
    buffer[8][1] = y_pos[2];

    // 10
    buffer[9][0] = x_pos[0];
    buffer[9][1] = y_pos[1];

    // 11
    buffer[10][0] = x_pos[1];
    buffer[10][1] = y_pos[0];

    // 12
    buffer[11][0] = x_pos[2];
    buffer[11][1] = y_pos[0];
}