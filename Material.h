//
//  Material.h
//  RayTracer
//
//  Created by David Abensur on 15/03/2017.
//  Copyright © 2017 David Abensur. All rights reserved.
//

#ifndef Material_h
#define Material_h
#include "Vector.h"

class Material
{
public:
    
    Material(Vector color = Vector(1.,1.,1.),bool isDiffuse = true, bool isSpecular = false,bool isTransparent = false,Vector emissivity = Vector(0.,0.,0.))
    {
        this->color = color;
        this->emissivity = emissivity;
        this->isDiffuse = isDiffuse;
        this->isSpecular = isSpecular;
        this->isTransparent = isTransparent;
    }
    
    Material(Material &m)
    {
        this->color = Vector(m.color);
        this->emissivity = Vector(m.emissivity);
        this->isDiffuse = m.isDiffuse;
        this->isSpecular = m.isSpecular;
        this->isTransparent = m.isTransparent;
    }
    
    Material(const Material &m){
        this->color = Vector(m.color);
        this->emissivity = Vector(m.emissivity);
        this->isDiffuse = m.isDiffuse;
        this->isSpecular = m.isSpecular;
        this->isTransparent = m.isTransparent;
    }
    
    Vector color;
    bool isDiffuse;
    bool isSpecular;
    bool isTransparent;
    Vector emissivity;

};

#endif /* Material_h */
