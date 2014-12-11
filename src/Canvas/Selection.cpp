//
//  Selection.cpp
//  simple_card
//
//  Created by sk on 10/15/14.
//
//

#include <Canvas.h>

void SelectionList::add(ItemId item_id) {

    for (int i = 0; i < items.size(); i++) {
        if (item_id.container_id == items[i].container_id &&
            item_id.item_id == items[i].item_id) {
            return;
        }
    }
    items.push_back(item_id);
}

void SelectionList::clear() {
    items.clear();
}

void Canvas::deleteSelection() {

    SelectionList s = selection;

    ChangeSelectionAction *select = new ChangeSelectionAction();
    select->prev_selection = selection;
    // new selection is empty
    addAction(select);

    // see which polylines are modified
    vector<bool> lines_modified;
    lines_modified.resize(lines.size());

    bool joints_modified = false;
    for (int i = 0; i < lines_modified.size(); i++) {
        lines_modified[i] = false;
    }
    for (int i = 0; i < s.items.size(); i++) {
        if (s.items[i].container_id > 0) {
            lines_modified[s.items[i].container_id] = true;
        } else {
            joints_modified = true;
        }
    }

    // modify this polylines
    for (int i = 0; i < lines.size(); i++) {
        if (!lines_modified[i]) {
            continue;
        }

        ModifyPolylineAction *clear = new ModifyPolylineAction();
        clear->p_before.cloneFrom(lines[i]);

        for (int j = 0; j < s.items.size(); j++) {

            if (s.items[j].container_id != i) {
                continue;
            }

            InteractiveObject *v = getItem(s.items[j]);

            if (v == NULL) continue;

            // delete
            v->release();

            delete v;
            v = NULL;
        }

        if (lines[i]->getLength() <= 2) {
            lines[i]->closed = false;
            if (lines[i]->front != NULL) {
                lines[i]->front->prev = NULL;
            }
            if (lines[i]->back != NULL) {
                lines[i]->back->next = NULL;
            }
        }
        lines[i]->update();

        clear->p_after.cloneFrom(lines[i]);
        addAction(clear);
    }

    // do joints deleting
    // save the before and after joints set
    if (joints_modified) {
        ModifyJointsAction *clear = new ModifyJointsAction();
        for (int i = 0; i < joints.size(); i++) {
            if (joints[i] != NULL) {
                clear->before.push_back((Joint *)joints[i]->getCopy());
            } else {
                clear->before.push_back(NULL);
            }
        }

        for (int j = 0; j < s.items.size(); j++) {

            if (s.items[j].container_id == -1) {
                if (s.items[j].item_id < joints.size()) {
                    delete joints[s.items[j].item_id];
                    joints[s.items[j].item_id] = NULL;
                }
            }
        }

        for (int i = 0; i < joints.size(); i++) {
            if (joints[i] != NULL) {
                clear->after.push_back((Joint *)joints[i]->getCopy());
            } else {
                clear->after.push_back(NULL);
            }
        }
        addAction(clear);
    }

//    for (int i = 0; i < joints.size(); i++) {
//        if (joints[i] != NULL) {
//            joints[i]->connected = false;
//        }
//    }

    hover_point = false;
    hover_point_p = NULL;

    hover_line = false;
    hover_line_p[0] = NULL;
    hover_line_p[1] = NULL;

    hover_polygon = false;
    hover_polygon_p = NULL;
}

void Canvas::clearSelection() {

    selection.clear();

    for (int i = 0; i < lines.size(); i++) {
        for (Vertex *v = lines[i]->front; v != NULL; v = v->next) {
            v->selected = false;
            if (v->next == lines[i]->front) break; // closed polylines
        }
        lines[i]->selected = false;
    }
    for (int i = 0; i < joints.size(); i++) {
        Joint *v = joints[i];
        v->selected = false;
    }
}

void Canvas::resetHover() {

    hover_point = false;
    hover_point_p = NULL;

    hover_line = false;
    hover_line_p[0] = NULL;
    hover_line_p[1] = NULL;

    hover_polygon = false;
    hover_polygon_p = NULL;

    for (int i = 0; i < lines.size(); i++) {
        if (lines[i] == NULL) {
            continue;
        }
        for (Vertex *v = lines[i]->front; v != NULL; v = v->next) {
            v->hover = false;
            if (v->next == lines[i]->front) break; // closed polylines
        }
        lines[i]->hover = false;
    }
    for (int i = 0; i < joints.size(); i++) {
        Joint *v = joints[i];
        if (v != NULL) {
            v->hover = false;
        }
    }
}

void Canvas::setHoverPoint(ofPoint p) {

    // check selection of points
    float min_d = FLT_MAX;
    InteractiveObject *min_d_v = NULL;
    for (int i = 0; i < lines.size(); i++) {
        if (lines[i] == NULL) {
            continue;
        }
        for (Vertex *v = lines[i]->front; v != NULL; v = v->next) {
            if (!(selection.items.size() == 1 && v->selected &&
                  ui_state == UI_MOVING_SELECTION)) {
                float d0 = (p - v->p).length();
                if (d0 < 2.0f && d0 < min_d) {
                    min_d = d0;
                    min_d_v = v;
                }
            }
            if (v->next == lines[i]->front) break; // closed polylines
        }
    }
    for (int i = 0; i < joints.size(); i++) {
        Joint *v = joints[i];
        if (v == NULL) continue;
        float d0 = (p - v->p).length();
        if (d0 < 2.0f && d0 < min_d) {
            min_d = d0;
            min_d_v = v;
        }
    }
    if (min_d_v != NULL) {
        hover_point_p = min_d_v;
        hover_point = true;
        min_d_v->hover = true;
    }

    if (ui_state == UI_MOVING_SELECTION) {
        for (int i = 0; i < selection.items.size(); i++) {
            InteractiveObject *v = getItem(selection.items[i]);
            if (v != NULL) {
                v->hover = true;
            }
        }
    }
}

void Canvas::setHover(ofPoint p) {

    setHoverPoint(p);

    if (!hover_point) {
        for (int i = 0; i < lines.size(); i++) {
            if (lines[i] == NULL) {
                continue;
            }
            if (!lines[i]->closed) continue;
            lines[i]->update(getPx(p));

            if (lines[i]->hover) {
                hover_polygon = true;
                hover_polygon_p = lines[i];
                for (Vertex *v = lines[i]->front; v != NULL; v = v->next) {
                    v->hover = true;
                    if (v->next == lines[i]->front) break; // closed polylines
                }
            }
        }
    }

    if (!hover_point && !hover_polygon) {
        for (int i = 1; i < (int)lines.size(); i++) {
            if (lines[i] == NULL) {
                continue;
            }
            // check selection of line segments
            for (Vertex *v = lines[i]->front; v != NULL && v->next != NULL; v = v->next) {
                float d = segmentDistance(v->p, v->next->p, p);
                float d1 = (p - v->p).length();
                float d2 = (p - v->next->p).length();
                if (d < 2.0f && d1 > 2.0f && d2 > 2.0f) {

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
    }

    // check if there is a hover over a link between joints
    for (int m = 0; m < joints.size(); m++) {
        Joint *j_m = joints[m];
        if (j_m == NULL) continue;
        for (int n = 0; n < j_m->links.size(); n++) {
            Joint *j_n = (Joint *)getItem(ItemId(-1, j_m->links[n]));
            if (j_n == NULL) continue;
            ofPoint mid = j_m->p + (j_n->p - j_m->p) * 0.5f;
            float d = segmentDistance(j_m->p, mid, p);
            if (d < 2.0f) {
                j_m->links_status[n].hover = true;
            } else {
                j_m->links_status[n].hover = false;
            }
        }
    }

}