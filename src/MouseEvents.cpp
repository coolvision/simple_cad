//
//  MouseEvents.cpp
//  simple_card
//
//  Created by sk on 9/30/14.
//
//

#include "ofApp.h"

void ofApp::mouseMoved(int x, int y ) {

    ofPoint p(x, y);

    // ok, this is not such a good way to do this,
    // check if the press is over a button
    if (cursor_toolbar.inside(p) || canvas_toolbar.inside(p)) {
        return;
    }

    if (ui_state == UI_ADD_VERTEX) {

        resetHover();

        for (int i = 0; i < lines.size(); i++) {
            for (Vertex *v = lines[i]->front; v != NULL && v->next != NULL; v = v->next) {
                float d = segmentDistance(*v, *v->next, p);
                if (d < 15.0f) {
                    hover_line = true;
                    add_v = lineProjection(*v, *v->next, p);
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

        // check selection of points
        float min_d = FLT_MAX;
        Vertex *min_d_v = NULL;
        for (int i = 0; i < lines.size(); i++) {
            for (Vertex *v = lines[i]->front; v != NULL; v = v->next) {
                float d0 = (p - *v).length();
                if (d0 < 15.0f && d0 < min_d) {
                    min_d = d0;
                    min_d_v = v;
                }
                if (v->next == lines[i]->front) break; // closed polylines
            }
        }
        if (min_d_v != NULL) {
            hover_point_p = min_d_v;
            hover_point = true;
            was_selected_point = true;
            min_d_v->hover = true;
        }

        for (int i = 0; i < lines.size(); i++) {
            // check selection of line segments
            for (Vertex *v = lines[i]->front; v != NULL && v->next != NULL; v = v->next) {
                float d = segmentDistance(*v, *v->next, p);
                if (d < 15.0f && !hover_point) {

                    hover_line_p[0] = v;
                    hover_line_p[1] = v->next;
                    hover_line = true;

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

        if (hover_point && hover_point_p) {
            cursor_toolbar.updatePosition(hover_point_p->x + toolbar_off.x,
                                          hover_point_p->y - toolbar_off.y);
        }
        if (hover_line && hover_line_p[0] && hover_line_p[1]) {
            updateToolbar(p);
        }

        if (!hover_point && !hover_line) {
            for (int i = 0; i < lines.size(); i++) {
                if (!lines[i]->closed) continue;
                lines[i]->updatePath();
                if (lines[i]->ofp.inside(p)) {
                    hover_polygon = true;
                    hover_polygon_p = lines[i];
                }
            }
        }
    }
}

void ofApp::mousePressed(int x, int y, int button) {

    ofPoint p = snap(ofPoint(x, y));

    start_click = p;

    // ok, this is not such a good way to do this,
    // check if the press is over a button
    if (cursor_toolbar.inside(p) || canvas_toolbar.inside(p)) {
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
            selection.add(hover_point_p);
            ui_state = UI_MOVING_POINT;
        }

        if (hover_line && hover_line_p[0] && hover_line_p[1]) {
            hover_line_p[0]->selected = true;
            hover_line_p[1]->selected = true;
            selection.add(hover_line_p[0]);
            selection.add(hover_line_p[1]);
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
        curr_line.addBack(p);
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
            select_button->hover = true;

            ui_state = UI_MOVING_POINT;
            hover_point = true;
            hover_line = false;
            if (hover_point_p) {
                hover_point_p->hover = false;
            }
            hover_point_p = v;
            v->hover = true;
        }
    }

    selection_r.set(p, 0.0f, 0.0f);
    if (ui_state == UI_SELECT) {
        ui_state = UI_MOUSE_SELECTION;
    }
}


void ofApp::mouseDragged(int x, int y, int button) {

    ofPoint p(x, y);

    //if (ui_state == UI_MOUSE_SELECTION) {
        ofPoint r = p - start_click;
        selection_r.set(start_click, r.x, r.y);
    //}

    if (ui_state == UI_DRAWING_LINE) {
        *curr_line.back = p;
        cursor_toolbar.updatePosition(p.x + toolbar_off.x,
                                      p.y - toolbar_off.y);
    }

    if (ui_state == UI_MOVING_POLYGON || ui_state == UI_MOVING_LINE ||
        ui_state == UI_MOVING_POINT) {
        for (int i = 0; i < selection.vertices.size(); i++) {
            selection.vertices[i]->p->updatePath();
            *selection.vertices[i] = selection.start_p[i] + (p - start_click);
        }
    }
}

void ofApp::mouseReleased(int x, int y, int button) {

    ofPoint p = snap(ofPoint(x, y));

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
                    }
                    if (v->next == lines[i]->front) break; // closed polylines
                }
            }
        }
    }


    // stop the drawing / dragging / selection
    if (ui_state == UI_DRAWING_LINE) {
        ui_state = UI_DRAW_LINE;

        Polyline *pl = connectLine(&start_click, &p);
        connectPolylines(pl);

        ui_state = UI_SELECT;
        unselectMode();
        select_button->hover = true;

        cursor_toolbar.updatePosition(p.x + toolbar_off.x,
                                      p.y - toolbar_off.y);
    }

    if (ui_state == UI_MOVING_POLYGON || ui_state == UI_MOVING_LINE ||
        ui_state == UI_MOVING_POINT) {

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
            *selection.vertices[i] = selection.start_p[i] + (p - start_click);
            selection.start_p[i] = *selection.vertices[i];
        }

        if (ui_state == UI_MOVING_POINT && hover_point_p) {
            connectPolylines(hover_point_p->p);
        }

        if (ui_state == UI_MOVING_LINE && hover_line_p[0] && hover_line_p[1]) {
            connectPolylines(hover_line_p[0]->p);
            connectPolylines(hover_line_p[1]->p);
        }

        ui_state = UI_SELECT;
    }

}

ofPoint ofApp::snap(ofPoint p) {
    
    p.x = round(p.x / points_step) * points_step;
    p.y = round(p.y / points_step) * points_step;
    
    return p;
}

