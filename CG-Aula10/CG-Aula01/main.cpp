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

float raio=200,cam_h=0,cam_v=0.3,camh_x=0,camh_y=0,x_tela, y_tela;

//Angulo de rotação dos centros
float cir1=0,cir2=0;

//LUZES
float pos[4]={0,60,256,1},
diff[3]={1,1,1},
amb[3]={0.2,0.2,0.2},
castanho[4]={0.7, 0.3, 0,1},
verde[4]={0, 0.5, 0,1},
amarelo[4]={0.6, 0.6, 0.2,1},
verde2[4]={0.0, 0.6, 0.2,1},
v_terreno[4]={0.7, 1, 0.7,1},
vermelho[4]={1, 0, 0,1};


//Variaveis para a VBO
int n_pontos;
float resN[3]={0,1,0};
GLuint buffer[3];

//Inicializações para carregar a Imagem
unsigned int t,texID, tw, th;
unsigned char *imageData, *texData;


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

void normal(int coluna, int linha){
    float v1[3],v2[3], modulo;
    v1[0]=v2[2]=0;
    v1[2]=v2[0]=2;
    
    if(coluna==0)
        coluna++;
    else if(coluna==tw)
            coluna--;
    
    if(linha==0)
        linha++;
    else if(linha==tw)
        linha--;
    
    v1[1]=h(coluna+1,linha) - h(coluna-1, linha);
    v2[1]=h(coluna,linha+1) - h(coluna, linha-1);
   
    
    //Vector Normal
    resN[0]= (v1[1] * v2[2]) - (v1[2] * v2[1]);
    resN[1]= (v1[2] * v2[0]) - (v1[0] * v2[2]);
    resN[2]= (v1[0] * v2[1]) - (v1[1] * v2[0]);
    
    //Modulo do vector
    modulo= sqrtf(powf(resN[0], 2) + powf(resN[1], 2)+ powf(resN[2], 2));
    
    //Vector Unitário
    resN[0]/=modulo;
    resN[1]/=modulo;
    resN[2]/=modulo;
        
    
}

//VBO do terreno
void construirVBO(float altura, float largura){
    float *vertexB=NULL, *normalB=NULL, *textB=NULL,altura_aux=altura,largura_aux, textI=0, textJ=0, texINC=1.0/5 ;
    int coluna,linha,i=0,n=0,t=0;
    
    altura/=th;
    largura/=tw;
    
    //Numero de coordenadas no array
    n_pontos=(tw*2*(th-1))*3;
    vertexB=(float*)malloc(n_pontos*sizeof(float));
    normalB=(float*)malloc(n_pontos*sizeof(float));
    textB=(float*)malloc((tw*2*(th-1))*2*sizeof(float));
    
    
    
    //Activar Buffers
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    //A cada iteração vamos buscar a altura dada pela função h(x,y)
    for (linha=0; linha<th-1; linha++) {
        largura_aux=0; textI=0;
        for (coluna=0; coluna<tw; coluna++) {
            //Vertice 1
            vertexB[i++]=altura_aux;
            vertexB[i++]=h(coluna,linha);
            vertexB[i++]=largura_aux;
            
            normal(coluna, linha);
            normalB[n++]=resN[0];
            normalB[n++]=resN[1];
            normalB[n++]=resN[2];
            
            textB[t++]=textI;
            textB[t++]=textJ;
            
            
            //Vertice 2
            vertexB[i++]=altura_aux-altura;
            vertexB[i++]=h(coluna,linha+1);
            vertexB[i++]=largura_aux;
            
            normal(coluna, linha+1);
            normalB[n++]=resN[0];
            normalB[n++]=resN[1];
            normalB[n++]=resN[2];
            
            textB[t++]=textI;
            textB[t++]=textJ+texINC;
            
            largura_aux+=largura;
            textI+=texINC;
        }
        textJ+=texINC;
        altura_aux-=altura;
    }
    
    //Aqui dizemos qual é GLuint que vamos usar e quandos buffers tem
    glGenBuffers(3, buffer);
    
    glBindBuffer(GL_ARRAY_BUFFER,buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, n_pontos*sizeof(float), vertexB, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER,buffer[1]);
    glBufferData(GL_ARRAY_BUFFER, n_pontos*sizeof(float), normalB,GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
    glBufferData(GL_ARRAY_BUFFER, (tw*2*(th-1))*2*sizeof(float), textB, GL_STATIC_DRAW);
    
    free(vertexB);
    free(normalB);
    free(textB);
    
    
}

void desenharVBO(){
    int i;
    glBindBuffer(GL_ARRAY_BUFFER,buffer[0]);
    glVertexPointer(3,GL_FLOAT,0,0);
    glBindBuffer(GL_ARRAY_BUFFER,buffer[1]);
    glNormalPointer(GL_FLOAT,0,0);
    glBindBuffer(GL_ARRAY_BUFFER,buffer[2]);
    glTexCoordPointer(2,GL_FLOAT,0,0);
    
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
    
    //Material
    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,castanho);
    glutSolidCone(0.1*tam, tam, 20, 10);
    
    glTranslatef(0, 0, 0.25*tam);
    x=rand()%3;
    switch (x) {
        case 0:
            glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,verde);
            break;
        case 1:
            glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,amarelo);
            break;
        case 2:
            glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,verde2);
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
        //glutSolidTeapot(1);
        glutSolidTetrahedron();
        glPopMatrix();
        glRotatef(angulo, 0, 1, 0);
    }
    glPopMatrix();
    
    //2º Circulo -- Teapot Vermelho virado para o circulo
    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, vermelho);
    glPushMatrix();
    glRotatef(cir2, 0, 1, 0);
    for(i=0;i<n_teapot;i++){
        glPushMatrix();
        glTranslatef(0, 1,32);
        
        //glutSolidTeapot(1);
        glutSolidTetrahedron();
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
	gluLookAt(30+raio*sin(cam_h+camh_x)*cos(cam_v+camh_y),raio*sin(cam_v+camh_y),30+raio*cos(cam_h+camh_x)*cos(cam_v+camh_y),
		      30.0,0.0,30.0,
			  0.0f,1.0f,0.0f);
    
    // por instruÁıes de desenho aqui
    
    //LUZES
    glLightfv(GL_LIGHT0, GL_POSITION,pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);

    //RELVADO
    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,v_terreno);
    glBindTexture(GL_TEXTURE_2D, texID);
    desenharVBO();
    glBindTexture(GL_TEXTURE_2D, 0);
    
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
    
    //Textura - Relva
    ilGenImages(1,&t);
    ilBindImage(t);
    ilLoadImage((ILstring)"relva.jpg");
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    texData = ilGetData();
    
    glGenTextures(1,&texID);
    glBindTexture(GL_TEXTURE_2D,texID);
    
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    

    
    //Terreno
    ilGenImages(1,&t);
    ilBindImage(t);
    ilLoadImage((ILstring)"terreno2.jpg");
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
    //imagem guardada no 'imageData'
    imageData = ilGetData();
    
    
    
}

void init(){
    
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
    glEnable(GL_TEXTURE_2D);
	
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    
    //Luzes
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
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
    
    init();
    
    // entrar no ciclo do GLUT aqui
	glutMainLoop();
    
    return 0;
    
}

