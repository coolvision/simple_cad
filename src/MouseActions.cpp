//
//  MouseEvents.cpp
//  simple_card
//
//  Created by sk on 9/30/14.
//
//

#include "ofApp.h"

void ofApp::mousePressed(int x, int y, int button) {

    ofPoint p_mm = snapMm(getMm(ofPoint(x, y)));
    // snap to the hovering point
    if (hover_point && hover_point_p) {
        p_mm = *hover_point_p;
    }

    start_click = p_mm;

    // ok, this is not such a good way to do this,
    // check if the press is over a button
    if (canvas_toolbar.inside(ofPoint(x, y))) {
        return;
    }

    if (ui_state == UI_MOVE_CANVAS || ui_state == UI_MOVING_CANVAS) {
        ofPoint p = ofPoint(x, y);
        start_click = p;
        canvas_offset_start = canvas_offset;
        ui_state = UI_MOVING_CANVAS;
        return;
    }

    // find hover objects:
    // points, lines, etc...
    if (ui_state == UI_SELECT) {

        // is the item already selected?
        move_already_selected = false;
        if (hover_point && hover_point_p) {
            if (hover_point_p->selected) {
                move_already_selected = true;
            }
        }
        if (hover_line && hover_line_p[0] && hover_line_p[1]) {
            if (hover_line_p[0]->selected &&
                hover_line_p[1]->selected) {
                move_already_selected = true;
            }
        }
        if (hover_polygon && hover_polygon_p)  {
            for (Vertex *v = hover_polygon_p->front; v != NULL; v = v->next) {
                if (v->selected) {
                    move_already_selected = true;
                }
                break;
            }
        }

        bool shift = ofGetKeyPressed(OF_KEY_SHIFT);
        if (!shift && !move_already_selected) {
            clearSelection();
        }

        // if there is a hover object, start dragging it
        if (hover_point && hover_point_p) {

            hover_point_p->selected = true;
            *hover_point_p = p_mm;

            selection.add(hover_point_p);

            selected_point = true;
            selected_point_p = hover_point_p;
            start_p = *hover_point_p;

            ui_state = UI_MOVING_POINT;
        }
        if (hover_line && hover_line_p[0] && hover_line_p[1]) {

            hover_line_p[0]->selected = true;
            hover_line_p[1]->selected = true;
            selection.add(hover_line_p[0]);
            selection.add(hover_line_p[1]);

            selected_line = true;
            selected_line_p[0] = hover_line_p[0];
            selected_line_p[1] = hover_line_p[1];

            ui_state = UI_MOVING_LINE;
        }
        if (hover_polygon && hover_polygon_p)  {
            for (Vertex *v = hover_polygon_p->front; v != NULL; v = v->next) {
                v->selected = true;
                selection.add(v);
                if (v->next == hover_polygon_p->front) break;
            }

            ui_state = UI_MOVING_POLYGON;
        }
    }

    // start drawing a line
    if (ui_state == UI_DRAW_LINE) {
        ui_state = UI_DRAWING_LINE;
        curr_line.release();
        curr_line.addBack(start_click);
        curr_line.addBack(p_mm);
        curr_line.back->hover = true;
        curr_line.front->hover = true;
    }

    if (ui_state == UI_ADD_VERTEX) {
        // insert a new point between the points of the hover line
        if (hover_line && hover_line_p[0] && hover_line_p[1]) {

            Vertex *v0 = hover_line_p[0];
            Vertex *v1 = hover_line_p[1];

            Vertex *v = new Vertex();
            *v = add_v;

            v->p = v0->p;
            v->prev = v0;
            v->next = v1;
            v0->next = v;
            v1->prev = v;

            unselectMode();
            select_button->selected = true;

            hover_line = false;

            if (selected_point_p) {
                selected_point_p->hover = false;
            }
            ui_state = UI_MOVING_POINT;
            selected_point = true;
            selected_point_p = v;
            start_p = *v;
            v->hover = true;
            v->selected = true;
        }
    }

    selection_r.set(p_mm, 0.0f, 0.0f);
    if (ui_state == UI_SELECT) {
        ui_state = UI_MOUSE_SELECTION;
    }
}

void ofApp::mouseReleased(int x, int y, int button) {

    ofPoint p_mm = snapMm(getMm(ofPoint(x, y)));

    // snap to the hovering point
    if (hover_point && hover_point_p) {
        p_mm = *hover_point_p;
    }

    if (ui_state == UI_MOUSE_SELECTION) {
        ui_state = UI_SELECT;
        ofRectangle r = selection_r;
        r.standardize();
        if (r.width > 10.0f && r.height > 10.0f) {
            clearSelection();
            for (int i = 0; i < lines.size(); i++) {
                for (Vertex *v = lines[i]->front; v != NULL; v = v->next) {
                    if (r.inside(*v)) {
                        v->selected = true;
                        selection.add(v);
                    }
                    if (v->next == lines[i]->front) break; // closed polylines
                }
            }
        }
    }


    // stop the drawing / dragging / selection
    if (ui_state == UI_DRAWING_LINE) {

        Polyline *pl = connectLine(&start_click, &p_mm);
        connectPolylines(pl);

        ui_state = UI_SELECT;
        unselectMode();
        select_button->selected = true;
    }

    if (ui_state == UI_MOVING_POINT && selected_point_p) {
        selected_point_p->p->updatePath();
        *selected_point_p = p_mm;
        connectPolylines(selected_point_p->p);
        ui_state = UI_SELECT;
    }

    if (ui_state == UI_MOVING_POLYGON || ui_state == UI_MOVING_LINE) {

        ofRectangle r = selection_r;
        r.standardize();
        if (move_already_selected && r.width < 2.0f && r.height < 2.0f) {
            clearSelection();
            // select the hovering item
            if (hover_point && hover_point_p) {
                hover_point_p->selected = true;
                selection.add(hover_point_p);
            }
            if (hover_line && hover_line_p[0] && hover_line_p[1]) {
                hover_line_p[0]->selected = true;
                hover_line_p[1]->selected = true;
                selection.add(hover_line_p[0]);
                selection.add(hover_line_p[1]);
            }
            if (hover_polygon && hover_polygon_p)  {
                for (Vertex *v = hover_polygon_p->front; v != NULL; v = v->next) {
                    v->selected = true;
                    selection.add(v);
                    if (v->next == hover_polygon_p->front) break;
                }
            }
            ui_state = UI_SELECT;
            return;
        }

        for (int i = 0; i < selection.vertices.size(); i++) {
            selection.vertices[i]->p->updatePath();
            *selection.vertices[i] = selection.start_p[i] + (p_mm - start_click);
            selection.start_p[i] = *selection.vertices[i];
        }

        if (ui_state == UI_MOVING_LINE && selected_line_p[0] && selected_line_p[1]) {
            connectPolylines(selected_line_p[0]->p);
            connectPolylines(selected_line_p[1]->p);
        }

        ui_state = UI_SELECT;
    }
}
