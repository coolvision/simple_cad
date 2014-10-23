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

    int off_x = 170;

    int j = 0;
    int n = c.actions.size();
    for (int i = MIN(c.curr_action_i, n-30); i < n; i++) {
        if (i < 0) {
            continue;
        }
        if (i == c.curr_action_i) {
            ofSetColor(ofColor::darkRed);
        } else {
            ofSetColor(ofColor::black);
        }
        font.draw(c.actions[i]->label, 16, ofGetWindowWidth() - off_x,
                  120 + 20 * j);
        j++;
    }

    ofSetColor(ofColor::black);
    for (int i = 0; i < c.selection.vertices.size(); i++) {
        VertexId v = c.selection.vertices[i];
        font.draw("line:" + ofToString(v.line_i) + " v:" + ofToString(v.v_i),
                  16, ofGetWindowWidth() - 400,
                  20 + 20 * i);
    }

    ofPoint p = c.snap(ofPoint(ofGetMouseX(), ofGetMouseY()));

    ofSetColor(0.0f);
    font.draw("FPS: " + ofToString((int)ofGetFrameRate()), 16,
              ofGetWindowWidth() - off_x, 20);
    font.draw("zoom: " + ofToString(c.zoom * 100) + "%", 16,
              ofGetWindowWidth() - off_x, 40);
    font.draw("x: " + ofToString(p.x) + " y: " + ofToString(p.y),
              16,
              ofGetWindowWidth() - off_x, 60);

    if (line_length_info != 0.0f) {
        font.draw("line: " + ofToString(line_length_info, 1) + " mm",
                  16, ofGetWindowWidth() - off_x, 80);
    }

    // objects
//==============================================================================
    for (int i = 0; i < c.lines.size(); i++) {
        c.drawLine(c.lines[i]);
    }
    if (c.ui_state == UI_DRAWING_LINE) {
        c.drawLine(&c.curr_line);
    }

    if (c.ui_state == UI_ADD_VERTEX && c.hover_line) {
        ofSetColor(ofColor::orangeRed);
        ofCircle(c.getPx(&c.add_v), 4.0f);
    }

    if (c.ui_state == UI_MOUSE_SELECTION) {
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

    ofPoint icon_offset(13, 13);
    if (c.ui_state == UI_ADD_JOINT_R) {
        ofSetColor(ofColor::red);
        c.joint_icon_r.draw(p - icon_offset);
    }
    if (c.ui_state == UI_ADD_JOINT_FIXED) {
        ofSetColor(ofColor::red);
        c.joint_icon_fixed.draw(p - icon_offset);
    }

    for (int i = 0; i < c.joints.size(); i++) {
        c.joints[i]->draw();
//        c.joints[i]->p = c.getMm(ofPoint(c.joints[i]->x, c.joints[i]->y));
//        ofPoint p1 = c.getPx(c.joints[i]->p);
//        ofSetColor(ofColor::red);
//        if (c.joints[i]->type == JOINT_FIXED) {
//            c.joint_icon_fixed.draw(p1 - icon_offset);
//        } else {
//            c.joint_icon_r.draw(p1 - icon_offset);
//        }
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
