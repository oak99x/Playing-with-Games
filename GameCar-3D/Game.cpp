// **********************************************************************
// PUCRS/Escola Polit�cnica
// COMPUTACAO GRAFICA
//
// Fundamentos de Computacao Grafica, aplicacoes 3D em OpenGL
//
// Marcio Sarroglia Pinho
// pinho@pucrs.br
//
// ALUNOS:
// Mateus de Carvalho de Freitas (mateus.carvalho99@edu.pucrs.br)
// Giorgia Coan Marques (giorgia.marques@edu.pucrs.br)
//
// **********************************************************************

#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>
#include <list>
#include <cstdlib>
#include <vector>
#include <string.h>


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
#include "Camera.h"
#include "Temporizador.h"
#include "ListaDeCoresRGB.h"
#include "Textura.h"

// **********************************************************************

Temporizador T;
double AccumDeltaT=0;
GLfloat AspectRatio, angulo=0;
int ModoDeProjecao = 1;
int ModoDeExibicao = 1;
double nFrames=0;
double TempoTotal=0;

Ponto MinMap;
Ponto MaxMap;

bool desenha  = false;
bool animando = false;
bool anda = false;

int pontuacao;
int combustivel  = 10;
float dBarra = 0.5;
float distancia  = 0;
float velocidade = 1.0f;

int qtdPersonagens = 5;
int qtdPredios     = 100;
int qtdArvores     = 100;
int qtdGas         = 30;
int qtdRespawn     = 2;

int respawn;

Modelo modelos[5];
string arquivos[] = {"Piso.txt", "Car2.obj", "Cube.obj", "Tree.obj", "Gas.obj"};
vector<vector<string>> texturas = { 
//Piso
{"Texturas/Grama/Grama.png", "Texturas/Asfalto/CROSS.png", "Texturas/Asfalto/DR.png", "Texturas/Asfalto/DLR.png",
 "Texturas/Asfalto/DL.png", "Texturas/Asfalto/LR.png", "Texturas/Asfalto/None.png",  "Texturas/Asfalto/UD.png",
 "Texturas/Asfalto/UDR.png", "Texturas/Asfalto/UDL.png", "Texturas/Asfalto/UR.png", "Texturas/Asfalto/ULR.png",
 "Texturas/Asfalto/UL.png"
 }
 };

Camera camera1(Ponto(0,0,0)); // 1 pessoa
Camera camera2(Ponto(0,0,0)); // 3 pessoa
Camera cameraAuxiliar(Ponto(0,0,0)); //para rotações horizontais e verticais
int idcamera       = 2; //id das camera
int ultimaCamera   = 0; //guarda o ultimo id da camera
float alturaCamera = 1.4;
float distaciaCamera3P = 10;

Camera lastPos(Ponto(0,0,0)); // guarda ultima posição da camera/carro em caso de colisão
Camera tempPos(Ponto(0,0,0)); // movimentar a camera sem movimentar o carro

//Personagens
Modelo Piso;
vector<Modelo> Predios;
vector<Modelo> Arvores;
vector<Modelo> Gas;

//identificador dos objetos na matrix
vector<int> prediosIdMatrix = {100, 150};
vector<int> arvoresIdMatrix = {0};
vector<int> gasIdMatrix     = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

void resetaCameraAuxiliar();
void AndaAutomatico();

class PlayerClass: public Modelo{
    public:

    void Movimenta(float x, float y, float z, float r){

        //limiatse dentro do piso
        //if( x<=MinMap.x && x>=MaxMap.x && z>=MinMap.z && z<=MaxMap.z){
        //limita-se só andar nas ruas
        //sobre o piso temos a aplicacao de uma escala e para tratar as posicoes de forma correta 
        //retira-se a escala, fazendo voltar para seu tamanho original.
        // 0.6 e 0.4 servem para chegar no limite das laterais da estrada//meio fio
        if( Piso.reader.Matrix[(int)(((x/-10.0)+0.6f))][((int)(((z/10.0))+0.6f))] != 0  && 
            Piso.reader.Matrix[(int)(((x/-10.0)+0.4f))][((int)(((z/10.0))+0.4f))] != 0 )
        {
            
            this->rotacao = this->rotacao + r;
            this->posicao = Ponto(x, y-alturaCamera, z);
            
            distancia += (velocidade*TempoTotal);

        }else{
            anda = false;
            camera1.m_pos = lastPos.m_pos;
        }
            
    }

    void Combustivel(){

        if(combustivel == 0){
            cout << "**************** GAME OVER ****************" << endl;
            exit(0);
        }

        if(distancia > 50){
            //combustivel+=- 1;
            dBarra += -0.05; //0.05 -> dBarra/combustivel == 0.5/10
            distancia = 0;
        }
    }
};

PlayerClass Carro;

void PosicaoNaMatrix(vector<Modelo> &personagens, int qtd, vector<int> &idMatrix){
    
    //nao quero personagens no mesmo quadrado
    //lista temporaria das posicoes que ja foram
    vector<Ponto> randomPos;

    for(int i=0; i < qtd; i++){

        bool continua = true;
        bool jaEsta = false;

        int posX = rand()%Piso.reader.l;
        int posZ = rand()%Piso.reader.c;
        
        while (continua)
        {
            posX = rand()%Piso.reader.l;
            posZ = rand()%Piso.reader.c;

            //verifica se ja posicao ja foi sorteada ou nao
            for (int k = 0; k < randomPos.size(); k++)
            {   

                if(randomPos.empty()){ break; }

                if(randomPos[k].x == posX && randomPos[k].z == posZ){
                    jaEsta = true;
                    break;
                }else{
                    jaEsta = false;
                }  
            }

            if(jaEsta == false){
                //Percorre matrix buscando a posicao com id correto
                for(int k=0; k<idMatrix.size(); k++){
                    if(Piso.reader.Matrix[posX][posZ] == idMatrix[k]){
                    
                        //a escala no eixo y eh para ficar alinhado com o chão
                        //a multiplicação da posicao pela escala e para cair no quadrado correto, ficando no centro
                        personagens[i].posicao = Ponto( -(posX*Piso.escala.x), (personagens[i].escala.y) , (posZ*Piso.escala.z));
                        randomPos.push_back(Ponto( posX, 0, posZ ));
                        continua = false;
                        break;
                    }
                }
            }
           
        }
    }   
}

void CarregaModelos()
{
    for(int i=0; i<qtdPersonagens;i++){
        
        if(arquivos[i].find("obj") != string::npos){
            modelos[i].leObjeto3D("Models/"+arquivos[i]);
            modelos[i].id = i;
        }else{
            modelos[i].leObjeto2D("Models/"+arquivos[i]);
            modelos[i].id = i;
        }
        
    }
}

void CarregaInstancias()
{

    camera1.m_pos = Ponto (-110,alturaCamera,230);
    
    Piso = modelos[0];
    Piso.escala = Ponto(10,0,10);
    Piso.nomeTexturas = texturas[0];
    Piso.carregaTexturas();

    Carro.id = modelos[1].id;
    Carro.reader = modelos[1].reader;
    Carro.posicao = Ponto(camera1.m_pos.x, camera1.m_pos.y-alturaCamera, camera1.m_pos.z);
    Carro.rotacao = 180;
    Carro.cor = Red;
    Carro.escala = Ponto(1,1,1);

    for(int i=0; i<qtdPredios;i++){

        Modelo predio;
        predio = modelos[2];
        predio.cor = rand()%100;
        predio.escala = Ponto(5, 1 + (rand() / (RAND_MAX / (10 - 1))), 5); // escala em y de 1 a 10
        Predios.push_back(predio);
    }

    for(int i=0; i<qtdArvores;i++){

        Modelo tree;
        tree = modelos[3];
        tree.rotacao = rand()%90; //rand
        tree.cor = LimeGreen;
        tree.escala = Ponto(0.5, 0.3, 0.5);
        Arvores.push_back(tree);
    }

    for(int i=0; i<qtdGas;i++){

        Modelo g;
        g = modelos[4];
        g.rotacao = rand()%90; //rand
        g.cor = OrangeRed;
        g.escala = Ponto(0.5, 0.5, 0.5);
        Gas.push_back(g);
    }

    //Posiciona cada objeto em seu lugar a partir do seu id na matrix
    PosicaoNaMatrix(Predios, qtdPredios, prediosIdMatrix); // 100 e 150 id na matrix
    PosicaoNaMatrix(Arvores, qtdArvores, arvoresIdMatrix); // 0 id na matrix
    PosicaoNaMatrix(Gas    , qtdGas    , gasIdMatrix); // 1 a 13 id na matrix

}

void DesenhaPersonagem(Modelo personagem)
{
    if(personagem.id == 0){
        glPushMatrix();
        personagem.desenha2d();
        glPopMatrix();
    }else{
        glPushMatrix();
        personagem.desenha3d();
        glPopMatrix();
    }
    
}

void DesenhaCenario()
{
    Carro.Combustivel();
    AndaAutomatico();
    
    DesenhaPersonagem(Piso);
    DesenhaPersonagem(Carro);

    for(int i=0; i<qtdPredios;i++){
       DesenhaPersonagem(Predios[i]);   
    }

    for(int i=0; i<qtdArvores;i++){
       DesenhaPersonagem(Arvores[i]);        
    } 

    for(int i=0; i<qtdGas;i++){
       Gas[i].rotacao += 2;
       DesenhaPersonagem(Gas[i]);        
    }  

}

//esfera contra esfera
//testamos aqui a distância entre os centros das esferas é menor ou igual à soma de seus raios.
bool verificaColisao(Modelo objetoA, Modelo objetoB){
    
    //Esfera contra Esfera
    //https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection#sphere_vs._sphere
    float distance = sqrt(  (objetoA.posicao.x - objetoB.posicao.x) * (objetoA.posicao.x - objetoB.posicao.x) +
                            (objetoA.posicao.y - objetoB.posicao.y) * (objetoA.posicao.y - objetoB.posicao.y) +
                            (objetoA.posicao.z - objetoB.posicao.z) * (objetoA.posicao.z - objetoB.posicao.z));

    // o tamanho do raio aqui é o tamanho original do raio * a escala aplicada no objeto * 1.5
    // 1.5 serve para aumentar esse raio em torno dos objetos,
    return distance < ((objetoA.reader.raio.z * objetoA.escala.z*1.5) + (objetoB.reader.raio.z * objetoB.escala.z*1.5));
}

void ColisaoDeObjetos(){

    for(int i=0; i<Gas.size(); i++){

        if(verificaColisao(Carro, Gas[i])){

            Gas.erase(Gas.begin() + i);
            qtdGas--;

            if((combustivel+1) > 10 ){
                combustivel = 10;
            }else{
                combustivel += 1;
            }

            dBarra += 0.05;
            distancia = 0;   
        }
    }

}

//usado para o reaparecimento das capsulas de combustives
void Respawn(){

    if(respawn == 100){

        vector<Modelo> gasTemp;
        for(int i=0; i<qtdRespawn; i++){
            Modelo g;
            g = modelos[4];
            g.rotacao = rand()%90; //rand
            g.cor = OrangeRed;
            g.escala = Ponto(0.5, 0.5, 0.5);
            gasTemp.push_back(g);
        }

        PosicaoNaMatrix(gasTemp, qtdRespawn, gasIdMatrix); // gasIdMatrix 1 id na matrix

        for(int i=0; i<gasTemp.size(); i++){

            bool jaTem = false;

            //nao queremos spawnar onjetos na mesma posicao, entao caso caia em alguma ja preenchida nao spawna
            for(int k=0; k<Gas.size(); k++){

                if( Gas[k].posicao.x == gasTemp[i].posicao.x &&
                    Gas[k].posicao.y == gasTemp[i].posicao.y &&
                    Gas[k].posicao.z == gasTemp[i].posicao.z ){
                    jaTem = true;
                    break;
                }
            }

            if(jaTem == false){
                Gas.push_back(gasTemp[i]);
                qtdGas++;
            }
        }

        respawn = 0;
    }

}

//apenas um visual a mais na tela
void BarraCombustivel(){

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    defineCor(OrangeRed);
    glTranslatef(0.25, 0.02, 0);

    if(dBarra < 0){ dBarra = 0; }
    if(dBarra > 0.5){ dBarra = 0.5; }

    glBegin ( GL_QUADS );
        glVertex2f(0, 0); //n muda
        glVertex2f(0, 0.03); //n muda
        glVertex2f(dBarra, 0.03); // muda x
        glVertex2f(dBarra, 0);// muda x
    glEnd();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
}

void AndaAutomatico(){

    lastPos = camera1.m_pos;

    if(anda){
        camera1.m_scl = velocidade;
        camera1.forward();

        //estando na camera2, faz a cameraAuxiliar acompanhar o carro
        camera2.m_pos = camera1.m_pos - (camera1.m_dir * 15);
        resetaCameraAuxiliar();
        Carro.Movimenta(camera1.m_pos.x, camera1.m_pos.y, camera1.m_pos.z,0);
    }
    
}

//*******************************************************************************************

void init()
{
    // Define a cor do fundo da tela (Cinza)
    glClearColor(0.5, 0.5, 0.5, 1.0f);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable (GL_CULL_FACE );
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV,  GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
    if (ModoDeExibicao) // Faces Preenchidas??
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    CarregaModelos();
    CarregaInstancias();

    //o zero em y nao importa
    //0.6 e 0.4 servem para chegar nos limites de cada retangulo do piso
    MinMap = Ponto( (0.4*10)  , 0 , (-0.4*10) );
    MaxMap = Ponto( (-(Piso.reader.l-0.6) *10) , 0 , ( (Piso.reader.c-0.6) *10) );
}

void animate()
{
    double dt;
    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;
    respawn++;


    if (AccumDeltaT > 1.0/30) // fixa a atualiza��o da tela em 30
    {
        AccumDeltaT = 0;
        angulo+=1;
        glutPostRedisplay();
    }
    if (TempoTotal > 1.0)
    {
        // cout << "Tempo Acumulado: "  << TempoTotal << " segundos. " ;
        // cout << "Nros de Frames sem desenho: " << nFrames << endl;
        // cout << "FPS(sem desenho): " << nFrames/TempoTotal << endl;
        TempoTotal = 0;
        nFrames = 0;
    }
}

void DefineLuz(void)
{
  // Define cores para um objeto dourado
  GLfloat LuzAmbiente[]   = {0.4, 0.4, 0.4 } ;
  GLfloat LuzDifusa[]   = {0.7, 0.7, 0.7};
  GLfloat LuzEspecular[] = {0.9f, 0.9f, 0.9 };
  GLfloat PosicaoLuz0[]  = {0.0f, 10.0f, 0.0f };  // Posi��o da Luz
  GLfloat Especularidade[] = {1.0f, 1.0f, 1.0f};

   // ****************  Fonte de Luz 0
    
  glEnable ( GL_COLOR_MATERIAL );

   // Habilita o uso de ilumina��o
  glEnable(GL_LIGHTING);

  // Ativa o uso da luz ambiente
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LuzAmbiente);
  // Define os parametros da luz n�mero Zero
  glLightfv(GL_LIGHT0, GL_AMBIENT, LuzAmbiente);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LuzDifusa  );
  glLightfv(GL_LIGHT0, GL_SPECULAR, LuzEspecular  );
  glLightfv(GL_LIGHT0, GL_POSITION, PosicaoLuz0 );
  glEnable(GL_LIGHT0);

  // Ativa o "Color Tracking"
  glEnable(GL_COLOR_MATERIAL);

  // Define a reflectancia do material
  glMaterialfv(GL_FRONT,GL_SPECULAR, Especularidade);

  // Define a concentra��oo do brilho.
  // Quanto maior o valor do Segundo parametro, mais
  // concentrado ser� o brilho. (Valores v�lidos: de 0 a 128)
  glMateriali(GL_FRONT,GL_SHININESS,51);

}

void MygluPerspective(float fieldOfView, float aspect, float zNear, float zFar )
{
    //https://stackoverflow.com/questions/2417697/gluperspective-was-removed-in-opengl-3-1-any-replacements/2417756#2417756
    // The following code is a fancy bit of math that is equivilant to calling:
    // gluPerspective( fieldOfView/2.0f, width/height , 0.1f, 255.0f )
    // We do it this way simply to avoid requiring glu.h
    //GLfloat zNear = 0.1f;
    //GLfloat zFar = 255.0f;
    //GLfloat aspect = float(width)/float(height);
    GLfloat fH = tan( float(fieldOfView / 360.0f * 3.14159f) ) * zNear;
    GLfloat fW = fH * aspect;
    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}

void PosicUser()
{
    // Define os par�metros da proje��o Perspectiva
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define o volume de visualiza��o sempre a partir da posicao do
    // observador
    if (ModoDeProjecao == 0)
        glOrtho(-10, 10, -10, 10, 0, 7); // Projecao paralela Orthografica
    else MygluPerspective(90,AspectRatio,0.01,100); // Projecao perspectiva

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(idcamera == 1){ camera1.ativar(); }

    if(idcamera == 2){ camera2.ativar3P(camera1, distaciaCamera3P); }
    
    if(idcamera == 4){ cameraAuxiliar.ativar(); }
    
}

void reshape( int w, int h )
{
    // Evita divis�o por zero, no caso de uam janela com largura 0.
	if(h == 0) h = 1;
    // Ajusta a rela��o entre largura e altura para evitar distor��o na imagem.
    // Veja fun��o "PosicUser".
	AspectRatio = 1.0f * w / h;
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a area a ser ocupada pela area OpenGL dentro da Janela
    glViewport(0, 0, w, h);

    PosicUser();
}

void display( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    DefineLuz();
	PosicUser();
	glMatrixMode(GL_MODELVIEW);

	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	Respawn();
    ColisaoDeObjetos();
    DesenhaCenario();
    BarraCombustivel();
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	glutSwapBuffers();
}

//*******************************************************************************************
// camera auxiliar serve olhar para cima ou para baixo, para esquerda ou direita
// para explorar o ambiente, enquanto o veículo se move.
void resetaCameraAuxiliar(){
    //Rotações internas
    if(ultimaCamera == 1){
        cameraAuxiliar.m_pos.x = camera1.m_pos.x;
        cameraAuxiliar.m_pos.y = camera1.m_pos.y;
        cameraAuxiliar.m_pos.z = camera1.m_pos.z;

    }
    if(ultimaCamera == 2){
        cameraAuxiliar.m_pos.x = camera2.m_pos.x;
        cameraAuxiliar.m_pos.y = camera2.m_pos.y + distaciaCamera3P;
        cameraAuxiliar.m_pos.z = camera2.m_pos.z;
    }
            
}

void keyboard ( unsigned char key, int x, int y )
{
    if(key != 32){
        if(idcamera == 1){ 
            ultimaCamera = 1; 
            cameraAuxiliar = camera1; 
        };

        if(idcamera == 2){ 
            ultimaCamera = 2; 
            cameraAuxiliar = camera1;
            cameraAuxiliar.m_pos =  camera2.m_pos;
            cameraAuxiliar.m_pos.y = cameraAuxiliar.m_pos.y+distaciaCamera3P;
            cameraAuxiliar.m_pitch+= -40.0;
        };

        idcamera = 4;
    }

    
    
	switch ( key )
	{
		case 27:          // Termina o programa qdo
			exit ( 0 );   // a tecla ESC for pressionada
			break;
        case 'z':
            desenha = !desenha;
        break;
        case 'p':
            ModoDeProjecao = !ModoDeProjecao;
            glutPostRedisplay();
            break;
        case 'e':
                ModoDeExibicao = !ModoDeExibicao;
                init();
                glutPostRedisplay();
                break;
        case 'f':
            // Vai para Full Screen
			glutFullScreen ( );
			break;
	    case 'r':
			// Reposiciona a janela
            //glutPositionWindow (50,50);
			glutReshapeWindow ( 700, 500 );
			break;

        case 32:
			if(anda == false){ anda = true;  }
            else{ anda = false; }
			break;
        //-----------------------------------------------------------------
        
        case '1':{
            idcamera = 1;
        }break;
        case '3':{
            idcamera = 2;
        }break;

        //-----------------------------------------------------------------

        case 'w':{
            cameraAuxiliar.updatePitch(5.0f);
            cameraAuxiliar.update();
        }break;
        case 's':{
            cameraAuxiliar.updatePitch(-5.0f);
            cameraAuxiliar.update();
        }break;
        case 'a':{
            cameraAuxiliar.updateYaw(-5.0f);
            cameraAuxiliar.update();
        }break;
        case 'd':{
            cameraAuxiliar.updateYaw(5.0f);
            cameraAuxiliar.update();
        }break;

		default:
			break;
	}
}

void arrow_keys ( int a_keys, int x, int y )
{
    lastPos = camera1.m_pos;

	switch ( a_keys )
	{
        case GLUT_KEY_UP:{
            camera1.m_scl = velocidade;
            camera1.forward();

            //estando na camera2, faz a cameraAuxiliar acompanhar o carro
            camera2.m_pos = camera1.m_pos - (camera1.m_dir * 15);
            resetaCameraAuxiliar();

            Carro.Movimenta(camera1.m_pos.x, camera1.m_pos.y, camera1.m_pos.z,0);   //anda para frente
        }break;
        case GLUT_KEY_DOWN:{
            camera1.m_scl = velocidade;
            camera1.back();

            //estando na camera2, faz a cameraAuxiliar acompanhar o carro 
            camera2.m_pos = camera1.m_pos - (camera1.m_dir * 15);
            resetaCameraAuxiliar();

            Carro.Movimenta(camera1.m_pos.x, camera1.m_pos.y, camera1.m_pos.z,0); // anda para tras
        }break;
        case GLUT_KEY_RIGHT:{
            camera1.updateYaw(5.0f);
            camera1.update();

            Carro.Movimenta(camera1.m_pos.x, camera1.m_pos.y, camera1.m_pos.z,-5);    //rotacao para a direita
        }break;
        case GLUT_KEY_LEFT:{
            camera1.updateYaw(-5.0f);
            camera1.update();

            Carro.Movimenta(camera1.m_pos.x, camera1.m_pos.y, camera1.m_pos.z,5); //rotacao para a esqueda
        }break;
		default:
			break;
	}
}

// ******************************************************************************************

int  main ( int argc, char** argv )
{
    srand(time(NULL));

    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (0,0);
    glutInitWindowSize  ( 700, 700 );
    glutCreateWindow    ( "Game Car" );

    init ();

    glutDisplayFunc ( display );
	glutReshapeFunc ( reshape );
	glutKeyboardFunc ( keyboard );
	glutSpecialFunc ( arrow_keys );
	glutIdleFunc ( animate );
    glutMainLoop ( );

    glDisable(GL_TEXTURE_2D);
    //glDeleteTextures()

    return 0;
}
