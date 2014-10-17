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

    if (c.ui_state == UI_MOVE_CANVAS || c.ui_state == UI_MOVING_CANVAS) {
        ofPoint p = ofPoint(x, y);
        c.start_click = p;
        c.canvas_offset_start = c.canvas_offset;
        c.ui_state = UI_MOVING_CANVAS;
        return;
    }

    // find hover objects:
    // points, lines, etc...
    if (c.ui_state == UI_SELECT) {

        bool shift = ofGetKeyPressed(OF_KEY_SHIFT);


        // add to the selection
        if (shift) {
            c.new_selection = c.selection;
        } else {
            c.new_selection.clear();
        }

        bool selected = false;

        // if there is a hover object, start dragging it
        if (c.hover_point && c.hover_point_p) {

            if (c.hover_point_p->selected) {
                selected = true;
            }

            // change selection
            c.new_selection.add(c.hover_point_p->getId());
            c.ui_state = UI_MOVING_SELECTION;

        } else if (c.hover_line && c.hover_line_p[0] && c.hover_line_p[1]) {

            if (c.hover_line_p[0]->selected && c.hover_line_p[1]->selected) {
                selected = true;
            }

            c.new_selection.add(c.hover_line_p[0]->getId());
            c.new_selection.add(c.hover_line_p[1]->getId());
            c.ui_state = UI_MOVING_SELECTION;

        } else if (c.hover_polygon && c.hover_polygon_p)  {

            selected = true;
            for (Vertex *v = c.hover_polygon_p->front; v != NULL; v = v->next) {
                if (!v->selected) {
                    selected = false;
                }
                c.new_selection.add(v->getId());
                if (v->next == c.hover_polygon_p->front) break;
            }
            c.ui_state = UI_MOVING_SELECTION;
        }

        if (c.ui_state == UI_MOVING_SELECTION) {
            if (!selected) {
                ChangeSelectionAction *select = new ChangeSelectionAction();
                select->prev_selection = c.selection;
                select->new_selection = c.new_selection;
                c.addAction(select);
            }
        } else {
            // clear selection
            ChangeSelectionAction *select = new ChangeSelectionAction();
            select->prev_selection = c.selection;
            c.addAction(select);
        }

        for (int i = 0; i < c.selection.vertices.size(); i++) {
            Vertex *vertex = c.getVertex(c.selection.vertices[i]);
            vertex->start_p = *vertex;
        }
    }

    // start drawing a line
    if (c.ui_state == UI_DRAW_LINE) {
        c.ui_state = UI_DRAWING_LINE;
        c.curr_line.release();
        c.curr_line.addBack(c.start_click);
        c.curr_line.addBack(p_mm);
        c.curr_line.back->hover = true;
        c.curr_line.front->hover = true;
    }

    if (c.ui_state == UI_ADD_VERTEX) {
        // insert a new point between the points of the hover line
        if (c.hover_line && c.hover_line_p[0] && c.hover_line_p[1]) {

            Vertex *v0 = c.hover_line_p[0];
            Vertex *v1 = c.hover_line_p[1];

            Vertex *v = new Vertex();
            *v = c.add_v;

            ModifyPolylineAction *add = new ModifyPolylineAction();
            add->p_before.cloneFrom(v0->p);

            v->p = v0->p;
            v->prev = v0;
            v->next = v1;
            v0->next = v;
            v1->prev = v;
            v->p->updateIndexes();
            v->start_p = *v;
            VertexId new_id = v->getId();

            add->p_after.cloneFrom(v0->p);
            c.addAction(add);

            unselectMode();
            select_button->selected = true;

            ChangeSelectionAction *select = new ChangeSelectionAction();
            select->prev_selection = c.selection;

            select->new_selection.add(new_id);
            c.addAction(select);

            c.resetHover();
            c.hover_point_p = c.getVertex(new_id);
            c.hover_point = true;

            c.ui_state = UI_MOVING_SELECTION;
        }
    }

    c.selection_r.set(p_mm, 0.0f, 0.0f);
    if (c.ui_state == UI_SELECT) {
        c.ui_state = UI_MOUSE_SELECTION;
    }
}

void ofApp::mouseReleased(int x, int y, int button) {

    ofPoint p_mm = c.snapMm(c.getMm(ofPoint(x, y)));

    // snap to the hovering point
    if (c.hover_point && c.hover_point_p) {
        p_mm = *c.hover_point_p;
    }

    if (c.ui_state == UI_MOUSE_SELECTION) {
        c.ui_state = UI_SELECT;
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
    if (c.ui_state == UI_DRAWING_LINE) {

        AddLineAction* add_line = new AddLineAction();
        add_line->p[0] = c.start_click;
        add_line->p[1] = p_mm;
        c.addAction(add_line);

        c.connectPolylines(c.lines.back());
        c.connectPolylines(c.lines.back());

        c.ui_state = UI_SELECT;
        unselectMode();
        select_button->selected = true;
    }

    if (c.ui_state == UI_MOVING_SELECTION) {

        for (int i = 0; i < c.selection.vertices.size(); i++) {
            Vertex *v = c.getVertex(c.selection.vertices[i]);
            *v = v->start_p;
        }

        ofPoint move_v = (p_mm - c.start_click);
        if (abs(move_v.x) < 1.0f && abs(move_v.y) < 1.0f) {
            ChangeSelectionAction *select = new ChangeSelectionAction();
            select->prev_selection = c.selection;
            select->new_selection = c.new_selection;
            c.addAction(select);
        } else {
            MoveSelectionAction *move_action = new MoveSelectionAction();
            move_action->selection = c.selection;
            move_action->v = move_v;
            c.addAction(move_action);

            if (c.selection.vertices.size() == 1) {
                Vertex *v = c.getVertex(c.selection.vertices[0]);

                ChangeSelectionAction *select = new ChangeSelectionAction();
                select->prev_selection = c.selection;
                c.addAction(select);

                c.connectPolylines(v->p);
            }

            // connect polylines after the line's dragging
            if (c.selection.vertices.size() == 2) {
                Vertex *v1 = c.getVertex(c.selection.vertices[0]);
                Vertex *v2 = c.getVertex(c.selection.vertices[1]);
                if (v1->p == v2->p) {
                    c.connectPolylines(v1->p);
                    c.connectPolylines(v1->p);
                }
            }
        }

        c.ui_state = UI_SELECT;
    }

    c.resetHover();
    c.setHover(p_mm);
}
