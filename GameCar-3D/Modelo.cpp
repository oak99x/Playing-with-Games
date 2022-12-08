//
//  Modelo.cpp
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 22/09/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//

#include "Modelo.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>
#include <list>
#include <cstdlib>

using namespace std;

Modelo::Modelo(){ 
    rotacao = 0;
    posicao = Ponto(0,0,0);
    escala = Ponto(1,1,1);
    velo = 0;
}

void Modelo::leObjeto2D(string nome)
{
    reader.OpenOBJ2D(nome);
}

void Modelo::leObjeto3D(string nome){
    reader.OpenOBJ3D(nome);
    reader.VetoresMaxEMin();
}

void Modelo::carregaTexturas(){

    for(int i=0; i<nomeTexturas.size(); i++){
        texturas[i].load(nomeTexturas[i]);
    }

}

void Modelo::desenha2d(){

    glPushMatrix();
    glTranslatef(posicao.x, posicao.y, posicao.z);
    glRotatef(rotacao, 0, 1, 0);
    glScalef(escala.x, escala.y, escala.z);
    DesenhaOBJ2D(reader, texturas);

    glPopMatrix();
     
}

void Modelo::desenha3d(){ 

    glPushMatrix();
        defineCor(cor);
        glTranslatef(posicao.x, posicao.y, posicao.z);
        glRotatef(rotacao, 0, 1, 0);
        glScalef(escala.x, escala.y, escala.z);
        DesenhaOBJ3D(reader, texturas);

    glPopMatrix();
}