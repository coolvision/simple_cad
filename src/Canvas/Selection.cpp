//
//  Selection.cpp
//  simple_card
//
//  Created by sk on 10/15/14.
//
//

#include <Canvas.h>

void SelectionList::add(VertexId v_id) {

    for (int i = 0; i < vertices.size(); i++) {
        if (v_id.line_i == vertices[i].line_i &&
            v_id.v_i == vertices[i].v_i) {
            return;
        }
    }
    vertices.push_back(v_id);
}

void SelectionList::clear() {
    vertices.clear();
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
    for (int i = 0; i < s.vertices.size(); i++) {
        lines_modified[s.vertices[i].line_i] = true;
    }

    // modify this polylines
    for (int i = 0; i < lines.size(); i++) {
        if (!lines_modified[i]) {
            continue;
        }

        ModifyPolylineAction *clear = new ModifyPolylineAction();
        clear->p_before.cloneFrom(lines[i]);

        for (int j = 0; j < s.vertices.size(); j++) {

            if (s.vertices[j].line_i != i) {
                continue;
            }

            Vertex *v = getVertex(s.vertices[j]);

            if (v == NULL) continue;

            // delete
            if (v->next != NULL) {
                v->next->prev = v->prev;
            }
            if (v->prev != NULL) {
                v->prev->next = v->next;
            }
            if (v == v->p->front && v == v->p->back) {
                v->p->front = NULL;
                v->p->back = NULL;
            } else if (v == v->p->front) {
                v->p->front = v->next;
            } else if (v == v->p->back) {
                v->p->back = v->prev;
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
        lines[i]->updatePath();

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
        for (Vertex *v = lines[i]->front; v != NULL; v = v->next) {
            v->selected = false;
            if (v->next == lines[i]->front) break; // closed polylines
        }
        lines[i]->selected = false;
    }

//    selected_point = false;
//    selected_point_p = NULL;
//
//    selected_line = false;
//    selected_line_p[0] = NULL;
//    selected_line_p[1] = NULL;
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
}

void Canvas::setHoverPoint(ofPoint p) {

    // check selection of points
    float min_d = FLT_MAX;
    Vertex *min_d_v = NULL;
    for (int i = 0; i < lines.size(); i++) {
        if (lines[i] == NULL) {
            continue;
        }
        for (Vertex *v = lines[i]->front; v != NULL; v = v->next) {
            float d0 = (p - *v).length();
            if (d0 < 2.0f && d0 < min_d) {
                min_d = d0;
                min_d_v = v;
            }
            if (v->next == lines[i]->front) break; // closed polylines
        }
    }
    if (min_d_v != NULL) {
        hover_point_p = min_d_v;
        hover_point = true;
        min_d_v->hover = true;
    }
}

void Canvas::setHover(ofPoint p) {

    setHoverPoint(p);

    for (int i = 0; i < lines.size(); i++) {
        if (lines[i] == NULL) {
            continue;
        }
        // check selection of line segments
        for (Vertex *v = lines[i]->front; v != NULL && v->next != NULL; v = v->next) {
            float d = segmentDistance(*v, *v->next, p);
            float d1 = (p - *v).length();
            float d2 = (p - *v->next).length();
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

    if (!hover_point && !hover_line) {
        for (int i = 0; i < lines.size(); i++) {
            if (lines[i] == NULL) {
                continue;
            }
            if (!lines[i]->closed) continue;
            lines[i]->updatePath();
            if (lines[i]->ofp.inside(getPx(p))) {
                hover_polygon = true;
                hover_polygon_p = lines[i];
            }
        }
    }
}