#include "state.h"
#include <unistd.h>

void start_timer(Timer* timer) {
    timer->start_time = time(NULL);
    timer->stopped = 0;
    timer->num_retries = 0;
    timer->last_retry_time = 0;
}

int get_elapsed_time(Timer* timer) {
    time_t now = time(NULL);
    int elapsed_seconds = (int) (now - timer->start_time);
    return elapsed_seconds;
}

