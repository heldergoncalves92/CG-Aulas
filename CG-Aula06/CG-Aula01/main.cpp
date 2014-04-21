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
#include <il.h>

float raio=200,cam_h=0.5,cam_v=0.3,camh_x=0,camh_y=0,x_tela, y_tela;

//Angulo de rotação dos centros
float cir1=0,cir2=0;

//Variaveis para a VBO
int n_pontos;
GLuint buffer[1];

//Inicializações para carregar a Imagem
unsigned int t, tw, th;
unsigned char *imageData;


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

//Numeros do imageData vão de 0 a 255
float h(int coluna, int linha){
    return imageData[linha*tw+coluna]/255.0 * 100;
}

//VBO do terreno
void construirVBO(float altura, float largura){
    float *vertexB=NULL, altura_aux=altura,largura_aux;
    int coluna,linha,i=0;
    
    altura/=th;
    largura/=tw;
    
    //Numero de coordenadas no array
    n_pontos=(tw*2*(th-1))*3;
    vertexB=(float*)malloc(n_pontos*sizeof(float));
    
    
    //Activar Buffers
    glEnableClientState(GL_VERTEX_ARRAY);
    
    //A cada iteração vamos buscar a altura dada pela função h(x,y)
    for (linha=0; linha<th-1; linha++) {
        largura_aux=0;
        for (coluna=0; coluna<tw; coluna++) {
            
            vertexB[i++]=altura_aux;
            vertexB[i++]=h(coluna,linha);
            vertexB[i++]=largura_aux;
            
            vertexB[i++]=altura_aux-altura;
            vertexB[i++]=h(coluna,linha+1);
            vertexB[i++]=largura_aux;
            
            largura_aux+=largura;
        }
        altura_aux-=altura;
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
    int i;
    glBindBuffer(GL_ARRAY_BUFFER,buffer[0]);
    glVertexPointer(3,GL_FLOAT,0,0);
    glPushMatrix();
    //Para ser mais facil de desenhar a VBO desenhei a parti de (0,0,0). Agora fazemos o translate para centrar o terreno
    glTranslatef(-(tw/2.0), 0, -(th/2.0));
   
    //Como estamos a desenhar no modo GL_TRIANGLE_STRIP temos de desenhar uma 'tira' de cada vez
    for (i=0; i<th; i++) {
        glDrawArrays(GL_TRIANGLE_STRIP, i*tw*2, 2*tw);
    }
    glPopMatrix();
    
}

void arvore(float tam){
    int x;
    glPushMatrix();
    //A função 'glutSolidCone' desenha o cone deitado. Como este é pretendido a pé usamos o rotate
    glRotatef(-90, 1, 0, 0);
    
    glColor3f(0.7, 0.3, 0);
    glutSolidCone(0.1*tam, tam, 20, 10);
    
    glTranslatef(0, 0, 0.25*tam);
    x=rand()%3;
    switch (x) {
        case 0:
            glColor3f(0, 0.5, 0);
            break;
        case 1:
            glColor3f(0.8, 1.0, 0.2);
            break;
        case 2:
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
        x=rand()%th/2-1;
        z=rand()%tw/2-1;
        
        //Caso tenha árvores com coordenadas que coincidem no circulo interior
        if(sqrt(x*x+z*z)<50){
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
        glTranslatef(x, h((z+tw/2),256-(x+th/2)), z); //Translate para a posição pretendida
        arvore(7);
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
        glTranslatef(0, 1,32);
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
    glColor3f(0.3, 0.7, 0);
    desenharVBO();
    
    //Colocar as arvores
    coloca_arvores(500);
    
    //DEsenho da parte central
    centro(15);
    
    //Mover circulos a cada re-desenho
    cir2++;
    cir1--;
	// End of frame
	
    glutSwapBuffers();
}



void carregarImagem(){
    
    ilGenImages(1,&t);
    ilBindImage(t);
    ilLoadImage((ILstring)"terreno2.jpg");
    //ilLoadImage((ILstring)"Isla Mágica.jpg");
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
    //imagem guardada no 'imageData'
    imageData = ilGetData();
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
    ilInit();
    
    //Carregar Imagem e construir a VBO
    carregarImagem();
    construirVBO(th,tw);
    
    // alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
    
    
    glPolygonMode(GL_FRONT,GL_POINT);
    
    // entrar no ciclo do GLUT aqui
	glutMainLoop();
    
    return 0;
    
}

