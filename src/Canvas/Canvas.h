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
#include "Joint.h"


// ui state machine
enum UIState {
    UI_DRAW_LINE = 0,
    UI_DRAWING_LINE,
    UI_SELECT,
    UI_MOUSE_SELECTION,
    UI_MOVING_SELECTION,
    UI_MOVING_POINT,
    UI_ADD_VERTEX,
    UI_MOVE_CANVAS,
    UI_MOVING_CANVAS,
    UI_ADD_JOINT_FIXED,
    UI_ADD_JOINT_R
};

class Canvas {
public:

    Canvas();
    void save(string path);
    void load(string path);

    void update();
    void draw();

    // motion
    void getConnectedJoints(int line_i, vector <Joint *> *connected_j);

    vector<Action *> actions;
    int curr_action_i;
    void resetActions();
    void addAction(Action *a);

    UIState ui_state;


// storage & vis
//==============================================================================
    vector<InteractiveContainer *> lines;
    InteractiveObject *getItem(ItemId item_id);

    Polyline curr_line;
    Vertex add_v;

    JointsContainer joints;

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
    SelectionList new_selection;

    ofRectangle selection_r;

    bool selected_point;
    InteractiveObject *selected_p;

    // points
    bool hover_point;
    InteractiveObject *hover_point_p;

    // line segments
    bool hover_line;
    InteractiveObject *hover_line_p[2];

    // polygons
    bool hover_polygon;
    InteractiveContainer *hover_polygon_p;

    ofPoint snap(ofPoint p);
    ofPoint snapMm(ofPoint p);
    ofPoint getPx(ofPoint p);
    ofPoint getPx(InteractiveObject *v);
    ofPoint getMm(ofPoint p);

    void resetHover();
    void setHover(ofPoint p);
    void setHoverPoint(ofPoint p);

    ofPoint start_click_snap;
    ofPoint start_click;
    ofPoint start_p;

    void connectPolylines(InteractiveContainer *p);
};