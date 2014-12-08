//
//  Vertex.h
//  simple_cad
//
//  Created by sk on 11/26/14.
//
//

#pragma once

#include "ofMain.h"
#include "InteractiveObject.h"

class Polyline;

class Vertex: public InteractiveObject {
public:
    Vertex() {
        next = NULL;
        prev = NULL;
//        parent = NULL;
    };
    virtual ~Vertex() {};

    void draw();
    InteractiveObject *getCopy();
    void release();
    void update();

    Vertex *next;
    Vertex *prev;

    static ofImage dot_icon;
};
