//
//  MouseUpdate.cpp
//  simple_card
//
//  Created by sk on 10/15/14.
//
//

#include "ofApp.h"

void ofApp::mouseMoved(int x, int y ) {

    ofPoint p(x, y);

    ofPoint p_mm = getMm(p);

    // ok, this is not such a good way to do this,
    // check if the press is over a button
    if (canvas_toolbar.inside(p)) {
        return;
    }

    if (ui_state == UI_ADD_VERTEX) {

        resetHover();

        for (int i = 0; i < lines.size(); i++) {
            for (Vertex *v = lines[i]->front; v != NULL && v->next != NULL; v = v->next) {
                float d = segmentDistance(*v, *v->next, p_mm);
                if (d < 2.0f) {
                    hover_line = true;
                    add_v = lineProjection(*v, *v->next, p_mm);
                    hover_line_p[0] = v;
                    hover_line_p[1] = v->next;
                    v->hover = true;
                    v->next->hover = true;
                    break;
                }
                if (hover_line) {
                    break;
                }
                if (v->next == lines[i]->front) break; // closed polylines
            }
        }
    }

    if (ui_state == UI_SELECT || ui_state == UI_DRAW_LINE) {
        // find hover objects:
        // points, lines, etc...
        resetHover();
        setHover(p_mm);
    }
}

void ofApp::mouseDragged(int x, int y, int button) {

    ofPoint p_mm = getMm(ofPoint(x, y));

    ofPoint r = p_mm - start_click;
    selection_r.set(start_click, r.x, r.y);

    if (ui_state == UI_MOVING_CANVAS) {
        ofPoint p = ofPoint(x, y);
        canvas_offset = canvas_offset_start + (p - start_click);
        setGrid();
        return;
    }

    if (ui_state == UI_DRAWING_LINE) {
        resetHover();
        setHover(p_mm);
        *curr_line.back = p_mm;
        curr_line.back->hover = true;
        curr_line.front->hover = true;
        ofPoint p = snapMm(getMm(ofPoint(x, y)));
        line_length_info = (p - *curr_line.front).length();
    }

    resetHover();
    setHoverPoint(p_mm);

    if (ui_state == UI_MOVING_POINT && selected_point_p) {
        selected_point_p->p->updatePath();
        *selected_point_p = start_p + (p_mm - start_click);
        selected_point_p->hover = true;
    }
    if (ui_state == UI_MOVING_LINE &&
        selected_line_p[0] && selected_line_p[1]) {
        selected_line_p[0]->hover = true;
        selected_line_p[1]->hover = true;
    }

    if (ui_state == UI_MOVING_POLYGON || ui_state == UI_MOVING_LINE) {
        for (int i = 0; i < selection.vertices.size(); i++) {
            selection.vertices[i]->p->updatePath();
            *selection.vertices[i] = selection.start_p[i] + (p_mm - start_click);
        }
    }

    if (ui_state == UI_MOVING_POINT && selected_point_p) {
        ofPoint p = snapMm(getMm(ofPoint(x, y)));
        if (selected_point_p->next == NULL && selected_point_p->prev != NULL) {
            line_length_info = (p - *selected_point_p->prev).length();
        }
        if (selected_point_p->prev == NULL && selected_point_p->next != NULL) {
            line_length_info = (p - *selected_point_p->next).length();
        }
    }
}

