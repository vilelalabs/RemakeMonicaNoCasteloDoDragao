#pragma once
#include "Imagem.h"
class Tile : public Imagem
{
private:
	//int lados_colisao; // indica quais lados do tile haver� detec��o de colis�o e quais n�o (fun��o espec�fica para colis�o para lidar com esses dados)


public:
	Tile();
	Tile(const char *arquivo, int w, int h);
	~Tile();
};

