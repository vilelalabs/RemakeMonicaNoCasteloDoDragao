#pragma once

#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>

#include "Tile.h"

using namespace ::std;


enum TIPO_COLISAO { TC_SEM_COLISAO, TC_PLATAFORMA, TC_PAREDE }; // TC_PLATAFORMA:	(colide apenas de cima para baixo) ...
																// TC_PAREDE:		(colide dos 4 lados)
																// TC_SEM_COLISAO:(transparente para colisões com o personagem)

struct ARQUIVO {
	int 	lvl;
	int  sublvl;
	int     lin;
	int     col;
	int  **mapa;
};

struct BLOCO {
	int x;
	int y;
	TIPO_COLISAO tipo_colisao;
};



class Map
{
private: 
	int tamVertical, tamHorizontal; // guarda número (e dimensão) de tiles horizontais e verticais
	int **map;
	BLOCO **bloco;
	ALLEGRO_BITMAP * imagem;

public:
	Map();
	~Map();


	int getMap(int indexLin, int indexCol);
	BLOCO **getBloco();
	int getBlocoX(int indexLin, int indexCol);
	int getBlocoY(int indexLin, int indexCol);
	TIPO_COLISAO getBlocoTipoColisao(int indexLin, int indexCol);

	bool loadMap(const char *nome_arquivo, Tile *tile);

	//getters and *setters (apenas funções internas podem setar o tam vertical, horizontal e imagem)
	int getTamVertical();	// retirado setter para evitar erros com tamanho setado maior que memória reservada
	int getTamHorizontal(); // retirado setter para evitar erros com tamanho setado maior que memória reservada

	ALLEGRO_BITMAP * getImagem();


};

