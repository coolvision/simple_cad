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

void InteractiveContainer::init(InteractiveObject *p) {
    InteractiveObject *i = p->getCopy();
    i->parent = this;
    front = i;
    back = front;
}

void InteractiveContainer::addBack(InteractiveObject *p) {

    if (front == NULL) {
        init(p);
    } else {
        // new element
        InteractiveObject *i = p->getCopy();
        i->parent = this;
        // update the list
        back->next = i;
        i->prev = back;
        back = i;
    }
    update();
}

void InteractiveContainer::addFront(InteractiveObject *p) {

    if (front == NULL) {
        init(p);
    } else {
        // new element
        InteractiveObject *i = p->getCopy();
        i->parent = this;
        // update the list
        front->prev = i;
        i->next = front;
        front = i;
    }
    update();
}

void InteractiveContainer::addBack(InteractiveContainer *p) {

    if (front == NULL || p->closed) {
        return;
    }

    for (InteractiveObject *v = p->front; v != NULL; v = v->next) {
        addBack(v);
    }
}

void InteractiveContainer::addFront(InteractiveContainer *p) {

    if (front == NULL || p->closed) {
        return;
    }

    for (InteractiveObject *v = p->back; v != NULL; v = v->prev) {
        addFront(v);
    }
}


void InteractiveContainer::cloneFrom(InteractiveContainer *p) {

    *this = *p;
    front = NULL;
    back = NULL;

    release();

    for (InteractiveObject *v = p->front; v != NULL; v = v->next) {
        addBack(v);
        if (v->next == p->front) break; // closed polylines
    }
    if (closed) {
        back->next = front;
        front->prev = back;
    }

    update();
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