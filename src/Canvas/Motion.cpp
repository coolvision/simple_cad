//
//  Motion.cpp
//  simple_cad
//
//  Created by sk on 11/13/14.
//
//

//#include "InteractiveObject.h"
//#include "Canvas.h"
//
////==============================================================================
//
//void UpdateRelative::apply(InteractiveContainer *c, Canvas *canvas) {
//
//    c->updated[type_i] = sent_stamp;
//
//    for (int m = 0; m < c->links.size(); m++) {
//        Joint *j = (Joint *)canvas->lines[0]->getItem(c->links[m]);
//
//        if (j->id == receiver_id) {
//
//            ofPoint shift = j->p - (c->p + c->links_rel[m]);
//            total_motion += shift.length();
//
//            for (InteractiveObject *v = c->front; v != NULL; v = v->next) {
//                v->p += shift;
//                if (v->next == c->front) break; // closed polylines
//            }
//
//            c->p = c->p + shift;
//
////            // rotate by the angle difference
////            float diff = j->angle - c->angle;
////            for (InteractiveObject *v = c->front; v != NULL; v = v->next) {
////                v->p.rotate(diff, j->p, ofPoint(0.0f, 0.0f, 1.0f));
////                if (v->next == c->front) break; // closed polylines
////            }
////            c->p.rotate(diff, j->p, ofPoint(0.0f, 0.0f, 1.0f));
////
////            for (int m = 0; m < c->links_rel.size(); m++) {
////                c->links_rel[m].rotate(diff, ofPoint(), ofPoint(0.0f, 0.0f, 1.0f));
////
////            }
////
////            c->angle = j->angle;
//
////            for (int m = 0; m < c->links.size(); m++) {
////                Joint *j = (Joint *)canvas->lines[0]->getItem(c->links[m]);
////                j->p = c->p + c->links_rel[m];
////            }
//            break;
//        }
//    }
//};
//
//
//void UpdateRelative::apply(InteractiveObject *i, Canvas *canvas) {
//
//    i->updated[type_i] = sent_stamp;
//
//    bool fixed = false;
//
//    vector <InteractiveContainer *> connected;
//    vector<ofPoint *> connected_rel;
//    canvas->getConnectedPolygons(i->id, &connected, &connected_rel);
//
//    for (int j = 0; j < connected.size(); j++) {
//
//        if (connected[j]->id == receiver_id) {
//
//            total_motion += ((connected[j]->p + *connected_rel[j]) - i->p).length();
//
////            i->angle = connected[j]->angle;
////            i->angle_slider = connected[j]->angle;
//
//            i->p = connected[j]->p + *connected_rel[j];
//        }
//    }
//};
//
//Motion *UpdateRelative::getCopy() {
//    UpdateRelative *j = new UpdateRelative();
//    *j = *this;
//    return (Motion *)j;
//}
//
////==============================================================================
//
//void Rotation::apply(InteractiveContainer *c, Canvas *canvas) {
//
//    c->updated[type_i] = sent_stamp;
//
//    if (c->fixed) {
//        return;
//    }
//
//    for (int m = 0; m < c->links.size(); m++) {
//        Joint *j = (Joint *)canvas->lines[0]->getItem(c->links[m]);
//
//        if (j->id == receiver_id) {
//
//            ofPoint shift = j->p - (c->p + c->links_rel[m]);
//            total_motion += shift.length();
//
//            for (InteractiveObject *v = c->front; v != NULL; v = v->next) {
//                v->p += shift;
//                if (v->next == c->front) break; // closed polylines
//            }
//
//            c->p = c->p + shift;
//
//            // rotate by the angle difference
//            float diff = j->angle - c->angle;
//            for (InteractiveObject *v = c->front; v != NULL; v = v->next) {
//                v->p.rotate(diff, j->p, ofPoint(0.0f, 0.0f, 1.0f));
//                if (v->next == c->front) break; // closed polylines
//            }
//            c->p.rotate(diff, j->p, ofPoint(0.0f, 0.0f, 1.0f));
//
//            for (int m = 0; m < c->links_rel.size(); m++) {
//                c->links_rel[m].rotate(diff, ofPoint(), ofPoint(0.0f, 0.0f, 1.0f));
//            }
//
//            c->angle = j->angle;
//
////            for (int m = 0; m < c->links.size(); m++) {
////                Joint *j = (Joint *)canvas->lines[0]->getItem(c->links[m]);
////                j->p = c->p + c->links_rel[m];
////            }
//            break;
//        }
//    }
//
//}
//
//void Rotation::apply(InteractiveObject *i, Canvas *canvas) {
//
//    i->updated[type_i] = sent_stamp;
//
//    bool fixed = false;
//
//    vector <InteractiveContainer *> connected;
//    vector<ofPoint *> connected_rel;
//    canvas->getConnectedPolygons(i->id, &connected, &connected_rel);
//
//    for (int j = 0; j < connected.size(); j++) {
//
//        if (connected[j]->id == receiver_id) {
//
//            total_motion += ((connected[j]->p + *connected_rel[j]) - i->p).length();
//
//            i->p = connected[j]->p + *connected_rel[j];
//
//            i->angle = connected[j]->angle;
//            i->angle_slider = connected[j]->angle;
//
//        }
////        else if (connected[j]->fixed) {
////
////            i->p = connected[j]->p + *connected_rel[j];
////        }
//
//    }
//}
//
//Motion *Rotation::getCopy() {
//
//    Rotation *j = new Rotation();
//    *j = *this;
//    return (Motion *)j;
//}
//
////==============================================================================
//
//void FitRelative::apply(InteractiveContainer *c, Canvas *canvas) {
//
//    c->updated[type_i] = sent_stamp;
//
//    if (c->controlled) {
//        return;
//    }
//
//    for (int m = 0; m < c->links.size(); m++) {
//        Joint *j = (Joint *)canvas->lines[0]->getItem(c->links[m]);
//
//        if (j->id == receiver_id) {
//
//            ofPoint shift = j->p - (c->p + c->links_rel[m]);
//            total_motion += shift.length();
//            for (InteractiveObject *v = c->front; v != NULL; v = v->next) {
//                v->p += shift;
//                if (v->next == c->front) break; // closed polylines
//            }
//            c->p = c->p + shift;
//        }
//    }
//
//    Joint *fitted = NULL;
//    Joint *off = NULL;
//
//    ofPoint target_p;
//    c->pivot_i++;
//    if (c->pivot_i >= c->links.size()) {
//        c->pivot_i = 0;
//    }
//    int p = 0;
//    for (int m = 0; m < c->links.size(); m++) {
//        Joint *j = (Joint *)canvas->lines[0]->getItem(c->links[m]);
//
//        // find a joint with a difference, and a joint without a difference
//        ofPoint shift = j->p - (c->p + c->links_rel[m]);
//        total_motion += shift.length();
//        if (shift.length() < 0.1) {
//            //if (p == c->pivot_i) {
//                fitted = j;
//                j->label = "fitted";
//            //}
//        } else {
//            off = j;
//            target_p = c->p + c->links_rel[m];
//            j->label = "off";
//        }
//        p++;
//    }
//
//    if (fitted != NULL && off != NULL) {
////
////        // now find a polygon to which "off" joint is connected
////        Joint *fitted2 = NULL;
////        ofPoint off2_p;
////        InteractiveContainer *c2 = NULL;
////
////        vector <InteractiveContainer *> connected;
////        vector<ofPoint *> connected_rel;
////        canvas->getConnectedPolygons(off->id, &connected, &connected_rel);
////        for (int j = 0; j < connected.size(); j++) {
////            c2 = connected[j];
////            if (c2->id != c->id) {
////                for (int m = 0; m < c2->links.size(); m++) {
////                    Joint *j = (Joint *)canvas->lines[0]->getItem(c2->links[m]);
////                    // find a joint without a difference
////                    ofPoint shift = j->p - (c2->p + c2->links_rel[m]);
////                    if (shift.length() < 0.001 && fitted2 == NULL) {
////                        fitted2 = j;
////                        fitted2->label = "fitted2";
////                    }
////                    if (j->id == off->id) {
////                        off2_p = c2->p + c2->links_rel[m];
////                    }
////                }
////                break;
////            }
////        }
//
////        if (c2 != NULL && fitted2 != NULL) {
////
////            double xi, yi, xi_prime, yi_prime;
////
////            int res = circle_circle_intersection(fitted->p.x, fitted->p.y,
////                                           (fitted->p - off->p).length(),
////                                           fitted2->p.x, fitted2->p.y,
////                                           (fitted2->p - off2_p).length(),
////                                           &xi, &yi, &xi_prime, &yi_prime);
////
////            target_p.set(xi, yi);
////
////            ofPoint v1 = off2_p - fitted2->p;
////            ofPoint v2 = target_p - fitted2->p;
////
////            fitted2->v1 = v1;
////            fitted2->v2 = v2;
////            fitted2->fit_angle = v1.angle(v2);
////
////            cout << "v1 " << v1.x << " " << v1.y << endl;
////            cout << "v2 " << v2.x << " " << v2.y << endl;
////
////            float angle = atan2(v1.y,v1.x) - atan2(v2.y,v2.x);
////
////            for (InteractiveObject *v = c2->front; v != NULL; v = v->next) {
////                v->p.rotateRad(angle, fitted2->p, ofPoint(0.0f, 0.0f, 1.0f));
////                if (v->next == c2->front) break; // closed polylines
////            }
////
////            c2->p.rotateRad(angle, fitted2->p, ofPoint(0.0f, 0.0f, 1.0f));
////
////            for (int m = 0; m < c2->links_rel.size(); m++) {
////                c2->links_rel[m].rotateRad(angle, ofPoint(), ofPoint(0.0f, 0.0f, 1.0f));
////            }
////
////
////
////        } else {
//
//            ofPoint v1 = off->p - fitted->p;
//            ofPoint v2 = target_p - fitted->p;
//
//            fitted->v1 = v1;
//            fitted->v2 = v2;
//            fitted->fit_angle = v1.angle(v2);
//
//            cout << "v1 " << v1.x << " " << v1.y << endl;
//            cout << "v2 " << v2.x << " " << v2.y << endl;
//
//            float angle = atan2(v1.y,v1.x) - atan2(v2.y,v2.x);
//
//            for (InteractiveObject *v = c->front; v != NULL; v = v->next) {
//                v->p.rotateRad(angle, fitted->p, ofPoint(0.0f, 0.0f, 1.0f));
//                if (v->next == c->front) break; // closed polylines
//            }
//
//            c->p.rotateRad(angle, fitted->p, ofPoint(0.0f, 0.0f, 1.0f));
//
//            for (int m = 0; m < c->links_rel.size(); m++) {
//                c->links_rel[m].rotateRad(angle, ofPoint(), ofPoint(0.0f, 0.0f, 1.0f));
//            }
//            
//            total_motion += (v1 - v2).length();
////        }
//    }
//
//    for (int m = 0; m < c->links.size(); m++) {
//        Joint *j = (Joint *)canvas->lines[0]->getItem(c->links[m]);
//        total_motion += (j->p - (c->p + c->links_rel[m])).length();
//        j->p = c->p + c->links_rel[m];
//    }
//
//
//};
//
//
//void FitRelative::apply(InteractiveObject *i, Canvas *canvas) {
//
//    i->updated[type_i] = sent_stamp;
//    
//};
//
//Motion *FitRelative::getCopy() {
//    FitRelative *j = new FitRelative();
//    *j = *this;
//    return (Motion *)j;
//}
//
////==============================================================================
//
//void LinearMotion::apply(InteractiveContainer *c, Canvas *canvas) {
//    for (InteractiveObject *v = c->front; v != NULL; v = v->next) {
//        apply(v, canvas);
//        if (v->next == c->front) break; // closed polylines
//    }
//};
//
//void LinearMotion::apply(InteractiveObject *i, Canvas *canvas) {
//    i->p += v;
//};
//
//Motion *LinearMotion::getCopy() {
//
//    LinearMotion *j = new LinearMotion();
//    *j = *this;
//    return (Motion *)j;
//}

