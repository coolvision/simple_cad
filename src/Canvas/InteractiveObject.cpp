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

int InteractiveContainer::getId() {
    return id;
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

ItemId InteractiveObject::getId() {

    ItemId i;
    i.container_id = parent->getId();
    i.item_id = id;

    return i;
}

// can be made more efficient by maintaining an array of the vertices
InteractiveObject *InteractiveContainer::getItem(int i) {

    if (i < items.size()) {
        return items[i];
    }
    return NULL;
}

void InteractiveContainer::release() {

    if (front != NULL) {
        InteractiveObject *v = front;
        InteractiveObject *tmp;
        while (v != NULL) {
            tmp = v->next;
            delete v;
            v = tmp;
            if (tmp == front) {  // closed polylines
                break;
            }
        }
    }
    front = NULL;
    back = NULL;

    update();
}

void InteractiveContainer::reverse() {

    if (front == NULL) {
        return;
    }

    InteractiveObject *tmp;
    InteractiveObject *v = front;
    for (InteractiveObject *v = front; v != NULL; v = tmp) {
        tmp = v->next;
        v->next = v->prev;
        v->prev = tmp;
        v = tmp;
        if (tmp == front) break; // closed polylines
    }
    tmp = front;
    front = back;
    back = tmp;
    update();
}

int InteractiveContainer::getLength() {
    int l = 0;
    for (InteractiveObject *v = front; v != NULL; v = v->next) {
        l++;
        if (v->next == front) break; // closed polylines
    }
    return l;
}