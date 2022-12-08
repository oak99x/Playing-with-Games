//
//  Modelo.hpp
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 22/09/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//

#ifndef Modelo_h
#define Modelo_h
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

//#include "Poligono.h"
//#include "Cor.h"
#include "Desenha.h"
#include "ReaderOBJ.h"
#include "Textura.h"
#include "Ponto.h"

#include "ListaDeCoresRGB.h"
#include <vector>

class Modelo
{
    public:
        ReaderOBJ reader;
        vector<string> nomeTexturas;
        Textura texturas[100];
        
        unsigned int cor;

        int id;
        Ponto posicao, escala;
        float rotacao;
        double velo;

        Modelo();
        void leObjeto3D(string nome);
        void leObjeto2D(string nome);
        void carregaTexturas();
        void desenha2d();
        void desenha3d();
};

#endif /* Modelo_hpp */
