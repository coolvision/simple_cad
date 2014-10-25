//
//  Joint.cpp
//  simple_cad
//
//  Created by sk on 10/23/14.
//
//

#include "Joint.h"
#include "Canvas.h"

void Joint::draw() {

    if (isMouseOver()) {
        hover = true;
    } else {
        hover = false;
    }
//    ofFill();
//    ofColor color(255);
//    ofSetColor(color);
//    if (hover || selected) {
//        ofSetColor(color - ofColor(20));
//    }
//    if (dragging) {
//        ofSetColor(50);
//    }
//
//    ofRect(x, y, width, height);

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

    ofPoint icon_offset(13, 13);

    p = c->snapMm(c->getMm(ofPoint(this->x, this->y) + icon_offset));

    ofPoint p1 = c->getPx(p) - icon_offset;

    this->x = p1.x;
    this->y = p1.y;
}

bool Joint::hitTest(int tx, int ty) {
    return ((tx > x) && (tx < x + width) && (ty > y) && (ty < y + height));
}

void Joint::mouseDragged(int x, int y, int button) {

    onDragUpdate(x, y, button);

//    if(hitTest(x, y)) {
//        cout << "hitTest(x, y) " << endl;
//        // if mouse is over the object
//        if(!_isMouseOver) {						// if wasn't over previous frame
//            //				onPress(x, y);							// call onPress - maybe not
//            _isMouseOver = true;						// update flag
//            onRollOver(x, y);						// call onRollOver
//        }
//        onDragOver(x, y, button);				// and trigger onDragOver
//    } else {
//        cout << "!hitTest(x, y) " << endl;
//        if(_isMouseOver) {					// if mouse is not over the object, but the flag is true (From previous frame)
//            onRollOut();							// call onRollOut
//            _isMouseOver = false;						// update flag
//        }
//        cout << "isMousePressed " << isMousePressed(button) << " " << button << endl;
//        if(isMousePressed(button)) {
//            cout << "if(isMousePressed(button)) { " << endl;
//            onDragOutside(x, y, button);
//        }
//        _isMousePressed[button] = false;
//    }

}



