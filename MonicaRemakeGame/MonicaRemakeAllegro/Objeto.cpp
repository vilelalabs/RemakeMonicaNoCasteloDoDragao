#include "stdafx.h"
#include "Objeto.h"

// Construtores

Objeto::Objeto()
{
}

Objeto::Objeto(const char *arquivo, int width, int height, float posX, float posY, float xSpeed, float ySpeed, int bordaX, int bordaY)
{
	this->setImagem(arquivo);
	this->setPosX(posX);
	this->setPosY(posY);
	this->setWidth(width);
	this->setHeight(height);

	this->bordaX = bordaX;
	this->bordaY = bordaY;
	
	this->xSpeed = xSpeed;
	this->ySpeed = ySpeed;

	this->setDir(3);
	this->setPosXant(posX);
	this->setPosYant(posY);

	caindo	= true;
	pulando = false;
	this->setVelPulo(3);
	pLimit	= 0;
	altPulo = 12; // altura do pulo

}


//Getters and Setters-------------------------------------------------------------------

int Objeto::getDir()
{
	return this->dir;
}
void Objeto::setDir(int dir)
{
	dir -= 2; // altera para imprimir flip do bitma corretamente ver Jogo.cpp linha 34

	this->dir = dir;
}

float Objeto::getPosXant()
{
	return this->posXant;
}

void Objeto::setPosXant(float posXant)
{
	this->posXant = posXant;
}

float Objeto::getPosYant()
{
	return this->posYant;
}

void Objeto::setPosYant(float posYant)
{
	this->posYant = posYant;
}


float Objeto::getXspeed()
{
	return this->xSpeed;
}
void Objeto::setXspeed(float xSpeed)
{
	this->xSpeed = xSpeed;
}

float Objeto::getYspeed()
{
	return this->ySpeed;
}
void Objeto::setYspeed(float ySpeed)
{
	this->ySpeed = ySpeed;
}

bool Objeto::getCaindo()
{
	return this->caindo;
}
void Objeto::setCaindo(bool caindo)
{
	this->caindo = caindo;
}

bool Objeto::getPulando()
{
	return this->pulando;
}
void Objeto::setPulando(bool pulando)
{
	this->pulando = pulando;
}

int Objeto::getAltPulo()
{
	return this->altPulo;
}
void Objeto::setAltPulo(int altPulo)
{
	this->altPulo = altPulo;
}

float Objeto::getVelPulo()
{
	return this->velPulo;
}

void Objeto::setVelPulo(float velPulo)
{
	this->velPulo = velPulo;
}

int Objeto::getPLimit()
{
	return this->pLimit;
}
void Objeto::setPLimit(int pLimit)
{
	this->pLimit = pLimit;
}

int Objeto::getBordaX()
{
	return this->bordaX;
}
void Objeto::setBordaX(int bordaX)
{
	this->bordaX = bordaX;
}
int Objeto::getBordaY()
{
	return this->bordaY;
}
void Objeto::setBordaY(int bordaY)
{
	this->bordaY = bordaY;
}


//--------------------------------------------------------------------------------------------

void Objeto::cair() {

		this->setPulando(false);
		this->setYspeed(this->getYspeed() + GRAVIDADE);
		this->setPosY(this->getPosY() + this->getYspeed());
}

void Objeto::pular() {
	if ((!this->getPulando()) && (this->getYspeed() == 0)) {
		this->setPLimit(this->getPosY());
		this->setPulando(true);
	}

}


//Destrutor
Objeto::~Objeto()
{

}