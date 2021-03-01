// state.h - based on figure 3.15 - rdt 3.0 sender (p 245)
#ifndef STATE_H
#define STATE_H

#include <time.h>

#define MAX_RETRIES 8
#define SECONDS_PER_RETRY 2

enum Status {init = 0, send_ACK = 1, wait_for_ACK = 2, wait_for_SEQ = 3, send_data_packets = 4, receive_SEQ = 5};

typedef struct{
    time_t start_time;
    int stopped;
    int num_retries;
    int last_retry_time;
} Timer;

typedef struct {
    enum Status status;
    int expected_SEQ;
    Timer timer;
} ClientState;

typedef struct {
    enum Status status;
    int expected_ACK;
    int next_SEQ;
    Timer timer;
} ServerState;

void start_timer(Timer* timer);
int get_elapsed_time(Timer* timer);

#endif
