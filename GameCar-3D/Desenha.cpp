//
//  Modelo.cpp
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 22/09/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//
#ifndef Desenha_h
#define Desenha_h
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

//#include "Modelo.h"
#include "Ponto.h"
#include "Desenha.h"
#include "ReaderOBJ.h"
#include "Textura.h"

#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>
#include <list>
#include <cstdlib>

using namespace std;

Desenha::Desenha()
{

}

void DesenhaOBJ2D(ReaderOBJ reader, Textura *tex)
{

    
    // para ver se id da textura estava se perdendo
    // for(int i=0; i<13;i++){
    //     cout << "D " << tex[i].id << endl;
    // }
    //exit(0);

    int l = reader.l; // largura da matrix
    int c = reader.c; // comprimento da matrix

    int aux = -1; // auxiliar para passar o id da textura

    //desenha o piso linha por linha
    //inicio da linha até final da linha
    for (int i = 0; i < l; i++) {

        if(i!=0){
            //volta para o inicio, pula para linha de baixo
            glTranslatef ( -1, 0, -c ); 
        }

        for (int j = 0; j < c; j++) {

            int ob = reader.Matrix[i][j];
            aux = reader.Matrix[i][j];
            int idCor = 0;

            for (int k = 0; k < reader.Pallet.size(); k++){
                if(ob == reader.Pallet[k].id){
                    idCor = k;
                    break;
                }
            }
        
            float r = reader.Pallet[idCor].R;
            float g = reader.Pallet[idCor].G;
            float b = reader.Pallet[idCor].B;

    //         if((r == 0) && (g == 0) && (b == 0)){ //ignora preto
    //             continue;
    //         }

            // cores vindas da matrix.txt
            // aqui bastaria um glColor3f(r,g,b) e logo abaixo um tex[aux].Bind(), mas como a textura nao abre
            // na init e sim na display - não encontramos a solucao, se comentar a linha tex[aux].Bind() isso ira
            // deixar as cores corretas para cada quadrado do piso;
            if(aux == 0 || aux == 100 || aux == 150){
                glColor3f(r,g,b);
            }else{
                glColor3f(0,0,0);
            }

            //textura
            tex[aux].Bind();
            
            glBegin ( GL_QUADS );
                glNormal3f(0,1,0);

                glTexCoord2f(-1,-1); glVertex3f(-0.5, 0,-0.5);

                glTexCoord2f(-1, 0); glVertex3f(-0.5, 0, 0.5);

                glTexCoord2f(0, 0); glVertex3f( 0.5, 0, 0.5);

                glTexCoord2f(0,-1); glVertex3f( 0.5, 0,-0.5);
            glEnd();

            tex[aux].UnBind();

            //defineCor(corBorda);
            // glColor3f(0,0,0);
            // glBegin ( GL_LINE_STRIP );
            //     glNormal3f(0,1,0);
            //     glVertex3f(-0.5, 0,-0.5);
            //     glVertex3f(-0.5, 0, 0.5);
            //     glVertex3f( 0.5, 0, 0.5);
            //     glVertex3f( 0.5, 0,-0.5);
            // glEnd();

            //pula uma coluna para frente, ou seja, vai para o proximo quadrado
            glTranslatef (0, 0, 1 );
        }
    }
}

void DesenhaOBJ3D(ReaderOBJ reader, Textura *tex)
{
    vector<Ponto> verticesV     = reader.verticesV;
    vector<Ponto> texturasVT    = reader.texturasVT;
    vector<Ponto> normaisVN     = reader.normaisVN;
    vector<vector<Ponto>> faceF = reader.faceF;

    glPushMatrix();
        
        for(int i=0; i < faceF.size(); i++)
        { 

            for(int k=0; k < faceF[i].size(); k++)
            { 
                Ponto v = verticesV[faceF[i][k].x];
                Ponto n = normaisVN[faceF[i][k].z];
                Ponto t = texturasVT[faceF[i][k].y];
               
               //com esta sequencia consiguimos renderizar objetos OBJ na melhor forma possivel
                if(faceF[i].size() == 3){
                    glBegin(GL_TRIANGLES);
                    glNormal3f(n.x, n.y, n.z);
                    glTexCoord2f(t.x, t.y);
                    glVertex3f(v.x,v.y,v.z);
                }
                if(faceF[i].size() == 4){
                    glBegin(GL_QUADS);
                    glNormal3f(n.x, n.y, n.z);
                    glTexCoord2f(t.x, t.y);
                    glVertex3f(v.x,v.y,v.z);         
                }
                if(faceF[i].size() > 4){
                    glBegin(GL_POLYGON);
                    glNormal3f(n.x, n.y, n.z);
                    glTexCoord2f(t.x, t.y);
                    glVertex3f(v.x,v.y,v.z);         
                }

            }

            glEnd(); 

        }
    glPopMatrix();

}


#endif /* Desenha_hpp */