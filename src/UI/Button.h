/*
 * Button.h
 *
 *  Created on: Nov 6, 2013
 *      Author: sk
 */

#pragma once

#include "ofMain.h"
#include "ofxMSAInteractiveObject.h"
#include "ofxFontStash.h"

class Button;

class ButtonClickEventData {
public:
    ButtonClickEventData(Button *b): button_p(b) {};
    Button *button_p;
};

class Button: public ofxMSAInteractiveObject {
public:

    ofPath icon_path;

    Button(string label, int x, int y, int w, int h);

    static ofxFontStash font;

    string label;

    ofImage icon;

    // gui
    void onPress(int x, int y, int button);
    void onDragOver(int x, int y, int button);
    void onDragOutside(int x, int y, int button);
    void onRelease(int x, int y, int button);
    void onReleaseOutside(int x, int y, int button);
    void onDragUpdate(int x, int y, int button);
    virtual bool hitTest(int tx, int ty);
    void setDraggable(bool d);
    void onReleaseAny(int x, int y, int button);

    bool radio; // mode selection, like a radio button
    bool toggle;

    bool hover;
    bool selected;
    bool dragging;
    bool draggable;
    ofPoint drag_start;

    // events
    ofEvent<ButtonClickEventData> click_event;

    // visual stuff
    ofColor border_color;
    ofColor hover_border_color;
    ofColor color;
    ofColor text_color;
    ofColor hover_color;
    ofColor hover_text_color;

    void draw();
};

class Toolbar: public ofRectangle {
public:
    vector<Button *> buttons;
    void updatePosition(float x, float y);
    void draw();

    int button_h;
    int button_w;
    int margin;

    bool horizontal;
};
