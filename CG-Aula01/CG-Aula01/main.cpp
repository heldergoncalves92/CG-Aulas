//
//  main.cpp
//  CG-Aula01
//
//  Created by Hélder José Alves Gonçalves on 27/02/14.
//  Copyright (c) 2014 Hélder José Alves Gonçalves. All rights reserved.
//

#include <iostream>

#include <GLUT/glut.h>
#include <math.h>

float x=0,a=0,b=0,c=0;

void changeSize(int w, int h) {
    
	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;
    
	// compute window's aspect ratio
	float ratio = w * 1.0 / h;
    
	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    
	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);
    
	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}



void renderScene(void) {
    
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	// set the camera
	glLoadIdentity();
	gluLookAt(0,0,5,
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);
    
    // pÙr instruÁıes de desenho aqui
    
    
    glColor3f(cos(a), cos(b), cos(c));
    glutWireTeapot(cos(x)+1.2);

    //Mudar tamanho
    x+=0.05;
    
    //Mudar cores
    a+=0.005;
    b+=0.02;
    c+=0.01;
    
    
	// End of frame
	
    glutSwapBuffers();
}



int main(int argc, char **argv) {
    
    // pÙr inicializaÁ„o aqui
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow("CG@DI");
    
    
    // pÙr registo de funÁıes aqui
    glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
    
    glutIdleFunc(renderScene);
    
    // alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
    
    // entrar no ciclo do GLUT aqui
	glutMainLoop();
    
    return 0;
    
}

