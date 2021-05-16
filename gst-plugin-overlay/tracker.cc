
#include <stdint.h>
#include <string.h>

#include <ml-meta/ml_meta.h>
#include "tracker.h"
#include "msg.h"

#define MAX_FIELDS (100)
#define MAX_BB (100)

// context
struct cam_context {
    uint32_t ctx_id;
    uint16_t cam;
    uint16_t width;
    uint16_t height;
    uint16_t model;
    // scale is multiplied by 10000 and rounded to 4 digits precision
    float scale_x;
    float scale_y;
    // bbox field list - e.g. "x,y,w,h,conf,cat"
    char  fields[MAX_FIELDS];
};

// bounding box
// 16 bytes
struct bbox {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    uint16_t cat;       // class index
    uint16_t res0;
    float    conf;
};
//const int bbox_size = sizeof(struct bbox);
typedef struct bbox BB;

// object detection result
// 16 bytes + 100*16 - max 1616
struct objdet_result {
    uint64_t time;
    uint32_t ctx_id;
    uint16_t numbb;
    uint16_t res1;
    BB       bb[MAX_BB];
};

const int result_prefix_len = 16;

Tracker::Tracker() {
    GST_WARNING("Tracker(), v.1.5.16.0");
    msg_init();
}

Tracker::~Tracker() {
    GST_WARNING("~Tracker()");
    msg_reset();
}

int Tracker::Track(GSList * meta_list) {
    guint meta_num = g_slist_length (meta_list);
    int  obj_count = 0;
    GstMLDetectionMeta * meta;
    GstMLClassificationResult * meta_info;
    struct objdet_result res;

    GST_WARNING("Track enter, num %u", meta_num);

    for (gint x = 0; x < meta_num; x++) {
        //gpointer metadata = g_slist_nth_data (meta_list, x);
        meta = (GstMLDetectionMeta *) g_slist_nth_data (meta_list, x);
        meta_info = (GstMLClassificationResult *) g_slist_nth_data (meta->box_info, 0);
        //todo: generalize
        GST_WARNING("idx %d, cat %s, conf %f", x, meta_info->name, meta_info->confidence);
        if (g_strcmp0 (meta_info->name, "1 person") == 0) {
            obj_count++;
            if (obj_count >= MAX_BB) {
                break;
            }
        }
    }

    GST_WARNING("obj_count %d", obj_count);
    if (obj_count == 0) {
        return 0;
    }
    res.numbb = obj_count;
    res.time = time(NULL);
    res.ctx_id = 1;

    // copy
    int idx = 0;
    int x, y, w, h;
    for (gint x = 0; x < meta_num; x++) {
        //gpointer metadata = g_slist_nth_data (meta_list, x);
        //GstMLDetectionMeta * meta = (GstMLDetectionMeta *) metadata;
        meta = (GstMLDetectionMeta *) g_slist_nth_data (meta_list, x);
        meta_info = (GstMLClassificationResult *) g_slist_nth_data (meta->box_info, 0);
        //todo: generalize
        if (g_strcmp0 (meta_info->name, "person") == 0) {
            res.bb[idx].conf = meta_info->confidence;
            x = res.bb[idx].x = meta->bounding_box.x;
            y = res.bb[idx].y = meta->bounding_box.y;
            w = res.bb[idx].width = meta->bounding_box.width;
            h = res.bb[idx].height = meta->bounding_box.height;
            GST_WARNING("idx %d, conf %f, x %d, y %d, w %d, h %d", idx, res.bb[idx].conf, x, y, w, h);
            idx++;
        }
    }
    int buflen = result_prefix_len + obj_count * sizeof(BB);
    // send to azc over posix msg queue
    int rc = msg_send((char *) &res, buflen);
    GST_WARNING("numbb %d, buflen %d, rc after msg_send %d", res.numbb, buflen, rc);

    return rc;
}

