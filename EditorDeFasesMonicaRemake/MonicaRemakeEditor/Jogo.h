#pragma once
#include "stdafx.h"
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>


#include "Colors.h"

#include "Objeto.h"
#include "Tile.h"
#include "Map.h"


#include <cstdlib>		// para gerar numero randomico sran e rand
#include <ctime>		// função time (gerar numero sempre diferente qdo o jogo inicia) - gera aleatoriedade

//largura e altura da resolução de tela
#define SCREEN_W	576
#define SCREEN_H	300

#define MENU_H		60		// altura do menu na tela

#define TILE_GRABBER_LIN 3	// linhas no menu para pegar os tiles
#define TILE_GRABBER_COL 32 // colunas no menu para pegar os tiles

//escala de "distorção" da tela real com a tela projetada (torna SCREEN_any = SCREEN_any*ESCALA;)
#define ESCALA   6



class Jogo
{
public:
	
	//inicialiações, construtor e destrutor
	Jogo();
	~Jogo();
	

	void init();
	//funções do programa principal
	void monitoraEventos();
	void input();
	void update();
	void render();
	bool isRunning();

	//funções auxiliares
	void cameraUpdateEditor(float * cameraPosition);
	void getVirtualMousePosition(int & ret_virtualMouseX, int & ret_virtualMouseY);
	void getTilePosition(int virt_mouseX, int virt_mouseY, int cameraX, int cameraY, int &ret_tilePosX, int &ret_tilePosY);
	
};

