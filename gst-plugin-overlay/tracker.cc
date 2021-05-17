
#include <stdint.h>
#include <string.h>

#include <ml-meta/ml_meta.h>
#include "tracker.h"
#include "msg.h"

//#define MAX_FIELDS (100)
#define MAX_BB (100)

using namespace std;

const int result_prefix_len = 16;
const int PERSON = 1;
const int CAR = 3;
const int MAX_FIELDS = 100;
const int MIN_TIME_DELTA = 2;

// 192.168.1.129
//width 1920, height 1056
//scale_ratio_x 1.000000, scale_ratio_y 1.829268
//x_offset 0, y_offset 0

// 192.168.1.132
//width 2048, height 1536
//scale_ratio_x 1.000000, scale_ratio_y 1.339286
//x_offset 0, y_offset 0

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

Tracker::Tracker() {
    GST_WARNING("Tracker(), v.1.5.16.2");
    tracked[PERSON] = 1;
    tracked[CAR] = 1;
    last_check = 0;
    msg_init();
}

Tracker::~Tracker() {
    GST_WARNING("~Tracker()");
    msg_reset();
}

int Tracker::Track(GSList * meta_list) {
    struct objdet_result res;

    int count = (int) g_slist_length (meta_list);
    int obj_count = 0;

    if (count == 0) {
        return 0;
    }
    long now = time(NULL);
    long time_delta = now - last_check;
    if (time_delta < MIN_TIME_DELTA) {
        return 0;
    }
    //GST_WARNING("Track enter, count %d, time_delta %ld", count, time_delta);
    counts.clear();
    int idx = 0;
    for (int i = 0; i < count; i++) {
        meta = (GstMLDetectionMeta *) g_slist_nth_data (meta_list, i);
        meta_info = (GstMLClassificationResult *) g_slist_nth_data (meta->box_info, 0);
        // expect id name for categories - i.e. 1 person
        // extract only the id - convert to int
        cat = 0;
        //GST_WARNING("idx %d, name %s, conf %.2f", i, meta_info->name, meta_info->confidence);
        sscanf(meta_info->name, "%d ", &cat);
        if (cat == 0 || cat > 80) {
            continue;
        }
        //GST_WARNING("idx %d, cat %d, tracked %d", idx, cat, this->tracked[cat]);
        if (this->tracked[cat] == 0) {
            continue;
        }
        obj_count++;
        if (obj_count >= MAX_BB) {
            break;
        }
        ++counts[cat];
        res.bb[idx].cat = cat;
        res.bb[idx].conf = meta_info->confidence;
        x = res.bb[idx].x = meta->bounding_box.x;
        y = res.bb[idx].y = meta->bounding_box.y;
        w = res.bb[idx].width = meta->bounding_box.width;
        h = res.bb[idx].height = meta->bounding_box.height;
        //GST_WARNING("idx %d, conf %f, x %d, y %d, w %d, h %d", idx, res.bb[idx].conf, x, y, w, h);
        idx++;
    }
    //GST_WARNING("obj_count %d", obj_count);
    if (obj_count == 0) {
        last_check = now;
        return 0;
    }

    //GST_WARNING("prev people: %d, cars: %d, map size: %d", prev_counts[PERSON], prev_counts[CAR], counts.size());
    delta = 0;
    for (auto it = counts.cbegin(); it != counts.cend(); ++it) {
        cat = (*it).first;
        count = (*it).second;
        if (count > 0 && count > prev_counts[cat]) {
            delta += count - prev_counts[cat];
        }
    }
    last_check = now;
    if (delta <= 0) {
        return 0;
    }
    prev_counts = counts;
    GST_WARNING("delta: %d, people: %d, cars: %d", delta, counts[PERSON], counts[CAR]);
    res.numbb = obj_count;
    res.time = now;
    res.ctx_id = 1;
    int size = result_prefix_len + obj_count * sizeof(BB);
    // send to azc over posix msg queue
    rc = msg_send((char *) &res, size);
    //GST_WARNING("numbb %d, buflen %d, rc after msg_send %d", res.numbb, size, rc);
    return rc;
}

