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
    Vertex() {};
    virtual ~Vertex() {};
    void draw();
    InteractiveObject *getCopy();
    static ofImage dot_icon;
};
