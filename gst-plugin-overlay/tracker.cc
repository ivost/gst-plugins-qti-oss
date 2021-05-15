#include <string.h>

#include "tracker.h"

int trk_init() {
    GST_WARNING("trk_init enter");
    return azc_init();
}

int trk_analyze(int num) {
    GST_WARNING("trk_analyze enter, num %d", num);
    int rc = azc_send();
    return num;
}

int trk_reset() {
    GST_WARNING("trk_reset enter");
    return azc_reset();
}
