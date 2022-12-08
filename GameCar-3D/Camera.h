
#ifndef Camera_hpp
#define Camera_hpp
#include <iostream>
using namespace std;

#include "Ponto.h"

class Camera
{
public:
	Ponto m_pos;
	Ponto m_dir;
	Ponto m_left;
	Ponto m_up;
	Ponto m_veloc;
	float m_scl;
	float m_yaw;
	float m_pitch;
	Ponto look;

	Camera(Ponto pos);
	Camera();
	
	void ativar();
	void forward();
	void back();
	void left();
	void right();
	void updateYaw(float dYaw);
	void updatePitch(float dPitch);
	void update();
	void ativar3P(Camera player, float distancia);
};



#endif /* Instancia_hpp */