//
//  Canvas.cpp
//  simple_card
//
//  Created by sk on 10/15/14.
//
//

#include "Canvas.h"

#include <iostream>
#include <fstream>

Canvas::Canvas() {

    curr_action_i = 0;

    lines.push_back(new JointsContainer());
    lines.back()->id = 0;
    lines.back()->z_index = 1;

    update_i = 0;
}

InteractiveObject *Canvas::getItem(ItemId item_id) {

//    cout << "Canvas::getItem " << item_id.container_id << " " << item_id.item_id << endl;
//    cout << "lines.size() " << lines.size() << endl;

    if (item_id.container_id < lines.size()) {
        return lines[item_id.container_id]->getItem(item_id.item_id);
    }
    return NULL;
}

void Canvas::addAction(Action *a) {

    resetActions();
    actions.push_back(a);
    actions.back()->doAction(this);
}

void Canvas::resetActions() {

    int n = 0;
    for (int i = 0; i < actions.size(); i++) {
        if (actions[i]->undo) n++;
    }
    for (int i = 0; i < n; i++) {
        delete actions.back();
        actions.pop_back();
    }
    curr_action_i = actions.size();
}

ofPoint Canvas::snap(ofPoint p) {

    if (px_step > 0) {
        p.x = round(p.x / px_step) * px_step;
        p.y = round(p.y / px_step) * px_step;
    }

    return p;
}

ofPoint Canvas::snapMm(ofPoint p) {

    p.x = round(p.x);
    p.y = round(p.y);

    return p;
}

ofPoint Canvas::getPx(ofPoint p) {

    // how many px per mm
    // points_step = 8.0f * zoom;
    p *= px_step;
    p += canvas_offset;

    return p;
}

ofPoint Canvas::getPx(InteractiveObject *v) {
    return getPx(v->p);
}

ofPoint Canvas::getMm(ofPoint p) {

    p -= canvas_offset;
    p /= px_step;
    
    return p;
}

void Canvas::zoomIn() {

    zoom += 0.2f;
    if (zoom > 5.0f) {
        zoom = 5.0f;
    }
}

void Canvas::zoomOut() {

    zoom -= 0.2f;
    if (zoom < 0.2f) {
        zoom = 0.2f;
    }
}

void Canvas::save(string path) {

    ofstream file(path.c_str());
    if (file.is_open()) {
        int lines_n = 0;
        for (int i = 0; i < lines.size(); i++) {
            int n = lines[i]->getLength();
            if (n > 0) lines_n++;
        }
        file << lines_n << endl;

        InteractiveContainer *l = lines[0];
        int i = 0;
        int n = l->getLength();
        file << n << " ";
        for (Joint *v = (Joint *)l->front; v != NULL; v = (Joint *)v->next) {
            file << v->p.x << " " << v->p.y << " " << v->joint_type << " ";
            if (v->next == lines[i]->front) break; // closed polylines
        }

        for (int i = 1; i < lines.size(); i++) {
            int n = lines[i]->getLength();
            if (n <= 0) continue;
            file << n << " ";
            file << lines[i]->closed << " ";
            for (InteractiveObject *v = lines[i]->front; v != NULL; v = v->next) {
                file << v->p.x << " " << v->p.y << " ";
                if (v->next == lines[i]->front) break; // closed polylines
            }
            file << endl;
        }
        file.close();
    }

}

void Canvas::load(string path) {

    cout << "load" << endl;

    for (int i = 0; i < lines.size(); i++) {
        lines[i]->release();
        delete lines[i];
    }
    lines.clear();

    lines.push_back(new JointsContainer());
    lines.back()->id = 0;
    lines.back()->z_index = 1;

    int lines_n;
    int n;

    ifstream file(path.c_str());
    if (file.is_open()) {

        file >> lines_n;

        InteractiveContainer *l = lines[0];
        int i = 0;
        file >> n;
        for (int i = 0; i < n; i++) {
            Joint v;
            file >> v.p.x;
            file >> v.p.y;
            file >> v.joint_type;
            lines[0]->addBack(&v);
        }

        for (i = 1; i < lines_n; i++) {
            lines.push_back(new Polyline());
            InteractiveContainer *l = lines.back();
            l->id = lines.size() - 1;
            file >> n;
            file >> l->closed;
            for (int i = 0; i < n; i++) {
                Vertex v;
                file >> v.p.x;
                file >> v.p.y;
                l->addBack(&v);
            }
            if (l->closed) {
                l->back->next = l->front;
                l->front->prev = l->back;
            }
            l->update();
        }
        file.close();
    }
    
}


int circle_circle_intersection(double x0, double y0, double r0,
                               double x1, double y1, double r1,
                               double *xi, double *yi,
                               double *xi_prime, double *yi_prime)
{
    double a, dx, dy, d, h, rx, ry;
    double x2, y2;

    /* dx and dy are the vertical and horizontal distances between
     * the circle centers.
     */
    dx = x1 - x0;
    dy = y1 - y0;

    /* Determine the straight-line distance between the centers. */
    //d = sqrt((dy*dy) + (dx*dx));
    d = hypot(dx,dy); // Suggested by Keith Briggs

    /* Check for solvability. */
    if (d > (r0 + r1))
    {
        /* no solution. circles do not intersect. */
        return 0;
    }
    if (d < fabs(r0 - r1))
    {
        /* no solution. one circle is contained in the other */
        return 0;
    }

    /* 'point 2' is the point where the line through the circle
     * intersection points crosses the line between the circle
     * centers.
     */

    /* Determine the distance from point 0 to point 2. */
    a = ((r0*r0) - (r1*r1) + (d*d)) / (2.0 * d) ;

    /* Determine the coordinates of point 2. */
    x2 = x0 + (dx * a/d);
    y2 = y0 + (dy * a/d);

    /* Determine the distance from point 2 to either of the
     * intersection points.
     */
    h = sqrt((r0*r0) - (a*a));

    /* Now determine the offsets of the intersection points from
     * point 2.
     */
    rx = -dy * (h/d);
    ry = dx * (h/d);

    /* Determine the absolute intersection points. */
    *xi = x2 + rx;
    *xi_prime = x2 - rx;
    *yi = y2 + ry;
    *yi_prime = y2 - ry;
    
    return 1;
}

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

ofPoint lineProjection(ofPoint v, ofPoint w, ofPoint p) {

    // Return minimum distance between line segment vw and point p
    float l2 = (v - w).lengthSquared();  // i.e. |w-v|^2 -  avoid a sqrt
    if (l2 == 0.0) {
        return p;   // v == w case
    }
    // Consider the line extending the segment, parameterized as v + t (w - v).
    // We find projection of point p onto the line.
    // It falls where t = [(p-v) . (w-v)] / |w-v|^2
    float t = (p - v).dot(w - v) / l2;

    return v + t * (w - v);
}