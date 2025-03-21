#include "debug-tick-timer-service.h"

/** Debug timer configuration */
typedef struct
{
    /** The external handler to call based on accelerated tick time */
    TickHandler external_handler;
    /** Units to subscribe to original handler for */
    TimeUnits external_units;
    /** The rate at which the seconds of a timer will run. This will skip time rather than change how fast seconds trigger */
    TimeScale time_scale;
    /** Accumluator of additional second from multiplier */
    time_t additional_seconds;
} DebugTimer;

static DebugTimer debug_timer;

/**
 * Internal tick handler to send ticks to the original handler based on the accelerated time scale
 * @param tick_time The current time to accelerate with additional seconds
 * @param units_changed The units that changed in relation to this handler being called
 */
static void internal_tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
    // The current second is subtracted
    uint8_t current_additional_seconds = -1;
    if (debug_timer.time_scale == QUARTER)
        current_additional_seconds += 15;

    if (debug_timer.time_scale == HALF)
        current_additional_seconds += 30;

    if (current_additional_seconds > 0)
        debug_timer.additional_seconds += current_additional_seconds;

    struct tm tick_time_stable;
    memcpy(&tick_time_stable, tick_time, sizeof(struct tm));

    time_t current_system_time = mktime(&tick_time_stable);

    time_t accelerated_time = current_system_time + debug_timer.additional_seconds;
    time_t accelerated_last_time = accelerated_time;
    if (current_additional_seconds > 0)
    {
        accelerated_last_time -= current_additional_seconds;
    }
    else
    {
        accelerated_last_time--;
    }

    // Get tm struct pointers copied to stable structs
    struct tm accelerated_tm;
    struct tm accelerated_last_tm;

    struct tm *accelerated_tm_ptr = localtime(&accelerated_time);
    memcpy(&accelerated_tm, accelerated_tm_ptr, sizeof(struct tm));

    struct tm *acclerated_last_tm_ptr = localtime(&accelerated_last_time);
    memcpy(&accelerated_last_tm, acclerated_last_tm_ptr, sizeof(struct tm));

    // Additional TimeUnits added as appropriate
    TimeUnits adjusted_units_changed = SECOND_UNIT;
    if (accelerated_tm.tm_min != accelerated_last_tm.tm_min)
        adjusted_units_changed |= MINUTE_UNIT;

    if (accelerated_tm.tm_hour != accelerated_last_tm.tm_hour)
        adjusted_units_changed |= HOUR_UNIT;

    if (accelerated_tm.tm_mday != accelerated_last_tm.tm_mday)
        adjusted_units_changed |= DAY_UNIT;

    if (accelerated_tm.tm_mon != accelerated_last_tm.tm_mon)
        adjusted_units_changed |= MONTH_UNIT;

    if (accelerated_tm.tm_year != accelerated_last_tm.tm_year)
        adjusted_units_changed |= YEAR_UNIT;

    if (adjusted_units_changed & debug_timer.external_units)
        debug_timer.external_handler(&accelerated_tm, adjusted_units_changed);
}

/**
 * Initialize the debug timer
 * @param units A representation of which time tick event to subscribe to
 * @param handler The callback to be executed on tick events
 * @param time_scale The rate at which time should run for tick events. This will skip time rather than trigger for each second
 */
void debug_tick_timer_service_subscribe(TimeUnits units, TickHandler handler, TimeScale time_scale)
{
    if (time_scale == REAL)
    {
        tick_timer_service_subscribe(units, handler);
    }
    else
    {
        debug_timer.external_handler = handler;
        debug_timer.external_units = units;
        debug_timer.time_scale = time_scale;
        debug_timer.additional_seconds = 0;

        tick_timer_service_subscribe(SECOND_UNIT, internal_tick_handler);
    }
}

/**
 * Unsubscribe from the debug tick timer service
 */
void debug_tick_timer_service_unsubscribe(void)
{
    tick_timer_service_unsubscribe();
}
