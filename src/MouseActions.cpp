//
//  MouseEvents.cpp
//  simple_card
//
//  Created by sk on 9/30/14.
//
//

#include "ofApp.h"

void ofApp::mousePressed(int x, int y, int button) {

    ofPoint p_mm = c.snapMm(c.getMm(ofPoint(x, y)));
    // snap to the hovering point
    if (c.hover_point && c.hover_point_p) {
        p_mm = *c.hover_point_p;
    }

    c.start_click = p_mm;

    // ok, this is not such a good way to do this,
    // check if the press is over a button
    if (canvas_toolbar.inside(ofPoint(x, y))) {
        return;
    }

    if (ui_state == UI_MOVE_CANVAS || ui_state == UI_MOVING_CANVAS) {
        ofPoint p = ofPoint(x, y);
        c.start_click = p;
        c.canvas_offset_start = c.canvas_offset;
        ui_state = UI_MOVING_CANVAS;
        return;
    }

    // find hover objects:
    // points, lines, etc...
    if (ui_state == UI_SELECT) {

//        bool shift = ofGetKeyPressed(OF_KEY_SHIFT);
//        if (!shift && !c.move_already_selected) {
//            c.clearSelection();
//        }

        // if there is a hover object, start dragging it
        if (c.hover_point && c.hover_point_p) {

//            c.hover_point_p->selected = true;
//            *c.hover_point_p = p_mm;
//
//            c.selection.add(c.hover_point_p->getId(), *c.hover_point_p);
//
//            c.selected_point = true;
//            c.selected_point_p = c.hover_point_p;
//            c.start_p = *c.hover_point_p;

            ui_state = UI_MOVING_POINT;
        }
        if (c.hover_line && c.hover_line_p[0] && c.hover_line_p[1]) {

//            c.hover_line_p[0]->selected = true;
//            c.hover_line_p[1]->selected = true;
//            c.selection.add(c.hover_line_p[0]->getId(), *c.hover_line_p[0]);
//            c.selection.add(c.hover_line_p[1]->getId(), *c.hover_line_p[1]);
//
//            c.selected_line = true;
//            c.selected_line_p[0] = c.hover_line_p[0];
//            c.selected_line_p[1] = c.hover_line_p[1];

            ui_state = UI_MOVING_LINE;
        }
        if (c.hover_polygon && c.hover_polygon_p)  {

//            for (Vertex *v = c.hover_polygon_p->front; v != NULL; v = v->next) {
//                v->selected = true;
//                c.selection.add(v->getId(), *v);
//                if (v->next == c.hover_polygon_p->front) break;
//            }

            ui_state = UI_MOVING_POLYGON;
        }

        if (ui_state == UI_MOVING_POINT ||
            ui_state == UI_MOVING_POLYGON || ui_state == UI_MOVING_LINE) {
            for (int i = 0; i < c.selection.vertices.size(); i++) {
                Vertex *vertex = c.getVertex(c.selection.vertices[i]);
                vertex->start_p = *vertex;
            }
        }

    }

    // start drawing a line
    if (ui_state == UI_DRAW_LINE) {
        ui_state = UI_DRAWING_LINE;
        c.curr_line.release();
        c.curr_line.addBack(c.start_click);
        c.curr_line.addBack(p_mm);
        c.curr_line.back->hover = true;
        c.curr_line.front->hover = true;
    }

//    if (ui_state == UI_ADD_VERTEX) {
//        // insert a new point between the points of the hover line
//        if (c.hover_line && c.hover_line_p[0] && c.hover_line_p[1]) {
//
//            Vertex *v0 = c.hover_line_p[0];
//            Vertex *v1 = c.hover_line_p[1];
//
//            Vertex *v = new Vertex();
//            *v = c.add_v;
//
//            v->p = v0->p;
//            v->prev = v0;
//            v->next = v1;
//            v0->next = v;
//            v1->prev = v;
//
//            unselectMode();
//            select_button->selected = true;
//
//            c.hover_line = false;
//
//            if (c.selected_point_p) {
//                c.selected_point_p->hover = false;
//            }
//            ui_state = UI_MOVING_POINT;
//            c.selected_point = true;
//            c.selected_point_p = v;
//            c.start_p = *v;
//            v->hover = true;
//            v->selected = true;
//        }
//    }

    c.selection_r.set(p_mm, 0.0f, 0.0f);
    if (ui_state == UI_SELECT) {
        ui_state = UI_MOUSE_SELECTION;
    }
}

void ofApp::mouseReleased(int x, int y, int button) {

    ofPoint p_mm = c.snapMm(c.getMm(ofPoint(x, y)));

    // snap to the hovering point
    if (c.hover_point && c.hover_point_p) {
        p_mm = *c.hover_point_p;
    }

    if (ui_state == UI_MOUSE_SELECTION) {
        ui_state = UI_SELECT;
        ofRectangle r = c.selection_r;
        r.standardize();
        if (r.width * points_step > 10.0f && r.height * points_step > 10.0f) {

            ChangeSelectionAction *select = new ChangeSelectionAction();
            select->prev_selection = c.selection;

            for (int i = 0; i < c.lines.size(); i++) {
                for (Vertex *v = c.lines[i]->front; v != NULL; v = v->next) {
                    if (r.inside(*v)) {
                        select->new_selection.add(v->getId());
                    }
                    if (v->next == c.lines[i]->front) break; // closed polylines
                }
            }
            c.addAction(select);
        }
    }


    // stop the drawing / dragging / selection
    if (ui_state == UI_DRAWING_LINE) {

        AddLineAction* add_line = new AddLineAction();
        add_line->p[0] = c.start_click;
        add_line->p[1] = p_mm;
        c.addAction(add_line);

//        if (pl != NULL) {
//            c.connectPolylines(pl);
//        }
//
        ui_state = UI_SELECT;
        unselectMode();
        select_button->selected = true;
    }


    if (ui_state == UI_MOVING_POINT ||
        ui_state == UI_MOVING_POLYGON || ui_state == UI_MOVING_LINE) {

        for (int i = 0; i < c.selection.vertices.size(); i++) {
            Vertex *v = c.getVertex(c.selection.vertices[i]);
            *v = v->start_p;
        }
        MoveSelectionAction *move_action = new MoveSelectionAction();
        move_action->selection = c.selection;
        move_action->v = (p_mm - c.start_click);
        c.addAction(move_action);

//        if (ui_state == UI_MOVING_LINE && c.selected_line_p[0] && c.selected_line_p[1]) {
//            c.connectPolylines(c.selected_line_p[0]->p);
//            c.connectPolylines(c.selected_line_p[1]->p);
//        }

        ui_state = UI_SELECT;
    }
}
