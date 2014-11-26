//
//  Geometry.cpp
//  simple_card
//
//  Created by sk on 10/1/14.
//
//

#include "Polyline.h"

void Polyline::draw() {

    if (front == NULL || back == NULL) {
        return;
    }

    ofSetColor(ofColor::red);
    ofCircle(front->getPx(p), 5);

    ofPushStyle();

    ofEnableSmoothing();
    ofEnableAntiAliasing();

    float point_size = 3.0f * InteractiveObject::zoom;
    if (point_size < 1.0f) {
        point_size = 1.0f;
    }

    selected = true;
    for (InteractiveObject *v = front; v != NULL && v->next != NULL; v = v->next) {
        if (!v->selected) {
            selected = false;
            break;
        }
        if (v->next == front) break; // closed polylines
    }

    if (hover) {
        path.setStrokeColor(ofColor::orangeRed);
    } else {
        path.setStrokeColor(80);
    }
    path.setStrokeWidth(1.0f);
    if (closed) {
        path.setFilled(true);
        if (hover || selected) {
            if (fixed) {
                path.setFillColor(ofColor(80, 80, 80, 200));
            } else {
                if (controlled) {
                    ofColor color = ofColor::steelBlue - ofColor(10);
                    color.a = 200;
                    path.setFillColor(color);
                } else {
                    path.setFillColor(ofColor(190, 190, 190, 200));
                }
            }
        } else {
            if (fixed) {
                path.setFillColor(ofColor(100, 100, 100, 200));
            } else {
                if (controlled) {
                    ofColor color = ofColor::steelBlue;
                    color.a = 200;
                    path.setFillColor(color);
                } else {
                    path.setFillColor(ofColor(200, 200, 200, 200));
                }
            }
        }
    } else {
        path.setFilled(false);
    }
    path.draw();

    ofSetLineWidth(1.0f);
    ofSetColor(ofColor::black);
    for (InteractiveObject *v = front; v != NULL && v->next != NULL; v = v->next) {
        if (v->hover && v->next->hover) {
            ofSetColor(ofColor::orangeRed);
            ofLine(v->getPx(v->p), v->getPx(v->next->p));
        } else if (v->selected && v->next->selected) {
            ofSetColor(ofColor::steelBlue);
            ofLine(v->getPx(v->p), v->getPx(v->next->p));
        }
        if (v->next == front) break; // closed polylines
    }

    for (InteractiveObject *v = front; v != NULL; v = v->next) {
        if (!v->hover && !v->selected) {
            v->draw();
        }
        if (v->next == front) break; // closed polylines
    }

    for (InteractiveObject *v = front; v != NULL; v = v->next) {
        if (v->selected && !v->hover) {
            v->draw();
        }
        if (v->next == front) break; // closed polylines
    }

    for (InteractiveObject *v = front; v != NULL; v = v->next) {
        if (v->hover) {
            v->draw();
        }
        if (v->next == front) break; // closed polylines
    }

    ofDisableAntiAliasing();
    ofDisableSmoothing();
}

bool Polyline::inside(ofPoint p) {

    if (ofp.inside(p)) {
        return true;
    }

    return false;
}

void Polyline::update(ofPoint p) {
    update();

    if (ofp.inside(p)) {
        hover = true;
    } else {
        hover = false;
    }
}

void Polyline::update() {

    updateIndexes();

    path.clear();
    for (InteractiveObject *v = front; v != NULL; v = v->next) {
        if (v == front) {
            path.newSubPath();
            path.moveTo(v->getPx(v->p));
        }
        path.lineTo(v->getPx(v->p));
        if (v->next == front) {
            path.lineTo(v->getPx(v->next->p));
            break; // closed polylines
        }
    }

    ofp.clear();
    for (InteractiveObject *v = front; v != NULL; v = v->next) {
        ofp.addVertex(v->getPx(v->p));
        if (v->next == front) break; // closed polylines
    }
    ofp.setClosed(closed);

    ofPoint center;
    int n = 0;
    for (InteractiveObject *v = front; v != NULL; v = v->next) {
        center += v->p;
        n++;
        if (v->next == front) break; // closed polylines
    }
    center /= (float)n;
    this->p = center;
}


int InteractiveContainer::getId() {
    return id;
}

void InteractiveContainer::updateIndexes() {

    int n = getLength();
    items.resize(n);

    int l = 0;
    for (InteractiveObject *v = front; v != NULL; v = v->next) {
        items[l] = v;
        v->id = l;
        l++;
        if (v->next == front) break; // closed polylines
    }
}

void InteractiveContainer::init(InteractiveObject *p) {
    InteractiveObject *i = p->getCopy();
    i->parent = this;
    front = i;
    back = front;
}

void InteractiveContainer::popBack() {

    if (front == NULL) {
        return;
    } else {
        InteractiveObject *tmp = back;

        if (back->prev != NULL) {
            back = back->prev;
            back->next = NULL;
            delete tmp;
        } else {
            delete back;
            back = NULL;
            front = NULL;
        }
    }
    updateIndexes();
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
    updateIndexes();
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
    updateIndexes();
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

    // cout << "InteractiveContainer::getItem " << i  << endl;
    // cout << "items() " << items.size() << endl;

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


