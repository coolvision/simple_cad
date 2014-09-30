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

void ofApp::mouseMoved(int x, int y ){

    ofPoint p(x, y);

    // find selected objects:
    // points, lines, etc...
    if (ui_state == UI_SELECT || ui_state == UI_DRAW_LINE) {

        selected_point = false;
        selected_line = false;

        for (int i = 0; i < lines.size(); i++) {
            for (int j = 0; j < lines[i].selected_p.size(); j++) {
                lines[i].selected_p[j] = false;
                lines[i].selected_line[j] = false;
            }
            lines[i].selected = false;
            lines[i].hover = false;
        }

        // check selection of points
        float min_d = FLT_MAX;
        int min_d_i = -1;
        int min_d_j = -1;
        for (int i = 0; i < lines.size(); i++) {
            for (int j = 0; j < lines[i].p.size(); j++) {
                float d0 = (p - lines[i].p[j]).length();
                if (d0 < 15.0f && d0 < min_d) {
                    min_d = d0;
                    min_d_i = i;
                    min_d_j = j;

                }
            }
        }
        if (min_d_j >= 0) {
            lines[min_d_i].selected_p[min_d_j] = true;
            selected_point_p = &lines[min_d_i].p[min_d_j];
            selected_point_i = min_d_j;
            selected_polyline_i = min_d_i;
            selected_point = true;
            was_selected_point = true;
            lines[min_d_i].hover = true;
        }

        for (int i = 0; i < lines.size(); i++) {
            // check selection of line segments
            for (int j = 0; j < lines[i].p.size()-1; j++) {
                float d = segmentDistance(lines[i].p[j], lines[i].p[j+1], p);
                if (d < 15.0f && !selected_point) {

                    // hightlight line segment and 2 points
                    lines[i].selected_line[j] = true;
                    lines[i].selected_p[j] = true;
                    lines[i].selected_p[j+1] = true;

                    selected_line_p[0] = &lines[i].p[j];
                    selected_line_p[1] = &lines[i].p[j+1];

                    selected_line = true;
                    selected_line_i[0] = j;
                    selected_line_i[1] = j+1;
                    lines[i].hover = true;
                    break;
                }
                if (selected_line) {
                    break;
                }
            }
        }

        if (selected_point) {
            add_line->x = selected_point_p->x + 5;
            add_line->y = selected_point_p->y - 29;
        }
    }
}

void ofApp::mouseDragged(int x, int y, int button){

    ofPoint p(x, y);

    if (ui_state == UI_DRAWING_LINE) {
        curr_line.p[0] = start_click;
        curr_line.p[1] = p;
        add_line->x = p.x + 5;
        add_line->y = p.y - 29;
    }

    if (ui_state == UI_MOVING_POINT) {
        *selected_point_p = p;
        add_line->x = p.x + 5;
        add_line->y = p.y - 29;
    }

    if (ui_state == UI_MOVING_LINE) {

        *selected_line_p[0] = curr_line.p[0] + (p - start_click);
        *selected_line_p[1] = curr_line.p[1] + (p - start_click);

        if (was_selected_point) {
            ofPoint p1 = *selected_point_p;
            add_line->x = p1.x + 5;
            add_line->y = p1.y - 29;
        }
    }
}

void ofApp::mousePressed(int x, int y, int button){

    ofPoint p = snap(ofPoint(x, y));

    start_click = p;

    // start drawing a line
    if (ui_state == UI_DRAW_LINE) {
        ui_state = UI_DRAWING_LINE;
        curr_line.p[0] = start_click;
        curr_line.p[1] = p;
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
            curr_line.p[0] = *selected_line_p[0];
            curr_line.p[1] = *selected_line_p[1];
        }
    }
}

void ofApp::mouseReleased(int x, int y, int button){

    ofPoint p = snap(ofPoint(x, y));

    // stop the drawing / dragging / selection
    if (ui_state == UI_DRAWING_LINE) {
        ui_state = UI_DRAW_LINE;

        bool add_point = false;
        for (int i = 0; i < lines.size(); i++) {
            if (start_click == lines[i].p.front()) {
                lines[i].p.push_front(p);
                lines[i].selected_p.push_front(false);
                add_point = true;
            } else if (p == lines[i].p.front()) {
                lines[i].p.push_front(start_click);
                lines[i].selected_p.push_front(false);
                add_point = true;
            } else if (start_click == lines[i].p.back()) {
                lines[i].p.push_back(p);
                lines[i].selected_p.push_back(false);
                add_point = true;
            } else if (p == lines[i].p.back()) {
                lines[i].p.push_back(p);
                lines[i].selected_p.push_back(false);
                add_point = true;
            }
        }

        if (add_point) {
            // check if this closes the polyline

        }

        if (!add_point) {
            Polyline l;
            l.p[0] = start_click;
            l.p[1] = p;
            lines.push_back(l);
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

        // if it's an end-point of a polyline,
        // check if it got connected to another line

        ui_state = UI_SELECT;
        *selected_point_p = p;
        add_line->x = p.x + 5;
        add_line->y = p.y - 29;
    }
    
    if (ui_state == UI_MOVING_LINE) {

        // one of the points can get connected to a polyline


        ui_state = UI_SELECT;
        *selected_line_p[0] = curr_line.p[0] + (p - start_click);
        *selected_line_p[1] = curr_line.p[1] + (p - start_click);

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