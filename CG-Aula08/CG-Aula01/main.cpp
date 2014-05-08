//
//  main.cpp
//  CG-Aula01
//
//  Created by Hélder José Alves Gonçalves on 27/02/14.
//  Copyright (c) 2014 Hélder José Alves Gonçalves. All rights reserved.
//

#include <iostream>
#include <glew.h>
#include "controlos.h"

float raio=10,cam_h=0.5,cam_v=0.3,camh_x=0,camh_y=0,x_tela, y_tela;
float pos[4]={4,3,0,1},
amb[3]={0,0,0.5},
diff[3]={1,1,1},
matt[4]={0,0,1,1};

//Variaveis para a VBO
int n_pontos;
GLuint buffer[2];


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
    int lados=40, i;
    float angulo=2*M_PI/lados, laux1, laux2=0;
    
    glBegin(GL_TRIANGLES);
    for(i=0;i<lados;i++){
        laux1=laux2;
        laux2+=angulo;
        //Base Superior
        
        //A normal é igual para todos os pontos
        glNormal3f(0, 1, 0);
        
        glVertex3f(0, alt, 0);
        glVertex3f(sin(laux1), alt, cos(laux1));
        glVertex3f(sin(laux2), alt, cos(laux2));
        
        //Base Inferior
        //A normal é igual para todos os pontos
        glNormal3f(0, -1, 0);
        
        glVertex3f(0, 0, 0);
        glVertex3f(sin(laux2), 0, cos(laux2));
        glVertex3f(sin(laux1), 0, cos(laux1));
        
        //Dois triângulos para os lados
        //Triangulo 1
        glNormal3f(sin(laux1), 0, cos(laux1));
        glVertex3f(sin(laux1), alt, cos(laux1));
        
        glVertex3f(sin(laux1), 0, cos(laux1));
        glNormal3f(sin(laux2), 0, cos(laux2));
        glVertex3f(sin(laux2), 0, cos(laux2));
        
        //Triangulo 1
        glNormal3f(sin(laux1), 0, cos(laux1));
        glVertex3f(sin(laux1), alt, cos(laux1));
        
        glNormal3f(sin(laux2), 0, cos(laux2));
        glVertex3f(sin(laux2), 0, cos(laux2));
        glVertex3f(sin(laux2), alt, cos(laux2));
    }
    glEnd();
}

//Contrução da VBO do cilindro. Esta função só é chamada uma única vez.
void cilindroVBO(float raio, float alt, int lados){
    
    int i=0,n=0;
    float *vertexB=NULL, *normalB=NULL;
    float angulo=2*M_PI/lados, laux1, laux2=0;
    
    //Numero de coordenadas no array (4 triangulos * lados * nº de pontos de cada triangulo + nº de coordenadas de cada ponto)
    n_pontos=4*lados*3*3;
    vertexB=(float*)malloc(n_pontos*sizeof(float));
    normalB=(float*)malloc(n_pontos*sizeof(float));
    
    
    //Activar Buffers
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    for(;lados>0;lados--){
        laux1=laux2;laux2+=angulo;
        
        //Base Superior
        vertexB[i++]=0;vertexB[i++]=alt;vertexB[i++]=0;
        vertexB[i++]=sin(laux1);vertexB[i++]=alt;vertexB[i++]=cos(laux1);
        vertexB[i++]=sin(laux2);vertexB[i++]=alt;vertexB[i++]=cos(laux2);
        
        normalB[n++]=0;normalB[n++]=1;normalB[n++]=0;
        normalB[n++]=0;normalB[n++]=1;normalB[n++]=0;
        normalB[n++]=0;normalB[n++]=1;normalB[n++]=0;
        
        //Base Inferior
        vertexB[i++]=0;vertexB[i++]=0;vertexB[i++]=0;
        vertexB[i++]=sin(laux2);vertexB[i++]=0;vertexB[i++]=cos(laux2);
        vertexB[i++]=sin(laux1);vertexB[i++]=0;vertexB[i++]=cos(laux1);

        normalB[n++]=0;normalB[n++]=-1;normalB[n++]=0;
        normalB[n++]=0;normalB[n++]=-1;normalB[n++]=0;
        normalB[n++]=0;normalB[n++]=-1;normalB[n++]=0;
        
        
        //Dois triângulos para os lados
        vertexB[i++]=sin(laux1);vertexB[i++]=alt;vertexB[i++]=cos(laux1);
        vertexB[i++]=sin(laux1);vertexB[i++]=0;vertexB[i++]=cos(laux1);
        vertexB[i++]=sin(laux2);vertexB[i++]=0;vertexB[i++]=cos(laux2);
        
        normalB[n++]=sin(laux1);normalB[n++]=0;normalB[n++]=cos(laux1);
        normalB[n++]=sin(laux1);normalB[n++]=0;normalB[n++]=cos(laux1);
        normalB[n++]=sin(laux2);normalB[n++]=0;normalB[n++]=cos(laux2);
        
        vertexB[i++]=sin(laux1);vertexB[i++]=alt;vertexB[i++]=cos(laux1);
        vertexB[i++]=sin(laux2);vertexB[i++]=0;vertexB[i++]=cos(laux2);
        vertexB[i++]=sin(laux2);vertexB[i++]=alt;vertexB[i++]=cos(laux2);
        
        normalB[n++]=sin(laux1);normalB[n++]=0;normalB[n++]=cos(laux1);
        normalB[n++]=sin(laux2);normalB[n++]=0;normalB[n++]=cos(laux2);
        normalB[n++]=sin(laux2);normalB[n++]=0;normalB[n++]=cos(laux2);
        
    }
    
    //Aqui dizemos qual é GLuint que vamos usar e quandos buffers tem
    glGenBuffers(2, buffer);
    
    // Informamos qual vai ser o buffer que vamos usar para guardar a VBO
    glBindBuffer(GL_ARRAY_BUFFER,buffer[0]);
    
    //Temos 2 campos importantes (2º e 3º), no 2º metemos a memória necessária para guardar todas as coordenadas, e no 3º informamos o array que tem as coordenadas
    glBufferData(GL_ARRAY_BUFFER, n_pontos*sizeof(float), vertexB, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER,buffer[1]);
    glBufferData(GL_ARRAY_BUFFER, n_pontos*sizeof(float), normalB,GL_STATIC_DRAW);
    
    free(vertexB);
    free(normalB);
}

void desenharVBO(){
    
    //Indicar para cada buffer qual a sua utilização e composição
    glBindBuffer(GL_ARRAY_BUFFER,buffer[0]);
    glVertexPointer(3,GL_FLOAT,0,0);
    glBindBuffer(GL_ARRAY_BUFFER,buffer[1]);
    glNormalPointer(GL_FLOAT,0,0);
    
    glDrawArrays(GL_TRIANGLES, 0, n_pontos);
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
    
    //Posição para as Luzes (Experimente definir a posição da Luz antes e depois da gluLookAt)
    glLightfv(GL_LIGHT0, GL_POSITION,pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
    
    
    //Material
    glMaterialfv(GL_FRONT,GL_SPECULAR,matt);
    
    //Real-Time
    //cilindro(1, 2);
    
    //VBO
    desenharVBO();

	// End of frame
	
    glutSwapBuffers();
}




int main(int argc, char **argv) {
    
    // pÙr inicializaÁ„o aqui
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(1200,800);
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
    

    //Callback do GLEW - Tem de estar depois de todos os callbacks do GLUT
    glewInit();
    
    // alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
    
    //Construir VBO || Esta função só é necessária chamar 1 vez
    cilindroVBO(2, 1,30);
    
    //Luzes
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // entrar no ciclo do GLUT aqui
	glutMainLoop();
    
    return 0;
    
}

