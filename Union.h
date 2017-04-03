//
//  Union.h
//  RayTracer
//
//  Created by David Abensur on 20/03/2017.
//  Copyright © 2017 David Abensur. All rights reserved.
//

#ifndef Union_h
#define Union_h

#include "ObjectCSG.h"
#include "IntersectpointsCSG.h"

class Union:public ObjectCSG
{
public:
    Union(ObjectCSG* a, ObjectCSG* b)
    {
        this->a = a;
        this->b = b;
    };
    ObjectCSG* a;
    ObjectCSG* b;
    bool isInside (const Vector & P) const {return a->isInside(P) || b->isInside(P);}
    void intersect(const Ray & r, std::vector<IntersectpointsCSG *> & IP) const;
};


void Union::intersect(const Ray & r, std::vector<IntersectpointsCSG *> & IP) const
{
    std::vector<IntersectpointsCSG *> IPa;
    std::vector<IntersectpointsCSG *> IPb;
    
    this->a->intersect(r, IPa);
    this->b->intersect(r, IPb);
    for (unsigned int i = 0; i < IPa.size(); ++i) {
        if (!this->b->isInside(IPa[i]->P)) {
            IP.push_back(IPa[i]);
        }
        else {
            delete IPa[i];
        }
    }
    for (unsigned int i = 0; i < IPb.size(); ++i) {
        if (!this->a->isInside(IPb[i]->P)) {
            IP.push_back(IPb[i]);
        }
        else {
            delete IPb[i];
        }
    }

}
#endif /* Union_h */
