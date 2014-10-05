//
//  Update.cpp
//  simple_card
//
//  Created by sk on 10/3/14.
//
//

#include "ofApp.h"

void SelectionList::add(Vertex *v) {

    for (int i = 0; i < vertices.size(); i++) {
        if (v == vertices[i]) {
            return;
        }
    }
    vertices.push_back(v);
    start_p.push_back(*v);
}

void SelectionList::clear() {
    vertices.clear();
    start_p.clear();
}

void ofApp::clearSelection() {

    selection.clear();

    for (int i = 0; i < lines.size(); i++) {
        for (Vertex *v = lines[i]->front; v != NULL; v = v->next) {
            v->selected = false;
            if (v->next == lines[i]->front) break; // closed polylines
        }
        lines[i]->selected = false;
    }
}

void ofApp::resetHover() {

    selected_point = false;
    selected_point_p = NULL;

    selected_line = false;
    selected_line_p[0] = NULL;
    selected_line_p[1] = NULL;

    selected_polygon = false;
    selected_polygon_p = NULL;


    for (int i = 0; i < lines.size(); i++) {
        for (Vertex *v = lines[i]->front; v != NULL; v = v->next) {
            v->hover = false;
            if (v->next == lines[i]->front) break; // closed polylines
        }
        lines[i]->hover = false;
    }
}

void ofApp::updateToolbar(ofPoint p) {

    if (was_selected_point && selected_line_p[0] && selected_line_p[1]) {
        float d0 = (*selected_line_p[0] - p).length();
        float d1 = (*selected_line_p[1] - p).length();
        if (d0 < d1) {
            cursor_toolbar.updatePosition(selected_line_p[0]->x + toolbar_off.x,
                                          selected_line_p[0]->y - toolbar_off.y);
        } else {
            cursor_toolbar.updatePosition(selected_line_p[1]->x + toolbar_off.x,
                                          selected_line_p[1]->y - toolbar_off.y);
        }
    }
}

