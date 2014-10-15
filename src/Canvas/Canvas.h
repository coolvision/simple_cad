//
//  Canvas.h
//  simple_card
//
//  Created by sk on 10/15/14.
//
//

#include "ofMain.h"

#include "Polyline.h"

class SelectionList {
public:
    vector<Vertex *> vertices;
    vector<ofPoint> start_p;
    void add(Vertex *v);
    void clear();
};

class Canvas {
public:

// content storage & vis
//==============================================================================
    vector<Polyline *> lines;
    void drawLine(Polyline *l);

    Polyline curr_line;
    Vertex add_v;

// zoom and offset
//==============================================================================
    float zoom;
    bool zoom_in;
    bool zoom_out;
    void zoomIn();
    void zoomOut();
    ofPoint canvas_offset;
    ofPoint canvas_offset_start;

// content selection and manipulation
//==============================================================================
    void clearSelection();
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