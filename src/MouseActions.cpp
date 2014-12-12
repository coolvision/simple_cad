//
//  MouseEvents.cpp
//  simple_card
//
//  Created by sk on 9/30/14.
//
//

#include "ofApp.h"

void ofApp::mousePressed(int x, int y, int button) {

    ofPoint p_mm = c.getMm(ofPoint(x, y));
    c.start_click_snap = c.snapMm(c.getMm(ofPoint(x, y)));
    c.start_click = c.getMm(ofPoint(x, y));

    // ok, this is not such a good way to do this,
    // check if the press is over a button
    if (canvas_toolbar.inside(ofPoint(x, y))) {
        return;
    }

    ofPoint icon_offset(13, 13);
    if (c.ui_state == UI_ADD_JOINT_R) {
        AddJointAction *add = new AddJointAction();
        add->p = p_mm;
        add->type = JOINT_REVOLUTE;
        c.addAction(add);
        c.ui_state = UI_SELECT;
    }
    if (c.ui_state == UI_ADD_JOINT_FIXED) {
        AddJointAction *add = new AddJointAction();
        add->p = p_mm;
        add->type = JOINT_FIXED;
        c.addAction(add);
        c.ui_state = UI_SELECT;
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
            c.hover_polygon_p->selected = true;
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

        for (int i = 0; i < c.selection.items.size(); i++) {
            InteractiveObject *item = c.getItem(c.selection.items[i]);
            if (item != NULL) {
                item->start_p = item->p;
                item->dragged = true;
            }
        }

        if (c.move_joints != NULL) {
            delete c.move_joints;
        }
        c.move_joints = new ModifyJointsAction();
        for (int i = 0; i < c.joints.size(); i++) {
            if (c.joints[i] != NULL) {
                c.move_joints->before.push_back((Joint *)c.joints[i]->getCopy());
            } else {
                c.move_joints->before.push_back(NULL);
            }
        }

    }

    // start drawing a line
    if (c.ui_state == UI_DRAW_LINE) {
        c.ui_state = UI_DRAWING_LINE;
        c.curr_line.release();
        Vertex v[2];
        v[0].p = c.start_click;
        v[1].p = p_mm;
        c.curr_line.addBack(&v[0]);
        c.curr_line.addBack(&v[1]);
        c.curr_line.back->hover = true;
        c.curr_line.front->hover = true;
    }

    if (c.ui_state == UI_ADD_VERTEX) {
        // insert a new point between the points of the hover line
        if (c.hover_line && c.hover_line_p[0] && c.hover_line_p[1]) {

            Vertex *v0 = c.hover_line_p[0];
            Vertex *v1 = c.hover_line_p[1];

            ModifyPolylineAction *add = new ModifyPolylineAction();
            add->p_before.cloneFrom((Polyline *)v0->parent);

            Vertex *v = new Vertex();
            *v = c.add_v;
            v->parent = v0->parent;
            v->prev = v0;
            v->next = v1;
            v0->next = v;
            v1->prev = v;
            v->parent->update();
            v->start_p = v->p;
            ItemId new_id = v->getId();

            add->p_after.cloneFrom((Polyline *)v0->parent);
            c.addAction(add);

            unselectMode();
            select_button->selected = true;

            ChangeSelectionAction *select = new ChangeSelectionAction();
            select->prev_selection = c.selection;

            select->new_selection.add(new_id);
            c.addAction(select);

            c.resetHover();
            c.hover_point_p = (Vertex *)c.getItem(new_id);
            c.hover_point = true;

            c.ui_state = UI_MOVING_SELECTION;

            c.updateDistances();
        }
    }

    c.selection_r.set(p_mm, 0.0f, 0.0f);
    if (c.ui_state == UI_SELECT) {
        c.ui_state = UI_MOUSE_SELECTION;
    }
}

void ofApp::mouseReleased(int x, int y, int button) {

    ofPoint p_mm = c.getMm(ofPoint(x, y));

    // snap to the hovering point
    if (c.hover_point && c.hover_point_p) {
        if (!c.hover_point_p->dragged) {
            p_mm = c.hover_point_p->p;
        }
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
                    if (r.inside(v->p)) {
                        select->new_selection.add(v->getId());
                    }
                    if (v->next == c.lines[i]->front) break; // closed polylines
                }
            }
            for (int i = 0; i < c.joints.size(); i++) {
                Joint *v = c.joints[i];
                if (v == NULL) continue;
                if (r.inside(v->p)) {
                    select->new_selection.add(v->getId());
                }
            }

            c.addAction(select);
        }
    }

    // stop the drawing / dragging / selection
    if (c.ui_state == UI_DRAWING_LINE) {

        AddLineAction* add_line = new AddLineAction();
        add_line->p[0].p = c.start_click;
        add_line->p[1].p = p_mm;
        c.addAction(add_line);

        c.connectPolylines(c.lines.back());
        c.connectPolylines(c.lines.back());

        c.ui_state = UI_SELECT;
        unselectMode();
        select_button->selected = true;
    }

    if (c.ui_state == UI_MOVING_SELECTION) {

        for (int i = 0; i < c.selection.items.size(); i++) {
            InteractiveObject *v = c.getItem(c.selection.items[i]);
            if (v == NULL) continue;
            v->dragged = false;
        }

        // update relative polygons and joints positions
        for (int i = 0; i < c.lines.size(); i++) {
            Polyline *l = c.lines[i];
            for (int m = 0; m < l->links.size(); m++) {
                Joint *j = (Joint *)c.getItem(ItemId(-1, l->links[m]));
                if (j == NULL) continue;
                l->links_rel[m] = j->p - l->p;
            }
        }

        ofPoint move_v = (p_mm - c.start_click);
        if (abs(move_v.x) < 0.1f && abs(move_v.y) < 0.1f) {
            ChangeSelectionAction *select = new ChangeSelectionAction();
            select->prev_selection = c.selection;
            select->new_selection = c.new_selection;
            c.addAction(select);
        } else {
            MoveSelectionAction *move_action = new MoveSelectionAction();
            move_action->selection = c.selection;
            move_action->v = move_v;
            c.addAction(move_action);

            if (c.move_joints != NULL) {
                for (int i = 0; i < c.joints.size(); i++) {
                    if (c.joints[i] != NULL) {
                        c.move_joints->after.push_back((Joint *)c.joints[i]->
                                                                    getCopy());
                    } else {
                        c.move_joints->after.push_back(NULL);
                    }
                }
                c.addAction(c.move_joints);
                c.move_joints = NULL;
            }

            if (c.selection.items.size() == 1) {
                Vertex *v = (Vertex *)c.getItem(c.selection.items[0]);

                ChangeSelectionAction *select = new ChangeSelectionAction();
                select->prev_selection = c.selection;
                c.addAction(select);

                c.connectPolylines((Polyline *)v->parent);
            }

            // connect polylines after the line's dragging
            if (c.selection.items.size() == 2) {
                Vertex *v1 = (Vertex *)c.getItem(c.selection.items[0]);
                Vertex *v2 = (Vertex *)c.getItem(c.selection.items[1]);
                if (v1->parent == v2->parent) {
                    c.connectPolylines((Polyline *)v1->parent);
                    c.connectPolylines((Polyline *)v1->parent);
                }
            }
        }

        c.ui_state = UI_SELECT;
    }

    c.resetHover();
    c.setHover(p_mm);
}
