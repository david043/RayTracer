//
//  Scene.h
//  RayTracer
//
//  Created by David Abensur on 25/01/17.
//  Copyright © 2017 David Abensur. All rights reserved.
//

#ifndef Scene_h
#define Scene_h
#include "Vector.h"
#include "Ray.h"
#include "Geometry.h"

class Scene {
public:
    
    std::vector<Object*> objects;
    Vector L;
    
    Scene(){
    objects =std::vector<Object*>();
    }

    bool intersect(const Ray& r, Vector &P , Vector &N, int& id, Material& material) const{
        
        double mint = 1E9;
        bool result = false;
        
        for(int i=0;i<objects.size();i++){
            
            Vector currentP,currentN;
            double t;
            Material M1;

            if(objects[i]->intersect(r,currentP,t,currentN, M1)){
                
                result = true;
                if(t<mint){
                    mint = t;
                    P = currentP;
                    N = currentN;
                    id = i;
                    material = M1;
                }
            }
        }
        return result;
    }

    
};

#endif /* Scene_h */
