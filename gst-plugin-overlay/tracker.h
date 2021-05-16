#pragma once

#include <gmodule.h>
#include <gst/gst.h>

#define HACK 1

// dealing with COCO categories only for now < 100 classes
#define MAX_CAT 100

class Tracker {
public:
    Tracker();
    ~Tracker();
    int Track(GSList * meta_list);
};

