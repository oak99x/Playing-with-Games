
#ifndef ReaderOBJ_hpp
#define ReaderOBJ_hpp
#include <iostream>
using namespace std;

#include "Ponto.h"
#include <vector>
#include <climits>

class Cor
{
    public:
        int id;
        float R,G,B;
};

class ReaderOBJ{
public:
    //2d
    vector<Cor> Pallet;
    int qtdCores;
    int Matrix[100][100];
    int l,c;

    //3d
    vector<Ponto> verticesV;
    vector<Ponto> texturasVT;
    vector<Ponto> normaisVN;
    vector<vector<Ponto>> faceF;

    //Vetor MAX e MIN
    Ponto vecMax;
    Ponto vecMin;
    Ponto raio;

    ReaderOBJ();

    void OpenOBJ2D(string nome);
    void OpenOBJ3D(string nome);
    void VetoresMaxEMin();
};

#endif /* ReaderOBJ_hpp */