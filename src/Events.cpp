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

void ofApp::addLineClick(ButtonClickEventData &d) {

    for (int i = 0; i < buttons.size(); i++) {
        buttons[i]->selected = false;
    }

    ui_state = UI_DRAW_LINE;
    d.button_p->selected = true;
    line_button->selected = true;
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