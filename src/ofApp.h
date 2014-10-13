#pragma once

#include "ofMain.h"

#include "ofxFontStash.h"
#include "ofxSvg.h"

#include "Geometry.h"

#include "Button.h"

// ui state machine
enum UIState {
    UI_DRAW_LINE = 0,
    UI_DRAWING_LINE,
    UI_SELECT,
    UI_MOUSE_SELECTION,
    UI_MOVING_POINT,
    UI_MOVING_LINE,
    UI_MOVING_POLYGON,
    UI_ADD_VERTEX,
    UI_MOVE_CANVAS
};

class SelectionList {
public:
    vector<Vertex *> vertices;
    vector<ofPoint> start_p;
    void add(Vertex *v);
    void clear();
};

class ofApp : public ofBaseApp {

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    // UI
//==============================================================================
    ofxFontStash font;

    // store and display available patches list
    void unselectMode();
    void SelectButtonClick(ButtonClickEventData &d);
    void LineButtonClick(ButtonClickEventData &d);
    void VertexButtonClick(ButtonClickEventData &d);
    void MoveButtonClick(ButtonClickEventData &d);
    void ZoomInButtonClick(ButtonClickEventData &d);
    void ZoomOutButtonClick(ButtonClickEventData &d);

    Button *select_button;
    Button *line_button;
    Button *vertex_button;
    Button *move_button;

    Button *add_line;
    Button *add_vertex;

    int button_h;
    int button_w;
    int margin_top;
    int margin_left;
    int margin_bottom;

    Toolbar canvas_toolbar;
    Toolbar cursor_toolbar;
    ofPoint toolbar_off;

    // content selecting
//==============================================================================
    void clearSelection();
    SelectionList selection;

    // content storage & vis
//==============================================================================
    vector<Polyline *> lines;
    void drawLine(Polyline *l);

    // UI state machine
//==============================================================================
    UIState ui_state;
    Polyline curr_line;
    float zoom;
    ofPoint canvas_offset; // for dragging the canvas
    ofPoint snap(ofPoint p);
    void updateToolbar(ofPoint p);
    void resetHover();
    
    ofPoint start_click;
    ofRectangle selection_r;

    bool was_selected_point;
    bool move_already_selected;
    
    Vertex add_v;

    // points
    bool hover_point;
    Vertex *hover_point_p;

    // line segments
    bool hover_line;
    Vertex *hover_line_p[2];

    // polygons
    bool hover_polygon;
    Polyline *hover_polygon_p;


    Polyline *connectLine(ofPoint *p1, ofPoint *p2);
    Polyline *connectPolylines(Polyline *p);

    // grid of points and lines
//==============================================================================
    void drawGrid();
    ofVbo grid_lines_vbo;
    ofVbo grid_points_vbo;
    int lines_step;
    int points_step;
    int lines_n_x;
    int lines_n_y;
    int points_n_x;
    int points_n_y;
};
