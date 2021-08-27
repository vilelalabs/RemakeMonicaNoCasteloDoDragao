#pragma once

#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include "Objeto.h"

//para definir o tipo de movimento que a plataforma irá fazer
enum TIPO_PLATAFORMA{ VERTICAL, HORIZONTAL};
// definie limites de movimento para a plataforma
enum LIM_MOV_PLATAFORMA {MINIMO, MAXIMO};

class PlataformaMovel : public Objeto
{
private:
	int limMov;
	TIPO_PLATAFORMA tipoPlat;
	LIM_MOV_PLATAFORMA noLimiteMov; // no limite máximo do movimento  definido por limMov seja o mínimo ou máximo

	int blocoRefX;
	int blocoRefY;



public:
	PlataformaMovel();
	PlataformaMovel(const char *arquivo, float posX, float posY, float plat_speed, int limMovimento, TIPO_PLATAFORMA tipoPlat);

	~PlataformaMovel();


	//getters and setters (quando aplicável)
	int getLimMov();
	TIPO_PLATAFORMA getTipoPlat();

	LIM_MOV_PLATAFORMA getNoLimiteMov();
	void setNoLimiteMov(LIM_MOV_PLATAFORMA noLimiteMov);

	void setBlocoRef(int blocoRefX, int blocoRefY);
	int	 getBlocoRefX();
	int  getBlocoRefY();

};

