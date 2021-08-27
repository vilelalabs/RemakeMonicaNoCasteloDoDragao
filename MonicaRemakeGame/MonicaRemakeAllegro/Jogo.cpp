#include "stdafx.h"
#include <iostream>

#include "Jogo.h"
#include <allegro5\allegro_primitives.h>



using namespace ::std;

ALLEGRO_EVENT_QUEUE * queue;
ALLEGRO_EVENT	 	  event;

ALLEGRO_DISPLAY		* display; 
ALLEGRO_BITMAP		* buffer = NULL;

ALLEGRO_TRANSFORM camera;

ALLEGRO_FONT		* font = NULL;
ALLEGRO_TIMER		* timer = NULL;


//adaptação fullscreen
ALLEGRO_MONITOR_INFO info;
int res_x_comp, res_y_comp;


bool run;		// para verificar se jogo ainda está rodando

int FPS = 60;			// Frames por segundo (mínimo: 4, devido a calculo no meio do código)
int cfps = 0;			//contador de FPS
bool atualiza = false;	// controla a atualização da tela através do timer no FPS selecionado

//enur e array para captação das teclas de movimento do jogador e direção de sprite(flip)
enum DIRECAO {NADA, CIMA, BAIXO, DIR, ESQ, PULO};
bool teclas[] = { false,false, false, false, false, false};
// ativa e desativa animação do sprite do jogador
bool anim = false;

//controla amplitude de movimentação da plataforma
bool  movplat = false;


//Inicialização de objetos
Objeto *player	= NULL;
Objeto *enemy	= NULL;

//Inicialização de Plataformas Móveis
PlataformaMovel *platmov[2] = { NULL, NULL };

//inicialização de tile-maps e mapas
Tile *tile = NULL;
Map  *mapa = NULL;

int spr = 0; // variável para variação de sprite

float cameraPosition[2] = { 0, 0};

//--------------------------------------------------- FIM VARIÁVEIS GLOBAIS --------------------------------------//

void Jogo::init() {
	
	// inicializações do allegro
	al_init();

	//add-ons e instalações
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_install_keyboard();
	al_init_primitives_addon();


	//adaptação fullscreen
	/*al_get_monitor_info(0, &info);
	res_x_comp = info.x2 - info.x1;
	res_y_comp = info.y2 - info.y1;*/
	res_x_comp = SCREEN_W * ESCALA;
	res_y_comp = SCREEN_H * ESCALA;

	//criações e inicializações

	//al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	al_set_new_display_flags(ALLEGRO_WINDOWED);
	display = al_create_display(res_x_comp, res_y_comp); //original: display = al_create_display(800, 600);

	//adaptação fullscreen--------------------------
	float red_x = res_x_comp / (float) SCREEN_W;
	float red_y = res_y_comp / (float) SCREEN_H;
	ALLEGRO_TRANSFORM transformar;
	al_identity_transform(&transformar);
	al_scale_transform(&transformar, red_x, red_y);
	al_use_transform(&transformar);
	
	queue = al_create_event_queue();
	font = al_load_ttf_font("fonts/arial.ttf", 10, 0);
	timer	= al_create_timer(1.0 / FPS);
	
	//inicializações do buffer de tela
	buffer = al_create_bitmap(SCREEN_W, SCREEN_H);

	//registro dos eventos no queue (fila de eventos)
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));


	//inicializações dos OBJETOS no jogo
	player = new Objeto("arq/monica_andando_sheet_t.png", 32, 32, 400, 50, 0, 0, 7, 2);
	assert(player->getImagem() != NULL);
	enemy = new Objeto("arq/grama.png", 32, 32, 120, 194, 0, 0, 0, 0);
	assert(enemy->getImagem() != NULL);

	//inicialização de plataformas móveis
	platmov[0] = new PlataformaMovel("arq/platmov.png", 0, 0, 0, 32, VERTICAL);
	platmov[1] = new PlataformaMovel("arq/platmov.png", 0, 0, 0, 88, HORIZONTAL);
	
	//Inicialização de TILE-maps no jogo	
	tile = new Tile("arq/tilemap16.png",16,16);
	assert(tile->getImagem() != NULL);

	//inicialização dos MAPAS do jogo (ou do primeiro mapa)
	mapa = new Map();

	if (!(mapa->loadMap("map/mapa-level_1-1.map", tile))) {
		cout << "Falha ao carregar o mapa!" << endl;
		run = false;
		return;
	}


	//por hora não utilizado
	//srand(time(NULL));	//gera numero aleatorio para a função rand

	//inicializa timers
	al_start_timer(timer);

	run = true;
	cout << "Inicializado com Sucesso!" << endl;
}

void Jogo::monitoraEventos() {
	al_wait_for_event(queue, &event);

	//guarda posição anterior do player	
	player->setPosXant(player->getPosX());
	player->setPosYant(player->getPosY());
}

void Jogo::input()
{
	if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)//sai do jogo com botão de fechar (X) da tela
		run = false;
	
	if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) //sai do jogo com ESC
				run = false;
			switch (event.keyboard.keycode) {

			case ALLEGRO_KEY_F1:				//reseta posição do personagem (apenas para DEBUG)
				player->setPosX(21.0);
				player->setPosY(4.0);
				player->setPulando(false);
				player->setCaindo(false);
				break;
			case ALLEGRO_KEY_F2:				//reseta posição do personagem (apenas para DEBUG)
				player->setPosX(550);
				player->setPosY(4.0);
				player->setPulando(false);
				player->setCaindo(false);
				break;

			case ALLEGRO_KEY_W :
				teclas[CIMA]	= true;
				break;
			case ALLEGRO_KEY_S:
				teclas[BAIXO]	= true;
				break;
			case ALLEGRO_KEY_A:
				teclas[ESQ]		= true;
				//teclas[DIR]		= false;
				player->setXspeed(1.0);
				break;
			case ALLEGRO_KEY_D:
				teclas[DIR]		= true;
				//teclas[ESQ]		= false;
				player->setXspeed(1.0);
				break;
			case ALLEGRO_KEY_K:
				teclas[PULO] = true;
				break;
			default:
				break;
			}
	}

	else if (event.type == ALLEGRO_EVENT_KEY_UP) {
		switch (event.keyboard.keycode) {
			case ALLEGRO_KEY_W:
				teclas[CIMA] = false;
				break;
			case ALLEGRO_KEY_S:
				teclas[BAIXO] = false;
				break;
			case ALLEGRO_KEY_A:
				teclas[ESQ] = false;
				break;
			case ALLEGRO_KEY_D:
				teclas[DIR] = false;
				break;
			case ALLEGRO_KEY_K:
				teclas[PULO] = false;
				break;
			default:
				break;
			}
	}

	else if (event.type == ALLEGRO_EVENT_TIMER) {
		
		if (teclas[PULO]) {
			player->pular();
		}
		if (teclas[CIMA]) {
			player->setPosY(player->getPosY() - player->getYspeed());
		}
		else if (teclas[BAIXO]) {
			player->setPosY(player->getPosY() + player->getYspeed());
		}
		else if (teclas[ESQ]) {
			player->setPosX(player->getPosX() - player->getXspeed());
			player->setDir(ESQ);
			anim = true;
		}
		else if (teclas[DIR]) {
			player->setPosX(player->getPosX() + player->getXspeed());
			player->setDir(DIR);
			anim = true;
		}
		else {
			anim = false;
		}

		cameraUpdate(cameraPosition, player->getPosX(), player->getPosY(), player->getWidth(), player->getHeight());
		cfps++;
	}
	
}

void Jogo::update()
{

	//faz personagem pular
	if (player->getPulando() && (player->getPosY() > (player->getPLimit() - player->getAltPulo()))) {

		player->setPosY(player->getPosY() + player->getYspeed());
		player->setYspeed(-(player->getVelPulo()));
		player->setCaindo(true);
	}
	else if (player->getCaindo()) {
		player->cair();		//ação da gravidade no jogador
	}
	else {
		player->setPulando(false);
		player->setYspeed(0);
	}
	player->setCaindo(true);
	
	

	//trata colisão com inimigo - NÃO FUNCIONAL, mas a caminho da correção - fazer parecido com plataforma
	/*if (colisaoSeletiva(player, enemy,0) == ESQ ) 
		player->setPosX(enemy->getPosX()-(player->getWidth()-player->getBordaX()));
	else if (colisaoSeletiva(player, enemy, 0) == CIMA)
		 player->setPosY(enemy->getPosY() - (player->getHeight() - player->getBordaY()));
	else if (colisaoSeletiva(player, enemy, 0) == DIR)
		player->setPosX(enemy->getPosX() + (enemy->getWidth() - player->getBordaX()));
	else if (colisaoSeletiva(player, enemy, 0) == BAIXO)
		player->setPosY(enemy->getPosY() + (player->getHeight() - player->getBordaY()));*/
	

	static int last_colisao = 0; // apenas marcador para DEBUG no "cout <<"

	// trata colisão com o mapa
	for (int i = 0; i < mapa->getTamVertical(); i++) {
		for (int j = 0; j < mapa->getTamHorizontal(); j++) {
			if (mapa->getMap(i, j) == 1 || (mapa->getMap(i, j) >= 2 && mapa->getMap(i, j) <= 6)) {
				switch (colisaoMapa(player, mapa, i, j)) {

				case DIR:
					if (last_colisao != DIR) {
						cout << endl << "colisao: DIREITA" << endl;
						last_colisao = DIR;
						cout << "Pos. bloco de colisao e valor: " << i << ", " << j << " = " << mapa->getMap(i, j) << endl;
					}
					player->setPosX(mapa->getBlocoX(i, j) + tile->getWidth() - player->getBordaX() + 1);
					break;
				case ESQ:
					if (last_colisao != ESQ) {
						cout << endl << "colisao: ESQUERDA" << endl;
						last_colisao = ESQ;
						cout << "Pos. bloco de colisao e valor: " << i << ", " << j << " = " << mapa->getMap(i, j) << endl;
					}

					player->setPosX(mapa->getBlocoX(i, j) - player->getWidth() + player->getBordaX() - 1);
					break;

				case CIMA:
					if (last_colisao != CIMA) {
						cout << endl << "colisao: CIMA" << endl;
						last_colisao = CIMA;
					}
					player->setPosY(mapa->getBlocoY(i, j) - player->getHeight()+player->getBordaY()-0.1);
					player->setCaindo(false);

					break;

				case BAIXO:  // no caso específico da plataforma, não quero colisão vinda de baixo
					if (last_colisao != BAIXO) {
						cout << endl << "colisao: BAIXO" << endl;
						last_colisao = BAIXO;
					}

					break;

				default:

					break;
				}
			}
		}		
	}


	/*----------------atualiza posição da plataforma móvel----------------------*/
	
	platmov[0]->setBlocoRef(mapa->getBlocoX(11, 24) - cameraPosition[0], mapa->getBlocoY(11, 24) - cameraPosition[1]);
	platmov[1]->setBlocoRef(mapa->getBlocoX(13, 45) - cameraPosition[0], mapa->getBlocoY(13, 45) - cameraPosition[1]);

	//verifica por todas plataformas na tela
	for (int i = 0; i < 2; i++) {									// "2" - máximo de plataformas na fase neste caso (mudar para tornar variável este valor)
		//indo para cima ou esquerda
		if (!(platmov[i]->getNoLimiteMov() == MINIMO)) {
			if (platmov[i]->getTipoPlat() == VERTICAL) {
				platmov[i]->setYspeed(platmov[i]->getYspeed() - 1); //yspeed--
				if (platmov[i]->getYspeed() < -(platmov[i]->getLimMov()))
					platmov[i]->setNoLimiteMov(MINIMO);
			}
			else if (platmov[i]->getTipoPlat() == HORIZONTAL) {
				platmov[i]->setXspeed(platmov[i]->getXspeed() - 1); //xspeed--
				if (platmov[i]->getXspeed() < -(platmov[i]->getLimMov()))
					platmov[i]->setNoLimiteMov(MINIMO);
			}

		}
		// e para baixo ou direita
		else {
			if (platmov[i]->getTipoPlat() == VERTICAL) {
				platmov[i]->setYspeed(platmov[i]->getYspeed() + 1); //yspeed++
				if (platmov[i]->getYspeed() > platmov[i]->getLimMov())
					platmov[i]->setNoLimiteMov(MAXIMO);
			}
			else if (platmov[i]->getTipoPlat() == HORIZONTAL) {
				platmov[i]->setXspeed(platmov[i]->getXspeed() + 1); //xspeed++
				if (platmov[i]->getXspeed() > platmov[i]->getLimMov())
					platmov[i]->setNoLimiteMov(MAXIMO);
			}
		}

		//define posição que será colocada na tela
		platmov[i]->setPosY(platmov[i]->getBlocoRefY() + platmov[i]->getYspeed());
		platmov[i]->setPosX(platmov[i]->getBlocoRefX() + platmov[i]->getXspeed());

		/* -------------------VERIFICA COLISÃO COM PLATAFORMA MOVEL ----------------*/
		//queremos colisão apenas vinda de CIMA
		if (platmov[i]->getTipoPlat() == VERTICAL) {
			if (colisaoPlatMov(player, platmov[i])) {
				player->setPosY(platmov[i]->getPosY() - player->getHeight() + player->getBordaY());
				player->setCaindo(false);
			}
		}
		else if(platmov[i]->getTipoPlat() == HORIZONTAL) {
			if (colisaoPlatMov(player, platmov[i])) {
				player->setPosY(platmov[i]->getPosY() - player->getHeight() + player->getBordaY());
				player->setPosX(cameraPosition [0] + platmov[i]->getPosX() - player->getWidth()/2 + player->getBordaX());
				player->setCaindo(false);
			}
		}
		
	}
	/*--------------------------------------------------------------------------*/
 
	//NECESSÁRIO TRATAR COLISÃO DE OBJETOS COM AS BORDAS DA TELA) - PRINCIPALMENTE PLAYER
	
	//dentro do FPS faz alteração de sprite quando há movimento  -------------------- ANIMAÇÃO do SPRITE
		if (anim) {
			if (spr >= (6 * 32))	// 6: numero de sprites; 32: largura e altua do sprite
				spr = 0;			// reseta do primeiro sprite

			if (!(event.timer.count % (FPS / 4))) //Em tese, altera sprite a cada 1/4s "60 % 20 = 0"
				spr += 32;
		}
		else {
			spr = 0;
		}

		atualiza = true;

}

void Jogo::render()
{
	

	if (al_is_event_queue_empty(queue) && atualiza) {

		//seta buffer como target e o limpa antes de plotar a tela nele
		
		al_set_target_bitmap(buffer);
		al_clear_to_color(BLACK);
		// desenha mapa na tela de acordo com a posição da camera
		//al_convert_mask_to_alpha(mapa->getImagem(), al_map_rgb(0, 130, 199)); // teste de transparencia
		al_draw_bitmap_region(mapa->getImagem(), cameraPosition[0], cameraPosition[1], SCREEN_W, SCREEN_H,0,0,0);




		// para identificação dos blocos de colisão na tela de jogo
		/*
		int n = 1;
		for (int i = 0; i < mapa->getTamVertical(); i++)
			for (int j = 0; j < mapa->getTamHorizontal(); j++) {
				al_draw_rectangle(blocos[i][j]->getPosX(), blocos[i][j]->getPosY(), blocos[i][j]->getPosX() + blocos[i][j]->getWidth(),
						blocos[i][j]->getPosY()+blocos[i][j]->getHeight(), BLACK, 0);
			}
		*/

		//al_draw_bitmap_region(enemy->getImagem(), 0, 0, 32, 32, enemy->getPosX(), enemy->getPosY(), enemy->getDir()-1); 

		// exibe plataforma móvel na tela
		for (int i = 0; i < 2; i++) 
			al_draw_bitmap(platmov[i]->getImagem(), platmov[i]->getPosX(), platmov[i]->getPosY(), 0);

		//exibição do player------------------------------------------------------------------------------------------
		float px, py;

		//define PX:
		// se está no começo da fase
		if (player->getPosX() <= (SCREEN_W / 2 - player->getWidth() / 2))
			px = player->getPosX();
		// se está no final da fase
		else if(player->getPosX()>(al_get_bitmap_width(mapa->getImagem())-SCREEN_W/2 - player->getWidth() / 2))		
			px =  ((SCREEN_W/2) + (player->getPosX()-(al_get_bitmap_width(mapa->getImagem()) - SCREEN_W / 2)));
		else 
			px =  (SCREEN_W/2-player->getWidth()/2);
		
		//define PY
		// se está no começo da fase
		if (player->getPosY() <= (SCREEN_H / 2 - player->getHeight() / 2))

			py =  player->getPosY();
		// se está no final da fase
		else if (player->getPosY()>(al_get_bitmap_height(mapa->getImagem()) - SCREEN_H / 2 - player->getHeight() / 2))
			py = ((SCREEN_H / 2) + (player->getPosY() - (al_get_bitmap_height(mapa->getImagem()) - SCREEN_H / 2)));
		else
			py = (SCREEN_H / 2 - player->getHeight() / 2);

		// exibe player na posição px,py
		al_draw_bitmap_region(player->getImagem(), spr, 0, player->getWidth(), player->getHeight(), px, py, player->getDir() - 1);

		//-------------------------------------------------------------------------------------------------------------------------

		/*al_draw_rectangle(player->getPosX(), player->getPosY(), player->getPosX()+32, player->getPosY()+32, BLUE, 1);
		al_draw_rectangle(enemy->getPosX(), enemy->getPosY(), enemy->getPosX()+32, enemy->getPosY()+32, RED, 1);*/

		// teste de texto e frames
		al_draw_text(font, RED, SCREEN_W / 2, 40, ALLEGRO_ALIGN_CENTRE, "Monica no Castelo do Dragao!"); //(exemplo de texto na tela)
		al_draw_textf(font, BLUE, 0, 0, 0, "FPS: %d", cfps*FPS);
		al_draw_textf(font, WHITE, 17, 20, 0, "cameraPos_X: %.2f", cameraPosition[0]);
		al_draw_textf(font, WHITE, 17, 30, 0, "cameraPos_Y: %.2f", cameraPosition[1]);
		/*al_draw_textf(font, WHITE, 17, 40, 0, "plat_X: %.2f", platmov->getPosX());
		al_draw_textf(font, WHITE, 17, 50, 0, "plat_Y: %.2f", platmov->getPosY());*/

		al_draw_textf(font, BLUE, SCREEN_W / 2, 80, ALLEGRO_ALIGN_CENTRE, "player pos: %.1f , %.1f", player->getPosX(), player->getPosY());

		al_set_target_bitmap(al_get_backbuffer(display));

		al_draw_bitmap(buffer, 0, 0, 0);
		
		al_flip_display();
		
		atualiza = false;
		cfps = 0;
	}


}

int Jogo::colisaoTotal(Objeto *objA, Objeto *objB) {

	int bordaAx = objA->getBordaX();							//distancia da borda do objeto A real à borda de colisão
	int bordaBx = objB->getBordaX();							//distancia da borda do objeto B real à borda de colisão
	int bordaAy = objA->getBordaY();							//distancia da borda do objeto A real à borda de colisão
	int bordaBy = objB->getBordaY();							//distancia da borda do objeto B real à borda de colisão

	float xA = objA->getPosX() + bordaAx;						// lado esquerdo do objeto A
	float yA = objA->getPosY() + bordaAy;						// topo do objeto A
	float xB = objB->getPosX() + bordaBx;						// lado esquerdo do objeto B
	float yB = objB->getPosY() + bordaBy;						// topo do objeto B
	float wA = objA->getPosX() + objA->getWidth()  - bordaAx;	// lado direito d objeto A
	float hA = objA->getPosY() + objA->getHeight() - bordaAy;   // inferior do objeto A
	float wB = objB->getPosX() + objB->getWidth()  - bordaBx;	// lado direito d objeto B
	float hB = objB->getPosY() + objB->getHeight() - bordaBy;	// inferior do objeto B

	
	if (	(xA >= xB && xA <= wB) && (yA >= yB && yA <= hB) ||
			(wA >= xB && wA <= wB) && (yA >= yB && yA <= hB) ||
			(xA >= xB && xA <= wB) && (hA >= yB && hA <= hB) ||
			(wA >= xB && wA <= wB) && (hA >= yB && hA <= hB) 

			) return 1;
	else
		return 0;

}

int Jogo::colisaoSeletiva(Objeto * objA, Objeto * objB, int lado){ // [int lado] ainda não utilizado

	float bordaAx = objA->getBordaX();							// distancia da borda do objeto A real à borda de colisão
	float bordaBx = objB->getBordaX();							// distancia da borda do objeto B real à borda de colisão
	float bordaAy = objA->getBordaY();							// distancia da borda do objeto A real à borda de colisão
	float bordaBy = objB->getBordaY();							// distancia da borda do objeto B real à borda de colisão

	float xA = objA->getPosX() + bordaAx;						// lado esquerdo do objeto A
	float yA = objA->getPosY() + bordaAy;						// topo do objeto A
	float xB = objB->getPosX() + bordaBx;						// lado esquerdo do objeto B
	float yB = objB->getPosY() + bordaBy;						// topo do objeto B
	float wA = objA->getPosX() + objA->getWidth()  - bordaAx;	// lado direito d objeto A
	float hA = objA->getPosY() + objA->getHeight() - bordaAy;   // inferior do objeto A
	float wB = objB->getPosX() + objB->getWidth()  - bordaBx;	// lado direito d objeto B
	float hB = objB->getPosY() + objB->getHeight() - bordaBy;	// inferior do objeto B

	if ((xA >= xB && xA <= wB) && (yA >= yB && yA <= hB) ||	(wA >= xB && wA <= wB) && (yA >= yB && yA <= hB) ||
		(xA >= xB && xA <= wB) && (hA >= yB && hA <= hB) ||	(wA >= xB && wA <= wB) && (hA >= yB && hA <= hB)   ) {
			if (xA >= wB && xA <= wB)		return DIR;
			else if (hA >= hB && yA <= hB)	return BAIXO;
			else if (xA <= xB && wA >= xB)	return ESQ;
			else if ((yA < yB && hA > yB))	return CIMA;
			else 							return NADA;
	}
	else									return 0; // = NADA

}

int Jogo::colisaoPlatMov(Objeto * objA, PlataformaMovel * objB)
{
	float bordaAx = objA->getBordaX();											// distancia da borda do objeto A real à borda de colisão
	float bordaAy = objA->getBordaY();											// distancia da borda do objeto A real à borda de colisão

	//para correção 
	float difColisaoY = (objA->getHeight() + objB->getHeight()) / 2 + bordaAy; //garante que os pés do personagem passaram o topo da plataforma móvel
	float difColisaoX = (objA->getWidth() + objB->getWidth()) / 2 + bordaAx;   //garante que os pés do personagem passaram a esquerda da plataforma móvel (?)

	float xA = objA->getPosX() + bordaAx;										// lado esquerdo do objeto A
	float yA = objA->getPosY() + bordaAy;										// topo do objeto A
	float xB = objB->getPosX() + cameraPosition[0];								// lado esquerdo do objeto B
	float yB = objB->getPosY() + cameraPosition[1];								// topo do objeto B
	float wA = objA->getPosX() + objA->getWidth()  - bordaAx;					// lado direito d objeto A
	float hA = objA->getPosY() + objA->getHeight() - bordaAy;					// inferior do objeto A
	float wB = objB->getPosX() + objB->getWidth()  + cameraPosition[0];			// lado direito d objeto B
	float hB = objB->getPosY() + objB->getHeight() + cameraPosition[1];			// inferior do objeto B

	
	if ((yA + difColisaoY < yB) && (hA > yB - bordaAy) && (wA > xB && xA < wB)) {
		//cout << (yA+ difColisaoY < yB) << ", "<< ((hA > yB - bordaAy)) << endl;
		return true;
	}
	else
		return false;

}

//sobrecarga da função para uso com BLOCOS ao invés de objetos
int Jogo::colisaoMapa(Objeto * obj, Map *mapa, int index_x, int index_y) { 

	float bordaAx = obj->getBordaX();											// distancia da borda do objeto A real à borda de colisão
	float bordaAy = obj->getBordaY();											// distancia da borda do objeto A real à borda de colisão

	float xA = obj->getPosX() + bordaAx;										// lado esquerdo do objeto A
	float yA = obj->getPosY() + bordaAy;										// topo do objeto A
	float xB = mapa->getBlocoX(index_x, index_y) ;								// lado esquerdo do objeto B
	float yB = mapa->getBlocoY(index_x, index_y) ;								// topo do objeto B
	float wA = obj->getPosX() + obj->getWidth() - bordaAx;						// lado direito do objeto A
	float hA = obj->getPosY() + obj->getHeight() - bordaAy;						// inferior do objeto A
	float wB = mapa->getBlocoX(index_x, index_y) + tile->getWidth() ;			// lado direito d objeto B
	float hB = mapa->getBlocoY(index_x, index_y) + tile->getHeight() ;			// inferior do objeto B

	if ((xA >= xB && xA <= wB) && (yA >= yB && yA <= hB) || (wA >= xB && wA <= wB) && (yA >= yB && yA <= hB) ||
		(xA >= xB && xA <= wB) && (hA >= yB && hA <= hB) || (wA >= xB && wA <= wB) && (hA >= yB && hA <= hB)) {
		if (xA >= wB && xA <= wB)			return DIR;
		else if (hA >= hB && yA <= hB)		return BAIXO;
		else if (xA <= xB && wA >= xB)		return ESQ;
		else if ((yA <= yB && hA >= yB))	return CIMA;
		else								return NADA;
	}
	else									return NADA; // 0

}

int Jogo::colisaoBox(Objeto * objA, Objeto * objB)
{
	float Ax = objA->getPosX();		float Ay = objA->getPosY();
	float Aw = objA->getWidth();	float Ah = objA->getHeight();
	float Bx = objB->getPosX();		float By = objB->getPosY();
	float Bw = objB->getWidth();	float Bh = objB->getHeight();

	if (Ax + Aw > Bx && Ax < Bx + Bw && Ay + Ah > By && Ay < By + Bh)
		return 1;

	return 0;
}

bool Jogo::isRunning() {

	return run;
}

void Jogo::cameraUpdate(float * cameraPosition, float playerX, float playerY, int playerW, int playerH)
{
	float focoX = player->getPosX() + player->getWidth() / 2;
	float focoY = player->getPosY() + player->getHeight() / 2;
	float telaX = al_get_bitmap_width(buffer);
	float telaY = al_get_bitmap_height(buffer);
	float mapaW = al_get_bitmap_width(mapa->getImagem());
	float mapaH = al_get_bitmap_height(mapa->getImagem());

	

	float cameraLeft	= focoX - telaX / 2;
	float cameraTop		= focoY - telaY / 2;

	if (cameraLeft < 0)						cameraLeft = 0;
	else if (cameraLeft > (mapaW - telaX))	cameraLeft = mapaW - telaX;
	
	if (cameraTop < 0)						cameraTop = 0;
	else if (cameraTop > mapaH - telaY)		cameraTop = mapaH - telaY;



	//para DEBUG
	//cout << "camLft: " << cameraLeft << " camTop: " << cameraTop << " camRgh: " << cameraRight << " camBtm: " << cameraBottom << endl;
	

	cameraPosition[0] = cameraLeft;
	cameraPosition[1] = cameraTop;


}

Jogo::Jogo() {}					// Construtor

Jogo::~Jogo() {					// Destrutor

	//hardware
	al_destroy_display(display);
	al_uninstall_keyboard();

	// auxiliares
	al_destroy_font(font);
	al_destroy_timer(timer);

	//bitmaps
	al_destroy_bitmap(buffer);

	//objetos, tiles, mapas, plataformas moveis
	delete player;

	delete enemy;

	for (int i = 0; i<2; i++)
		delete platmov[i];

	delete tile;
	delete mapa;

	//fila de eventos
	al_destroy_event_queue(queue);


	cout << "Instancia \"Jogo\" destruida!" << endl;
}
