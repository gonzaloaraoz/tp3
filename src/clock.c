#include <clock.h>
#include <string.h>
#define TIME_SIZE 6
#define START_VALUE 0
#define SECONDS_UNITS 5

#define SECONDS_TENS 4
#define MINUTE_UNITS 3
#define MINUTE_TENS 2
#define MAX_TENS_VALUE
#define MAX_UNITS_VALUE
#define INITIAL_VALUE
#define ALARM_SIZE 4


struct clock_s{
    bool valid;
    bool enabled;
    uint16_t ticks_per_second;
    uint16_t ticks_count;
    uint8_t time[TIME_SIZE];
    uint8_t alarm[ALARM_SIZE];
    clock_event_t event_hanler;
};

static struct clock_s instances ;

/*
static const uint8_t BCD_LIMITS[] = {MAX_TENS_VALUE, MAX_UNITS_VALUE};

void IncrementTime(clock_t clock) {
    for (int index = siseof(clock->time) - 1; index >= 0; index--) {
        clock->time[index]++;
        if (clock->time[index] > BCD_LIMITS[index % 2]) {
            clock->time[index] = INITIAL_VALUE;
        } else {
            break;
        }
    }
}
*/

clock_t ClockCreate(uint16_t ticks_pers_second, clock_event_t event_handler) {
    instances.valid = false;
    instances.enabled = false;
    instances.event_hanler = event_handler;
    instances.ticks_count = START_VALUE;
    instances.ticks_per_second = ticks_pers_second;
    memset(instances.time, START_VALUE, TIME_SIZE);
    return &instances;
}

bool ClockGetTime(clock_t clock, uint8_t * time, uint8_t size) {
    memcpy(time, clock->time, size);
    return clock->valid;
}

void ClockSetupTime(clock_t clock, uint8_t const * const time, uint8_t size) {
    memcpy(clock->time, time, size);
    clock->valid = true;
    
}

void ClockNewTicket(clock_t clock){
    clock->ticks_count++;
    if (clock->ticks_count == clock->ticks_per_second) {
        clock->ticks_count=START_VALUE;
        clock->time[SECONDS_UNITS]++;
        if(clock->time[SECONDS_UNITS] == 10){
            clock->time[SECONDS_UNITS] = 0;
            clock->time[SECONDS_TENS]++;
                if(clock->time[SECONDS_TENS] == 6){
                    clock->time[SECONDS_TENS] = 0;
                    clock->time[MINUTE_UNITS]++;
                    if(clock->time[MINUTE_UNITS] == 10){
                        clock->time[MINUTE_UNITS] = 0;
                        clock->time[MINUTE_TENS]++;
                    }
                }
        
        }
        bool activate = (clock->time[SECONDS_TENS]==0 ) && (clock->time[SECONDS_UNITS]==0 );
        for(int index =0; index < ALARM_SIZE; index++){
            if (clock->alarm[index] != clock->time[index]){
                activate = false;
                break;
            }
        }
        if (activate && clock->enabled) {
            clock->event_hanler(clock,true);
        }

    }
}

void ClockSetupAlarm(clock_t clock, uint8_t const * const alarm, uint8_t size) {
    memcpy(clock->alarm, alarm, size);
    clock->enabled = true;
}

bool ClockGetAlarm(clock_t clock, uint8_t * alarm, uint8_t size){
    memcpy(alarm, clock->alarm, size);
    return clock->enabled;
}


bool ClockToggleAlarm(clock_t clock){
    clock->enabled = !clock->enabled;
    return clock->enabled;
}
