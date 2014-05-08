//
//  controlos.cpp
//  CG-Aula06
//
//  Created by Hélder José Alves Gonçalves on 03/04/14.
//  Copyright (c) 2014 Hélder José Alves Gonçalves. All rights reserved.
//

#include "controlos.h"



void teclado_normal(unsigned char tecla,int x, int y){
    switch (tecla) {
        case 'a':
            raio-=1;
            break;
        case 'd':
            raio+=1;
            break;
            
        default:
            break;
    }
    //glutPostRedisplay();
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
    //glutPostRedisplay();
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
    //glutPostRedisplay();
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
        
        //glutPostRedisplay();
    }
}
