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

// vector<Cor> Pallet;
// unsigned int Objeto[100][100];
// unsigned int qtdCores;
// unsigned int l,c; //linha coluna
int l,c; //linha coluna

Modelo::Modelo()
{
    
}

// struct Cor
// {
//     float R, G, B;
// }cor;

void Modelo::leObjeto(string nome)
{
    ifstream input;
    input.open(nome, ios::in);
    if (!input)
    {
        cout << "Erro ao abrir " << nome << ". " << endl;
        exit(0);
    }
    cout << "Lendo arquivo " << nome << "...";

    input >> qtdCores;
    float n,r,g,b; // using int

    for (int i=0; i< qtdCores; i++)
    {
        input >> n >> r >> g >> b;
        Cor cor;
        cor.R = r;
        cor.G = g;
        cor.B = b;
        Pallet.push_back(cor);
    }

    input >> l >> c;

    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++) {
            input >> Objeto[i][j];
        }
    }

    input.close();
    cout << "leitura concluida." << endl;

}

void Modelo::desenhaLinhas()
{
    
    float c1= l/2;
	float c2= c/2;

    for (int i = 0; i < l; i++) {

        if(i!=0){
            glTranslatef ( -1, -c, 0.0f );
        }

        for (int j = 0; j < c; j++) {

            int ob = Objeto[i][j]-1;

            float r = Pallet[ob].R;
            float g = Pallet[ob].G;
            float b = Pallet[ob].B; 

            glColor3f(r,g,b);
            glTranslatef (0, 1, 0.0f );
            glBegin(GL_QUADS);
                glVertex2d(0-c2, 0-c1*1.5);
                glVertex2d(0-c2, 1-c1*1.5);
                glVertex2d(1-c2, 1-c1*1.5);
                glVertex2d(1-c2, 0-c1*1.5);
            glEnd();
        }
    }
}