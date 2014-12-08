//
//  Geometry.cpp
//  simple_card
//
//  Created by sk on 10/1/14.
//
//

#include "Polyline.h"

// can be made more efficient by maintaining an array of the vertices
InteractiveObject *Polyline::getItem(int i) {

    if (i < items.size()) {
        return items[i];
    }
    return NULL;
}

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
    for (Vertex *v = front; v != NULL && v->next != NULL; v = v->next) {
        if (!v->selected) {
            selected = false;
            break;
        }
        if (v->next == front) break; // closed polylines
    }

    if (controlled) {
        path.setStrokeColor(ofColor::darkRed);
    } else if (selected) {
        path.setStrokeColor(ofColor::steelBlue);
    } else if (hover) {
        path.setStrokeColor(ofColor::orangeRed);
    } else {
        path.setStrokeColor(80);
    }
    path.setStrokeWidth(1.0f);
    if (closed) {
        path.setFilled(true);
        if (hover || selected) {
            path.setFillColor(ofColor(190, 190, 190, 200));
        } else {
            path.setFillColor(ofColor(200, 200, 200, 200));
        }
    } else {
        path.setFilled(false);
    }
    path.draw();

    ofSetLineWidth(1.0f);
    ofSetColor(ofColor::black);
    for (Vertex *v = front; v != NULL && v->next != NULL; v = v->next) {
        if (!hover && !controlled && (v->hover && v->next->hover)) {
            ofSetColor(ofColor::orangeRed);
            ofLine(v->getPx(v->p), v->getPx(v->next->p));
        } else if (!selected && !controlled && (v->selected && v->next->selected)) {
            ofSetColor(ofColor::steelBlue);
            ofLine(v->getPx(v->p), v->getPx(v->next->p));
        }
        if (v->next == front) break; // closed polylines
    }

    for (Vertex *v = front; v != NULL; v = v->next) {
        if (!v->hover && !v->selected) {
            v->draw();
        }
        if (v->next == front) break; // closed polylines
    }

    for (Vertex *v = front; v != NULL; v = v->next) {
        if (v->selected && !v->hover) {
            v->draw();
        }
        if (v->next == front) break; // closed polylines
    }

    for (Vertex *v = front; v != NULL; v = v->next) {
        if (v->hover) {
            v->draw();
        }
        if (v->next == front) break; // closed polylines
    }


//    for (int i = 0; i < links.size(); i++) {
//        ofDrawBitmapStringHighlight(ofToString(links[i]), front->getPx(p) + ofPoint(0, i * 15));
//    }


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
    for (Vertex *v = front; v != NULL; v = v->next) {
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
    for (Vertex *v = front; v != NULL; v = v->next) {
        ofp.addVertex(v->getPx(v->p));
        if (v->next == front) break; // closed polylines
    }
    ofp.setClosed(closed);

    ofPoint center;
    int n = 0;
    for (Vertex *v = front; v != NULL; v = v->next) {
        center += v->p;
        n++;
        if (v->next == front) break; // closed polylines
    }
    center /= (float)n;
    this->p = center;
}

void Polyline::updateIndexes() {

    int n = getLength();
    items.resize(n);

    int l = 0;
    for (Vertex *v = front; v != NULL; v = v->next) {
        items[l] = v;
        v->id = l;
        l++;
        if (v->next == front) break; // closed polylines
    }
}

void Polyline::init(Vertex *p) {
    Vertex *i = (Vertex *)p->getCopy();
    i->parent = this;
    front = i;
    back = front;
}

void Polyline::popBack() {

    if (front == NULL) {
        return;
    } else {
        Vertex *tmp = back;

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

void Polyline::addBack(Vertex *p) {

    if (front == NULL) {
        init(p);
    } else {
        // new element
        Vertex *i = (Vertex *)p->getCopy();
        i->parent = this;
        // update the list
        back->next = i;
        i->prev = back;
        back = i;
    }
    updateIndexes();
}

void Polyline::addFront(Vertex *p) {

    if (front == NULL) {
        init(p);
    } else {
        // new element
        Vertex *i = (Vertex *)p->getCopy();
        i->parent = this;
        // update the list
        front->prev = i;
        i->next = front;
        front = i;
    }
    updateIndexes();
}

void Polyline::addBack(Polyline *p) {

    if (front == NULL || p->closed) {
        return;
    }

    for (Vertex *v = p->front; v != NULL; v = v->next) {
        addBack(v);
    }
}

void Polyline::addFront(Polyline *p) {

    if (front == NULL || p->closed) {
        return;
    }

    for (Vertex *v = p->back; v != NULL; v = v->prev) {
        addFront(v);
    }
}

void Polyline::cloneFrom(Polyline *p) {

    *this = *p;
    front = NULL;
    back = NULL;

    release();

    for (Vertex *v = p->front; v != NULL; v = v->next) {
        addBack(v);
        if (v->next == p->front) break; // closed polylines
    }
    if (closed) {
        back->next = front;
        front->prev = back;
    }

    update();
}

void Polyline::release() {

    if (front != NULL) {
        Vertex *v = front;
        Vertex *tmp;
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

void Polyline::reverse() {

    if (front == NULL) {
        return;
    }

    Vertex *tmp;
    Vertex *v = front;
    for (Vertex *v = front; v != NULL; v = tmp) {
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

int Polyline::getLength() {
    int l = 0;
    for (Vertex *v = front; v != NULL; v = v->next) {
        l++;
        if (v->next == front) break; // closed polylines
    }
    return l;
}


