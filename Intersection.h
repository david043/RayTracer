//
//  Intersection.h
//  RayTracer
//
//  Created by David Abensur on 21/03/2017.
//  Copyright © 2017 David Abensur. All rights reserved.
//

#ifndef Intersection_h
#define Intersection_h

#include "ObjectCSG.h"

class Intersection : public ObjectCSG
{
public:
    Intersection(ObjectCSG* a, ObjectCSG* b){
        this->a = a;
        this->b = b;
    }
    ObjectCSG* a;
    ObjectCSG* b;
    bool isInside(const Vector &P) const {return this->a->isInside(P) && this->b->isInside(P);}
    void intersect(const Ray & r , std::vector<IntersectpointsCSG *> & LI) const;
    
};

void Intersection::intersect(const Ray & r , std::vector<IntersectpointsCSG *> & LI) const{
    std::vector<IntersectpointsCSG *> LIa;
    std::vector<IntersectpointsCSG *> LIb;
    
    this->a->intersect(r, LIa);
    this->b->intersect(r, LIb);
    for (int i = 0; i < LIa.size(); ++i) {
        if (this->b->isInside(LIa[i]->P)) {
            LI.push_back(LIa[i]);
        }
        else {
            delete LIa[i];
        }
    }
    for (int i = 0; i < LIb.size(); ++i) {
        if (this->a->isInside(LIb[i]->P)) {
            LI.push_back(LIb[i]);
        }
        else {
            delete LIb[i];
        }
    }

}
#endif /* Intersection_h */
