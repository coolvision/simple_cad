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
        ofCircle(add_v, 4.0f);
    }

    if (ui_state == UI_MOUSE_SELECTION) {
        ofSetColor(150);
        ofSetLineWidth(1.0f);
        ofNoFill();
        ofDisableAntiAliasing();
        ofDisableSmoothing();
        ofRect(selection_r);
    }


    // buttons
//==============================================================================

    canvas_toolbar.draw();
    cursor_toolbar.draw();

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

    if (l->front == NULL || l->back == NULL) {
        return;
    }

    ofPushStyle();

    ofEnableSmoothing();
    ofEnableAntiAliasing();

    ofDrawBitmapString(ofToString(l->closed) + " " + ofToString((int)l->front) + " " + ofToString((int)l->back),
                     *l->front + ofPoint(100, 100));

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
        if (v->selected && v->next->selected) {
            ofSetColor(ofColor::steelBlue);
        } else if (v->hover && v->next->hover) {
            ofSetColor(ofColor::orangeRed);
        } else {
            ofSetColor(ofColor::black);
        }
        ofLine(*v, *v->next);
        if (v->next == l->front) break; // closed polylines
    }

    ofSetColor(ofColor::red);
    ofCircle(*l->front, 8.0f);

    ofSetColor(ofColor::green);
    ofCircle(*l->back, 10.0f);

    ofSetColor(ofColor::black);
    for (Vertex *v = l->front; v != NULL; v = v->next) {
        if (!v->hover && !v->selected) {
            ofCircle(*v, 4.0f);
        }

        ofDrawBitmapString(ofToString((int)v), *v + ofPoint(15, 15));
        ofDrawBitmapString(ofToString((int)v->prev), *v + ofPoint(15, 30));
        ofDrawBitmapString(ofToString((int)v->next), *v + ofPoint(15, 45));
        ofDrawBitmapString(ofToString((int)v->p), *v + ofPoint(15, 60));

        if (v->next == l->front) break; // closed polylines
    }

    ofSetColor(ofColor::orangeRed);
    for (Vertex *v = l->front; v != NULL; v = v->next) {
        if (!v->selected && v->hover) {
            ofCircle(*v, 4.0f);
        }
        if (v->next == l->front) break; // closed polylines
    }

    ofSetColor(ofColor::steelBlue);
    for (Vertex *v = l->front; v != NULL; v = v->next) {
        if (v->selected) {
            ofCircle(*v, 4.0f);
        }
        if (v->next == l->front) break; // closed polylines
    }

    ofDisableAntiAliasing();
    ofDisableSmoothing();
}
