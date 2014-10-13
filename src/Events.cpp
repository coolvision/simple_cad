//
//  Events.cpp
//  simple_card
//
//  Created by sk on 9/26/14.
//
//

#include "ofApp.h"

void ofApp::keyPressed(int key){

    if (key == OF_KEY_BACKSPACE || key == OF_KEY_DEL) {

        for (int i = 0; i < selection.vertices.size(); i++) {

            Vertex *v = selection.vertices[i];

            if (v == NULL) continue;

            // delete
            if (v->next != NULL) {
                v->next->prev = v->prev;
            }
            if (v->prev != NULL) {
                v->prev->next = v->next;
            }
            if (v == v->p->front && v == v->p->back) {
                v->p->front = NULL;
                v->p->back = NULL;
            } else if (v == v->p->front) {
                v->p->front = v->next;
            } else if (v == v->p->back) {
                v->p->back = v->prev;
            }

            delete v;
            v = NULL;
        }
        selection.clear();

        for (int i = 0; i < lines.size(); i++) {
            if (lines[i]->getLength() <= 2) {
                lines[i]->closed = false;
                if (lines[i]->front != NULL) {
                    lines[i]->front->prev = NULL;
                }
                if (lines[i]->back != NULL) {
                    lines[i]->back->next = NULL;
                }
            }
            lines[i]->updatePath();
        }

        hover_point = false;
        hover_point_p = NULL;

        hover_line = false;
        hover_line_p[0] = NULL;
        hover_line_p[1] = NULL;

        hover_polygon = false;
        hover_polygon_p = NULL;
    }
}

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

    hover_line = false;
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

void ofApp::keyReleased(int key){

}

void ofApp::windowResized(int w, int h){

}

void ofApp::gotMessage(ofMessage msg){
    
}

void ofApp::dragEvent(ofDragInfo dragInfo){
    
}