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
}

Vertex *Canvas::getVertex(ItemId v_id) {

    if (v_id.container_id < lines.size()) {
        return lines[v_id.container_id]->getVertex(v_id.item_id);
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

ofPoint Canvas::getPx(Vertex *v) {
    return getPx(*v);
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
        for (int i = 0; i < lines.size(); i++) {
            int n = lines[i]->getLength();
            if (n <= 0) continue;
            file << n << " ";
            file << lines[i]->closed << " ";
            for (Vertex *v = lines[i]->front; v != NULL; v = v->next) {
                file << v->x << " " << v->y << " ";
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

    int lines_n;
    int n;

    ifstream file(path.c_str());
    if (file.is_open()) {

        file >> lines_n;

        cout << "load " << lines_n <<  " lines" << endl;

        for (int i = 0; i < lines_n; i++) {
            lines.push_back(new Polyline());
            lines.back()->id = lines.size() - 1;
            file >> n;
            file >> lines.back()->closed;
            for (int i = 0; i < n; i++) {
                ofPoint p;
                file >> p.x;
                file >> p.y;
                lines.back()->addBack(p);
            }
            if (lines.back()->closed) {
                lines.back()->toPolygon();
            }
        }
        file.close();
    }

}