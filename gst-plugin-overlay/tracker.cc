
#include <stdint.h>
#include <string.h>

#include <ml-meta/ml_meta.h>
#include "tracker.h"
#include "msg.h"

#define MAX_FIELDS (100)

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
struct bbox {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    float    conf;
    uint16_t cat;       // class index
};

typedef bbox BB;

// object detection result
struct objdet_result {
    uint64_t time;
    uint32_t ctx_id;
    uint16_t numbb;
    struct   bbox * bb;
};

int trk_init() {
    GST_WARNING("trk_init enter");
    return msg_init();
}

int trk_analyze(GSList * meta_list) {
    guint meta_num = g_slist_length (meta_list);
    int  obj_count = 0;
    GstMLDetectionMeta * meta;
    GstMLClassificationResult * meta_info;
    struct objdet_result res;

    for (gint x = 0; x < meta_num; x++) {
        //gpointer metadata = g_slist_nth_data (meta_list, x);
        meta = (GstMLDetectionMeta *) g_slist_nth_data (meta_list, x);
        meta_info = (GstMLClassificationResult *) g_slist_nth_data (meta->box_info, 0);
        //todo: generalize
        if (g_strcmp0 (meta_info->name, "person") == 0) {
            obj_count++;
        }
    }
    if (obj_count = 0) {
        return 0;
    }

    res.numbb = obj_count;
    res.bb = new BB[obj_count];
    res.time = time(NULL);
    res.ctx_id = 1;

    // copy
    int idx = 0;
    for (gint x = 0; x < meta_num; x++) {
        //gpointer metadata = g_slist_nth_data (meta_list, x);
        //GstMLDetectionMeta * meta = (GstMLDetectionMeta *) metadata;
        meta = (GstMLDetectionMeta *) g_slist_nth_data (meta_list, x);
        meta_info = (GstMLClassificationResult *) g_slist_nth_data (meta->box_info, 0);
        //todo: generalize
        if (g_strcmp0 (meta_info->name, "person") == 0) {
            res.bb[idx].conf = meta_info->confidence;
            res.bb[idx].x = meta->bounding_box.x;
            res.bb[idx].y = meta->bounding_box.y;
            res.bb[idx].width = meta->bounding_box.width;
            res.bb[idx].height = meta->bounding_box.height;
            idx++;
        }
    }
    int buflen = sizeof(objdet_result) + obj_count * sizeof(BB);
    // send to azc over posix msg queue
    int rc = msg_send((char *) &res, buflen);

    GST_WARNING("buflen %d, rc after msg_send %d", buflen, rc);

    delete [] res.bb;
    return rc;
}

int trk_reset() {
    GST_WARNING("trk_reset enter");
    return msg_reset();
}
