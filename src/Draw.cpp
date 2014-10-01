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

    if (l->closed) {
        l->path.setFilled(true);
        l->path.setFillColor(ofColor(200, 200, 200, 200));
    } else {
        l->path.setFilled(false);
    }
    l->path.draw();

    ofSetLineWidth(1.0f);
    ofSetColor(ofColor::black);
    for (Vertex *v = l->front; v != NULL && v->next != NULL; v = v->next) {
        if (v->hover && v->next->hover) {
            ofSetColor(ofColor::orangeRed);
        } else {
            ofSetColor(ofColor::black);
        }
        ofLine(*v, *v->next);
        if (v->next == l->front) break; // closed polylines
    }

    ofSetColor(ofColor::red);
    ofCircle(*l->front, 8.0f);

    ofSetColor(ofColor::black);
    for (Vertex *v = l->front; v != NULL; v = v->next) {
        if (!v->hover) {
            ofCircle(*v, 4.0f);
        }
        if (v->next == l->front) break; // closed polylines
    }

    ofSetColor(ofColor::orangeRed);
    for (Vertex *v = l->front; v != NULL; v = v->next) {
        if (v->hover) {
            ofCircle(*v, 4.0f);
        }
        if (v->next == l->front) break; // closed polylines
    }

    ofDisableAntiAliasing();
    ofDisableSmoothing();
}
