#include <clock.h>
#include <string.h>

struct clock_s{
    bool valid;
    uint8_t time[6];
};

static struct clock_s instances;


clock_t ClockCreate(uint16_t ticks_pers_second){
    instances.valid = false;
    memset(instances.time, 0, 6);
    return &instances;
}

bool ClockGetTime(clock_t clock, uint8_t * time, uint8_t size) {
    memcpy(clock->time, time, size);
    return clock->valid;
}

void ClockSetupTime(clock_t clock, uint8_t const * const time, uint8_t size) {
    memcpy(time, clock->time, size);
    clock->valid = true;
    
}