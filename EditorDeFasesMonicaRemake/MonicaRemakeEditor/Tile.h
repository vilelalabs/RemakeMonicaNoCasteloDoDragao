#pragma once
#include "Imagem.h"
class Tile : public Imagem
{
private:
	//int lados_colisao; // indica quais lados do tile haverá detecção de colisão e quais não (função específica para colisão para lidar com esses dados)


public:
	Tile();
	Tile(const char *arquivo, int w, int h);
	~Tile();
};

