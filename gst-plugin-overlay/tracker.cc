#include <string.h>

#include "tracker.h"
#include "msg.h"

int trk_init() {
    GST_WARNING("trk_init enter");
    return msg_init();
}

int trk_analyze(int num) {
    if (num <= 0) {
        return 0;
    }
    // GST_WARNING("trk_analyze enter, num %d", num);
    char *buf = "0x00\x01\0xFF";
    int rc = msg_send(buf, 3);
    GST_WARNING("rc after msg_send %d", rc);
    return rc;
}

int trk_reset() {
    GST_WARNING("trk_reset enter");
    return msg_reset();
}
