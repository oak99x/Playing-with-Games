
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

#include "Camera.h"
#include "Ponto.h"

Camera::Camera(Ponto pos)
{
	m_pos = pos;
	m_dir = Ponto(0, 0, -1);
	m_left = Ponto(-1, 0, 0);
	m_up = Ponto(0, 1, 0);
	m_veloc = Ponto(0, 0, 0);
}


Camera::Camera()
{
}

void Camera::ativar()
{
	look = m_pos + m_dir;
	        //observador                //alvo                   //camera de cima
	gluLookAt(m_pos.x, m_pos.y, m_pos.z, look.x, look.y, look.z, m_up.x, m_up.y, m_up.z);
}

void Camera::forward() // anda para frente
{
	m_veloc = operator*(m_dir, m_scl);
	m_pos = operator+(m_pos, m_veloc);
}

void Camera::back() // anda para tras
{
	m_veloc = operator*(m_dir, -m_scl);
	m_pos = operator+(m_pos, m_veloc);
}

void Camera::left() // anda para esquerda
{
	m_veloc = operator*(m_left, m_scl);
	m_pos = operator+(m_pos, m_veloc);
}

void Camera::right() // anda para direita
{
	m_veloc = operator*(m_left, -m_scl);
	m_pos = operator+(m_pos, m_veloc);
}

void Camera::updateYaw(float dYaw)
{
	m_yaw += dYaw; //rotação
}

void Camera::updatePitch(float pitch)
{
	m_pitch += pitch; //rotação
}

void Camera::update(){
	float ang1 = GrauParaRad(m_yaw);
	float ang2 = GrauParaRad(m_pitch);

	m_dir.x = sin(ang1);
	m_dir.y = sin(ang2);
	m_dir.z = -cos(ang1);

	m_dir = Normaliza(m_dir);
}

void Camera::ativar3P(Camera player, float distancia){

	m_pos = player.m_pos - (player.m_dir * 15);
	look = player.m_pos + player.m_dir;
	
	        //observador                //alvo                   //camera de cima
	gluLookAt(m_pos.x, m_pos.y+distancia, m_pos.z, look.x, look.y, look.z, m_up.x, m_up.y, m_up.z);
}



