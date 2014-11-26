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
    };
    virtual ~Vertex() {};

    InteractiveObject *next;
    InteractiveObject *prev;
    
    void draw();
    InteractiveObject *getCopy();
    static ofImage dot_icon;
};
