//
//  Geometry.h
//  simple_card
//
//  Created by sk on 9/24/14.
//
//

#pragma once

#include "ofMain.h"

class Line {
public:
    ofPoint p[2]; // points in mm
    bool selected[3]; // select one of the points or the whole line
    void draw();
};
