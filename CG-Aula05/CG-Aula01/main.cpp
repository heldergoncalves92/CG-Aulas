//
//  main.cpp
//  CG-Aula01
//
//  Created by Hélder José Alves Gonçalves on 27/02/14.
//  Copyright (c) 2014 Hélder José Alves Gonçalves. All rights reserved.
//


// Nota: A biblioteca 'glew.h' tem de preceder à biblioteca 'glut.h'
#include <GL/glew.h>
#include <GLUT/glut.h>
#include <iostream>
#include <math.h>

float raio=5,cam_h=0,cam_v=0.5, n_pontos;
GLuint buffer[1];

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

//Desenho do cilindro no modo imediato
void cilindro(float raio, float alt, int lados){
    int i;
    float angulo=2*M_PI/lados, laux1, laux2=0;
    
    glBegin(GL_TRIANGLES);
    for(i=0;i<lados;i++){
        laux1=laux2;
        laux2+=angulo;
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

//Contrução da VBO do cilindro. Esta função só é chamada uma única vez.
void cilindroVBO(float raio, float alt, int lados){
 
    int i=0;
    float *vertexB=NULL;
    float angulo=2*M_PI/lados, laux1, laux2=0;
    
    //Numero de coordenadas no array (4 triangulos * lados * nº de pontos de cada triangulo + nº de coordenadas de cada ponto)
    n_pontos=4*lados*3*3;
    vertexB=(float*)malloc(n_pontos*sizeof(float));
    
    
    //Activar Buffers
    glEnableClientState(GL_VERTEX_ARRAY);
    
    for(;lados>0;lados--){
        laux1=laux2;laux2+=angulo;
        
        //Base Superior
        vertexB[i++]=0;vertexB[i++]=alt;vertexB[i++]=0;
        vertexB[i++]=sin(laux1);vertexB[i++]=alt;vertexB[i++]=cos(laux1);
        vertexB[i++]=sin(laux2);vertexB[i++]=alt;vertexB[i++]=cos(laux2);
        
        //Base Inferior
        vertexB[i++]=0;vertexB[i++]=0;vertexB[i++]=0;
        vertexB[i++]=sin(laux2);vertexB[i++]=0;vertexB[i++]=cos(laux2);
        vertexB[i++]=sin(laux1);vertexB[i++]=0;vertexB[i++]=cos(laux1);

        
        //Dois triângulos para os lados
        vertexB[i++]=sin(laux1);vertexB[i++]=alt;vertexB[i++]=cos(laux1);
        vertexB[i++]=sin(laux1);vertexB[i++]=0;vertexB[i++]=cos(laux1);
        vertexB[i++]=sin(laux2);vertexB[i++]=0;vertexB[i++]=cos(laux2);
        
        vertexB[i++]=sin(laux1);vertexB[i++]=alt;vertexB[i++]=cos(laux1);
        vertexB[i++]=sin(laux2);vertexB[i++]=0;vertexB[i++]=cos(laux2);
        vertexB[i++]=sin(laux2);vertexB[i++]=alt;vertexB[i++]=cos(laux2);
        
    }
    
    //Aqui dizemos qual é GLuint que vamos usar e quandos buffers tem
    glGenBuffers(1, buffer);
    
    // Informamos qual vai ser o buffer que vamos usar para guardar a VBO
    glBindBuffer(GL_ARRAY_BUFFER,buffer[0]);
    
    //Temos 2 campos importantes (2º e 3º), no 2º metemos a memória necessária para guardar todas as coordenadas, e no 3º informamos o array que tem as coordenadas
    glBufferData(GL_ARRAY_BUFFER, n_pontos*sizeof(float), vertexB, GL_STATIC_DRAW);
    
    free(vertexB);
}

void desenharVBO(){
    glBindBuffer(GL_ARRAY_BUFFER,buffer[0]);
    glVertexPointer(3,GL_FLOAT,0,0);
    glDrawArrays(GL_TRIANGLES, 0, n_pontos);
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
    
    desenharVBO();
    
   //Medir FPS (Só dá para ver direito de desligarmos o V-Sync)
    frame++;
    times=glutGet(GLUT_ELAPSED_TIME);
    if (times - timebase > 1000) {
        fps = frame*1000.0/(times-timebase);
        timebase = times;
        frame = 0; 
    }
    sprintf(print, "%d",fps);
    glutSetWindowTitle(print);

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
    glutIdleFunc(renderScene);
    
    // funções do teclado e rato
	glutKeyboardFunc(teclado_normal);
    glutSpecialFunc(teclado_especial);
    
    
    //MENU
    glutCreateMenu(front_menu);
    glutAddMenuEntry("GL POINT",1);
    glutAddMenuEntry("GL LINE",2);
    glutAddMenuEntry("GL FILL",3);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    //Callback do GLEW - Tem de estar depois de todos os callbacks do GLUT
    glewInit();
    
    //Construir VBO || Esta função só é necessária chamar 1 vez
    cilindroVBO(2, 1,30);
    
    // alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
    
    glPolygonMode(GL_FRONT, GL_LINE);
    // entrar no ciclo do GLUT aqui
	glutMainLoop();
    
    return 0;
    
}

