
#ifndef Textura_h
#define Textura_h
#include <iostream>
using namespace std;


#ifdef WIN32
#include <windows.h>
#include <glut.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include "stb_image.h"
#include<iostream>
#include <vector>

class Textura
{ 
    public:
        unsigned int id;
        int largura, altura, canais;

        Textura();
        void load(std::string filePath);
	    void Bind();
	    void UnBind();
};

#endif /* Textura_h */
