//
//  Events.cpp
//  simple_card
//
//  Created by sk on 9/26/14.
//
//

#include "ofApp.h"

void ofApp::unselectMode() {

    for (int i = 0; i < canvas_toolbar.buttons.size(); i++) {
        canvas_toolbar.buttons[i]->selected = false;
    }
    for (int i = 0; i < cursor_toolbar.buttons.size(); i++) {
        cursor_toolbar.buttons[i]->selected = false;
    }
}

void ofApp::SelectButtonClick(ButtonClickEventData &d) {

    unselectMode();

    d.button_p->selected = true;

    ui_state = UI_SELECT;
}

void ofApp::LineButtonClick(ButtonClickEventData &d) {

    unselectMode();

    line_button->selected = true;
    add_line->selected = true;
    ui_state = UI_DRAW_LINE;
}

void ofApp::VertexButtonClick(ButtonClickEventData &d) {

    unselectMode();

    vertex_button->selected = true;
    add_vertex->selected = true;
    ui_state = UI_ADD_VERTEX;

    selected_line = false;
}

void ofApp::MoveButtonClick(ButtonClickEventData &d) {

    unselectMode();

    d.button_p->selected = true;

    ui_state = UI_MOVE_CANVAS;
}

void ofApp::ZoomInButtonClick(ButtonClickEventData &d) {

}

void ofApp::ZoomOutButtonClick(ButtonClickEventData &d) {

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