#include "ReaderOBJ.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>
#include <list>
#include <cstdlib>
#include <sstream>

using namespace std;

ReaderOBJ::ReaderOBJ(){}

void ReaderOBJ::OpenOBJ2D(string nome){

    ifstream input;
    input.open(nome, ios::in);
    if (!input)
    {
        cout << "Erro ao abrir " << nome << ". " << endl;
        exit(0);
    }
    cout << "Lendo arquivo " << nome << "...";

    input >> qtdCores;
    float id,r,g,b; // using int

    for (int i=0; i< qtdCores; i++)
    {
        input >> id >> r >> g >> b;
        Cor cor;
        cor.id = id;
        cor.R = r;
        cor.G = g;
        cor.B = b;
        Pallet.push_back(cor);
    }

    input >> l;
    input >> c;
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++) {
            input >> Matrix[i][j];
        }
    }

    input.close();
    cout << "leitura concluida." << endl;

}

void ReaderOBJ::OpenOBJ3D(string nome){

    float x = 0.f, y = 0.f, z = 0.f;

    ifstream input;

    input.open(nome, ios::in);
    if (!input)
    {
        cout << "Erro ao abrir " << nome << ". " << endl;
        exit(0);
    }
    string line;
    while (getline(input, line))
    {

        if (line.substr(0,2) == "v ")
        {
            float x = 0.f, y = 0.f, z = 0.f;
            stringstream ss(line.erase(0, 1));
			ss >> x >> y >> z;
            Ponto v = Ponto(x,y,z);
            verticesV.push_back(v);
        }
        if (line.compare(0, 2, "vt") == 0)
		{
            float x = 0.f, y = 0.f, z = 0.f;
			stringstream ss(line.erase(0, 2));
			ss >> x >> y;
            Ponto v = Ponto(x,y);
            texturasVT.push_back(v);
		}
        if (line.compare(0, 2, "vn") == 0)
		{
            float x = 0.f, y = 0.f, z = 0.f;
			stringstream ss(line.erase(0, 2));
			ss >> x >> y >> z;
			Ponto v = Ponto(x,y,z);
            normaisVN.push_back(v);
		}
        if (line.compare(0, 1, "f") == 0)
        {
            stringstream ss(line.erase(0, 2)); // 1/2/8 3/13/8 4/14/8
            string oneCorner;
            string v,t, n;
            
            vector<Ponto> tempFaces;
            
            while (getline(ss, oneCorner, ' ')){
                            
                stringstream oneCornerSS(oneCorner);    // 1/2/8
                getline(oneCornerSS, v, '/');
				getline(oneCornerSS, t, '/');
				getline(oneCornerSS, n, '/');

                //if(v == ""){ v=="0"; }
                if(t == ""){ t="0"; }
                //if(n == ""){ n=="0"; }

                int indexV = stoi(v)-1;//*3
                int indexT = stoi(t)-1;//*2
                int indexN = stoi(n)-1;//*3

                Ponto index = Ponto(indexV,indexT,indexN);
                tempFaces.push_back(index);
                
            }
            faceF.push_back(tempFaces);
        }
    }
}

//Necessario para calcular a colisao depois, então separasse aqui alguns pontos importantes aqui
void ReaderOBJ::VetoresMaxEMin(){

    Ponto min(INT_MAX,INT_MAX,INT_MAX);
    Ponto max(INT_MIN,INT_MIN,INT_MIN);

    for(int i=0; i< verticesV.size(); i++)
    {
        if (verticesV[i].x < min.x) min.x = verticesV[i].x;
        if (verticesV[i].y < min.y) min.y = verticesV[i].y;
        if (verticesV[i].z < min.z) min.z = verticesV[i].z;

        if (verticesV[i].x > max.x) max.x = verticesV[i].x;
        if (verticesV[i].y > max.y) max.y = verticesV[i].y;
        if (verticesV[i].z > max.z) max.z = verticesV[i].z;
    }

    vecMin = min;
    vecMax = max;

    raio.x = vecMax.x/2.0f;
    raio.y = vecMax.y/2.0f;
    raio.z = vecMax.z/2.0f;
}

