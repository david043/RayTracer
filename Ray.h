//
//  Ray.h
//  RayTracer
//
//  Created by David Abensur on 25/01/17.
//  Copyright © 2017 David Abensur. All rights reserved.
//

#ifndef Ray_h
#define Ray_h

#include "Vector.h"

class Ray{
public:
    
    Vector C;
    Vector u;
    
    Ray(Vector C,Vector u){
        this->C=C;
        this->u=u;
    }
    
};


#endif /* Ray_h */
