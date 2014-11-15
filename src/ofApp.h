#pragma once

#include "ofMain.h"

#include "ofxFontStash.h"

#include "Polyline.h"
#include "Canvas.h"

#include "Button.h"



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

// canvas
//==============================================================================
    Canvas c;

    ofPoint curr_p;
    ofPoint prev_p;

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
    void addFixedJointClick(ButtonClickEventData &d);
    void addRJointClick(ButtonClickEventData &d);

    Button *select_button;
    Button *line_button;
    Button *vertex_button;
    Button *move_button;

    int button_h;
    int button_w;
    int margin_top;
    int margin_left;
    int margin_bottom;

    Toolbar canvas_toolbar;

    // UI state machine
//==============================================================================
    float line_length_info;
    bool zoom_in;
    bool zoom_out;

    bool do_update;

    // grid of points and lines
//==============================================================================
    void setGrid();
    void drawGrid();
    ofVbo grid_lines_vbo;
    ofVbo grid_points_vbo;
    ofVbo grid_points_sub_vbo;
    int lines_step;
    int points_step;
    int lines_n_x;
    int lines_n_y;
    int points_n_x;
    int points_n_y;
};
