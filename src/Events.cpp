//
//  Events.cpp
//  simple_card
//
//  Created by sk on 9/26/14.
//
//

#include "ofApp.h"

void ofApp::keyPressed(int key){

    //cout << "keyPressed " << key << endl;

    // is it this way on most keyboards???
    if (key == '=') {
        //cout << "zoom_in = true " << endl;
        zoom_in = true;
    }
    if (key == '-') {
        //cout << "zoom_out = true " << endl;
        zoom_out = true;
    }

    if (key == OF_KEY_BACKSPACE || key == OF_KEY_DEL) {
        c.deleteSelection();
    }

    bool cmd = ofGetKeyPressed(OF_KEY_COMMAND);
    bool ctrl = ofGetKeyPressed(OF_KEY_CONTROL);
    bool shift = ofGetKeyPressed(OF_KEY_SHIFT);
    if (key == 'z' && (cmd || ctrl) && !shift) {

        if (c.curr_action_i > c.actions.size() - 1) {
            c.curr_action_i = c.actions.size() - 1;
        }
        if (c.curr_action_i < 0) {
            c.curr_action_i = 0;
        }

        c.actions[c.curr_action_i]->undoAction(&c);
        c.curr_action_i--;
        if (c.curr_action_i < 0) {
            c.curr_action_i = 0;
        }

        cout << "c.curr_action_i " << c.curr_action_i << endl;
    }
    if ((key == 'y' && (cmd || ctrl)) ||
        (key == 'z' && (cmd || ctrl) && shift)) {

        if (c.curr_action_i > c.actions.size() - 1) {
            c.curr_action_i = c.actions.size() - 1;
        }
        if (c.curr_action_i < 0) {
            c.curr_action_i = 0;
        }
        if (c.actions[c.curr_action_i]->undo) {
            c.actions[c.curr_action_i]->doAction(&c);
            c.curr_action_i++;
        } else {
            c.curr_action_i++;
            if (c.curr_action_i > c.actions.size() - 1) {
                c.curr_action_i = c.actions.size() - 1;
            }
            c.actions[c.curr_action_i]->doAction(&c);
        }
        if (c.curr_action_i > c.actions.size() - 1) {
            c.curr_action_i = c.actions.size() - 1;
        }

        cout << "c.curr_action_i " << c.curr_action_i << endl;
    }
}

void ofApp::unselectMode() {

    for (int i = 0; i < canvas_toolbar.buttons.size(); i++) {
        canvas_toolbar.buttons[i]->selected = false;
    }
}

void ofApp::SelectButtonClick(ButtonClickEventData &d) {

    unselectMode();
    d.button_p->selected = true;
    ui_state = UI_SELECT;
}


void ofApp::MoveButtonClick(ButtonClickEventData &d) {

    unselectMode();
    d.button_p->selected = true;
    ui_state = UI_MOVE_CANVAS;
}


void ofApp::LineButtonClick(ButtonClickEventData &d) {

    unselectMode();

    line_button->selected = true;
    //add_line->selected = true;
    ui_state = UI_DRAW_LINE;
}

void ofApp::VertexButtonClick(ButtonClickEventData &d) {

    unselectMode();

    vertex_button->selected = true;
    ui_state = UI_ADD_VERTEX;
}

void ofApp::ZoomInButtonClick(ButtonClickEventData &d) {
    c.zoomIn();
    setGrid();
}

void ofApp::ZoomOutButtonClick(ButtonClickEventData &d) {
    c.zoomOut();
    setGrid();
}

void ofApp::update() {

    if (zoom_in && (ofGetKeyPressed(OF_KEY_CONTROL) ||
                    ofGetKeyPressed(OF_KEY_COMMAND))) {
        c.zoomIn();
        zoom_in = false;
        setGrid();
    }

    if (zoom_out && (ofGetKeyPressed(OF_KEY_CONTROL) ||
                     ofGetKeyPressed(OF_KEY_COMMAND))) {
        c.zoomOut();
        zoom_out = false;
        setGrid();
    }
}

void ofApp::keyReleased(int key){

}

void ofApp::windowResized(int w, int h){

}

void ofApp::gotMessage(ofMessage msg){
    
}

void ofApp::dragEvent(ofDragInfo dragInfo){
    
}