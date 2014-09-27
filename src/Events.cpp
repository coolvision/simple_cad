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

void ofApp::mouseDragged(int x, int y, int button){

    if (ui_state == UI_DRAWING_LINE) {
        curr_line.p[0] = start_click;
        curr_line.p[1].set(x, y);
    }
}

void ofApp::mousePressed(int x, int y, int button){

    // start drawing a line
    if (ui_state == UI_DRAW_LINE) {
        ui_state = UI_DRAWING_LINE;
        start_click.set(x, y);
    }

    // find selected objects:
    // points, lines, etc...

    // mark them as selected/unselected
    // (will draw them depensing on that)

    // and start dragging a specific object



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
}

void ofApp::update(){

}

void ofApp::keyPressed(int key){

}

void ofApp::keyReleased(int key){

}

void ofApp::mouseMoved(int x, int y ){


}

void ofApp::windowResized(int w, int h){

}

void ofApp::gotMessage(ofMessage msg){

}

void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
