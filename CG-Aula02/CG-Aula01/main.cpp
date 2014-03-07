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

#include <time.h>

float alt_cam=5, alt=2, translate_x=0, rotate=0;

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

//Piramide com Base no plano XOZ e centro da base na origem
void piramide(float lado,float alt){
    
    glBegin(GL_TRIANGLES);
    //LADOS
    glColor3f(1, 0, 0);
    glVertex3f(0, alt, 0);
    glVertex3f(-lado, 0, lado);
    glVertex3f(lado, 0, lado);
    
    glColor3f(0, 1, 0);
    glVertex3f(0, alt, 0);
    glVertex3f(lado, 0, lado);
    glVertex3f(lado, 0, -lado);
    
    glColor3f(0, 0, 1);
    glVertex3f(0, alt, 0);
    glVertex3f(lado, 0, -lado);
    glVertex3f(-lado, 0, -lado);
    
    
    glColor3f(0, 1, 1);
    glVertex3f(0, alt, 0);
    glVertex3f(-lado, 0, -lado);
    glVertex3f(-lado, 0, lado);
    
    //Base
    glColor3f(0.3,0.3,0.3);
    glVertex3f(lado, 0, -lado);
    glVertex3f(-lado, 0, lado);
    glVertex3f(-lado, 0, -lado);
    
    glVertex3f(lado, 0, -lado);
    glVertex3f(lado, 0, lado);
    glVertex3f(-lado, 0, lado);
    
    glEnd();
}

void renderScene(void) {
    
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	// set the camera
	glLoadIdentity();
    
    //Temos a câmera a andar à volta do centro carteziano
	gluLookAt(0,alt_cam,10,
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);
    
    // pÙr instruÁıes de desenho aqui
    glTranslatef(translate_x, 0, 0);
    glRotatef(rotate, 0, 1, 0);
  
    
    piramide(2,alt);
    

	// End of frame
	
    glutSwapBuffers();
}



void teclado_normal(unsigned char tecla,int x, int y){
    switch (tecla) {
        case 'a':
            translate_x-=0.1;
            break;
        case 'd':
            translate_x+=0.1;
            break;
        case 'w':
            alt+=0.1;
            break;
        case 's':
            alt-=0.1;
            break;
            
        default:
            break;
    }
    glutPostRedisplay();
}

void teclado_especial(int tecla,int x, int y){
    switch (tecla) {
        case GLUT_KEY_UP:
            alt_cam++;
            break;
        case GLUT_KEY_DOWN:
            alt_cam--;
            break;
            
        case GLUT_KEY_LEFT:
            rotate-=5;
            break;
        case GLUT_KEY_RIGHT:
            rotate+=5;
            break;

        default:
            break;
    }
    glutPostRedisplay();
}

void menu(int op){
    
    
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

void back_menu(int op){
    switch (op) {
        case 1:
            glPolygonMode(GL_BACK,GL_POINT);
            break;
        case 2:
            glPolygonMode(GL_BACK,GL_LINE);
            break;
        case 3:
            glPolygonMode(GL_BACK,GL_FILL);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void cull_menu(int op){
    switch (op) {
        case 1:
            glEnable(GL_CULL_FACE);
            break;
        case 2:
            glDisable(GL_CULL_FACE);
            break;
        case 3:
            glCullFace(GL_FRONT);
            break;
        case 4:
            glCullFace(GL_BACK);
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
    
    
    //MENUS
    int id_backmenu,id_Cullmenu,
    
    id_frontmenu= glutCreateMenu(front_menu);
    glutAddMenuEntry("GL POINT",1);
    glutAddMenuEntry("GL LINE",2);
    glutAddMenuEntry("GL FILL",3);
    
    id_backmenu= glutCreateMenu(back_menu);
    glutAddMenuEntry("GL POINT",1);
    glutAddMenuEntry("GL LINE",2);
    glutAddMenuEntry("GL FILL",3);
    
    id_Cullmenu= glutCreateMenu(cull_menu);
    glutAddMenuEntry("Activar",1);
    glutAddMenuEntry("Desactivar",2);
    glutAddMenuEntry("FRONT",3);
    glutAddMenuEntry("BACK",4);
    
    glutCreateMenu(menu);
    glutAddSubMenu("GL FRONT",id_frontmenu);
    glutAddSubMenu("GL BACK",id_backmenu);
    glutAddSubMenu("GL CULL FACE",id_Cullmenu);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
    

    // alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
    
    // entrar no ciclo do GLUT aqui
	glutMainLoop();
    
    return 0;
}

