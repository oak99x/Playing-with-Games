// **********************************************************************
// PUCRS/Escola Polit�cnica
// COMPUTA��O GR�FICA
//
// Programa basico para criar aplicacoes 2D em OpenGL
//
// Marcio Sarroglia Pinho
// pinho@pucrs.br
// **********************************************************************

// Para uso no Xcode:
// Abra o menu Product -> Scheme -> Edit Scheme -> Use custom working directory
// Selecione a pasta onde voce descompactou o ZIP que continha este arquivo.
//

#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>
#include <list>
#include <cstdlib>


using namespace std;

#ifdef WIN32
#include <windows.h>
#include <glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include "Ponto.h"
#include "Modelo.h"
#include "Instancia.h"
#include "ListaDeCoresRGB.h"


#include "Temporizador.h"
Temporizador T;
double AccumDeltaT=0;

// Modelo Tiro;
// Modelo Nave;
// Modelo Inimigo;

// Limites l�gicos da �rea de desenho
Ponto Min, Max;

bool desenha = false;
float angulo=0.0;
bool animando = false;

int qtdPersonagens = 3;
int idPersonagem;
Modelo modelos[3];
string arquivos[] = {"teste.txt", "tiro2.txt", "ini2.txt"};

void DesenhaNave();
void DesenhaTiro();
void DesenhaInimigo();
void DesenhaPersonagem();
Ponto CalculaBezier3(Ponto PC[], double t);
void CriaCurvas(Ponto inicial, Ponto nave, Ponto final, Ponto PC[]);
Ponto geraPontos();


class TiroClass: public Instancia{  
    public:

    void Update(){
        //Anda para frente
        if(this->rotacao == 90){
            this->posicao = Ponto(this->posicao.x, this->posicao.y+1);
        }
        //Anda para esquerda
        if(this->rotacao == 180){
            this->posicao = Ponto(this->posicao.x-1, this->posicao.y);
        }
        //Anda para direita
        if(this->rotacao == 0){
            this->posicao = Ponto(this->posicao.x+1, this->posicao.y);
        }
        //Anda para tras
        if(this->rotacao == 270){
            this->posicao = Ponto(this->posicao.x, this->posicao.y-1);
        }
            
    }
};

list<TiroClass> tiros;

class PlayerClass: public Instancia{
    public:
    int vida = 3;

    void Movimenta(int x, int y, float r){
        if(this->posicao.x+x > Min.x && this->posicao.x+x < Max.x &&
           this->posicao.y+y > Min.y && this->posicao.y+y < Max.x){
            this->posicao = Ponto(this->posicao.x + x, this->posicao.y + y);
        }
        this->rotacao = r;  
    }

    void Atira(){
        //contadorTiros++;
        TiroClass t;
        t.posicao = this->posicao;
        t.rotacao = this->rotacao;
        //t.escala = Ponto (1,1);
        idPersonagem = 1;
        t.modelo = DesenhaPersonagem;
        tiros.push_back(t);
    } 

    void Vida(){
        vida = vida-1;
    } 
};

PlayerClass navePlayer;

class InimigoClass: public Instancia{
    public:

    Ponto Curva[3];

    void Curvas(){
        //CriaCurvas
        Curva[0] = this->posicao;
        Curva[1] = navePlayer.posicao;
        Curva[2] = geraPontos();
    }

    void Atira(){
        //nframes
        int valor = rand()%300;
        if(valor == 50){
            TiroClass t;
            t.posicao = this->posicao;
            t.rotacao = this->rotacao;
            //t.escala = Ponto (1,1);
            idPersonagem = 1;
            t.modelo = DesenhaPersonagem;
            tiros.push_back(t);
            valor = 0; 
        }             
    }

    void Update(){

        if(Curva[2].x == this->posicao.x && Curva[2].y == this->posicao.y){

            Curva[0] = this->posicao;
            Curva[1] = navePlayer.posicao;
            Curva[2] = geraPontos();
        }

        this->velo += 0.01;
        this->posicao = CalculaBezier3(Curva, this->velo);
        if(this->velo  > 1.0){
            this->velo = 0;
        }
    }
};

list<InimigoClass> inimigos;

//******************************************************************************
Ponto CalculaBezier3(Ponto PC[], double t)
{
    Ponto P;
    double UmMenosT = 1-t;

    P =  PC[0] * UmMenosT * UmMenosT + PC[1] * 2 * UmMenosT * t + PC[2] * t*t;
    return P;
}

Ponto geraPontos()
{
    Ponto P;
    /* gerando valores aleatórios com ponto flutuante*/
    P.x = Min.x + (rand() / (RAND_MAX / (Max.x - Min.x)));
    P.y = Min.y + (rand() / (RAND_MAX / (Max.y - Min.y)));

    return P;
}
//*******************************************************************************

void CarregaModelos()
{
    // Nave.leObjeto("teste.txt");
    // Tiro.leObjeto("tiro2.txt");
    // Inimigo.leObjeto("ini.txt");
    for(int i=0; i<qtdPersonagens;i++){
        modelos[i].leObjeto(arquivos[i]);
    }
    
}

//============================================================================================
//=============================================================================================

// void DesenhaNave()  // modelo - veio de arquivo
// {
//     glPushMatrix();
//         //glColor3f(0, 0, 1);
//         //glTranslatef(-1, -1, 0);
//         glScaled(0.6,0.6,0);
//         Nave.desenhaLinhas();
//     glPopMatrix();
// }

// void DesenhaTiro()  // modelo - veio de arquivo
// {
//     glPushMatrix();
//         glColor3f(0, 1, 1);
//         //glTranslatef(-1, -1, 0);
//         //glScaled(0.6,0.6,0);
//         Tiro.desenhaLinhas();
//     glPopMatrix();
// }

// void DesenhaInimigo()  // modelo - veio de arquivo
// {
//     glPushMatrix();
//         glColor3f(1, 0, 0);
//         //glTranslatef(-1, -1, 0);
//         glScaled(0.6,0.6,0);
//         Inimigo.desenhaLinhas();
//     glPopMatrix();
// }
void DesenhaPersonagem()  // modelo - veio de arquivo
{
    for(int i=0; i<qtdPersonagens;i++){
        glPushMatrix();
        glColor3f(1, 0, 0);
        //glTranslatef(-1, -1, 0);
        glScaled(0.5,0.5,0);
        modelos[idPersonagem].desenhaLinhas();
        glPopMatrix();
    }
}



// No trabalho, esta fun��o dever instanciar todos os
// personagens do cen�rio
void CarregaInstancias()
{
    navePlayer.posicao = Ponto (0,0);
    navePlayer.escala = Ponto (3,3);
    navePlayer.rotacao = 90;
    idPersonagem = 0;
    navePlayer.modelo = DesenhaPersonagem;

    int ang[] = {0,90,180,270};
    for(int i=0; i<3;i++)
    {  
        InimigoClass inimigo;
        inimigo.posicao = geraPontos();
        inimigo.rotacao = ang[rand()%4];
        inimigo.escala = Ponto (3,3);
        idPersonagem = 2;
        inimigo.modelo = DesenhaPersonagem;
        inimigo.Curvas();
        inimigos.push_back(inimigo);
    }
}

void ColisaoDeObjetos(){

}

void DesenhaCenario()
{

    //UpdateGeral
    idPersonagem = 0;
    navePlayer.desenha();
    
    if(!tiros.empty()){  
        for(int i=0;i<tiros.size();i++){
            glPushMatrix();
                TiroClass t = tiros.front();
                t.Update();
                idPersonagem = 1;
                t.desenha();
                tiros.push_back(t);
                tiros.pop_front();
            glPopMatrix();
	    }  
    }

    if(!inimigos.empty()){
        
        for(int i=0;i<inimigos.size();i++){
            glPushMatrix();
                InimigoClass in = inimigos.front();
                in.Atira();
                in.Update();
                idPersonagem = 2;
                in.desenha();
                inimigos.push_back(in);
                inimigos.pop_front();
            glPopMatrix();
	    } 
    }   
}
//==========================================================================================
//==========================================================================================
// **********************************************************************
//
// **********************************************************************
void init()
{
    // Define a cor do fundo da tela (AZUL)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    Min = Ponto(-50,-50);
    Max = Ponto(50,50);
    
    CarregaModelos();
    CarregaInstancias();
}

double nFrames=0;
double TempoTotal=0;
// **********************************************************************
//
// **********************************************************************
void animate()
{
    double dt;
    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;


    if (AccumDeltaT > 1.0/30) // fixa a atualiza��o da tela em 30
    {
        AccumDeltaT = 0;
        angulo+=1;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0)
    {
        cout << "Tempo Acumulado: "  << TempoTotal << " segundos. " ;
        cout << "Nros de Frames sem desenho: " << nFrames << endl;
        cout << "FPS(sem desenho): " << nFrames/TempoTotal << endl;
        TempoTotal = 0;
        nFrames = 0;
    }
}
// **********************************************************************
//  void reshape( int w, int h )
//  trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h )
{
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a area a ser ocupada pela area OpenGL dentro da Janela
    glViewport(0, 0, w, h);
    // Define os limites logicos da area OpenGL dentro da Janela
    glOrtho(Min.x,Max.x,
            Min.y,Max.y,
            -10,+10);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
// **********************************************************************
//
// **********************************************************************

#define LARG

// **********************************************************************
//  void display( void )
void DesenhaEixos()
{
    Ponto Meio;
    Meio.x = (Max.x+Min.x)/2;
    Meio.y = (Max.y+Min.y)/2;
    Meio.z = (Max.z+Min.z)/2;

    glBegin(GL_LINES);
    //  eixo horizontal
        glVertex2f(Min.x,Meio.y);
        glVertex2f(Max.x,Meio.y);
    //  eixo vertical
        glVertex2f(Meio.x,Min.y);
        glVertex2f(Meio.x,Max.y);
    glEnd();
}
//
// **********************************************************************
void display( void )
{
	// Limpa a tela coma cor de fundo
	glClear(GL_COLOR_BUFFER_BIT);

    // Define os limites l�gicos da �rea OpenGL dentro da Janela
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLineWidth(1);
	glColor3f(1,1,1); // R, G, B  [0..1]
    DesenhaEixos();

	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// Coloque aqui as chamadas das rotinas que desenham os objetos
    glLineWidth(2);
    DesenhaCenario();
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	glutSwapBuffers();
}
// **********************************************************************
// ContaTempo(double tempo)
//      conta um certo n�mero de segundos e informa quanto frames
// se passaram neste per�odo.
// **********************************************************************
void ContaTempo(double tempo)
{
    Temporizador T;

    unsigned long cont = 0;
    cout << "Inicio contagem de " << tempo << "segundos ..." << flush;
    while(true)
    {
        tempo -= T.getDeltaT();
        cont++;
        if (tempo <= 0.0)
        {
            cout << "fim! - Passaram-se " << cont << " frames." << endl;
            break;
        }
    }

}
// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
// **********************************************************************

void keyboard ( unsigned char key, int x, int y )
{

	switch ( key )
	{
		case 27:        // Termina o programa qdo
			exit ( 0 );   // a tecla ESC for pressionada
			break;
        case 't':
            ContaTempo(3);
            break;
        case 'z':
            desenha = !desenha;
        break;
        case 'f':       // Se pressionar UP
			glutFullScreen ( ); // Vai para Full Screen
			break;
	    case 'r':     // Se pressionar UP
								// Reposiciona a janela
            glutPositionWindow (50,50);
			glutReshapeWindow ( 700, 500 );
			break;
        case 32:{
            navePlayer.Atira();
        }break;
		default:
			break;
	}
}
// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
	switch ( a_keys )
	{
        case GLUT_KEY_UP:{
            navePlayer.Movimenta(0,2,90);   //anda para cima
        }break;
        case GLUT_KEY_DOWN:{
            navePlayer.Movimenta(0,-2,270); // anda para baixo
        }break;
        case GLUT_KEY_RIGHT:{
            navePlayer.Movimenta(2,0,0);    //anda para a direita
        }break;
        case GLUT_KEY_LEFT:{
            navePlayer.Movimenta(-2,0,180); //anda para a esqueda
        }break;
		default:
			break;
	}
}

// **********************************************************************
//  void main ( int argc, char** argv )
//
// **********************************************************************
int  main ( int argc, char** argv )
{
    srand(time(NULL));
    cout << "Programa OpenGL" << endl;

    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (0,0);

    // Define o tamanho inicial da janela grafica do programa
    glutInitWindowSize  ( 650, 500);

    // Cria a janela na tela, definindo o nome da
    // que aparecera na barra de t�tulo da janela.
    glutCreateWindow    ( "Primeiro Programa em OpenGL" );

    // executa algumas inicializa��es
    init ();

    // Define que o tratador de evento para
    // o redesenho da tela. A funcao "display"
    // ser� chamada automaticamente quando
    // for necess�rio redesenhar a janela
    glutDisplayFunc ( display );

    // Define que o tratador de evento para
    // o invalida��o da tela. A funcao "display"
    // ser� chamada automaticamente sempre que a
    // m�quina estiver ociosa (idle)
    glutIdleFunc(animate);

    // Define que o tratador de evento para
    // o redimensionamento da janela. A funcao "reshape"
    // ser� chamada automaticamente quando
    // o usu�rio alterar o tamanho da janela
    glutReshapeFunc ( reshape );

    // Define que o tratador de evento para
    // as teclas. A funcao "keyboard"
    // ser� chamada automaticamente sempre
    // o usu�rio pressionar uma tecla comum
    glutKeyboardFunc ( keyboard );

    // Define que o tratador de evento para
    // as teclas especiais(F1, F2,... ALT-A,
    // ALT-B, Teclas de Seta, ...).
    // A funcao "arrow_keys" ser� chamada
    // automaticamente sempre o usu�rio
    // pressionar uma tecla especial
    glutSpecialFunc ( arrow_keys );

    // inicia o tratamento dos eventos
    glutMainLoop ( );

    return 0;
}
