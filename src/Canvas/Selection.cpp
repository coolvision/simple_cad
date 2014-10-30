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

    for (int i = 0; i < lines_modified.size(); i++) {
        lines_modified[i] = false;
    }
    for (int i = 0; i < s.items.size(); i++) {
        lines_modified[s.items[i].container_id] = true;
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
            if (v->next != NULL) {
                v->next->prev = v->prev;
            }
            if (v->prev != NULL) {
                v->prev->next = v->next;
            }
            if (v == v->parent->front && v == v->parent->back) {
                v->parent->front = NULL;
                v->parent->back = NULL;
            } else if (v == v->parent->front) {
                v->parent->front = v->next;
            } else if (v == v->parent->back) {
                v->parent->back = v->prev;
            }
            
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
        for (InteractiveObject *v = lines[i]->front; v != NULL; v = v->next) {
            v->selected = false;
            if (v->next == lines[i]->front) break; // closed polylines
        }
        lines[i]->selected = false;
    }

    selected_point = false;;
    selected_p = NULL;
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
        for (InteractiveObject *v = lines[i]->front; v != NULL; v = v->next) {
            v->hover = false;
            if (v->next == lines[i]->front) break; // closed polylines
        }
        lines[i]->hover = false;
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
        for (InteractiveObject *v = lines[i]->front; v != NULL; v = v->next) {
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
    if (min_d_v != NULL) {
        hover_point_p = min_d_v;
        hover_point = true;
        min_d_v->hover = true;
    }

    if (ui_state == UI_MOVING_SELECTION) {
        for (int i = 0; i < selection.items.size(); i++) {
            InteractiveObject *v = getItem(selection.items[i]);
            v->hover = true;
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
            lines[i]->update();
            if (lines[i]->ofp.inside(getPx(p))) {
                hover_polygon = true;
                hover_polygon_p = lines[i];
                lines[i]->hover = true;
            }
        }
    }

    if (!hover_point && !hover_polygon) {
        for (int i = 0; i < lines.size(); i++) {
            if (lines[i] == NULL) {
                continue;
            }
            // check selection of line segments
            for (InteractiveObject *v = lines[i]->front; v != NULL && v->next != NULL; v = v->next) {
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
}