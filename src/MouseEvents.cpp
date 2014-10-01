//
//  MouseEvents.cpp
//  simple_card
//
//  Created by sk on 9/30/14.
//
//

#include "ofApp.h"

float segmentDistance(ofPoint v, ofPoint w, ofPoint p) {

    // Return minimum distance between line segment vw and point p
    float l2 = (v - w).lengthSquared();  // i.e. |w-v|^2 -  avoid a sqrt

    if (l2 == 0.0) {
        return (p, v).length();   // v == w case
    }
    // Consider the line extending the segment, parameterized as v + t (w - v).
    // We find projection of point p onto the line.
    // It falls where t = [(p-v) . (w-v)] / |w-v|^2
    float t = (p - v).dot(w - v) / l2;
    if (t < 0.0) {
        return (p - v).length();       // Beyond the 'v' end of the segment
    } else if (t > 1.0) {
        return (p - w).length();  // Beyond the 'w' end of the segment
    }
    ofPoint projection = v + t * (w - v);  // Projection falls on the segment

    return (p - projection).length();
}

void ofApp::mouseMoved(int x, int y ) {

    ofPoint p(x, y);

    // find selected objects:
    // points, lines, etc...
    if (ui_state == UI_SELECT || ui_state == UI_DRAW_LINE) {

        selected_point = false;
        selected_line = false;

        for (int i = 0; i < lines.size(); i++) {
            for (Vertex *v = lines[i]->front; v != NULL; v = v->next) {
                v->hover = false;
                if (v->next == lines[i]->front) break; // closed polylines
            }
            lines[i]->hover = false;
        }

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
            min_d_v->p->hover = true;
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
                    v->p->hover = true;
                    break;
                }
                if (selected_line) {
                    break;
                }
                if (v->next == lines[i]->front) break; // closed polylines
            }
        }

        if (selected_point) {
            add_line->x = selected_point_p->x + 5;
            add_line->y = selected_point_p->y - 29;
        }
    }
}

void ofApp::mouseDragged(int x, int y, int button) {

    ofPoint p(x, y);

    if (ui_state == UI_DRAWING_LINE) {
        *curr_line.back = p;
        add_line->x = p.x + 5;
        add_line->y = p.y - 29;
    }

    if (ui_state == UI_MOVING_POINT) {
        *selected_point_p = p;
        selected_point_p->p->updatePath();
        add_line->x = p.x + 5;
        add_line->y = p.y - 29;
    }

    if (ui_state == UI_MOVING_LINE) {

        *selected_line_p[0] = *curr_line.front + (p - start_click);
        *selected_line_p[1] = *curr_line.back + (p - start_click);

        selected_line_p[0]->p->updatePath();

        if (was_selected_point) {
            ofPoint p1 = *selected_point_p;
            add_line->x = p1.x + 5;
            add_line->y = p1.y - 29;
        }
    }
}

void ofApp::mousePressed(int x, int y, int button) {

    ofPoint p = snap(ofPoint(x, y));

    start_click = p;

    // start drawing a line
    if (ui_state == UI_DRAW_LINE) {
        ui_state = UI_DRAWING_LINE;
        curr_line.release();
        curr_line.addBack(start_click);
        curr_line.addBack(p);
    }

    // find selected objects:
    // points, lines, etc...
    if (ui_state == UI_SELECT) {

        // if there is a selected object, start dragging it
        if (selected_point) {
            ui_state = UI_MOVING_POINT;
        }
        if (selected_line) {
            ui_state = UI_MOVING_LINE;
            curr_line.release();
            curr_line.addBack(*selected_line_p[0]);
            curr_line.addBack(*selected_line_p[1]);
        }
    }
}

// connect to an existing polyline, or add a new one
Polyline *ofApp::connectLine(ofPoint *p1, ofPoint *p2) {



}

// check if endpoints overlap, and if they do, connect the polylines
Polyline *ofApp::connectPolyline(Polyline *p) {


}

void ofApp::mouseReleased(int x, int y, int button) {

    ofPoint p = snap(ofPoint(x, y));

    // stop the drawing / dragging / selection
    if (ui_state == UI_DRAWING_LINE) {
        ui_state = UI_DRAW_LINE;

        bool new_line = true;

        for (int i = 0; i < lines.size(); i++) {
            if (lines[i]->closed) continue;
            if ((start_click == *lines[i]->front && p == *lines[i]->back) ||
                (start_click == *lines[i]->back && p == *lines[i]->front)) {
                // does it, by chance close the polyline?
                lines[i]->back->next = lines[i]->front;
                lines[i]->front->prev = lines[i]->back;
                lines[i]->closed = true;
                new_line = false;
            } else if (start_click == *lines[i]->front) {
                lines[i]->addFront(p);
                new_line = false;
            } else if (p == *lines[i]->front) {
                lines[i]->addFront(start_click);
                new_line = false;
            } else if (start_click == *lines[i]->back) {
                lines[i]->addBack(p);
                new_line = false;
            } else if (p == *lines[i]->back) {
                lines[i]->addBack(start_click);
                new_line = false;
            }
        }

        // check if the second point is connected to anything
        if (!new_line) {


        }

        if (new_line) {
            *curr_line.back = p;
            lines.push_back(new Polyline());
            lines.back()->cloneFrom(&curr_line);
        }

        ui_state = UI_SELECT;

        for (int i = 0; i < buttons.size(); i++) {
            buttons[i]->selected = false;
        }
        select_button->selected = true;

        add_line->x = p.x + 5;
        add_line->y = p.y - 29;
    }

    if (ui_state == UI_MOVING_POINT) {
        ui_state = UI_SELECT;
        *selected_point_p = p;
        add_line->x = p.x + 5;
        add_line->y = p.y - 29;
    }

    if (ui_state == UI_MOVING_LINE) {

        ui_state = UI_SELECT;
        *selected_line_p[0] = *curr_line.front + (p - start_click);
        *selected_line_p[1] = *curr_line.back + (p - start_click);

        if (was_selected_point) {
            ofPoint p1 = *selected_point_p;
            add_line->x = p1.x + 5;
            add_line->y = p1.y - 29;
        }
    }
}

ofPoint ofApp::snap(ofPoint p) {

    p.x = round(p.x / points_step) * points_step;
    p.y = round(p.y / points_step) * points_step;

    return p;
}