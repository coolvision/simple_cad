//
//  Canvas.h
//  simple_card
//
//  Created by sk on 10/15/14.
//
//

#pragma once

#include "ofMain.h"

#include "Polyline.h"
#include "Actions.h"

class Canvas {
public:

    Canvas();

    vector<Action *> actions;
    int curr_action_i;
    void resetActions();

// content storage & vis
//==============================================================================
    vector<Polyline *> lines;
    void drawLine(Polyline *l);

    Polyline curr_line;
    Vertex add_v;

// zoom and offset
//==============================================================================
    float zoom;
    void zoomIn();
    void zoomOut();
    ofPoint canvas_offset;
    ofPoint canvas_offset_start;
    int px_step; // px per 1mm

// content selection and manipulation
//==============================================================================
    void clearSelection();
    void deleteSelection();
    SelectionList selection;

    ofRectangle selection_r;

    // points
    bool hover_point;
    Vertex *hover_point_p;
    bool selected_point;
    Vertex *selected_point_p;

    // line segments
    bool hover_line;
    Vertex *hover_line_p[2];
    bool selected_line;
    Vertex *selected_line_p[2];

    // polygons
    bool hover_polygon;
    Polyline *hover_polygon_p;

    bool was_selected_point;
    bool move_already_selected;

    ofPoint snap(ofPoint p);
    ofPoint snapMm(ofPoint p);
    ofPoint getPx(ofPoint p);
    ofPoint getPx(Vertex *v);
    ofPoint getMm(ofPoint p);

    void resetHover();
    void setHover(ofPoint p);
    void setHoverPoint(ofPoint p);

    ofPoint start_click;
    ofPoint start_p;

    Polyline *connectLine(ofPoint *p1, ofPoint *p2);
    Polyline *connectPolylines(Polyline *p);
};