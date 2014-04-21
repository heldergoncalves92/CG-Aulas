//
//  controlos.h
//  CG-Aula06
//
//  Created by Hélder José Alves Gonçalves on 03/04/14.
//  Copyright (c) 2014 Hélder José Alves Gonçalves. All rights reserved.
//

#ifndef __CG_Aula06__controlos__
#define __CG_Aula06__controlos__


#include <iostream>
#include <GLUT/glut.h>
#include <math.h>

extern  float raio,cam_h,cam_v,camh_x,camh_y;
extern  float x_tela, y_tela; //Variaveis para guardar posição da tela em que se carrega no rato

static int estado_botao=0;

void teclado_normal(unsigned char tecla,int x, int y);
void teclado_especial(int tecla,int x, int y);
void front_menu(int op);
void rato(int botao, int estado, int x, int y);
void mov_rato(int x, int y);

#endif /* defined(__CG_Aula06__controlos__) */
