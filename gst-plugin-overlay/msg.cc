#include <string.h>

#include "msg.h"
#include <mqueue.h>

#define QUEUE_NAME "/Qazc"
#define MAX_MSG (2048)

static mqd_t mq = -1;
static char msg[MAX_MSG];

int msg_init() {
    if (mq >= 0) {
        return 0;
    }
    mq = mq_open(QUEUE_NAME, O_WRONLY);
    if (mq < 0) {
        return -1;
    }
    return 0;
}

int msg_send(char *buffer, int size) {
    if (mq < 0) {
        if (msg_init() < 0) {
            return -1;
        }
    }
    if (size <= 0) {
        return -2;
    }
    if (size > MAX_MSG) {
        return -3;
    }
    memcpy(msg, buffer, size);
    return mq_send(mq, msg, size, 0);
}

int msg_reset() {
    if (mq < 0) {
        return -1;
    }
    return mq_close(mq);
}
