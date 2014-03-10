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

float raio=5,cam_h=0,cam_v=0.5;

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

void cilindro(float raio, float alt){
    int lados=20, i;
    float angulo=2*M_PI/lados, laux1, laux2=0;

    glBegin(GL_TRIANGLES);
    for(i=0;i<lados;i++){
        laux1=laux2;laux2+=angulo;
        //Base Superior
        glColor3f(0, 1, 0);
        glVertex3f(0, alt, 0);
        glVertex3f(sin(laux1), alt, cos(laux1));
        glVertex3f(sin(laux2), alt, cos(laux2));
        
        //Base Inferior
        glVertex3f(0, 0, 0);
        glVertex3f(sin(laux2), 0, cos(laux2));
        glVertex3f(sin(laux1), 0, cos(laux1));
        
        //Dois triângulos para os lados
        glColor3f(1, 1, 0);
        glVertex3f(sin(laux1), alt, cos(laux1));
        glVertex3f(sin(laux1), 0, cos(laux1));
        glVertex3f(sin(laux2), 0, cos(laux2));
        
        glColor3f(1, 0.5, 0);
        glVertex3f(sin(laux1), alt, cos(laux1));
        glVertex3f(sin(laux2), 0, cos(laux2));
        glVertex3f(sin(laux2), alt, cos(laux2));
    }
    glEnd();
}

void renderScene(void) {
    
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	// set the camera
	glLoadIdentity();
    
    //Câmera em modo explorador
	gluLookAt(raio*sin(cam_h)*cos(cam_v),raio*sin(cam_v),raio*cos(cam_h)*cos(cam_v),
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);
    
    // pÙr instruÁıes de desenho aqui
    
    cilindro(2, 1);
    
    
	// End of frame
	
    glutSwapBuffers();
}

void teclado_normal(unsigned char tecla,int x, int y){
    switch (tecla) {
        case 'a':
            raio-=0.1;
            break;
        case 'd':
            raio+=0.1;
            break;
            
        default:
            break;
    }
    glutPostRedisplay();
}

void teclado_especial(int tecla,int x, int y){
    switch (tecla) {
        case GLUT_KEY_UP:
            if(cam_v+0.05<M_PI_2)   //Para câmera não virar ao contrário
                cam_v+=0.05;
            break;
        case GLUT_KEY_DOWN:
            if(cam_v-0.05>-M_PI_2)  //Para câmera não virar ao contrário
                cam_v-=0.05;
            break;
            
        case GLUT_KEY_LEFT:
            cam_h-=0.05;
            break;
        case GLUT_KEY_RIGHT:
            cam_h+=0.05;
            break;
            
        default:
            break;
    }
    glutPostRedisplay();
}

void front_menu(int op){
    switch (op) {
        case 1:
            glPolygonMode(GL_FRONT,GL_POINT);
            break;
        case 2:
            glPolygonMode(GL_FRONT,GL_LINE);
            break;
        case 3:
            glPolygonMode(GL_FRONT,GL_FILL);
            break;
        default:
            break;
    }
    glutPostRedisplay();
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
    
    // funções do teclado e rato
	glutKeyboardFunc(teclado_normal);
    glutSpecialFunc(teclado_especial);
    
    
    //MENU
    glutCreateMenu(front_menu);
    glutAddMenuEntry("GL POINT",1);
    glutAddMenuEntry("GL LINE",2);
    glutAddMenuEntry("GL FILL",3);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    
    // alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
    
    // entrar no ciclo do GLUT aqui
	glutMainLoop();
    
    return 0;
    
}

