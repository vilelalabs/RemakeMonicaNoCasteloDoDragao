#include "stdafx.h"
#include "Imagem.h"


Imagem::Imagem() {}

Imagem::Imagem(const char *arquivo, float posX, float posY, float w, float h )
{
	this->setImagem(arquivo);
	this->posX = posX;
	this->posY = posY;
	this->w = w; // al_get_bitmap_width(this->getImagem());
	this->h = h; // al_get_bitmap_height(this->getImagem());
}


ALLEGRO_BITMAP * Imagem::getImagem()
{
	return this->imagem;
}
void Imagem::setImagem(const char *arquivo)
{
	imagem = al_load_bitmap(arquivo);
}


float Imagem::getPosX()
{
	return this->posX;
}
void Imagem::setPosX(float posX)
{

	this->posX = posX;
}

float Imagem::getPosY()
{
	return this->posY;
}
void Imagem::setPosY(float posY)
{
	this->posY = posY;
}

float Imagem::getWidth()
{
	return this->w;
}
void Imagem::setWidth(float w)
{
	this->w = w;
}

float Imagem::getHeight()
{
	return this->h;
}
void Imagem::setHeight(float h)
{
	this->h = h;
}


Imagem::~Imagem()
{
	//destrói o bmp do objeto
	al_destroy_bitmap(imagem);
}
