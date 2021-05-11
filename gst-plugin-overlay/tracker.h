#ifndef __GST_INSG_TRACKER_H__
#define __GST_INSG_TRACKER_H__

#include <gmodule.h>
#include <gst/gst.h>

#define HACK 1

int trk_init();

int trk_analyze(int num);

int trk_reset();

#endif // __GST_INSG_TRACKER_H__