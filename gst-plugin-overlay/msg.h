#ifndef __GST_INSG_mq_H__
#define __GST_INSG_mq_H__

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