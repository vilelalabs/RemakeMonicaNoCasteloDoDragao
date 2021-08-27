#pragma once
#include "stdafx.h"
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>


#include "Colors.h"


#include "Tile.h"
#include "Map.h"
#include "PlataformaMovel.h" //contem o #include "Objeto.h"


#include <cstdlib>		// para gerar numero randomico sran e rand
#include <ctime>		// fun��o time (gerar numero sempre diferente qdo o jogo inicia) - gera aleatoriedade

//largura e altura da resolu��o de tela
#define SCREEN_W	320
#define SCREEN_H	240
#define ESCALA		8

class Jogo
{
public:
	
	//inicialia��es, construtor e destrutor
	Jogo();
	~Jogo();
	
	//testes de fun��es de colis�o
	int colisaoTotal(Objeto* objA, Objeto* objB);				
	int colisaoSeletiva(Objeto * objA, Objeto * objB, int lado);
	int colisaoPlatMov(Objeto * objA, PlataformaMovel * objB);
	int colisaoMapa(Objeto * obj, Map *mapa, int index_x, int index_y);  // exclusivo para colis�o com os blocos do mapa //usando - une colisaoTotal e colisaoSeletiva
	int colisaoBox(Objeto* objA, Objeto* objB);


	void init();
	//fun��es do programa principal
	void monitoraEventos();
	void input();
	void update();
	void render();
	bool isRunning();

	//fun��es auxiliares
	void cameraUpdate(float * cameraPosition, float playerX, float playerY, int playerW, int playerH);
	
};

