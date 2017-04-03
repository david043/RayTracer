//
//  Vector.h
//  RayTracer
//
//  Created by David Abensur on 18/01/17.
//  Copyright © 2017 David Abensur. All rights reserved.
//

#ifndef Vector_h
#define Vector_h

#include <CMath>
#include <Vector>
#include <random>

std::default_random_engine engine;
std::uniform_real_distribution <double> distrib(0,1);


class Vector
{
public:
    
    explicit Vector(double x = 0, double y = 0,double z = 0){
        xyz[0] = x;
        xyz[1] = y;
        xyz[2] = z;
    }
    
    Vector(const Vector& b){
        memcpy(xyz,b.xyz,3*sizeof(double));
    }
    
    double operator[](int i) const
    {
        return xyz[i];
    }
    
    double& operator[](int i)
    {
        return xyz[i];
    }
    
    double squaredNorm(){
        return xyz[0]*xyz[0]+xyz[1]*xyz[1]+xyz[2]*xyz[2];
    }
    
    void Normalize()
    {
        double n = sqrt(squaredNorm());
        xyz[0]/=n;
        xyz[1]/=n;
        xyz[2]/=n;
    }
    
    Vector& operator+=(const Vector& b){
        xyz[0]+=b[0];
        xyz[1]+=b[1];
        xyz[2]+=b[2];
        
        return *this;
    }
    
    Vector cross(const Vector& b) const
    {
        Vector res;
        
        res[0] = (*this)[1]*b[2] - (*this)[2]*b[1];
        res[1] = (*this)[2]*b[0] - (*this)[0]*b[2];
        res[2] = (*this)[0]*b[1] - (*this)[1]*b[0];
        
        return res;
    }
 
    
    Vector reflect(const Vector& N) const;
    Vector refract(const Vector& N, double n1,double n2, bool &is_refracted) const;
    Vector random_cos(const Vector& N);

    double xyz[3];
    
};



double sqr(double x){return x*x;}

inline Vector operator+ (const Vector& a,const Vector &b)
{
    return Vector(a[0]+b[0],a[1]+b[1],a[2]+b[2]);
}

inline Vector operator- (const Vector& a,const Vector &b)
{
    return Vector(a[0]-b[0],a[1]-b[1],a[2]-b[2]);
}

inline Vector operator* (const Vector& a,const double &b)
{
    return Vector(b*a[0],b*a[1],b*a[2]);
}

inline Vector operator* (const double &b,const Vector &a)
{
    return Vector(b*a[0],b*a[1],b*a[2]);
}

inline Vector operator* (const Vector &a,const Vector &b)
{
    return Vector(a[0]*b[0],a[1]*b[1],a[2]*b[2]);
}

inline Vector operator- (const Vector &a)
{
    return Vector(-a[0],-a[1],-a[2]);
}

double dot(const Vector& a,const Vector &b)
{
    return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}


// Réflection
Vector Vector::reflect(const Vector& N) const {
    return *this-2.*dot(*this,N)*N;
}

// Réfraction
Vector Vector::refract(const Vector& N, double n1,double n2, bool &is_refracted) const{
    
    double cos_theta_i = dot(*this,N);
    Vector Rt = ((*this)-cos_theta_i*N)*(n1/n2);
    double D = 1 - sqr(n1/n2)*(1-sqr(cos_theta_i));
    if(D<0)
    {
        is_refracted = false;
        return Vector(0.,0.,0.);
    }
    else
    {
        Vector Rn = -sqrt(D)*N;
        is_refracted = true;
        return Rt+Rn;
    }
}


Vector Vector::random_cos(const Vector& N) {
    
    double u = distrib(engine);
    double v = distrib(engine);
    
    double x = cos(2*M_PI*u)*sqrt(1-v);
    double y = sin(2*M_PI*u)*sqrt(1-v);
    double z = sqrt(v);

    
    Vector tan1;
    if(N[0] != 0 || N[1]!=0)
        tan1 = Vector(N[1],-N[0],0);
    else
        tan1 = Vector(0,N[2],-N[1]);

    tan1.Normalize();

    Vector tan2 = N.cross(tan1);
    
    // tan2 est orthogonal à la fois à N et tan1
    return x*tan1+y*tan2+z*N;
    
  
}


#endif /* Vector_h */
