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

    int j = 0;
    for (int i = c.actions.size()-30; i < c.actions.size(); i++) {
        if (i < 0) {
            continue;
        }
        if (i >= c.actions.size()) {
            continue;
        }
        if (i == c.curr_action_i) {
            ofSetColor(ofColor::darkRed);
        } else {
            ofSetColor(ofColor::black);
        }
        font.draw(c.actions[i]->label, 16, ofGetWindowWidth() - 150,
                  120 + 20 * j);
        j++;
    }

    for (int i = 0; i < c.selection.vertices.size(); i++) {
        VertexId v = c.selection.vertices[i];
        ofPoint p = c.selection.start_p[i];
        font.draw("line:" + ofToString(v.line_i) + " v:" + ofToString(v.v_i) +
                  " x:" + ofToString(p.x) + " y:" + ofToString(p.y),
                  16, ofGetWindowWidth() - 400,
                  20 + 20 * i);
    }


    // objects
//==============================================================================
    for (int i = 0; i < c.lines.size(); i++) {
        c.drawLine(c.lines[i]);
    }
    if (ui_state == UI_DRAWING_LINE) {
        c.drawLine(&c.curr_line);
    }

    if (ui_state == UI_ADD_VERTEX && c.hover_line) {
        ofSetColor(ofColor::orangeRed);
        ofCircle(c.getPx(&c.add_v), 4.0f);
    }

    if (ui_state == UI_MOUSE_SELECTION) {
        ofSetColor(150);
        ofSetLineWidth(1.0f);
        ofNoFill();
        ofDisableAntiAliasing();
        ofDisableSmoothing();

        ofRectangle r = c.selection_r;
        r.standardize();
        ofPoint p1(r.x, r.y);
        ofPoint p2(r.x + r.width, r.y + r.height);
        p1 = c.getPx(p1);
        p2 = c.getPx(p2);
        ofRectangle selection_px;
        selection_px.set(p1, p2);
        ofRect(selection_px);
    }

    // buttons
//==============================================================================
    canvas_toolbar.draw();

    // info
//==============================================================================

    ofPoint p = c.snap(ofPoint(ofGetMouseX(), ofGetMouseY()));

    ofSetColor(0.0f);
    int off_x = 130;
    font.draw("FPS: " + ofToString((int)ofGetFrameRate()), 16,
              ofGetWindowWidth() - 150, 20);
    font.draw("zoom: " + ofToString(c.zoom * 100) + "%", 16,
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
    ofLine(c.canvas_offset.x, 0, c.canvas_offset.x, h);
    ofLine(0, c.canvas_offset.y, w, c.canvas_offset.y);

    ofPopStyle();
}
