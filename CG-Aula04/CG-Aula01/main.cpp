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
#include <stdlib.h>

float raio=180,cam_h=0,cam_v=0.5,camh_x=0,camh_y=0, cir1=0,cir2=0;
float x_tela, y_tela; //Variaveis para guardar posição da tela em que se carrega no rato

int estado_botao=0;

//Para ver o numero de FPS
int times ,timebase, frame=0, fps=0;
char print[20]="";


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

void arvore(float tam){
    int x;
    glPushMatrix();
    //A função 'glutSolidCone' desenha o cone deitado. Como este é pretendido a pé usamos o rotate
    glRotatef(-90, 1, 0, 0);
    
    glColor3f(0.7, 0.3, 0);
    glutSolidCone(0.1*tam, tam, 20, 10);
    
    glTranslatef(0, 0, 0.25*tam);
    
    x=rand()%2;
    switch (x) {
        case 0:
            glColor3f(0, 0.5, 0);
            break;

        case 1:
            glColor3f(0.0, 0.6, 0.2);
            break;
            
        default:
            break;
    }

    glutSolidCone(0.25*tam, 0.75*tam, 20, 10);

    glPopMatrix();
    
}

void coloca_arvores(int n_arvores){
    int x,z;
    srand(4); //Inicia a sequência aleatória para ser sempre a mesma a sequência a ser gerada
    while (n_arvores>0) {
        x=rand()%99;
        z=rand()%99;
        
        //Caso tenha árvores com coordenadas que coincidem no circulo interior
        //if(x<50 && z<50){
        if(sqrt(x*x+z*z)<45){
            if(rand()%2==0)
                x=rand()%49+50;
            else
                z=rand()%49+50;
        }
        
        if (rand()%2==0)
            x=-x;
        if (rand()%2==0)
            z=-z;

        //Fazemos um Push para preservar o ponto de origem
        glPushMatrix();
            glTranslatef(x, 0, z); //Translate para a posição pretendida
            arvore(5);
        glPopMatrix();
        n_arvores--;
    }
}

void centro(int n_teapot){
    int i;float angulo=360/n_teapot;
    //Torus Cor-de-Rosa
    glColor3f(1, 0, 0.3);
    glutSolidTorus(0.5, 1.5, 5, 20);
    
    //1º Circulo -- Teapot Azul virado para fora
    glPushMatrix();
    glRotatef(cir1, 0, 1, 0);
    glColor3f(0, 0, 1);
    for(i=0;i<n_teapot;i++){
        glPushMatrix();
            glTranslatef(15, 1,0);
            glutSolidTeapot(1);
        glPopMatrix();
        glRotatef(angulo, 0, 1, 0);
    }
    glPopMatrix();
    
    //2º Circulo -- Teapot Vermelho virado para o circulo
    glColor3f(1, 0, 0);
    glPushMatrix();
    glRotatef(cir2, 0, 1, 0);
    for(i=0;i<n_teapot;i++){
        glPushMatrix();
        glTranslatef(0, 1,35);
        glutSolidTeapot(1);
        glPopMatrix();
        glRotatef(angulo, 0, 1, 0);
    }
    glPopMatrix();
}

void renderScene(void) {
    
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	// set the camera
	glLoadIdentity();
    
    //Câmera em modo explorador (Câmera move-se nas bordas de 1 esfera)
	gluLookAt(raio*sin(cam_h+camh_x)*cos(cam_v+camh_y),raio*sin(cam_v+camh_y),raio*cos(cam_h+camh_x)*cos(cam_v+camh_y),
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);
    // por instruÁıes de desenho aqui
    
    //RELVADO
    glColor3f(0.3, 0.9, 0);
    glBegin(GL_TRIANGLES);
    glVertex3f(100, 0, 100);
    glVertex3f(100, 0, -100);
    glVertex3f(-100, 0, 100);

    glVertex3f(100, 0, -100);
    glVertex3f(-100, 0, -100);
    glVertex3f(-100, 0, 100);
    glEnd();
    
    //Colocar as arvores
    coloca_arvores(500);
    
    //DEsenho da parte central
    centro(15);
    
    //Mover circulos a cada re-desenho
    cir2++;
    cir1--;
	// End of frame
    
    //Brincadeira para ver o número de FPS, analisado ao pormenor na Aula 5
    frame++;
    times=glutGet(GLUT_ELAPSED_TIME);
    if (times - timebase > 1000) {
        fps = frame*1000.0/(times-timebase);
        timebase = times;
        frame = 0;
    }
    sprintf(print, "FPS: %d",fps);
    glutSetWindowTitle(print);
	
    glutSwapBuffers();
}

void teclado_normal(unsigned char tecla,int x, int y){
    switch (tecla) {
        case 'a':
            raio-=4;
            break;
        case 'd':
            raio+=4;
            break;
            
        default:
            break;
    }
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
}

void rato(int botao, int estado, int x, int y){
    if (botao==GLUT_LEFT_BUTTON){
        if (estado==GLUT_DOWN){
            estado_botao=1;
            x_tela=x;
            y_tela=y;
        }
        else{
            estado_botao=0;
            cam_v+=camh_y;
            cam_h+=camh_x;
            camh_x=0;
            camh_y=0;
        }
    }
}

void mov_rato(int x, int y){
    float teste;
    if(estado_botao){
        if(x_tela!=x)
            camh_x= (x_tela-x)*0.007;
        
        if(y_tela!=y){
            teste= (y_tela-y)*0.002;
            if(teste+cam_v>-M_PI_2 && teste+cam_v<M_PI_2 )
                camh_y=teste;
        }
        
    }
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
    
    // funções do teclado e rato
	glutKeyboardFunc(teclado_normal);
    glutSpecialFunc(teclado_especial);
    glutMouseFunc(rato);
    glutMotionFunc(mov_rato);
    
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

