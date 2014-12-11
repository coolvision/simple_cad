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

float segmentDistance(ofPoint v, ofPoint w, ofPoint p);
ofPoint lineProjection(ofPoint v, ofPoint w, ofPoint p);
int circle_circle_intersection(double x0, double y0, double r0,
                               double x1, double y1, double r1,
                               double *xi, double *yi,
                               double *xi_prime, double *yi_prime);

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

    int update_i;
    int updating_angle_i;

    Canvas();
    void save(string path);
    void load(string path);

    void update(bool update_angles = false);
    void updateDistances();
    void draw();
    void connectJoints();
    void updateAngles();
    void updatePolygons();
    void updateSupported();
    void updateRotationTargets();

    void getLinks(Joint *j, vector<Joint *> *links, vector<float> *length = NULL);

    // motion
    void getConnectedPolygons(int joint_id,
                            vector<InteractiveContainer *> *connected,
                            vector<ofPoint *> *connected_rel);

    vector<Action *> actions;
    int curr_action_i;
    void resetActions();
    void addAction(Action *a);

    UIState ui_state;

// storage & vis
//==============================================================================
    vector<Polyline *> lines;
    vector<Joint *> joints;

    // pointers to all vertices and joints
    // not sure how useful it can be...
    vector<InteractiveObject *> items;

    InteractiveObject *getItem(ItemId item_id);

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
    SelectionList new_selection;

    ofRectangle selection_r;

    // points
    bool hover_point;
    InteractiveObject *hover_point_p;

    // line segments
    bool hover_line;
    Vertex *hover_line_p[2];

    // polygons
    bool hover_polygon;
    Polyline *hover_polygon_p;

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

    void connectPolylines(Polyline *p);
};