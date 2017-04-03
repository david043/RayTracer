
#include <iostream>

#include "CImg.h"
#include <CMath>
#include "Vector.h"
#include "Scene.h"
#include "Sphere.h"
#include "SphereCSG.h"
#include "Union.h"
#include "Substraction.h"
#include "Intersection.h"
#include "CylinderCSG.h"
#include "BoxCSG.h"

#define NB_REBONDS 5


Vector getColor(const Scene &S, const Ray &R, int recursion)
{
    
    Vector N,P;
    int id;
    Vector finale(0.,0.,0.);
    Material material;
    
    if(S.intersect(R,P,N,id,material))
    {
    
         if(recursion == NB_REBONDS)
            {
                finale = finale+material.emissivity;
         }
        
        
        
        if(material.isDiffuse)
        {
            // Source ponctuelle
            
            /* Vector l = S.L-P;
             double distLight2 = l.squaredNorm();
             l.Normalize();
             
             double shadow_coeff = 1;
             Vector Pprime, Nprime;
             int idprime;
             Vector I = (S.L-P);
             I.Normalize();
             
             if(S.intersect(Ray(P+0.001*N,I),Pprime,Nprime,idprime))
             {
             if((Pprime-P).squaredNorm() < distLight2)
             {shadow_coeff = 0.;}
             }
             
             finale += shadow_coeff * std::max(0.,1000*dot(N,l)/distLight2)*S.objects[id]->color;
        */
            
            
            
            double eps = 0.001;
            Vector xP = P - dynamic_cast<Sphere*>(S.objects[0])->O;
            xP.Normalize();
            Vector rand_dir;
            rand_dir.random_cos(xP);
            Vector lightSourcePoint = rand_dir*dynamic_cast<Sphere*>(S.objects[0])->R + dynamic_cast<Sphere*>(S.objects[0])->O;
            Vector w_o = (lightSourcePoint-P);
            w_o.Normalize();
            double distLight2 = (P-lightSourcePoint).squaredNorm();
            double shadow_coef = 1.;
            Vector P2,N2;
            int idprime;
            Material materialprime;
            
            if(S.intersect(Ray(P+eps*N,w_o), P2, N2, idprime,materialprime))
            {
                Vector dist = P-P2;
                if(distLight2<dist.squaredNorm())
                {
                    shadow_coef = 0.;
                }
            }
            
             double pdf = std::max(0.001,dot(xP,rand_dir));
            double cos_thetai =std::max(0.,dot(N,w_o));
            finale += shadow_coef*cos_thetai/distLight2*dot(N2,-w_o)/pdf*material.color;
            
        
            if(recursion>0)
            {
                Vector rand_dir ;
                rand_dir.random_cos(N);
                Vector ind = getColor(S,Ray(P+0.001*N,rand_dir),recursion-1);
                finale += ind*material.color;
            }
            
            
        }
        
        // Si la surface est speculaire
        if(material.isSpecular)
        {
            if(recursion > 0)
            {
                Vector refl = R.u-2.*dot(R.u,N)*N;
                finale += getColor(S,Ray(P+1E-9*N,refl),recursion-1);
            }
        }
        
        // Si la surface est transparente, ex : Verre
        if(material.isTransparent)
        {
            if(recursion > 0)
            {
                double n1 = 1;
                double n2 = 1.3;
                bool is_refracted;
                Vector normale = N;
                if(dot(N,R.u)>0){
                    std::swap(n1,n2);
                    normale = -normale;
                }
                
                Vector refr = R.u.refract(normale,n1,n2,is_refracted);
                if(is_refracted)
                {
                    finale += getColor(S,Ray(P+1E-9*refr,refr),recursion-1);
                }
            }
        }
        
    }
    
    return finale;
}

int main(int argc, const char * argv[])
{
    
    //Dimensions de la fenêtre
    int W =  1024;
    int H = 1024;
    
    // Nombre de rayons
    int N = 30;
    
    
    std::vector<unsigned char> pixels(W*H*3,0);
    
    // Position de la caméra
    Vector C(0,10,100);
    Vector O(0,0,0);
    
    // Champ visuel
    double fov = 60*3.14/180;
    
    
    Scene S;
    Vector Light = Vector(-30,50,50);
    
    Material mat(Vector(1.,1.,1.),true,false,false);
    Material mat2(Vector(1.,0.5,0.),false,true,false);
    
    S.objects.push_back(new Sphere(Light,10, Material(Vector(0.,0.,0.),false,false,false,Vector(300.,300.,300.))));
    
    
    SphereCSG* sphereCSG = new SphereCSG(Vector(-3,10,25), 10, mat2);
    SphereCSG* sphereCSG2 = new SphereCSG(Vector(5,10,20), 10, mat2);
    CylinderCSG* cylCSG = new CylinderCSG(Vector(30,10,30),Vector(1,1,1), 7, mat2);
   // Substraction* substractionCSG = new Substraction(sphereCSG2, boxCSG);
    
   // S.objects.push_back(substractionCSG);
   //S.objects.push_back(cylCSG);

    // Maillage
    // S.objects.push_back(new Geometry("girl.obj"));

    
    S.objects.push_back(new Sphere(Vector(0,0,20),10,Material(Vector(1.,0.,0.),true,false,false)));
    
    // sphère derrière devant
    S.objects.push_back(new Sphere(Vector(0,0,-1000),940,Material(Vector(0.,1.,0.),true,false,false)));
    S.objects.push_back(new Sphere(Vector(0,0,1050),940,Material(Vector(1.,0.,0.),true,false,false)));
    
    // sphères gauche et droite
    S.objects.push_back(new Sphere(Vector(-1000,0,0),940,Material(Vector(0.,0.,1.),true,false,false)));
    S.objects.push_back(new Sphere(Vector(1000,0,0),940,Material(Vector(0.,0.,1.),true,false,false)));
    
    // sphères en bas et haut
    S.objects.push_back(new Sphere(Vector(0,-1000,0),990,Material(Vector(1.,1.,0.),true,false,false)));
    S.objects.push_back(new Sphere(Vector(0,1000,0),940,Material(Vector(1.,0.,1.),true,false,false)));
    
    
#pragma omp parallel for
    for(int i=0;i<H;i++)
    {
        std::cout<< (double)i/(double)H*100. << " % completed : ("<<i<<")\n";
        
        for(int j=0;j<W;j++)
        {
            
            Vector sum_int(0.,0.,0.);
            
            for(int k = 0;k<N;k++)
            {
                
                
                double r1 = distrib(engine);
                double r2 = distrib(engine);
                double x = sqrt(-2*log(r1))*cos((2.*M_PI*r2)*0.5) + j-W/2;
                double y = sqrt(-2*log(r1))*sin((2.*M_PI*r2)*0.5) +H-i-1 - H/2.;
                
                Vector u(x,y,-W/(2.*tan(fov/2.)));
                
                u.Normalize();
                
                // Sans profondeur de champ
                Vector intensity = getColor(S,Ray(C,u),5);
                
                //profondeur de champ
                // Création d'un nouveau vecteur décalé à l'origine mais vers le même point
                /*
                      Vector Pfocal = C + 80*u;
                 double r1bis = distrib(engine);
                 double r2bis = distrib(engine);
                 double xbis = sqrt(-2*log(r1bis))*cos((2.*M_PI*r2bis)*0.5);
                 double ybis = sqrt(-2*log(r1bis))*sin((2.*M_PI*r2bis)*0.5);
                 
                 Vector ubis = Pfocal - (C+Vector(xbis,ybis,0));
                 ubis.Normalize();
                 
                 Vector intensity = getColor(S,Ray(C+Vector(xbis,ybis,0),ubis),5);
                */
                
                
                sum_int+=intensity;
            }
            
            sum_int = sum_int*(1./N);
            
            
            //  correction gamma
            pixels[i*W+j] = std::min(255.,255*pow(sum_int[0],1./2/2));
            pixels[i*W+j+W*H] = std::min(255.,255*pow(sum_int[1],1./2/2));
            pixels[i*W+j + 2*W*H] = std::min(255.,255*pow(sum_int[2],1./2/2));
            
        }
    }
    
    cimg_library::CImg<unsigned char> cimg(&pixels[0],W,H,1,3);
    cimg.save("image.bmp");
    
    return 0;
    
    
    
}
