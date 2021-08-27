#include "stdafx.h"
#include "PlataformaMovel.h"


PlataformaMovel::PlataformaMovel()
{
}

PlataformaMovel::PlataformaMovel(const char *arquivo, float posX, float posY, float plat_speed, int limMov, TIPO_PLATAFORMA tipoPlat)
{
	this->setImagem(arquivo);
	this->setPosX(posX);
	this->setPosY(posY);
	this->setWidth(al_get_bitmap_width(this->getImagem()));
	this->setHeight(al_get_bitmap_height(this->getImagem()));

	if(tipoPlat == HORIZONTAL)
		this->setXspeed(plat_speed);
	else if (tipoPlat = VERTICAL)
		this->setYspeed(plat_speed);

	this->setPosXant(posX);
	this->setPosYant(posY);

	this->limMov = limMov;
	this->tipoPlat = tipoPlat;

	this->setNoLimiteMov(MINIMO); // sempre começa o movimento em direção ao lado mínimo (topo ou esquerda)


}



PlataformaMovel::~PlataformaMovel()
{
}

int PlataformaMovel::getLimMov()
{
	return this->limMov;
}

TIPO_PLATAFORMA PlataformaMovel::getTipoPlat()
{
	return this->tipoPlat;
}

LIM_MOV_PLATAFORMA PlataformaMovel::getNoLimiteMov()
{
	return this->noLimiteMov;
}

void PlataformaMovel::setNoLimiteMov(LIM_MOV_PLATAFORMA noLimiteMov)
{
	this->noLimiteMov = noLimiteMov;
}

void PlataformaMovel::setBlocoRef(int blocoRefX, int blocoRefY)
{
	this->blocoRefX = blocoRefX;
	this->blocoRefY = blocoRefY;
}
int PlataformaMovel::getBlocoRefX()
{
	return this->blocoRefX;
}
int PlataformaMovel::getBlocoRefY()
{
	return this->blocoRefY;
}

