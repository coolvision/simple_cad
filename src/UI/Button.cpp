/*
 * Button.cpp
 *
 *  Created on: Nov 8, 2013
 *      Author: sk
 */

#include "Button.h"

ofxFontStash Button::font;

void Toolbar::draw() {
    for (int i = 0; i < buttons.size(); i++) {
        buttons[i]->draw();
    }
}

void Toolbar::updatePosition(float x, float y) {

    this->x = x;
    this->y = y;

    if (horizontal) {
        int start_x = x;
        int inc_x = button_w + margin;
        for (int i = 0; i < buttons.size(); i++) {
            buttons[i]->x = x + inc_x * i;
            buttons[i]->y = y;
        }
        width = buttons.size() * button_w + (buttons.size() - 1) * margin;
        height = button_h;
    } else {
        int start_y = y;
        int inc_y = button_h + margin;
        for (int i = 0; i < buttons.size(); i++) {
            buttons[i]->x = x;
            buttons[i]->y = y + inc_y * i;
        }
        width = button_w;
        height = buttons.size() * button_h + (buttons.size() - 1) * margin;;
    }
}

void Button::draw() {

    if (isMouseOver()) {
        hover = true;
    } else {
        hover = false;
    }

    ofFill();
    ofSetColor(color);
    if (hover || selected) {
        ofSetColor(color - ofColor(20));
    }
    ofRect(x, y, width, height);

    if (hover || selected) {
        ofSetColor(ofColor::orangeRed);
    } else {
        ofSetColor(ofColor::black);
    }
    icon.draw((int) (x + 3), (int) y + 3);

    ofPopStyle();
}

Button::Button(string label, int x, int y, int w, int h) {

    disableAllEvents();

    enableMouseEvents();

    selected = false;
    hover = false;
    dragging = false;
    draggable = false;

    radio = false;
    toggle = false;

    border_color.set(200.0f);
    hover_border_color.set(200.0f);

    color.set(255.0f);
    text_color.set(0);

    hover_color.set(50.0f);
    hover_text_color.set(ofColor::lightSteelBlue);

    this->label = label;
    this->x = x;
    this->y = y;
    width = w;
    height = h;
}

void Button::setDraggable(bool d) {
    this->draggable = d;
}

void Button::onPress(int x, int y, int button) {

    ButtonClickEventData d(this);
    ofNotifyEvent(click_event, d);

    dragging = true;
    drag_start.set(x - this->x, y - this->y);
}

void Button::onDragOver(int x, int y, int button) {

    onDragUpdate(x, y, button);
}

void Button::onDragOutside(int x, int y, int button) {

    onDragUpdate(x, y, button);
}

void Button::onDragUpdate(int x, int y, int button) {

    if (draggable && dragging) {
        this->x = x - drag_start.x;
        this->y = y - drag_start.y;
    }
}

void Button::onRelease(int x, int y, int button) {

    dragging = false;

    onReleaseAny(x, y, button);
}

void Button::onReleaseOutside(int x, int y, int button) {
    dragging = false;

    onReleaseAny(x, y, button);
}

void Button::onReleaseAny(int x, int y, int button) {

}

bool Button::hitTest(int tx, int ty) {
    return ((tx > x) && (tx < x + width) && (ty > y) && (ty < y + height));
}
