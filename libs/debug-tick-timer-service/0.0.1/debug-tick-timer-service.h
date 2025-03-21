#include "pebble.h"

/**
 * An enum representing the rate as which time will progress
 */
typedef enum
{
    REAL,
    QUARTER,
    HALF
} TimeScale;

void debug_tick_timer_service_subscribe(TimeUnits units, TickHandler handler, TimeScale time_scale);
void debug_tick_timer_service_unsubscribe(void);
