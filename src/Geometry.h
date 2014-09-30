//
//  Geometry.h
//  simple_card
//
//  Created by sk on 9/24/14.
//
//

#pragma once

#include "ofMain.h"

class Polyline {
public:
    Polyline() {
        selected = false;
        p.resize(2);
        selected_p.resize(2);
        selected_p[0] = false;
        selected_p[1] = false;
    };
    deque<ofPoint> p;

    deque<bool> selected_p; // points selection
    deque<bool> selected_line; // line segments selection

    bool selected;  // whole polyline selection
    bool hover; // highlighting with mouse hover
};
