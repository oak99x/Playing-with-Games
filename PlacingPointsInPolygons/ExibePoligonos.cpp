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
#include <chrono>
#include <fstream>
#include <map>
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
#include "Poligono.h"

#include "Temporizador.h"
Temporizador T;
double AccumDeltaT = 0;

Poligono Mapa;
Poligono ConvexHull;
Poligono ConjuntoDePonto;

// Limites logicos da area de desenho
Ponto Min, Max, PontoClicado;

bool desenha = false;
bool FoiClicado = false;

class Faixa
{
    vector<int> ArestasNaFaixa;

public:
    void CadastraAresta(int a)
    {
        ArestasNaFaixa.push_back(a);
    }
    int getNroDeArestas()
    {
        return ArestasNaFaixa.size(); //
    }

    int getAresta(int i)
    {
        return ArestasNaFaixa[i];
    }
};

class ConjuntoDeFaixas
{

    vector<Faixa> TodasAsFaixas;        // guarda as arestas
    map<int, vector<float>> PosDaFaixa; // guarda a posição de cada faixa exemplo: faixa1{0,1} faixa2{1,2}

public:
    ConjuntoDeFaixas()
    {
    }
    void CadastraArestaNaFaixa(int f, int a)
    {
        TodasAsFaixas[f].CadastraAresta(a);
    }

    int qtdFaixas;
    void CriaFaixas(int qtdDeFaixas) // pode ser substitu�da por uma construtora
    {
        qtdFaixas = qtdDeFaixas;
        // array guardando de onde até onde ex: faixa1: 0 100 faixa2:100 200
        float distancia = (Max.y + abs(Min.y)) / qtdFaixas;
        float passa = Min.y;
        for (int i = 0; i < qtdDeFaixas; i++)
        {
            vector<float> marcaPontoIF;
            marcaPontoIF.push_back(passa);
            marcaPontoIF.push_back(passa + distancia);
            PosDaFaixa[i] = marcaPontoIF;

            passa = (passa + distancia);
        }

        for (int i = 0; i < qtdDeFaixas; i++)
            TodasAsFaixas.push_back(Faixa());
    }

    Faixa getFaixa(int f)
    {
        return TodasAsFaixas[f];
    }

    vector<float> getPosDaFaixa(int f)
    {
        return PosDaFaixa[f];
    }

    void DesenhaFaixas()
    {

        GLfloat distancia = (Max.y + abs(Min.y)) / (qtdFaixas); // qtd de faixas - 1
        GLfloat passa = Min.y;

        for (int i = 0; i <= qtdFaixas; i++)
        {
            glBegin(GL_LINES);
            //  eixo horizontal
            glVertex2f(Min.x, passa);
            glVertex2f(Max.x, passa);
            glEnd();

            passa = (passa + distancia);
        }
    }
};

ConjuntoDeFaixas EspacoDividido;
float angulo = 0.0;

void ImprimeFaixas()
{
    cout  << "=================================================================================="<< endl;
    cout  << "Cadastro de Arestas nas Faixas"<< endl;
    for (int i = 0; i < 10; i++)
    {
        cout << "Faixa " << i << ": ";
        Faixa f;
        f = EspacoDividido.getFaixa(i);
        for (int a = 0; a < f.getNroDeArestas(); a++)
        {
            cout << f.getAresta(a) << " ";
        }
        cout << endl;
    }
    cout  << "================================================================================="<< endl;
}

// MINHAS FUNCOES ===============================================================================
// FUNÇÕES INICIAIS =============================================================================
// Gerar pontos aleatorios na tela de desenho
void geraPontos()
{

    srand(time(NULL));

    for (int i = 0; i <= 200; i++)
    {
        /* gerando valores aleatórios com ponto flutuante*/
        Ponto P;
        // P.x = rand() / (Max.x - Min.x + 1);
        // P.y = rand() / (Max.y - Min.y + 1);
        P.x = Min.x + (rand() / (RAND_MAX / (Max.x - Min.x)));
        P.y = Min.y + (rand() / (RAND_MAX / (Max.y - Min.y)));

        ConjuntoDePonto.insereVertice(P);
    }
}

// Determinar se três pontos constituem uma "viragem à esquerda" ou uma "viragem à direita"
// não requer o cálculo do ângulo real entre os dois segmentos de linha e pode ser alcançado
// apenas com aritmética simples. Por três pontos P1 = (x1, y1), P2 = (x2, y2), P3 = (x3, y3),
// calcule a coordenada z do produto vetorial dos dois vetores P1P2 e P1P3, que é dado pela
// expressão prodV = (P2.x - P1.x)*(P3.y - P1.y) - (P2.y - P1.y)*(P3.x - P1.x).
// Se o resultado for == 0, os pontos são colineares; se for > 0, os três pontos
// constituem uma "virada à esquerda" ou orientação no sentido anti-horário,
// caso contrário, uma "virada à direita" ou orientação no sentido horário (para
// pontos numerados no sentido anti-horário).
int orient(Ponto a, Ponto b, Ponto c)
{
    // int v = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
    int v = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    if (v == 0)
        return 0;           // colinear
    return (v > 0) ? 1 : 2; // clock or counterclock wise
}

// Gift wrapping algorithm
// No caso bidimensional, o algoritmo também é conhecido como Jarvis march , em homenagem
// a RA Jarvis, que o publicou em 1973; tem complexidade de tempo O ( nh ) , onde n é o número
// de pontos eh é o número de pontos no casco convexo. A ideia do Algoritmo de Jarvis é simples,
// começamos do ponto mais à esquerda (ou ponto com valor mínimo da coordenada x) e continuamos
// envolvendo os pontos no sentido anti-horário. A grande questão é, dado um ponto p como ponto
// atual, como encontrar o próximo ponto na saída? A ideia é usar orientação() aqui. O próximo
// ponto é selecionado como o ponto que supera todos os outros pontos no sentido anti-horário,
// ou seja, o próximo ponto é q se para qualquer outro ponto r, temos “orientação(p, q, r) = anti-horário”.
void convexHull()
{
    int size = Mapa.getNVertices();
    // Deve haver pelo menos 3 pontos
    if (size < 3)
    {
        return;
    }

    int l = 0; // initialize result.

    // encontar ponto mais a esqueda
    for (int i = 1; i < size; i++)
    {
        if (Mapa.getVertice(i).x < Mapa.getVertice(l).x)
        {
            l = i; // find left most point
        }
    }

    // Comece do ponto mais à esquerda, continue se movendo no sentido anti-horário
    // até chegar ao ponto inicial novamente
    int p = l; // 0,0 pos9
    int q;

    do
    {
        // Procura um ponto 'q' tal que a orientação(p, i, q) seja
        // sentido anti-horário para todos os pontos 'i'
        q = (p + 1) % size;
        for (int i = 0; i < size; i++)
        {
            if (orient(Mapa.getVertice(p), Mapa.getVertice(i), Mapa.getVertice(q)) == 2)
            {
                q = i;
            }
        }

        ConvexHull.insereVertice(Mapa.getVertice(q));
        p = q; // Defina p como q para a próxima iteração
    } while (p != l);
}

// Coloracao dos pontos dependendo de sua localização
// VerificaPonto pega o poligono e o ponto passados e verifica desenhando uma linha horizontal
// à direita de cada ponto e estenda-a até o "infinito"(fora da area do poligono). Apos conta-se
// o número de vezes que a linha cruza com as arestas do polígono. Um ponto está dentro do polígono
// se a contagem de interseções for ímpar ou ponto está em uma aresta do polígono - return true.
// Se nenhuma das condições for verdadeira, então ponto fica do lado de fora - return false.
bool verificaPonto(Poligono poligono, Ponto ponto)
{
    bool result;
    // Nao eh um poligono
    if (poligono.getNVertices() < 3)
    {
        return false;
    }

    int count = 0;
    Ponto Esq;
    Ponto Dir(-1, 0);
    Esq = ponto + Dir * 100;

    Ponto P1, P2;
    for (int i = 0; i < poligono.getNVertices(); i++)
    {
        poligono.getAresta(i, P1, P2);
        // if(PassaPelaFaixa(i,F))
        if (HaInterseccao(ponto, Esq, P1, P2))
        {
            count++;
        }
    }

    if (count % 2 == 0)
    {
        result = false;
    }
    else
    {
        result = true;
    }

    return result;
}

// Recebe dois booleanos e a partir deles decide de que cor o ponto será colorido
void pintaPonto(bool poligono, bool convex, Ponto ponto)
{
    if (poligono == true)
    { // dentro do poligono = AZUL
        glBegin(GL_POINTS);
        glColor3f(0, 0, 1); // R, G, B  [0..1]
        glVertex3f(ponto.x, ponto.y, ponto.z);
        glEnd();
    }
    else if (poligono == false && convex == true)
    { // fora do poligono mais dentro do convex = AMARELO
        glBegin(GL_POINTS);
        glColor3f(1, 1, 0); // R, G, B  [0..1]
        glVertex3f(ponto.x, ponto.y, ponto.z);
        glEnd();
    }
    else
    { // fora dos dois = VERMELHO
        glBegin(GL_POINTS);
        glColor3f(1, 0, 0); // R, G, B  [0..1]
        glVertex3f(ponto.x, ponto.y, ponto.z);
        glEnd();
    }
}

void verificaPontos()
{
    for (int i = 0; i < ConjuntoDePonto.getNVertices(); i++)
    {
        bool pintaP = verificaPonto(Mapa, ConjuntoDePonto.getVertice(i));
        bool pintaC = verificaPonto(ConvexHull, ConjuntoDePonto.getVertice(i));
        pintaPonto(pintaP, pintaC, ConjuntoDePonto.getVertice(i));
    }
}

void cadastroArestas(Poligono poligono){
    EspacoDividido.CriaFaixas(10);

    for(int i=0; i<poligono.getNVertices(); i++){

        //Aresta
        Ponto I = poligono.getVertice(i);
        Ponto F;
        if(i==poligono.getNVertices()-1){
            F = poligono.getVertice(0);
        }else{
            F = poligono.getVertice(i+1);
        }

        //Após pegar a aresta, comparamos quais faixas tem um valor entre os dois extremos da aresta
        //Na faixa que tiver entao cadastra-se a aresta, aqui no caso seu numero correspondente
        for(int j=0; j<10; j++){
            if(I.y <= F.y){ //**=
                if(EspacoDividido.getPosDaFaixa(j)[0] >= I.y && EspacoDividido.getPosDaFaixa(j)[1] <= F.y){
                    EspacoDividido.CadastraArestaNaFaixa(j,i);
                }
            }else{
                if(EspacoDividido.getPosDaFaixa(j)[0] >= F.y && EspacoDividido.getPosDaFaixa(j)[1] <= I.y){
                    EspacoDividido.CadastraArestaNaFaixa(j,i);
                }
            }
            
        }
    }

    ImprimeFaixas();

}

// ALGORITMOS DE TESTE DE INCLUSAO=============================================================
//============================================================================================
void forcaBruta()
{
    auto start = std::chrono::high_resolution_clock::now();
    int pontosMapa = 0;   // contador de pontos no Poligono
    int pontosConvex = 0; // contador de pontos no Convex
    int haMapa = 0;       // contador do numero de vezes que a função HaIntersecao foi chamada no Poligono
    int haConvex = 0;     // contador do numero de vezes que a função HaIntersecao foi chamada no Convex

    int sizePontos = ConjuntoDePonto.getNVertices();
    int sizeMapa = Mapa.getNVertices();
    int sizeConvex = ConvexHull.getNVertices();

    // Gera limites de verificação para descartar todos os pontos que estão fora do convex
    // Assim se o numeros de n pontos for alto uma boa quantidade já é eliminada antes do
    // teste de intersecao
    Ponto maisAlto = ConvexHull.getVertice(0);
    Ponto maisBaixo = ConvexHull.getVertice(0);
    Ponto maisEsquerda = ConvexHull.getVertice(0);
    Ponto maisDireita = ConvexHull.getVertice(0);

    for (int i = 0; i < ConvexHull.getNVertices(); i++)
    {
        // limite Mais Alto
        if (maisAlto.y < ConvexHull.getVertice(i).y)
        {
            maisAlto = ConvexHull.getVertice(i);
        }
        // limite Mais Baixo
        if (maisBaixo.y > ConvexHull.getVertice(i).y)
        {
            maisBaixo = ConvexHull.getVertice(i);
        }
        // limite Mais a Esquerda
        if (maisEsquerda.x > ConvexHull.getVertice(i).x)
        {
            maisEsquerda = ConvexHull.getVertice(i);
        }
        // limite Mais a Direita
        if (maisDireita.x < ConvexHull.getVertice(i).x)
        {
            maisDireita = ConvexHull.getVertice(i);
        }
    }
    //==========================================================================

    for (int i = 0; i < sizePontos; i++)
    {
        Ponto ponto = ConjuntoDePonto.getVertice(i);

        // if para descartar pontos fora do convex
        if (ponto.x < maisEsquerda.x || ponto.x > maisDireita.x)
        {
            continue;
        }

        if (ponto.y < maisBaixo.y || ponto.y > maisAlto.y)
        {
            continue;
        }

        Ponto Esq;
        Ponto Dir(-1, 0);
        Esq = ponto + Dir * 100;
        Ponto P1, P2;

        // Para o poligono normal
        for (int i = 0; i < sizeMapa; i++)
        {
            Mapa.getAresta(i, P1, P2);
            HaInterseccao(ponto, Esq, P1, P2);
            haMapa++;  
        }
        if ((getContadorInt() % 2) != 0){ pontosMapa++;}
        resetContadorInt();

        // Para o poligono convex
        for (int i = 0; i < sizeConvex; i++)
        {
            ConvexHull.getAresta(i, P1, P2);
            HaInterseccao(ponto, Esq, P1, P2);
            haConvex++;  
        }
        if ((getContadorInt() % 2) != 0){ pontosConvex++;}
        resetContadorInt();
        
    }

    auto end = std::chrono::high_resolution_clock::now()-start;
    long long int_s = std::chrono::duration_cast<std::chrono::microseconds> (end).count();

    cout << "Forca Bruta  :: Total de pontos dentro Poligono -> " << pontosMapa << endl;
    cout << "HaIntersecao :: Total de chamadas no Poligono   -> " << haMapa << endl;
    cout << "Forca Bruta  :: Total de pontos dentro Convex   -> " << pontosConvex << endl;
    cout << "HaIntersecao :: Total de chamadas no Convex     -> " << haConvex << endl;
    cout << "Tempo de execução: " << int_s << " microseconds" << endl;
    cout << "=================================================================================" << endl;
}

void forcaBrutaFaixas()
{
    auto start = std::chrono::high_resolution_clock::now();
    int sizeMapa = Mapa.getNVertices();
    int sizeConvex = ConvexHull.getNVertices();

    int pontosMapa = 0;   // contador de pontos no Poligono
    int pontosConvex = 0; // contador de pontos no Convex
    int haMapa = 0;       // contador do numero de vezes que a função HaIntersecao foi chamada no Poligono
    int haConvex = 0;     // contador do numero de vezes que a função HaIntersecao foi chamada no Convex

    for (int i = 0; i < ConjuntoDePonto.getNVertices(); i++){
        Ponto ponto = ConjuntoDePonto.getVertice(i);

        Ponto Esq;
        Ponto Dir(-1, 0);
        Esq = ponto + Dir * 100;
        Ponto P1, P2;

        // Para o poligono normal
        for (int i = 0; i < sizeMapa; i++)
        {
            Mapa.getAresta(i, P1, P2);
            if(P1.y < P2.y){
                if(ponto.y >= P1.y && ponto.y <= P2.y){
                    HaInterseccao(ponto, Esq, P1, P2);
                    haMapa++;
                }
            }else{
                if(ponto.y >= P2.y  && ponto.y <= P1.y){
                    HaInterseccao(ponto, Esq, P1, P2);
                    haMapa++;
                }
            }
             
        }
        if ((getContadorInt() % 2) != 0){ pontosMapa++;}
        resetContadorInt();

        // Para o poligono convex
        for (int i = 0; i < sizeConvex; i++)
        {
            ConvexHull.getAresta(i, P1, P2);
            if(P1.y < P2.y){
                if(ponto.y >= P1.y && ponto.y <= P2.y){
                    HaInterseccao(ponto, Esq, P1, P2);
                    haConvex++;
                }
            }else{
                if(ponto.y >= P2.y  && ponto.y <= P1.y){
                    HaInterseccao(ponto, Esq, P1, P2);
                    haConvex++;
                }
            }  
        }
        if ((getContadorInt() % 2) != 0){ pontosConvex++;}
        resetContadorInt();


    }

    auto end = std::chrono::high_resolution_clock::now()-start;
    long long int_s = std::chrono::duration_cast<std::chrono::microseconds> (end).count();

    cout << "Forca Bruta Pela Faixa :: Total de pontos dentro Poligono -> " << pontosMapa << endl;
    cout << "HaIntersecao :: Total de chamadas no Poligono             -> " << haMapa << endl;
    cout << "Forca Bruta Pela Faixa :: Total de pontos dentro Convex   -> " << pontosConvex << endl;
    cout << "HaIntersecao :: Total de chamadas no Convex               -> " << haConvex << endl;
    cout << "Tempo de execução: " << int_s << " microseconds" << endl;
    cout << "=================================================================================" << endl;

}

void inclusaoConvexHull()
{
    auto start = std::chrono::high_resolution_clock::now();
    int contaP = 0;
    int prodV = 0;
    Ponto A;
    Ponto B;

    for(int i=0; i<ConjuntoDePonto.getNVertices(); i++){

        bool taDentro = false;
        Ponto ponto = ConjuntoDePonto.getVertice(i);

        for(int j=0; j<ConvexHull.getNVertices(); j++){

            A = ConvexHull.getVertice(j);
            //Ponto B
            if(j==ConvexHull.getNVertices()-1){
                B = ConvexHull.getVertice(0);
            }else{
                B = ConvexHull.getVertice(j+1);
            }

            Ponto V1 = MontaVetor(A, B);
            Ponto V2 = MontaVetor(A, ponto);

            Ponto Z;
            ProdVetorial(V1, V2, Z);
            prodV++;

            if (Z.z <=0){
                taDentro = true;
            }else{
                taDentro = false;
                break;
            }  
        }

        if(taDentro){
            contaP++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now()-start;
    long long int_s = std::chrono::duration_cast<std::chrono::microseconds> (end).count();

    cout << "Teste de inclusão em polígono convexo :: Total de pontos dentro -> " << contaP << endl;
    cout << "Numero de vezes que o ProVetorial foi chamado -> " << prodV << endl;
    cout << "Tempo de execução: " << int_s << " microseconds" << endl;
    cout << "===============================================================================" << endl;
}

//========================================================================
//========================================================================

void init()
{
    // Define a cor do fundo da tela (AZUL)
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    Mapa.LePoligono("PoligonoDeTeste.txt");
    // Mapa.LePoligono("EstadoRS.txt");
    Mapa.obtemLimites(Min, Max);

    Min.x--;
    Min.y--;
    Max.x++;
    Max.y++;
    
    convexHull();
    geraPontos();
    cadastroArestas(Mapa);
    //cadastroArestas(ConvexHull);
    forcaBruta();
    inclusaoConvexHull();
    forcaBrutaFaixas();
}

double nFrames = 0;
double TempoTotal = 0;
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

    if (AccumDeltaT > 1.0 / 30) // fixa a atualiza��o da tela em 30
    {
        AccumDeltaT = 0;
        // angulo+=0.05;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0)
    {
        // cout << "\n==================================================" << endl;
        // cout << "Tempo Acumulado: " << TempoTotal << " segundos." << endl;
        // cout << "Nros de Frames sem desenho: " << nFrames << endl;
        // cout << "FPS(sem desenho): " << nFrames / TempoTotal << endl;
        // cout << "==================================================" << endl;
        // TempoTotal = 0;
        // nFrames = 0;
    }
}

// **********************************************************************
//  void reshape( int w, int h )
//  trata o redimensionamento da janela OpenGL
// **********************************************************************
void reshape(int w, int h)
{
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a area a ser ocupada pela area OpenGL dentro da Janela
    glViewport(0, 0, w, h);
    // Define os limites logicos da area OpenGL dentro da Janela
    glOrtho(Min.x, Max.x,
            Min.y, Max.y,
            0, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
// **********************************************************************
void DesenhaLinha(Ponto P1, Ponto P2)
{
    glBegin(GL_LINES);
    glVertex3f(P1.x, P1.y, P1.z);
    glVertex3f(P2.x, P2.y, P2.z);
    glEnd();
}

// **********************************************************************
void DesenhaEixos()
{
    Ponto Meio;
    Meio.x = (Max.x + Min.x) / 2;
    Meio.y = (Max.y + Min.y) / 2;
    Meio.z = (Max.z + Min.z) / 2;

    glBegin(GL_LINES);
    //  eixo horizontal
    glVertex2f(Min.x, Meio.y);
    glVertex2f(Max.x, Meio.y);
    //  eixo vertical
    glVertex2f(Meio.x, Min.y);
    glVertex2f(Meio.x, Max.y);
    glEnd();
}

// **********************************************************************
//  void display( void )
// **********************************************************************
void display(void)
{

    // Limpa a tela coma cor de fundo
    glClear(GL_COLOR_BUFFER_BIT);

    // Define os limites l�gicos da �rea OpenGL dentro da Janela
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    // Coloque aqui as chamadas das rotinas que desenham os objetos
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    glLineWidth(2);
    glColor3f(0.3, 0.3, 0.3); // R, G, B  [0..1]
    EspacoDividido.DesenhaFaixas();
    // DesenhaFaixas(10);

    glLineWidth(1);
    glColor3f(1, 1, 1); // R, G, B  [0..1]
    // DesenhaEixos();

    glRotatef(angulo, 0, 0, 1);
    glLineWidth(2);
    glColor3f(1, 1, 0); // R, G, B  [0..1]
    Mapa.desenhaPoligono();

    glRotatef(angulo, 0, 0, 1);
    glLineWidth(2);
    glColor3f(0, 1, 1); // R, G, B  [0..1]
    ConvexHull.desenhaPoligono();

    //====================================
    // glRotatef(angulo, 0,0,1);
    // glLineWidth(1);
    // glColor3f(0,1,1);
    // ConjuntoDePonto.desenhaVertices();

    glRotatef(angulo, 0, 0, 1);
    glLineWidth(1);
    // glColor3f(1,0,0);
    verificaPontos();
    //=====================================

    if (FoiClicado == true)
    {

        Ponto Esq;
        Ponto Dir(-1, 0);
        Esq = PontoClicado + Dir * 100;
        glColor3f(0, 1, 0); // R, G, B  [0..1]
        DesenhaLinha(PontoClicado, Esq);
        // F = CalculaFaixa(PontoClicado);

        glColor3f(1, 0, 0); // R, G, B  [0..1]
        Ponto P1, P2;
        for (int i = 0; i < Mapa.getNVertices(); i++)
        {
           Mapa.getAresta(i, P1, P2);
            // if(PassaPelaFaixa(i,F))
            if (HaInterseccao(PontoClicado, Esq, P1, P2))
                Mapa.desenhaAresta(i);
        }
    }

    // Mapa.desenhaVertices();

    if (desenha)
        ConvexHull.desenhaPoligono();

    glPointSize(5);
    glColor3f(0, 1, 0); // R, G, B  [0..1]
                        // Mapa.desenhaVertices();

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
    while (true)
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

void keyboard(unsigned char key, int x, int y)
{

    switch (key)
    {
    case 27:     // Termina o programa qdo
        exit(0); // a tecla ESC for pressionada
        break;
    case 't':
        ContaTempo(3);
        break;
    case ' ':
        desenha = !desenha;
        break;
    case 'f':
        
        break;
    default:
        break;
    }
}
// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
// **********************************************************************
void arrow_keys(int a_keys, int x, int y)
{
    switch (a_keys)
    {
    case GLUT_KEY_UP:     // Se pressionar UP
        glutFullScreen(); // Vai para Full Screen
        break;
    case GLUT_KEY_DOWN: // Se pressionar UP
                        // Reposiciona a janela
        glutPositionWindow(50, 50);
        glutReshapeWindow(700, 500);
        break;
    default:
        break;
    }
}
// **********************************************************************
// Esta fun��o captura o clique do botao direito do mouse sobre a �rea de
// desenho e converte a coordenada para o sistema de refer�ncia definido
// na glOrtho (ver fun��o reshape)
// Este c�digo � baseado em http://hamala.se/forums/viewtopic.php?t=20
// **********************************************************************
void Mouse(int button, int state, int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16], projection[16];
    GLfloat wx = x, wy, wz;
    GLdouble ox = 0.0, oy = 0.0, oz = 0.0;

    if (state != GLUT_DOWN)
        return;
    if (button != GLUT_RIGHT_BUTTON)
        return;
    cout << "Botao da direita! ";

    glGetIntegerv(GL_VIEWPORT, viewport);
    y = viewport[3] - y;
    wy = y;
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &wz);
    gluUnProject(wx, wy, wz, modelview, projection, viewport, &ox, &oy, &oz);
    PontoClicado = Ponto(ox, oy, oz);
    PontoClicado.imprime("- Ponto no universo: ", "\n");
    FoiClicado = true;
}

// **********************************************************************
//  void main ( int argc, char** argv )
//
// **********************************************************************
int main(int argc, char **argv)
{
    cout << "Programa OpenGL" << endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowPosition(0, 0);

    // Define o tamanho inicial da janela grafica do programa
    glutInitWindowSize(650, 500);

    // Cria a janela na tela, definindo o nome da
    // que aparecera na barra de titulo da janela.
    glutCreateWindow("Poligonos em OpenGL");

    // executa algumas inicializa��es
    init();

    // Define que o tratador de evento para
    // o redesenho da tela. A funcao "display"
    // ser� chamada automaticamente quando
    // for necess�rio redesenhar a janela
    glutDisplayFunc(display);

    // Define que o tratador de evento para
    // o invalida��o da tela. A funcao "display"
    // ser� chamada automaticamente sempre que a
    // m�quina estiver ociosa (idle)
    glutIdleFunc(animate);

    // Define que o tratador de evento para
    // o redimensionamento da janela. A funcao "reshape"
    // ser� chamada automaticamente quando
    // o usu�rio alterar o tamanho da janela
    glutReshapeFunc(reshape);

    // Define que o tratador de evento para
    // as teclas. A funcao "keyboard"
    // ser� chamada automaticamente sempre
    // o usu�rio pressionar uma tecla comum
    glutKeyboardFunc(keyboard);

    // Define que o tratador de evento para
    // as teclas especiais(F1, F2,... ALT-A,
    // ALT-B, Teclas de Seta, ...).
    // A funcao "arrow_keys" ser� chamada
    // automaticamente sempre o usu�rio
    // pressionar uma tecla especial
    glutSpecialFunc(arrow_keys);

    glutMouseFunc(Mouse);

    // inicia o tratamento dos eventos
    glutMainLoop();

    return 0;
}
