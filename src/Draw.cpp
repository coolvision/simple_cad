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
        lines[i].draw();
    }
    if (ui_state == UI_DRAWING_LINE) {
        curr_line.draw();
    }

    // buttons
//==============================================================================
    ofFill();

    // toolbox
    for (int i = 0; i < buttons.size(); i++) {
        buttons[i]->draw();
    }

    // info
//==============================================================================

    ofSetColor(0.0f);
    font.draw("FPS: " + ofToString((int)ofGetFrameRate()), 16,
              ofGetWindowWidth() - 130, 20);
    font.draw("zoom: 100%", 16,
              ofGetWindowWidth() - 130, 40);
    font.draw("x: " + ofToString(ofGetMouseX()) + " y: " + ofToString(ofGetMouseY()),
              16,
              ofGetWindowWidth() - 130, 60);
    
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

    ofPopStyle();
}

void Line::draw() {

    ofPushStyle();

    ofEnableSmoothing();
    ofEnableAntiAliasing();

    if (selected[2]) {
        ofSetColor(ofColor::orangeRed);
    } else {
        ofSetColor(ofColor::black);
    }
    ofSetLineWidth(1.0f);

    ofLine(p[0], p[1]);

    ofFill();
    if (selected[0] || selected[2]) {
        ofSetColor(ofColor::orangeRed);
    } else {
        ofSetColor(ofColor::black);
    }
    ofCircle(p[0], 4.0f);
    if (selected[1] || selected[2]) {
        ofSetColor(ofColor::orangeRed);
    } else {
        ofSetColor(ofColor::black);
    }
    ofCircle(p[1], 4.0f);

    ofDisableAntiAliasing();

    ofPopStyle();
}
