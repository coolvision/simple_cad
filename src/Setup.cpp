//
//  Setup.cpp
//  simple_card
//
//  Created by sk on 9/26/14.
//
//

#include "ofApp.h"

void ofApp::setup(){

    ofDisableAntiAliasing();
    ofDisableSmoothing();

    font.setup("DejaVuSansMono.ttf");

    c.zoom = 1.0f;

    ofSetFrameRate(30);

    setGrid();

    screen_grid.loadImage(ofToDataPath("screen.gif"));

//==============================================================================
    Button::font.setup("DejaVuSansMono.ttf");

    // ui layout settings
    button_h = 24;
    button_w = 24;
    margin_top = 20;
    margin_left = 20;
    margin_bottom = 10;

    // toolbox buttons
    int start_y = margin_top;
    int inc_y = button_h + margin_bottom;

    canvas_toolbar.y = start_y;
    canvas_toolbar.x = margin_left;
    canvas_toolbar.button_h = 24;
    canvas_toolbar.button_w = 24;
    canvas_toolbar.margin = margin_bottom;

    int i = 0;

    select_button = new Button("select", margin_left,
                                start_y, button_w, button_h);
    select_button->icon.loadImage(ofToDataPath("icons/cursor/icon.png"));
    ofAddListener(select_button->click_event, this, &ofApp::SelectButtonClick);
    canvas_toolbar.buttons.push_back(select_button);
    select_button->hover = true;
    i++;

    line_button = new Button("draw line",
                        margin_left, start_y + inc_y * i, button_w, button_h);
    line_button->icon.loadImage(ofToDataPath("icons/polyline/icon.png"));
    ofAddListener(line_button->click_event, this, &ofApp::LineButtonClick);
    canvas_toolbar.buttons.push_back(line_button);
    i++;

    vertex_button = new Button("add vertex",
                             margin_left, start_y + inc_y * i, button_w, button_h);
    vertex_button->icon.loadImage(ofToDataPath("icons/dot/icon.png"));
    ofAddListener(vertex_button->click_event, this, &ofApp::VertexButtonClick);
    canvas_toolbar.buttons.push_back(vertex_button);
    i++;

    move_button = new Button("move", margin_left,
                        start_y + inc_y * i, button_w, button_h);
    move_button->icon.loadImage(ofToDataPath("icons/move/icon.png"));
    ofAddListener(move_button->click_event, this, &ofApp::MoveButtonClick);
    canvas_toolbar.buttons.push_back(move_button);
    i++;

    Button *button = new Button("zoom in",
                        margin_left, start_y + inc_y * i, button_w, button_h);
    button->icon.loadImage(ofToDataPath("icons/plus/icon.png"));
    ofAddListener(button->click_event, this, &ofApp::ZoomInButtonClick);
    canvas_toolbar.buttons.push_back(button);
    i++;

    button = new Button("zoom out",
                        margin_left, start_y + inc_y * i, button_w, button_h);
    button->icon.loadImage(ofToDataPath("icons/minus/icon.png"));
    ofAddListener(button->click_event, this, &ofApp::ZoomOutButtonClick);
    canvas_toolbar.buttons.push_back(button);
    i++;

    button = new Button("revolute", margin_left,
                             start_y + inc_y * i, button_w, button_h);
    button->icon.loadImage(ofToDataPath("icons/revolute.png"));
    ofAddListener(button->click_event, this, &ofApp::addRJointClick);
    canvas_toolbar.buttons.push_back(button);
    i++;

    button = new Button("fixed",
                                margin_left, start_y + inc_y * i, button_w, button_h);
    button->icon.loadImage(ofToDataPath("icons/fix3.png"));
    ofAddListener(button->click_event, this, &ofApp::addFixedJointClick);
    canvas_toolbar.buttons.push_back(button);
    i++;

    button = new Button("gear",
                        margin_left, start_y + inc_y * i, button_w, button_h);
    button->icon.loadImage(ofToDataPath("icons/gear.png"));
    //ofAddListener(button->click_event, this, &ofApp::ZoomOutButtonClick);
    canvas_toolbar.buttons.push_back(button);

    Joint::joint_icon_fixed.loadImage(ofToDataPath("icons/fix3_canvas.png"));
    Joint::joint_icon_r.loadImage(ofToDataPath("icons/revolute2_canvas.png"));
    Vertex::dot_icon.loadImage(ofToDataPath("icons/dot.png"));

//==============================================================================
    // default start state
    c.ui_state = UI_SELECT;
    
    c.clearSelection();
    c.resetHover();

    zoom_in = false;
    zoom_out = false;

    line_length_info = 0.0f;
}

void ofApp::setGrid() {

    float w = ofGetWindowWidth();
    float h = ofGetWindowHeight();

    points_step = 4 * c.zoom;
    if (points_step < 1) {
        points_step = 1;
    }
    c.px_step = points_step;
    lines_step = points_step * 5;

    int off_x = -1 + (int)c.canvas_offset.x % lines_step;
    int off_y = (int)c.canvas_offset.y % lines_step;

    InteractiveObject::points_step = points_step;
    InteractiveObject::zoom = c.zoom;
    InteractiveObject::offset = c.canvas_offset;
    for (int i = 0; i < c.lines.size(); i++) {
        c.lines[i]->update();
    }
    
    // mm grid
    points_n_x = (w / points_step) + 10;
    points_n_y = (h / points_step) + 10;
    lines_n_x = (w / lines_step) + 1 + 2;
    lines_n_y = (h / lines_step) + 1 + 2;

    ofVec2f *points_v = new ofVec2f[points_n_x * points_n_y];
    if (points_step > 1) {
        for (int i = 0; i < points_n_y; i++) {
            for (int j = 0; j < points_n_x; j++) {
                points_v[i * points_n_x + j].set(off_x + (j-10) * points_step,
                                                 off_y + (i-10) * points_step);
            }
        }
        grid_points_vbo.setVertexData(points_v, points_n_x * points_n_y, GL_STATIC_DRAW);
    } else {
        grid_points_vbo.clear();
    }

    // 5 mm grid
    ofVec2f *grid_v = new ofVec2f[(lines_n_x + lines_n_y) * 2];
    int line_i = 0;
    for (int i = 0; i < lines_n_y; i++) {
        grid_v[line_i*2].set(0, off_y + (i-2) * lines_step);
        grid_v[line_i*2+1].set(w, off_y + (i-2) * lines_step);
        line_i++;
    }
    for (int i = 0; i < lines_n_x; i++) {
        grid_v[line_i*2].set(off_x + (i-2) * lines_step + 1, 0);
        grid_v[line_i*2+1].set(off_x + (i-2) * lines_step + 1, h);
        line_i++;
    }
    grid_lines_vbo.setVertexData(grid_v, (lines_n_x + lines_n_y) * 2, GL_STATIC_DRAW);

    delete[] grid_v;
    delete[] points_v;
}
