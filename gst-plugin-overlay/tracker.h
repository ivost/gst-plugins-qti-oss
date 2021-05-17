#pragma once

#include <gmodule.h>
#include <gst/gst.h>
#include <map>

#define HACK 1

// dealing with COCO categories only for now < 100 classes
#define MAX_CAT 100

class Tracker {
public:
    Tracker();
    ~Tracker();
    int Track(GSList * meta_list);
private:
    long last_check;
    int rc;
    int cat;
    int delta;
    int x, y, w, h;
    // which categories to track?
    std::map<int, int> tracked;
    // count per category
    std::map<int, int> counts;
    // count per category
    std::map<int, int> prev_counts;
    GstMLDetectionMeta * meta;
    GstMLClassificationResult * meta_info;
};

