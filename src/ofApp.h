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
    UI_MOVING_POINT,
    UI_MOVING_LINE,
    UI_MOVE_CANVAS
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
    vector<Button *> buttons;
    void SelectButtonClick(ButtonClickEventData &d);
    void LineButtonClick(ButtonClickEventData &d);
    void MoveButtonClick(ButtonClickEventData &d);
    void ZoomInButtonClick(ButtonClickEventData &d);
    void ZoomOutButtonClick(ButtonClickEventData &d);
    void addLineClick(ButtonClickEventData &d);

    Button *select_button;
    Button *line_button;
    Button *move_button;
    Button *add_line;

    int button_h;
    int button_w;
    int margin_top;
    int margin_left;
    int margin_bottom;

    // content storage & vis
//==============================================================================
    vector<Polyline> lines;
    void drawLine(Polyline *l);

    // UI state machine
//==============================================================================
    UIState ui_state;
    Polyline curr_line;
    float zoom;
    ofPoint canvas_offset; // for dragging the canvas
    ofPoint snap(ofPoint p);

    ofPoint start_click;

    ofPoint add_line_start;

    bool was_selected_point;

    // points selection
    bool selected_point;
    ofPoint *selected_point_p;
    int selected_point_i;

    // line segments selection
    bool selected_line;
    ofPoint *selected_line_p[2];
    bool selected_line_i[2];

    // polyline selection
    bool selected_polyline;
    int selected_polyline_i;

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
