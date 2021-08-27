#include "stdafx.h"
#include "Tile.h"


Tile::Tile() {}

Tile::Tile(const char *arquivo, int w, int h) {
	//arquivo com todos os blocos
	this->setImagem(arquivo);

	//tamanho padrão de cada bloco
	this->setWidth(w);
	this->setHeight(h);
}




Tile::~Tile()
{
}
