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

    curr_p = p_mm;

    // ok, this is not such a good way to do this,
    // check if the press is over a button
    if (canvas_toolbar.inside(p)) {
        return;
    }
    
    if (c.ui_state == UI_ADD_VERTEX) {

        c.resetHover();

        for (int i = 0; i < c.lines.size(); i++) {
            for (Vertex *v = c.lines[i]->front; v != NULL && v->next != NULL; v = v->next) {
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

    prev_p = curr_p;
    curr_p = p_mm;

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
            v->p += curr_p - prev_p;
            v->update();
        }

        // find all joints connected to current selected joints
        // "seed" joints
        for (int j = 0; j < c.lines.size(); j++) {
            c.lines[j]->controlled = false;
        }
        for (int j = 0; j < c.joints.size(); j++) {
            if (c.joints[j] != NULL) {
                c.joints[j]->discovered = false;
                c.joints[j]->updated = false;
                c.joints[j]->controlled = false;
            }
        }

        vector<Joint *> selected_joints;
        for (int i = 0; i < c.lines.size(); i++) {
            if (c.lines[i]->selected) {
                for (int q = 0; q < c.lines[i]->links.size(); q++) {
                    Joint *j = (Joint *)c.getItem(ItemId(-1, c.lines[i]->links[q]));
                    if (j != NULL) {
                        selected_joints.push_back(j);
                    }
                }
            }
        }
        for (int i = 0; i < selected_joints.size(); i++) {
            updateConnectedJoints(selected_joints[i]);
        }

        for (int i = 0; i < c.lines.size(); i++) {
            if (c.lines[i]->selected) {
                c.lines[i]->controlled = true;
                for (int q = 0; q < c.lines[i]->links.size(); q++) {
                    Joint *j = (Joint *)c.getItem(ItemId(-1, c.lines[i]->links[q]));
                    if (j != NULL) {
                        j->p += curr_p - prev_p;
                        j->controlled = true;
                        j->updated = true;
                    }
                }
            }
            c.lines[i]->update();
        }

        for (int i = 0; i < c.lines.size(); i++) {
            if (c.lines[i]->selected) {
                for (int q = 0; q < c.lines[i]->links.size(); q++) {
                    Joint *j = (Joint *)c.getItem(ItemId(-1, c.lines[i]->links[q]));
                    if (j != NULL) {
                        if (j->supported) {
                            moveSupported(curr_p - prev_p, j);
                        }
                    }
                }

            }
            c.lines[i]->update();
        }

    }
}

void ofApp::updateConnectedJoints(Joint *j) {

    if (j != NULL) {
        if (j->discovered) return;
        j->discovered = true;
        j->moved_i = c.update_i;
        for (int i = 0; i < j->links.size(); i++) {
            Joint *j_i = (Joint *)c.getItem(ItemId(-1, j->links[i]));
            if (j_i != NULL) {
                updateConnectedJoints(j_i);
            }
        }
    }
}

void ofApp::moveSupported(ofPoint m, Joint *j) {

    if (j != NULL) {
        if (j->supported) {
            Joint *j_m = (Joint *)c.getItem(ItemId(-1, j->s_id[0]));
            Joint *j_n = (Joint *)c.getItem(ItemId(-1, j->s_id[1]));
            if (j_m != NULL) {
                if (!j_m->controlled && !j_m->updated) {
                    j_m->p += m;
                    j_m->updated = true;
                    moveSupported(m, j_m);
                }
            }
            if (j_n != NULL) {
                if (!j_n->controlled && !j_n->updated) {
                    j_n->p += m;
                    j_n->updated = true;
                    moveSupported(m, j_n);
                }
            }
        }
    }
}