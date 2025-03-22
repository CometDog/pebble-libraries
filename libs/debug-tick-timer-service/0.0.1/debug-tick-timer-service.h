#include "pebble.h"

/**
 * An enum representing the rate as which time will progress
 */
typedef enum
{
    REAL,
    QUARTER_MINUTE,
    HALF_MINUTE,
    FULL_MINUTE,
    QUARTER_HOUR,
    HALF_HOUR,
    FULL_HOUR,
    QUARTER_DAY,
    HALF_DAY,
    FULL_DAY
} TimeScale;

void debug_tick_timer_service_subscribe(TimeUnits units, TickHandler handler, TimeScale time_scale);
void debug_tick_timer_service_unsubscribe(void);
