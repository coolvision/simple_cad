//
//  Joint.cpp
//  simple_cad
//
//  Created by sk on 10/23/14.
//
//

#include "Joint.h"

void Joint::draw() {

    if (isMouseOver()) {
        hover = true;
    } else {
        hover = false;
    }
    ofFill();
    ofColor color(255);
    ofSetColor(color);
    if (hover || selected) {
        ofSetColor(color - ofColor(20));
    }
    if (dragging) {
        ofSetColor(50);
    }

    ofRect(x, y, width, height);

}

Joint::Joint() {

    disableAllEvents();

    enableMouseEvents();

    selected = false;
    hover = false;
    dragging = false;
    draggable = true;

    width = 25;
    height = 25;
}

void Joint::setDraggable(bool d) {
    this->draggable = d;
}

void Joint::onPress(int x, int y, int button) {

//    JointClickEventData d(this);
//    ofNotifyEvent(click_event, d);

    dragging = true;
    drag_start.set(x - this->x, y - this->y);
}

void Joint::onDragOver(int x, int y, int button) {

    cout << "Joint::onDragOver " << endl;

    onDragUpdate(x, y, button);
}

void Joint::onDragOutside(int x, int y, int button) {

    cout << "Joint::onDragOutside " << endl;

    onDragUpdate(x, y, button);
}

void Joint::onDragUpdate(int x, int y, int button) {

    if (draggable && dragging) {
        this->x = x - drag_start.x;
        this->y = y - drag_start.y;
    }
}

void Joint::onRelease(int x, int y, int button) {

    dragging = false;

    onReleaseAny(x, y, button);
}

void Joint::onReleaseOutside(int x, int y, int button) {

    dragging = false;

    onReleaseAny(x, y, button);
}

void Joint::onReleaseAny(int x, int y, int button) {

}

bool Joint::hitTest(int tx, int ty) {
    return ((tx > x) && (tx < x + width) && (ty > y) && (ty < y + height));
}
