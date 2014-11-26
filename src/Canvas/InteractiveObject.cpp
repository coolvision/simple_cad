//
//  InteractiveObject.cpp
//  simple_cad
//
//  Created by sk on 10/28/14.
//
//

#include "InteractiveObject.h"

int InteractiveObject::points_step = 0;
ofPoint InteractiveObject::offset;
float InteractiveObject::zoom;

ItemId InteractiveContainer::getId() {
    ItemId i;
    i.container_id = id;
    i.item_id = 0;
    return i;
}

ItemId InteractiveObject::getId() {

    ItemId i;
    i.container_id = parent->getId().container_id;
    i.item_id = id;

    return i;
}

ofPoint InteractiveObject::getPx(ofPoint p) {

    p *= points_step;
    p += offset;

    return p;
}

ofPoint InteractiveObject::getPx() {

    // how many px per mm
    // points_step = 8.0f * zoom;

    ofPoint p = this->p;
    p *= points_step;
    p += offset;

    return p;
}
