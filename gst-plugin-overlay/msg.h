#ifndef __GST_INSG_mq_H__
#define __GST_INSG_mq_H__

#define QUEUE_NAME "/Qazc"
#define MAX_MSG (8000)

#ifdef __cplusplus
extern "C" {
#endif

int msg_init();

int msg_send(char *buffer, int size);

int msg_reset();

#ifdef __cplusplus
}
#endif

#endif // __GST_INSG_mq_H__