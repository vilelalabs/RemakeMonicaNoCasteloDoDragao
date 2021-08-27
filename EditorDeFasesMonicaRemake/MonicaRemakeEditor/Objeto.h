#pragma once
#include "stdafx.h"
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_image.h>

#include "Imagem.h"

#define GRAVIDADE 0.2

class Objeto :  public Imagem
{

private:
	int   dir;				//direção de movimento
	float xSpeed, ySpeed;	//velocidade de movimento

	float posXant;
	float posYant;

	//manejo de queda e pulo
	bool  caindo;
	bool  pulando;
	float velPulo;
	int   altPulo;
	//auxiliar
	int pLimit;	//guarda posição Y do personagem durante pulo

	//int xdelay, ydelay;
	//int xcount, ycount;
	//int curframe, maxframe, animdir; // animdir = dir?;
	//int framecount, framedelay;
	//int animtype;			// tipo de animação :andando, pulando, atacando, levando dano, morrendo;

	

public:

	//construtores
	Objeto();
	Objeto(const char *arquivo, float posx, float posy, float xSpeed, float ySpeed, int bordaX, int bordaY);


	//getters and setters
	int		getDir();
	void	setDir(int dir);

	//utilizadas para o pulo e queda
	float	getPosXant();
	void	setPosXant(float posXant);
	float	getPosYant();
	void	setPosYant(float posYant);

	float	getXspeed();
	void	setXspeed(float xSpeed);
	float	getYspeed();
	void	setYspeed(float ySpeed);
	bool	getCaindo();
	void	setCaindo(bool caindo);
	bool	getPulando();
	void	setPulando(bool pulando);
	int		getAltPulo();
	void	setAltPulo(int altPulo);
	float	getVelPulo();
	void	setVelPulo(float velPulo);
	int		getPLimit();
	void	setPLimit(int pLimit);

	int		getBordaX();
	void	setBordaX(int bordaX);
	int		getBordaY();
	void	setBordaY(int bordaY);
	
	void cair();
	void pular();

	//Destrutor
	~Objeto();
	
};