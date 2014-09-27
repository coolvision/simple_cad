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

    zoom = 1.0f;

    ofSetFrameRate(30);

    float w = ofGetWindowWidth();
    float h = ofGetWindowHeight();

    points_step = 8.0f;
    lines_step = points_step * 5;
    lines_n_x = (w / lines_step) + 1;
    lines_n_y = (h / lines_step) + 1;

    int off_x = 0;

    ofVec2f *grid_v = new ofVec2f[(lines_n_x + lines_n_y) * 2];
    int line_i = 0;
    for (int i = 0; i < lines_n_y; i++) {
        grid_v[line_i*2].set(off_x, i * lines_step);
        grid_v[line_i*2+1].set(w, i * lines_step);
        line_i++;
    }
    for (int i = 0; i < lines_n_x; i++) {
        grid_v[line_i*2].set(off_x + i * lines_step + 1, 0);
        grid_v[line_i*2+1].set(off_x + i * lines_step + 1, h);
        line_i++;
    }
	grid_lines_vbo.setVertexData(grid_v, (lines_n_x + lines_n_y) * 2, GL_STATIC_DRAW);

    points_n_x = (w / points_step);
    points_n_y = (h / points_step);
    ofVec2f *points_v = new ofVec2f[points_n_x * points_n_y];
    for (int i = 0; i < points_n_y; i++) {
        for (int j = 0; j < points_n_x; j++) {
            points_v[i * points_n_x + j].set(off_x + j * points_step, i * points_step);
        }
    }
	grid_points_vbo.setVertexData(points_v, points_n_x * points_n_y, GL_STATIC_DRAW);

    delete[] grid_v;
    delete[] points_v;

    // default start state
    ui_state = UI_SELECT;

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

    Button *button = new Button("select", margin_left,
                                start_y, button_w, button_h);
    button->icon.loadImage(ofToDataPath("icons/cursor/icon.png"));
    ofAddListener(button->click_event, this, &ofApp::SelectButtonClick);
    buttons.push_back(button);
    button->selected = true;

    button = new Button("draw line",
                        margin_left, start_y + inc_y * 1, button_w, button_h);
    button->icon.loadImage(ofToDataPath("icons/line/icon.png"));
    ofAddListener(button->click_event, this, &ofApp::LineButtonClick);
    buttons.push_back(button);

    button = new Button("move", margin_left,
                        start_y + inc_y * 2, button_w, button_h);
    button->icon.loadImage(ofToDataPath("icons/move/icon.png"));
    ofAddListener(button->click_event, this, &ofApp::MoveButtonClick);
    buttons.push_back(button);

    button = new Button("zoom in",
                        margin_left, start_y + inc_y * 3, button_w, button_h);
    button->icon.loadImage(ofToDataPath("icons/plus/icon.png"));
    ofAddListener(button->click_event, this, &ofApp::ZoomInButtonClick);
    buttons.push_back(button);

    button = new Button("zoom out",
                        margin_left, start_y + inc_y * 4, button_w, button_h);
    button->icon.loadImage(ofToDataPath("icons/minus/icon.png"));
    ofAddListener(button->click_event, this, &ofApp::ZoomOutButtonClick);
    buttons.push_back(button);
}