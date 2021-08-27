#pragma once
#include "stdafx.h"
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>
class Imagem
{
protected:
	ALLEGRO_BITMAP *imagem;
	float bordaX, bordaY;
	float posX, posY;		//posição na tela
	float w, h;				//largura e altura

public:
	Imagem();
	Imagem(const char *arquivo, float posX, float posY, float w, float h);



	ALLEGRO_BITMAP * getImagem();
	void setImagem(const char *arquivo);

	float	getPosX();
	void	setPosX(float posX);
	float	getPosY();
	void	setPosY(float posY);
	float	getWidth();
	void	setWidth(float w);
	float	getHeight();
	void	setHeight(float h);

	~Imagem();
};

