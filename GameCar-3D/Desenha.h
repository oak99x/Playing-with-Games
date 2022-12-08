
#ifndef Desenha_hpp
#define Desenha_hpp
#include <iostream>
using namespace std;

#include "Ponto.h"
#include "ReaderOBJ.h"
#include "Textura.h"
#include <vector>
#include <list>

class Desenha{

vector<Ponto> verticesV;
vector<Ponto> texturasVT;
vector<Ponto> normaisVN;
vector<vector<Ponto>> faceF;

//vector<Textura> texturas;

public:
    Desenha();

};

void DesenhaOBJ2D(ReaderOBJ reader, Textura *tex);
void DesenhaOBJ3D(ReaderOBJ reader, Textura *tex);


#endif /* Desenha_hpp */