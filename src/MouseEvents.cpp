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
                    selected_line = true;
                    add_v = lineProjection(*v, *v->next, p);
                    selected_line_p[0] = v;
                    selected_line_p[1] = v->next;
                    v->hover = true;
                    v->next->hover = true;
                    break;
                }
                if (selected_line) {
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
            selected_point_p = min_d_v;
            selected_point = true;
            was_selected_point = true;
            min_d_v->hover = true;
        }

        for (int i = 0; i < lines.size(); i++) {
            // check selection of line segments
            for (Vertex *v = lines[i]->front; v != NULL && v->next != NULL; v = v->next) {
                float d = segmentDistance(*v, *v->next, p);
                if (d < 15.0f && !selected_point) {

                    selected_line_p[0] = v;
                    selected_line_p[1] = v->next;
                    selected_line = true;

                    v->hover = true;
                    v->next->hover = true;
                    break;
                }
                if (selected_line) {
                    break;
                }
                if (v->next == lines[i]->front) break; // closed polylines
            }
        }

        if (selected_point && selected_point_p) {
            cursor_toolbar.updatePosition(selected_point_p->x + toolbar_off.x,
                                          selected_point_p->y - toolbar_off.y);
        }
        if (selected_line && selected_line_p[0] && selected_line_p[1]) {
            updateToolbar(p);
        }

        if (!selected_point && !selected_line) {
            for (int i = 0; i < lines.size(); i++) {
                if (!lines[i]->closed) continue;
                lines[i]->updatePath();
                if (lines[i]->ofp.inside(p)) {
                    selected_polygon = true;
                    selected_polygon_p = lines[i];
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

        bool shift = ofGetKeyPressed(OF_KEY_SHIFT);

        if (!shift) {
            clearSelection();
        }

        // if there is a hover object, start dragging it
        if (selected_point && selected_point_p) {

            selected_point_p->selected = true;
            selection.add(selected_point_p);

            ui_state = UI_MOVING_POINT;
        }

        if (selected_line && selected_line_p[0] && selected_line_p[1]) {

            selected_line_p[0]->selected = true;
            selected_line_p[1]->selected = true;
            selection.add(selected_line_p[0]);
            selection.add(selected_line_p[1]);

            ui_state = UI_MOVING_LINE;
            curr_line.release();
            curr_line.addBack(*selected_line_p[0]);
            curr_line.addBack(*selected_line_p[1]);
        }

        if (selected_polygon && selected_polygon_p)  {
            for (Vertex *v = selected_polygon_p->front; v != NULL; v = v->next) {

                v->selected = true;
                selection.add(v);

                if (v->next == selected_polygon_p->front) break;
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
        if (selected_line && selected_line_p[0] && selected_line_p[1]) {

            Vertex *v0 = selected_line_p[0];
            Vertex *v1 = selected_line_p[1];

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
            selected_point = true;
            selected_line = false;
            if (selected_point_p) {
                selected_point_p->hover = false;
            }
            selected_point_p = v;
            v->hover = true;
        }
    }

    if (ui_state == UI_SELECT) {
        ui_state = UI_MOUSE_SELECTION;
        selection_r.set(p, 0.0f, 0.0f);
    }

}


void ofApp::mouseDragged(int x, int y, int button) {

    ofPoint p(x, y);

    if (ui_state == UI_MOUSE_SELECTION) {
        ofPoint r = p - start_click;
        selection_r.set(start_click, r.x, r.y);
    }

    if (ui_state == UI_DRAWING_LINE) {
        *curr_line.back = p;
        cursor_toolbar.updatePosition(p.x + toolbar_off.x,
                                      p.y - toolbar_off.y);
    }

    if (ui_state == UI_MOVING_POINT && selected_point_p) {
        *selected_point_p = p;
        selected_point_p->p->updatePath();
        cursor_toolbar.updatePosition(p.x + toolbar_off.x,
                                      p.y - toolbar_off.y);
    }

    if (ui_state == UI_MOVING_LINE && selected_line_p[0] && selected_line_p[1]) {

        *selected_line_p[0] = *curr_line.front + (p - start_click);
        *selected_line_p[1] = *curr_line.back + (p - start_click);
        selected_line_p[0]->p->updatePath();

        updateToolbar(p);
    }
    
    if (ui_state == UI_MOVING_POLYGON) {
        for (int i = 0; i < selection.vertices.size(); i++) {
            if (i == 0) {
                selection.vertices[i]->p->updatePath();
            }
            *selection.vertices[i] = selection.start_p[i] + (p - start_click);
        }
    }
}

void ofApp::mouseReleased(int x, int y, int button) {

    ofPoint p = snap(ofPoint(x, y));

    // clear the selection
    if (!selected_point && !selected_line && !selected_polygon) {
        clearSelection();
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

    if (ui_state == UI_MOVING_POINT && selected_point_p) {
        ui_state = UI_SELECT;
        *selected_point_p = p;

        connectPolylines(selected_point_p->p);

        cursor_toolbar.updatePosition(p.x + toolbar_off.x,
                                      p.y - toolbar_off.y);
    }

    if (ui_state == UI_MOVING_LINE && selected_line_p[0] && selected_line_p[1]) {

        ui_state = UI_SELECT;
        *selected_line_p[0] = *curr_line.front + (p - start_click);
        *selected_line_p[1] = *curr_line.back + (p - start_click);

        connectPolylines(selected_line_p[0]->p);
        connectPolylines(selected_line_p[1]->p);

        updateToolbar(p);
    }

    if (ui_state == UI_MOVING_POLYGON) {
        ui_state = UI_SELECT;
        for (int i = 0; i < selection.vertices.size(); i++) {
            if (i == 0) {
                selection.vertices[i]->p->updatePath();
            }
            *selection.vertices[i] = selection.start_p[i] + (p - start_click);
        }
    }
}

ofPoint ofApp::snap(ofPoint p) {

    p.x = round(p.x / points_step) * points_step;
    p.y = round(p.y / points_step) * points_step;

    return p;
}

