#ifndef __GST_INSG_TRACKER_H__
#define __GST_INSG_TRACKER_H__

#include <gmodule.h>
#include <gst/gst.h>

#ifdef __cplusplus
extern "C" {
#endif

int azc_init();

int azc_send();

int azc_reset();

#ifdef __cplusplus
}
#endif

#define HACK 1

int trk_init();

int trk_analyze(int num);

int trk_reset();

#endif // __GST_INSG_TRACKER_H__