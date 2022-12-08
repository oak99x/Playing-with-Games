//
//  Modelo.hpp
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 22/09/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//

#ifndef Modelo_h
#define Modelo_h

#include "Poligono.h"
#include "Cor.h"

class Modelo: public Poligono
{
    public: vector<Cor> Pallet;
    unsigned int Objeto[100][100];
    unsigned int qtdCores;
    //int l,c; //linha coluna

public:
    Modelo();
    void leObjeto(string nome);
    void desenhaLinhas();   
};

#endif /* Modelo_hpp */
