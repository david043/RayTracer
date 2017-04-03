//
//  Geometry.h
//  RayTracer
//
//  Created by David Abensur on 15/02/2017.
//  Copyright © 2017 David Abensur. All rights reserved.
//

#ifndef Geometry_h
#define Geometry_h

#include <iostream>
#include "Vector.h"
#include "Ray.h"
#include "BoundingBox.h"
#include "Material.h"
#include "Object.h"


class Geometry : public Object {
public:
    
    Geometry(const char* obj) {
        
        FILE* f = fopen(obj, "r");
        int curGroup = -1;
        while (!feof(f)) {
            char line[255];
            fgets(line, 255, f);
            if (line[0]=='u' && line[1]=='s') {
                curGroup++;
            }
            if (line[0]=='v' && line[1]==' ') {
                Vector vec;
                //sscanf(line, "v %lf %lf %lf\n", &vec[2], &vec[0], &vec[1]); // car
                sscanf(line, "v %lf %lf %lf\n", &vec[0], &vec[2], &vec[1]); // girl
                //sscanf(line, "v %lf %lf %lf\n", &vec[0], &vec[1], &vec[2]);  // dragon
                //vec[2] = -vec[2];
                vec[1] -=1.2;
                vertices.push_back(vec*30);
            }
            if (line[0]=='v' && line[1]=='n') {
                Vector vec;
                //sscanf(line, "vn %lf %lf %lf\n", &vec[2], &vec[0], &vec[1]); //car
                sscanf(line, "vn %lf %lf %lf\n", &vec[0], &vec[2], &vec[1]); //girl
                //vec[2] = -vec[2];
                normals.push_back(vec);
            }
            if (line[0]=='v' && line[1]=='t') {
                Vector vec;
                sscanf(line, "vt %lf %lf\n", &vec[0], &vec[1]);
                uvs.push_back(vec);
            }
            if (line[0]=='f') {
                int i0, i1, i2;
                int j0, j1, j2;
                int k0, k1, k2;
                faceGroup.push_back(curGroup);
                int nn = sscanf(line, "f %u/%u/%u %u/%u/%u %u/%u/%u\n", &i0, &j0, &k0, &i1, &j1, &k1, &i2, &j2, &k2);
                if (nn==9) {
                    faces.push_back(i0-1);
                    faces.push_back(i1-1);
                    faces.push_back(i2-1);
                    uvIds.push_back(j0-1);
                    uvIds.push_back(j1-1);
                    uvIds.push_back(j2-1);
                    normalIds.push_back(k0-1);
                    normalIds.push_back(k1-1);
                    normalIds.push_back(k2-1);
                } else {
                    int i3, j3;
                    nn = sscanf(line, "f %u/%u %u/%u %u/%u %u/%u\n", &i0, &j0, &i1, &j1, &i2, &j2, &i3, &j3); //le dragon contient des quads
                    if (nn == 8) {
                        faces.push_back(i0-1);
                        faces.push_back(i1-1);
                        faces.push_back(i2-1);
                        faces.push_back(i0-1);
                        faces.push_back(i2-1);
                        faces.push_back(i3-1);
                        faceGroup.push_back(curGroup);
                        uvIds.push_back(j0-1);
                        uvIds.push_back(j1-1);
                        uvIds.push_back(j2-1);
                        uvIds.push_back(j0-1);
                        uvIds.push_back(j2-1);
                        uvIds.push_back(j3-1);
                    } else {
                        nn = sscanf(line, "f %u/%u %u/%u %u/%u\n", &i0, &j0, &i1, &j1, &i2, &j2);
                        faces.push_back(i0-1);
                        faces.push_back(i1-1);
                        faces.push_back(i2-1);
                        uvIds.push_back(j0-1);
                        uvIds.push_back(j1-1);
                        uvIds.push_back(j2-1);
                    }
                }
                
            }
            
        }
        fclose(f);
        
        build_bbox();
    }
    
    
    void build_bbox(){
        
        bbox.bmin = Vector(1E99,1E99,1E99);
        bbox.bmax = Vector(-1E99,-1E99,-1E99);
        
        for(int i= 0; i<vertices.size();i++)
        {
            bbox.bmin[0] = std::min(bbox.bmin[0],vertices[i][0]);
            bbox.bmin[1] = std::min(bbox.bmin[1],vertices[i][1]);
            bbox.bmin[2] = std::min(bbox.bmin[2],vertices[i][2]);
            
            bbox.bmax[0] = std::max(bbox.bmax[0],vertices[i][0]);
            bbox.bmax[1] = std::max(bbox.bmax[1],vertices[i][1]);
            bbox.bmax[2] = std::max(bbox.bmax[2],vertices[i][2]);

        }
        
    
    }
    
    bool intersect(const Ray &r, int id, Vector &N, double &t ) const
    {
        const Vector &A = vertices[faces[id*3]];
        const Vector &B = vertices[faces[id*3+1]];
        const Vector &C = vertices[faces[id*3+2]];
    
        N = (C-A).cross(B-A);
        N.Normalize();
        
        t = dot(N,A-r.C)/dot(N,r.u);
        
        if(t<=0) return false;
        
        Vector P = r.C +t*r.u;
        
        Vector u = B-A;
        Vector v = C-A;
        Vector w = P-A;
        
        // Compute dot products
        double uu = dot(u, u);
        double uv = dot(u, v);
        double uw = dot(u, w);
        double vv = dot(v, v);
        double vw = dot(v,w);
        
        // Compute barycentric coordinates
        double detM =uu*vv - uv*uv;
        double alpha = (uw*vv - vw*uv)/detM;
        double beta = (uu*vw - uv*uw)/detM;
        
        // Check if point is in triangle
        if (alpha >= 0 && beta >= 0 && alpha+beta<=1)
        { return true;
        }
      
        return false;
        
    }
    
    bool intersect(const Ray &r, Vector& P, double &t, Vector& N, Material& M) const
    {
        
        if(!bbox.intersect(r)) {return false;}
        
        
        
        t = 1E99;
        bool has_intersection = false;
        
        for(int i = 0;i<faces.size()/3;i++)
        {
            double current_t;
            Vector current_n;
            
            if(intersect(r,i,current_n,current_t))
            {
                has_intersection = true;
                if(current_t < t)
                {
                    t = current_t;
                    N = current_n;
                    P = r.C +t*r.u;
                }
            }
        }
        
        M = material;
        return has_intersection;
    }
    
    std::vector<int> faceGroup;
    std::vector<int> faces;
    std::vector<int> normalIds;
    std::vector<int> uvIds;
    std::vector<Vector> vertices;
    std::vector<Vector> normals;
    std::vector<Vector> uvs; // Vector en 3D mais on n'utilise que 2 composantes
    
    BoundingBox bbox;
};
#endif /* Geometry_h */
