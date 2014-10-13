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

    hover_point = false;
    hover_point_p = NULL;

    hover_line = false;
    hover_line_p[0] = NULL;
    hover_line_p[1] = NULL;

    hover_polygon = false;
    hover_polygon_p = NULL;


    for (int i = 0; i < lines.size(); i++) {
        for (Vertex *v = lines[i]->front; v != NULL; v = v->next) {
            v->hover = false;
            if (v->next == lines[i]->front) break; // closed polylines
        }
        lines[i]->hover = false;
    }
}

void ofApp::updateToolbar(ofPoint p) {

    if (was_selected_point && hover_line_p[0] && hover_line_p[1]) {
        float d0 = (*hover_line_p[0] - p).length();
        float d1 = (*hover_line_p[1] - p).length();
        if (d0 < d1) {
            cursor_toolbar.updatePosition(hover_line_p[0]->x + toolbar_off.x,
                                          hover_line_p[0]->y - toolbar_off.y);
        } else {
            cursor_toolbar.updatePosition(hover_line_p[1]->x + toolbar_off.x,
                                          hover_line_p[1]->y - toolbar_off.y);
        }
    }
}

