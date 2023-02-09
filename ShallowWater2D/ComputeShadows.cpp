
#pragma once

#include <vector>
#include <iostream>
#include <math.h>
//#include <GL/glut.h
#include "include/GL/glut.h"
#include <Windows.h>
#include "Shadows.h"

using namespace std;


std::shared_ptr<ComputeShadows> ShadowsSolver;

float xAlpha = 0.0f;
float zAlpha = 0.0f;
float x = 0.0f;
float y = 0.0f;


void MoveCamera() {

	if (GetKeyState(VK_UP) < 0) xAlpha++;
	if (GetKeyState(VK_DOWN) < 0) xAlpha--;
	if (GetKeyState(VK_LEFT) < 0) zAlpha++;
	if (GetKeyState(VK_RIGHT) < 0) zAlpha--;

	float speed = 0.3f;
	if (GetKeyState('W') < 0){
		speed *= -1;
		y += speed * 1.0f;
	}
	if (GetKeyState('S') < 0) {
		speed *= 1;
		y += speed * 1.0f;
	}
	if (GetKeyState('A') < 0) {
		speed *= 1;
		x += speed * 1.0f;
	}
	if (GetKeyState('D') < 0) {
		speed *= -1;
		x += speed * 1.0f;
	}
	
	glRotatef(-xAlpha*0.3f, 1, 0, 0); 
	glRotatef(-zAlpha*0.3f, 0, 0, 1);
	glTranslatef(x, y, 0);
	
}

void display( ) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


	glPushMatrix();
	MoveCamera();


	std::vector<Particle> Particles = ShadowsSolver->sbParticles;
	std::vector<float> Shadows = ShadowsSolver->sbShadows;

	for (int i = 0; i < Particles.size(); i++) {
		glColor3d(1 * Shadows[i], 0, 1 * Shadows[i]);
		
		glPointSize(Particles[i].radius*100);
		glEnable(GL_POINT_SMOOTH);
		glBegin(GL_POINTS);
		glVertex3d(Particles[i].pos.x, Particles[i].pos.y, Particles[i].pos.z); 
		glEnd();
		glDisable(GL_POINT_SMOOTH);
	}




	glPopMatrix();
	glutSwapBuffers();
}


void timer(int = 0) {

	display();
	glutTimerFunc(10, timer, 0);
}


int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(880, 880);
	glutInitWindowPosition(20, 86); 
	glutCreateWindow("Shadows");
	glClearColor(0.9, 0.9, 0.9, 1.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	

	ShadowsSolver.reset(new ComputeShadows(1000, 10, 40));
	ShadowsSolver->csComputeSelfShadowing();

	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glFrustum(-5, 5, -5, 5, 5, 2000); 

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0,  0 , -50);
	
	glutDisplayFunc(display);
	timer();

	glutMainLoop();


return 0;
}

