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
        drawLine(&lines[i]);
    }
    if (ui_state == UI_DRAWING_LINE) {
        drawLine(&curr_line);
    }

    // buttons
//==============================================================================
    ofFill();

    // toolbox
    for (int i = 0; i < buttons.size(); i++) {
        buttons[i]->draw();
    }

    if (was_selected_point) {
        add_line->draw();
        if (add_line->hover) {
            ofSetColor(ofColor::orangeRed);
            ofCircle(*selected_point_p, 4.0f);
        }
    }

    // info
//==============================================================================

    ofPoint p;
    p.x = ((int)ofGetMouseX() / points_step) * points_step;
    p.y = ((int)ofGetMouseY() / points_step) * points_step;

    ofSetColor(0.0f);
    font.draw("FPS: " + ofToString((int)ofGetFrameRate()), 16,
              ofGetWindowWidth() - 130, 20);
    font.draw("zoom: 100%", 16,
              ofGetWindowWidth() - 130, 40);
    font.draw("x: " + ofToString(p.x) + " y: " + ofToString(p.y),
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

void ofApp::drawLine(Polyline *l) {

    ofPushStyle();

    ofEnableSmoothing();
    ofEnableAntiAliasing();


    ofSetLineWidth(1.0f);

    for (int i = 0; i < l->p.size()-1; i++) {
        if (l->selected_line[i]) {
            ofSetColor(ofColor::orangeRed);
        } else {
            if (l->hover) {
                ofSetColor(100);
            } else {
                ofSetColor(ofColor::black);
            }
        }
        ofLine(l->p[i], l->p[i+1]);
    }

    if (l->hover) {
        ofSetColor(100);
    } else {

    }
    for (int i = 0; i < l->p.size(); i++) {
        ofSetColor(ofColor::black);
        if (i == 0) {
            ofSetColor(ofColor::red);
        }
        if (i == l->p.size()-1) {
            ofSetColor(ofColor::green);
        }
        if (!l->selected_p[i]) {
            ofCircle(l->p[i], 4.0f);
        }
    }
    ofSetColor(ofColor::black);
    ofDrawBitmapString(ofToString(l->p.size()), l->p[0] + ofPoint(20, 20));

    ofSetColor(ofColor::orangeRed);
    for (int i = 0; i < l->p.size(); i++) {
        if (l->selected_p[i]) {
            ofCircle(l->p[i], 4.0f);
        }
    }



    ofDisableAntiAliasing();
    ofDisableSmoothing();
}
