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

    ofPoint p_mm = c.getMm(p);

    // ok, this is not such a good way to do this,
    // check if the press is over a button
    if (canvas_toolbar.inside(p)) {
        return;
    }

    if (ui_state == UI_ADD_VERTEX) {

        c.resetHover();

        for (int i = 0; i < c.lines.size(); i++) {
            for (Vertex *v = c.lines[i]->front; v != NULL && v->next != NULL; v = v->next) {
                float d = segmentDistance(*v, *v->next, p_mm);
                if (d < 2.0f) {
                    c.hover_line = true;
                    c.add_v = lineProjection(*v, *v->next, p_mm);
                    c.hover_line_p[0] = v;
                    c.hover_line_p[1] = v->next;
                    v->hover = true;
                    v->next->hover = true;
                    break;
                }
                if (c.hover_line) {
                    break;
                }
                if (v->next == c.lines[i]->front) break; // closed polylines
            }
        }
    }

    if (ui_state == UI_SELECT || ui_state == UI_DRAW_LINE) {
        // find hover objects:
        // points, lines, etc...
        c.resetHover();
        c.setHover(p_mm);
    }
}

void ofApp::mouseDragged(int x, int y, int button) {

    ofPoint p_mm = c.getMm(ofPoint(x, y));

    ofPoint r = p_mm - c.start_click;
    c.selection_r.set(c.start_click, r.x, r.y);

    if (ui_state == UI_MOVING_CANVAS) {
        ofPoint p = ofPoint(x, y);
        c.canvas_offset = c.canvas_offset_start + (p - c.start_click);
        setGrid();
        return;
    }

    if (ui_state == UI_DRAWING_LINE) {
        c.resetHover();
        c.setHover(p_mm);
        *c.curr_line.back = p_mm;
        c.curr_line.back->hover = true;
        c.curr_line.front->hover = true;
        ofPoint p = c.snapMm(c.getMm(ofPoint(x, y)));
        line_length_info = (p - *c.curr_line.front).length();
    }

    c.resetHover();
    c.setHoverPoint(p_mm);

//    if (ui_state == UI_MOVING_POINT && c.selected_point_p) {
//        c.selected_point_p->p->updatePath();
//        *c.selected_point_p = c.start_p + (p_mm - c.start_click);
//        c.selected_point_p->hover = true;
//    }
//    if (ui_state == UI_MOVING_LINE &&
//        c.selected_line_p[0] && c.selected_line_p[1]) {
//        c.selected_line_p[0]->hover = true;
//        c.selected_line_p[1]->hover = true;
//    }

    if (ui_state == UI_MOVING_POINT ||
        ui_state == UI_MOVING_POLYGON || ui_state == UI_MOVING_LINE) {
        for (int i = 0; i < c.selection.vertices.size(); i++) {
            Vertex *v = c.getVertex(c.selection.vertices[i]);
            v->p->updatePath();
            *v = v->start_p + (p_mm - c.start_click);
        }
    }

    if (ui_state == UI_MOVING_POINT && c.selected_point_p) {
        ofPoint p = c.snapMm(c.getMm(ofPoint(x, y)));
        if (c.selected_point_p->next == NULL && c.selected_point_p->prev != NULL) {
            line_length_info = (p - *c.selected_point_p->prev).length();
        }
        if (c.selected_point_p->prev == NULL && c.selected_point_p->next != NULL) {
            line_length_info = (p - *c.selected_point_p->next).length();
        }
    }
}

