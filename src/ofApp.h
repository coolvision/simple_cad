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
    UI_MOVE_CANVAS,
    UI_MOVING_CANVAS,
    UI_SELECT,
    UI_SELECTING,
    UI_DRAGGING
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

    int button_h;
    int button_w;
    int margin_top;
    int margin_left;
    int margin_bottom;

    // content storage & vis
//==============================================================================
    vector<Line> lines;

    // UI state machine
//==============================================================================
    UIState ui_state;
    Line curr_line;
    float zoom;
    ofPoint canvas_offset; // for dragging the canvas

    ofPoint start_click;

    // grid of points and lines
//==============================================================================
    void drawGrid();
    ofVbo grid_lines_vbo;
    ofVbo grid_points_vbo;
    float lines_step;
    float points_step;
    int lines_n_x;
    int lines_n_y;
    int points_n_x;
    int points_n_y;
};
