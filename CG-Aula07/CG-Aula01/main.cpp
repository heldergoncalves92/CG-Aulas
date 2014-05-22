//
//  main.cpp
//  CG-Aula01
//
//  Created by Hélder José Alves Gonçalves on 27/02/14.
//  Copyright (c) 2014 Hélder José Alves Gonçalves. All rights reserved.
//

#include <iostream>
#include "controlos.h"

float raio=200,cam_h=0.5,cam_v=0.3,camh_x=0,camh_y=0,x_tela, y_tela;


float camX = 0, camY=0, camZ = 5;
int startX, startY, tracking = 0;

int alpha = 0, beta = 0, r = 5;

#define POINT_COUNT 5
// Points that make up the loop for catmull-rom interpolation
float p[POINT_COUNT][3] = {{-1,-1,0},{-1,1,0},{1,1,0},{0,0,-1},{1,-1,0}};


void getCatmullRomPoint(float t, int *indices, float *res) {
    int i;
    float res_aux[4];
	// catmull-rom matrix
	float m[4][4] = {{-0.5f,  1.5f, -1.5f,  0.5f},
        { 1.0f, -2.5f,  2.0f, -0.5f},
        {-0.5f,  0.0f,  0.5f,  0.0f},
        { 0.0f,  1.0f,  0.0f,  0.0f}};
    
    
    // Calcular o ponto res = T * M * P
    // sendo Pi = p[indices[i]]
    
    
    //Sem derivada
   // for (i=0; i<4; i++)
     //   res_aux[i]= pow(t,3) * m[0][i] +  pow(t,2) * m[1][i] + t * m[2][i] + m[3][i];
    
    
    //Com derivada (Esta é a versão para ser utilizada a derivada dos pontos, mas ainda não está concluida)
    for (i=0; i<4; i++)
        res_aux[i]= 3*pow(t,2) * m[0][i] +  2*t * m[1][i] + m[2][i];
    
    
    //Calculo do RES
	for(i=0;i<3;i++){
        res[i]=res_aux[0]*p[indices[0]][i] + res_aux[1]*p[indices[1]][i] +res_aux[2]*p[indices[2]][i] + res_aux[3]*p[indices[3]][i];
    }
}


// given  global t, returns the point in the curve
void getGlobalCatmullRomPoint(float gt, float *res) {
    
	float t = gt * POINT_COUNT; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment
    
	// indices store the points
	int indices[4];
	indices[0] = (index + POINT_COUNT-1)%POINT_COUNT;	indices[1] = (indices[0]+1)%POINT_COUNT;
	indices[2] = (indices[1]+1)%POINT_COUNT; indices[3] = (indices[2]+1)%POINT_COUNT;
    
	getCatmullRomPoint(t, indices, res);
}

void renderCatmullRomCurve() {
    float  gtt;
    float res[3];
    // desenhar a curva usando segmentos de reta - GL_LINE_LOOP
    
    glBegin(GL_LINE_LOOP);
    for (gtt=0; gtt<1; gtt+=0.0001){
        getGlobalCatmullRomPoint(gtt,res);
        glVertex3fv(res);
    }
    glEnd();
}

void changeSize(int w, int h) {
    
	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;
    
	// compute window's aspect ratio
	float ratio = w * 1.0 / h;
    
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    
	// Set the correct perspective
	gluPerspective(45,ratio,1,1000);
    
	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void renderScene(void) {
    
	static float a = 0;
	float res[3];
    
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);
    
	renderCatmullRomCurve();
    
	glPushMatrix();
	getGlobalCatmullRomPoint(a,res);
	glTranslatef(res[0],res[1],res[2]);
	glScalef(0.1, 0.1, 0.1);
	glutWireTeapot(1);
	glPopMatrix();
    
    // End of frame
	glutSwapBuffers();
    
	a-=0.001;
    
    //Com derivada
    //a+= 0.01/length(derivada);
}


void processMouseButtons(int button, int state, int xx, int yy)
{
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			
			r -= yy - startY;
			if (r < 3)
				r = 3.0;
		}
		tracking = 0;
	}
}


void processMouseMotion(int xx, int yy)
{
	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;
    
	if (!tracking)
		return;
    
	deltaX = xx - startX;
	deltaY = yy - startY;
    
	if (tracking == 1) {
        
		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;
        
		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;
        
		rAux = r;
	}
	else if (tracking == 2) {
        
		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY;
		if (rAux < 3)
			rAux = 3;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux *							     sin(betaAux * 3.14 / 180.0);
}




int main(int argc, char **argv) {
    
    // pÙr inicializaÁ„o aqui
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(1200,800);
    glutCreateWindow("CG@DI");
    
    // registo de funÁıes
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
    
    // pÙr aqui registo da funÁıes do rato
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
    
    //MENU
    glutCreateMenu(front_menu);
    glutAddMenuEntry("GL POINT",1);
    glutAddMenuEntry("GL LINE",2);
    glutAddMenuEntry("GL FILL",3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    
    // alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    
    // entrar no ciclo do GLUT aqui
	glutMainLoop();
    
    return 0;
    
}

