//
//  Draw.cpp
//  simple_card
//
//  Created by sk on 9/20/14.
//
//

#include "ofApp.h"

void ofApp::draw(){

    ofPushStyle();
    ofBackground(220);

    drawGrid();

    // objects
//==============================================================================
    for (int i = 0; i < lines.size(); i++) {
        drawLine(lines[i]);
    }
    if (ui_state == UI_DRAWING_LINE) {
        drawLine(&curr_line);
    }

    if (ui_state == UI_ADD_VERTEX && hover_line) {
        ofSetColor(ofColor::orangeRed);
        ofCircle(getPx(&add_v), 4.0f);
    }

    if (ui_state == UI_MOUSE_SELECTION) {
        ofSetColor(150);
        ofSetLineWidth(1.0f);
        ofNoFill();
        ofDisableAntiAliasing();
        ofDisableSmoothing();

        ofRectangle r = selection_r;
        r.standardize();
        ofPoint p1(r.x, r.y);
        ofPoint p2(r.x + r.width, r.y + r.height);
        p1 = getPx(p1);
        p2 = getPx(p2);
        ofRectangle selection_px;
        selection_px.set(p1, p2);
        ofRect(selection_px);
    }

    // buttons
//==============================================================================
    canvas_toolbar.draw();

    // info
//==============================================================================

    ofPoint p = snap(ofPoint(ofGetMouseX(), ofGetMouseY()));

    ofSetColor(0.0f);
    int off_x = 130;
    font.draw("FPS: " + ofToString((int)ofGetFrameRate()), 16,
              ofGetWindowWidth() - 150, 20);
    font.draw("zoom: " + ofToString(zoom * 100) + "%", 16,
              ofGetWindowWidth() - 150, 40);
    font.draw("x: " + ofToString(p.x) + " y: " + ofToString(p.y),
              16,
              ofGetWindowWidth() - 150, 60);

    if (line_length_info != 0.0f) {
        font.draw("line: " + ofToString(line_length_info, 1) + " mm",
                  16, ofGetWindowWidth() - 150, 80);
    }

    ofPopStyle();
}

void ofApp::drawGrid() {

    ofPushStyle();

    ofDisableAntiAliasing();
    ofDisableSmoothing();

    float w = ofGetWindowWidth();
    float h = ofGetWindowHeight();

    ofSetColor(100.0f);
    grid_points_vbo.draw(GL_POINTS, 0, points_n_x * points_n_y);

    ofSetColor(100.0f);
    ofSetLineWidth(1.0f);
    grid_lines_vbo.draw(GL_LINES, 0, (lines_n_x + lines_n_y) * 2);

    ofSetColor(ofColor::darkRed);
    ofSetLineWidth(2.0f);
    ofLine(canvas_offset.x, 0, canvas_offset.x, h);
    ofLine(0, canvas_offset.y, w, canvas_offset.y);

    ofPopStyle();
}
