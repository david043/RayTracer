//
//  Substraction.h
//  RayTracer
//
//  Created by David Abensur on 20/03/2017.
//  Copyright © 2017 David Abensur. All rights reserved.
//

#ifndef Substraction_h
#define Substraction_h
#include "ObjectCSG.h"

class Substraction : public ObjectCSG
{
public:
    Substraction(ObjectCSG* a, ObjectCSG* b){
        this->a = a;
        this->b = b;
    };
    
    ObjectCSG* a;
    ObjectCSG* b;
    
    bool isInside(const Vector & P) const {return this->a->isInside(P) && !this->b->isInside(P);}
    void intersect(const Ray & r, std::vector<IntersectpointsCSG *> & IP) const;
};

void Substraction::intersect(const Ray & r, std::vector<IntersectpointsCSG *> & IP) const
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
        if (this->a->isInside(IPb[i]->P)) {
            Vector inverseNormale = (-1)*IPb[i]->N;
            IP.push_back(new IntersectpointsCSG(IPb[i]->P, inverseNormale, IPb[i]->M));
        }
        delete IPb[i];
    }

}


#endif /* Substraction_h */
