
#ifdef WIN32
#include <windows.h>
//#include "gl\glut.h"
#include <glut.h>
#endif


#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

//#include "Ponto.h"

class Linha {
	float minx,miny, maxx, maxy; // envelope

public:
	float x1,y1,x2,y2;

    void geraLinha(int limite, int TamMax);
	void desenhaLinha();

};
