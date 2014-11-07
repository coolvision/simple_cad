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
    
    if (c.ui_state == UI_ADD_VERTEX) {

        c.resetHover();

        for (int i = 0; i < c.lines.size(); i++) {
            for (InteractiveObject *v = c.lines[i]->front; v != NULL && v->next != NULL; v = v->next) {
                float d = segmentDistance(v->p, v->next->p, p_mm);
                if (d < 2.0f) {
                    c.hover_line = true;
                    c.add_v.p = lineProjection(v->p, v->next->p, p_mm);
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

    if (c.ui_state == UI_SELECT || c.ui_state == UI_DRAW_LINE) {
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

    if (c.ui_state == UI_MOVING_CANVAS) {
        ofPoint p = ofPoint(x, y);
        c.canvas_offset = c.canvas_offset_start + (p - c.start_click);
        setGrid();
        return;
    }

    if (c.ui_state == UI_DRAWING_LINE) {
        c.resetHover();
        c.setHover(p_mm);
        c.curr_line.back->p = p_mm;
        c.curr_line.back->hover = true;
        c.curr_line.front->hover = true;
        ofPoint p = c.snapMm(c.getMm(ofPoint(x, y)));
        line_length_info = (p - c.curr_line.front->p).length();
    }

    c.resetHover();
    c.setHoverPoint(p_mm);

    if (c.ui_state == UI_MOVING_SELECTION) {
        for (int i = 0; i < c.selection.items.size(); i++) {
            InteractiveObject *v = c.getItem(c.selection.items[i]);
            v->p = v->start_p + (p_mm - c.start_click);
            v->parent->update();
        }
    }

//    if (c.ui_state == UI_MOVING_POINT) {
//        c.selected_p->parent->update();
//        c.selected_p->p = c.selected_p->start_p + (p_mm - c.start_click);
//    }
}

