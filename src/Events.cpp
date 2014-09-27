//
//  Events.cpp
//  simple_card
//
//  Created by sk on 9/26/14.
//
//

#include "ofApp.h"

void ofApp::SelectButtonClick(ButtonClickEventData &d) {

    for (int i = 0; i < buttons.size(); i++) {
        buttons[i]->selected = false;
    }

    d.button_p->selected = true;

    ui_state = UI_SELECT;
}

void ofApp::LineButtonClick(ButtonClickEventData &d) {

    for (int i = 0; i < buttons.size(); i++) {
        buttons[i]->selected = false;
    }

    d.button_p->selected = true;

    ui_state = UI_DRAW_LINE;
}

void ofApp::MoveButtonClick(ButtonClickEventData &d) {

    for (int i = 0; i < buttons.size(); i++) {
        buttons[i]->selected = false;
    }

    d.button_p->selected = true;

    ui_state = UI_MOVE_CANVAS;
}

void ofApp::ZoomInButtonClick(ButtonClickEventData &d) {

}

void ofApp::ZoomOutButtonClick(ButtonClickEventData &d) {

}

float segmentDistance(ofPoint v, ofPoint w, ofPoint p) {

    // Return minimum distance between line segment vw and point p
    float l2 = (v - w).lengthSquared();  // i.e. |w-v|^2 -  avoid a sqrt

    if (l2 == 0.0) {
        return (p, v).length();   // v == w case
    }

    // Consider the line extending the segment, parameterized as v + t (w - v).
    // We find projection of point p onto the line.
    // It falls where t = [(p-v) . (w-v)] / |w-v|^2
    float t = (p - v).dot(w - v) / l2;

    if (t < 0.0) {
        return (p - v).length();       // Beyond the 'v' end of the segment
    } else if (t > 1.0) {
        return (p - w).length();  // Beyond the 'w' end of the segment
    }

    ofPoint projection = v + t * (w - v);  // Projection falls on the segment

    return (p - projection).length();
}

void ofApp::mouseMoved(int x, int y ){

    // find selected objects:
    // points, lines, etc...
    if (ui_state == UI_SELECT) {
        ofPoint p(x, y);

        selected_point = false;
        selected_line = false;

        for (int i = 0; i < lines.size(); i++) {
            lines[i].selected[0] = false;
            lines[i].selected[1] = false;
            lines[i].selected[2] = false;
        }

        // mark them as selected
        // (will draw them depensing on that)
        for (int i = 0; i < lines.size(); i++) {
            float d0 = (p - lines[i].p[0]).length();
            float d1 = (p - lines[i].p[1]).length();
            if (d0 < 15.0f) {
                lines[i].selected[0] = true;
                selected_point_p = &lines[i].p[0];
                selected_point = true;
                break;
            }
            if (d1 < 15.0f) {
                lines[i].selected[1] = true;
                selected_point_p = &lines[i].p[1];
                selected_point = true;
                break;
            }

            float d = segmentDistance(lines[i].p[0], lines[i].p[1], p);
            if (d < 15.0f && d0 > 15.0f && d1 > 15.0f) {
                lines[i].selected[2] = true;
                selected_line_p = &lines[i];
                selected_line = true;
                break;
            }
        }
    }
}

void ofApp::mousePressed(int x, int y, int button){

    start_click.set(x, y);

    // start drawing a line
    if (ui_state == UI_DRAW_LINE) {
        ui_state = UI_DRAWING_LINE;
    }

    // find selected objects:
    // points, lines, etc...
    if (ui_state == UI_SELECT) {

        // if there is a selected object, start dragging it
        if (selected_point) {
            ui_state = UI_MOVING_POINT;
        }
        if (selected_line) {
            ui_state = UI_MOVING_LINE;
            curr_line = *selected_line_p;
        }
    }
}

void ofApp::mouseDragged(int x, int y, int button){

    if (ui_state == UI_DRAWING_LINE) {
        curr_line.p[0] = start_click;
        curr_line.p[1].set(x, y);
    }

    if (ui_state == UI_MOVING_POINT) {
        selected_point_p->set(x, y);

    }
    if (ui_state == UI_MOVING_LINE) {
        ofPoint p(x, y);
        selected_line_p->p[0] = curr_line.p[0] + (p - start_click);
        selected_line_p->p[1] = curr_line.p[1] + (p - start_click);
    }
}

void ofApp::mouseReleased(int x, int y, int button){

    // stop the drawing / dragging / selection
    if (ui_state == UI_DRAWING_LINE) {
        ui_state = UI_DRAW_LINE;

        Line l;
        l.p[0] = start_click;
        l.p[1].set(x, y);
        
        lines.push_back(l);
    }

    if (ui_state == UI_MOVING_POINT) {
        ui_state = UI_SELECT;
        selected_point_p->set(x, y);

    }
    if (ui_state == UI_MOVING_LINE) {
        ui_state = UI_SELECT;
        ofPoint p(x, y);
        selected_line_p->p[0] = curr_line.p[0] + (p - start_click);
        selected_line_p->p[1] = curr_line.p[1] + (p - start_click);
    }
}

void ofApp::update(){

}

void ofApp::keyPressed(int key){

}

void ofApp::keyReleased(int key){

}

void ofApp::windowResized(int w, int h){

}

void ofApp::gotMessage(ofMessage msg){

}

void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
