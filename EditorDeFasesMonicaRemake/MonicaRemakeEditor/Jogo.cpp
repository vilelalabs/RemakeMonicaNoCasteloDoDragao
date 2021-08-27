#include "stdafx.h"
#include <iostream>

#include "Jogo.h"
#include <allegro5\allegro_primitives.h>



using namespace ::std;

ALLEGRO_EVENT_QUEUE * queue;
ALLEGRO_EVENT	 	  event;

ALLEGRO_DISPLAY		* display; 
ALLEGRO_BITMAP		* buffer	= NULL;
ALLEGRO_BITMAP		* menu		= NULL;
ALLEGRO_BITMAP		* mouseTile	= NULL;

ALLEGRO_TRANSFORM camera;

ALLEGRO_FONT		* font = NULL;
ALLEGRO_TIMER		* timer = NULL;


//adapta��o fullscreen
ALLEGRO_MONITOR_INFO info;
int res_x_comp, res_y_comp;


bool run;		// para verificar se jogo ainda est� rodando

int FPS = 10;			// Frames por segundo (m�nimo: 4, devido a calculo no meio do c�digo)
int cfps = 0;			//contador de FPS
bool atualiza = false;	// controla a atualiza��o da tela atrav�s do timer no FPS selecionado

//enur e array para capta��o das teclas de movimento do jogador e dire��o de sprite(flip)
enum DIRECAO {NADA, CIMA, BAIXO, DIR, ESQ, PULO};
bool teclas[] = { false,false, false, false, false, false};
// ativa e desativa anima��o do sprite do jogador
bool anim = false;


//Inicializa��o de objetos
Objeto *focoScroll = NULL;

//inicializa��o de tile-maps e mapas
Tile *tile = NULL;
Map  *mapa = NULL;

int spr = 0; // vari�vel para varia��o de sprite

//posi��o superior da camera de vis�o
float cameraPosition[2] = {0, 0};

//referencias dentro do menu
// posi��o do menu na tela
int menu_x = 0;
int menu_y = SCREEN_H-MENU_H ;

// posi��o do pegador de tiles do menu (x,y,w,h)
int refGrabTiles_x = menu_x + 57;
int refGrabTiles_y = menu_y + 6;	
int grabTilesAreaWidth = 512;
int grabTilesAreaHeight = 50;

int selected_tile = 0; // guarda o tipo de tile selecionado

//--------------------------------------------------- FIM VARI�VEIS GLOBAIS --------------------------------------//

void Jogo::init() {
	
	// inicializa��es do allegro
	al_init();

	//add-ons e instala��es
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_install_keyboard();
	al_install_mouse();
	al_init_primitives_addon();


	//adapta��o fullscreen
	/*al_get_monitor_info(0, &info);
	res_x_comp = info.x2 - info.x1;
	res_y_comp = info.y2 - info.y1;*/
	res_x_comp = SCREEN_W*ESCALA;
	res_y_comp = SCREEN_H*ESCALA;


	//res_x_comp = SCREEN_W;
	//res_y_comp = SCREEN_H;

	//cria��es e inicializa��es

	//al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	al_set_new_display_flags(ALLEGRO_WINDOWED);
	al_set_new_display_flags(ALLEGRO_NOFRAME);
	display = al_create_display(res_x_comp, res_y_comp); //original: display = al_create_display(800, 600);

	//adapta��o fullscreen--------------------------
	float red_x = res_x_comp / (float) SCREEN_W;
	float red_y = res_y_comp / (float) SCREEN_H;
	ALLEGRO_TRANSFORM transformar;
	al_identity_transform(&transformar);
	al_scale_transform(&transformar, red_x, red_y);
	al_use_transform(&transformar);
	
	queue = al_create_event_queue();
	font = al_load_ttf_font("fonts/arial.ttf", 10, 0);
	timer	= al_create_timer(1.0 / FPS);
	
	//inicializa��es do buffer de tela e menu
	buffer = al_create_bitmap(SCREEN_W, SCREEN_H);
	//menu =   al_create_bitmap(SCREEN_W, SCREEN_H-240); // 50 pixels de altura
	menu = al_load_bitmap("arq/menu_inf_NEW3.png");
	assert(menu != NULL);

	//registro dos eventos no queue (fila de eventos)
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));


	//inicializa��es dos OBJETOS no jogo
	focoScroll = new Objeto("arq/monica_andando_sheet_t.png", 40, 150, 0, 0, 7, 2);
	assert(focoScroll->getImagem() != NULL);
	
	//Inicializa��o de TILE-maps no jogo	
	tile = new Tile("arq/tilemap16.png",16,16);
	assert(tile->getImagem() != NULL);

	//seta tamanho de tile que estar� ativo
	mouseTile = al_create_bitmap(tile->getWidth(), tile->getHeight());
	al_set_target_bitmap(mouseTile);
	al_draw_bitmap_region(tile->getImagem(), 0, 0, tile->getWidth(), tile->getHeight() , 0, 0, 0);

	//inicializa��o dos MAPAS do jogo (ou do primeiro mapa)
	mapa = new Map();

	if (!(mapa->loadMap("map/mapa-level_1-1.map", tile))) {
		cout << "Falha ao carregar o mapa!" << endl;
		run = false;
		return;
	}

	mapa->drawMap(tile);


	srand(time(NULL));	//gera numero aleatorio para a fun��o rand

	//inicializa timers
	al_start_timer(timer);

	run = true;
	cout << "Inicializado com Sucesso!" << endl;
}

void Jogo::monitoraEventos() {
	al_wait_for_event(queue, &event);

	//guarda posi��o anterior do focoScroll	
	focoScroll->setPosXant(focoScroll->getPosX());
	focoScroll->setPosYant(focoScroll->getPosY());
}

void Jogo::input()
{
	if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)//sai do jogo com bot�o de fechar (X) da tela
		run = false;

	else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
		int vmx, vmy;
		int grab_x = refGrabTiles_x * ESCALA;
		int grab_y = refGrabTiles_y * ESCALA;
		getVirtualMousePosition(vmx, vmy);

		//(se est� na �rea de pegar tiles)
		if (vmx > grab_x && vmx < grab_x + grabTilesAreaWidth * ESCALA &&
			vmy > grab_y && vmy < grab_y + grabTilesAreaHeight * ESCALA) 
				al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
		
		else
			al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
		
	}
	
	else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
		if (event.mouse.button & 1) { // botao esquerdo

			int vmx, vmy;
			int grab_x = refGrabTiles_x * ESCALA;
			int grab_y = refGrabTiles_y * ESCALA;
			getVirtualMousePosition(vmx, vmy);
			

			// (se est� na �rea de pegar tiles)
			if (vmx > grab_x && vmx < grab_x + grabTilesAreaWidth * ESCALA &&
					vmy > grab_y && vmy < grab_y + grabTilesAreaHeight * ESCALA) {

				cout << "Mouse na area de pegar tiles!" << endl;
				int tilex = vmx - grab_x;
				int tiley = vmy - grab_y;
				//posi��o do arq do tile que sera efetivamente desenhada "no" mouse
				int tiledesx = 0;
				int tiledesy = 0;

				//cria set tile de compara��o  ---------------------------------(tirar do evento de mouse e fixar no init futuramente)
				int tile_sel[TILE_GRABBER_LIN][TILE_GRABBER_COL];

				for(int lin = 0; lin<TILE_GRABBER_LIN; lin++)
					for (int col = 0; col < TILE_GRABBER_COL; col++) {
						if (lin == 0) {
							tile_sel[lin][col] = col * tile->getWidth();
						}
						else {
							tile_sel[lin][col] = (lin*TILE_GRABBER_COL* tile->getWidth())+(col * tile->getWidth());
						}
						//cout << "tilesel[" << lin << "][" << col << "] = " << tile_sel[lin][col] << endl;
					}
					//system("pause");-------------------------------------------------------------------------------------------------

				for (int lin = 0; lin<TILE_GRABBER_LIN; lin++)
					for (int col = 0; col < TILE_GRABBER_COL; col++) {

						if (tiley > 0 && tiley < 16*ESCALA && tilex > tile_sel[lin][col]*ESCALA && tilex < tile_sel[lin][col+1]*ESCALA) {
							tiledesx = tile_sel[lin][col];
							tiledesy = 0; //fixo por enquanto
							//seta o tipo de tile que ser� guardado na matriz
							selected_tile = tiledesx / tile->getWidth();
						}
						else if (tiley > (16+1)*ESCALA && tiley < 32 * ESCALA && tilex > tile_sel[lin][col] * ESCALA && tilex < tile_sel[lin][col + 1] * ESCALA) {
							if (tilex >= tile_sel[lin][col] * ESCALA && tilex < tile_sel[lin][col + 1] * ESCALA) {
								tiledesx = tile_sel[1][col];
								tiledesy = 16; //fixo por enquanto
							}
							//seta o tipo de tile que ser� guardado na matriz
							selected_tile = tiledesx / tile->getWidth();
							tiledesx -= al_get_bitmap_width(tile->getImagem());
						}
						/*else if (tiley >(32 + 2) && tiley < 64 * ESCALA && tilex > tile_sel[lin][col] * ESCALA && tilex < tile_sel[lin][col + 1] * ESCALA) {
							tiledesx = tile_sel[lin][col];
							tiledesy = 16; //fixo por enquanto
						    //seta o tipo de tile que ser� guardado na matriz
							selected_tile = tile_sel[2][col] / tile->getWidth();
						}*/

						//cout << "tile_sel[lin][col] = " << tile_sel[lin][col] << endl;
						//cout << "tilex = " << tilex << ", tiley = " << tiley <<  endl;
						//cout << selected_tile << endl;

						/*cout << "tilesel*ESCALA[" << lin << "][" << col << "] = " << tile_sel[lin][col]*ESCALA <<
												" - tilex =" << tilex << ", tiledesx = " << tiledesx <<endl;*/
					}
				//system("pause");
				// torna o desenho do mouse o tile
				al_set_target_bitmap(mouseTile);
				cout << "tiledesx = " << tiledesx << ", tiledesy = " << tiledesy << endl;
				al_draw_bitmap_region(tile->getImagem(), tiledesx, tiledesy, tile->getWidth(), tile->getHeight(), 0, 0, 0);

			}
			


			// (se esta na �rea do mapa)
			else if (vmx > 0 && vmx < SCREEN_W * ESCALA && vmy > 0 && vmy < (menu_y) * ESCALA) {
					cout << "clique na area do mapa: " << vmx << ", " << vmy << endl;

					int tilePosX, tilePosY;

					getTilePosition(vmx, vmy, cameraPosition[0], cameraPosition[1], tilePosY, tilePosX);
					mapa->setMap(tilePosX, tilePosY, selected_tile);
					//cout << mapa->getMap(tilePosX, tilePosY) << endl;
			}
		
		}
		if (event.mouse.button & 2) { // botao direito

			int vmx, vmy;
			int grab_x = refGrabTiles_x * ESCALA;
			int grab_y = refGrabTiles_y * ESCALA;
			getVirtualMousePosition(vmx, vmy);

			// apaga um bloco j� colocado
		   if (vmx > 0 && vmx < SCREEN_W * ESCALA &&
				vmy > 0 && vmy < (menu_y)* ESCALA) {

			   int temp_selected_tile = selected_tile;
			   selected_tile = 0;

				int tilePosX, tilePosY;

				getTilePosition(vmx, vmy, cameraPosition[0], cameraPosition[1], tilePosY, tilePosX);
				mapa->setMap(tilePosX, tilePosY, selected_tile);

				selected_tile = temp_selected_tile;
			}

		}
	}

	else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
		if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) { //sai do jogo com ESC
			
			// perguntar antes se deseja salvar o mapa
			char sair = 'n';
			cout << "Verifique se o mapa foi salvo. Deseja sair? (Y/N): ";
			cin >> sair;
			if (sair == 'y' || sair == 'Y')
				run = false;
		}
			switch (event.keyboard.keycode) {

			case ALLEGRO_KEY_S:
				if(!mapa->saveMap())
					cout << "Falha ao salvar!" << endl;
				break;

			case ALLEGRO_KEY_N:
				if (!mapa->newMap())
					cout << "Falha ao criar novo mapa!" << endl;

				if (!(mapa->loadMap("map/mapa-level_1-1.map", tile))) {
					cout << "Falha ao carregar o mapa!" << endl;
					run = false;
					return;
				}

				mapa->clearMap();
				break;

			case ALLEGRO_KEY_C:
				mapa->clearMap();
				break;

			case ALLEGRO_KEY_UP :
				teclas[CIMA]	= true;
				cameraPosition[1] -= 16;
				break;
			case ALLEGRO_KEY_DOWN:
				teclas[BAIXO]	= true;
				cameraPosition[1] += 16;
				break;
			case ALLEGRO_KEY_LEFT:
				teclas[ESQ]		= true;
				cameraPosition[0] -= 16;
				break;
			case ALLEGRO_KEY_RIGHT:
				teclas[DIR]		= true;
				cameraPosition[0] += 16;
				break;
			default:
				break;
			}
	}

	else if (event.type == ALLEGRO_EVENT_TIMER) {
		atualiza = true;
		cfps++;
	}

	//verifica bordas mostradas na tela
	cameraUpdateEditor(cameraPosition);
	
}

void Jogo::update()
{
	//colis�o, anima��o de sprite e pulo removidos do original
	
}

void Jogo::render()
{	

	if (al_is_event_queue_empty(queue) && atualiza) {

		//desenha mapa
		mapa->drawMap(tile);
		
		al_set_target_bitmap(buffer);
		al_clear_to_color(SKY);


		//desenha mapa
		
		al_draw_bitmap_region(mapa->getImagem(), cameraPosition[0], cameraPosition[1], SCREEN_W, SCREEN_H,0,0,0);
		//desenha menu inferior
		al_draw_bitmap(menu, 0, 240, 0);
		//diz  qual mapa est� sendo editado
		al_draw_textf(font, BLACK, menu_x+2, menu_y+2, 0, "Lvl: %d-%d", mapa->getLevel(), mapa->getSubLevel());

		//desenha barra de sele��o de tiles
		al_draw_bitmap_region(tile->getImagem(), 0, 0, TILE_GRABBER_COL * tile->getWidth(), tile->getHeight(), refGrabTiles_x, refGrabTiles_y, 0);
		al_draw_bitmap_region(tile->getImagem(), 0, tile->getHeight(), TILE_GRABBER_COL * tile->getWidth(), tile->getHeight(),
			refGrabTiles_x, refGrabTiles_y+ tile->getHeight()+1, 0);

		// para identifica��o dos blocos de colis�o na tela de jogo
		/*
		int n = 1;
		for (int i = 0; i < mapa->getTamVertical(); i++)
			for (int j = 0; j < mapa->getTamHorizontal(); j++) {
				al_draw_rectangle() */  // reformular apenas para desenhar grade na tela (grade poder� ser removida pelo usu�rio)

		/*if (focoScroll->getPosX() <= (SCREEN_W / 2 - focoScroll->getWidth() / 2))															// se est� no come�o da fase
			al_draw_bitmap_region(focoScroll->getImagem(), spr, 0, 32, 32, focoScroll->getPosX(), focoScroll->getPosY(), focoScroll->getDir() - 1);
		else if (focoScroll->getPosX()>(al_get_bitmap_width(mapa->getImagem()) - SCREEN_W / 2 - focoScroll->getWidth() / 2))				// se est� no final da fase
			al_draw_bitmap_region(focoScroll->getImagem(), spr, 0, 32, 32, ((SCREEN_W / 2) +
			(focoScroll->getPosX() - (al_get_bitmap_width(mapa->getImagem()) - SCREEN_W / 2))), focoScroll->getPosY(), focoScroll->getDir() - 1);
		else
			al_draw_bitmap_region(focoScroll->getImagem(), spr, 0, 32, 32, (SCREEN_W / 2 - focoScroll->getWidth() / 2), focoScroll->getPosY(), focoScroll->getDir() - 1);
		*/

		// teste de texto e frames
		al_draw_text(font, RED, SCREEN_W / 2, 5, ALLEGRO_ALIGN_CENTRE, "Monica no Castelo do Dragao EDITOR!"); //(exemplo de texto na tela)
		al_draw_textf(font, BLUE, 0, 0, 0, "FPS: %d", cfps*FPS);
		al_draw_textf(font, WHITE, 17, 20, 0, "cameraPos_X: %.2f", cameraPosition[0]);
		al_draw_textf(font, WHITE, 17, 30, 0, "cameraPos_Y: %.2f", cameraPosition[1]);

		int vmx, vmy;
		getVirtualMousePosition(vmx, vmy);
		int tilePos_x, tilePos_y;
		getTilePosition(vmx, vmy, cameraPosition[0], cameraPosition[1], tilePos_x, tilePos_y);

		//desenha tile na posi��o do mouse (centralizado
		al_draw_bitmap(mouseTile, (vmx/ESCALA - tile->getWidth()/4), (vmy/ESCALA - tile->getHeight() / 4), 0);

		//DEBUG exibir na tela
		al_draw_textf(font, WHITE, 17, 40, 0, "virtual.mouse.x: %d", vmx);
		al_draw_textf(font, WHITE, 17, 50, 0, "virtual.mouse.y: %d", vmy);
		al_draw_textf(font, WHITE, 17, 60, 0, "tilePos.x: %d", tilePos_x);
		al_draw_textf(font, WHITE, 17, 70, 0, "tilePos.y: %d", tilePos_y);

		al_draw_textf(font, BLUE, SCREEN_W / 2, 10, ALLEGRO_ALIGN_CENTRE, "focoScroll pos: %.1f , %.1f", focoScroll->getPosX(), focoScroll->getPosY());

		al_set_target_bitmap(al_get_backbuffer(display));


		//desenha e mostra na tela
		al_draw_bitmap(buffer, 0, 0, 0);
		
		al_flip_display();
		
		atualiza = false;
		cfps = 0;
	}

}

bool Jogo::isRunning() {

	return run;
}

void Jogo::cameraUpdateEditor(float * cameraPosition)
{
	int telaX = (int)al_get_bitmap_width(buffer);
	int telaY = (int)al_get_bitmap_height(buffer);
	int mapaW = (int)al_get_bitmap_width(mapa->getImagem());
	int mapaH = (int)al_get_bitmap_height(mapa->getImagem());


	if (cameraPosition[0] <= 0)						cameraPosition[0] = 0;
	else if (cameraPosition[0] >= (mapaW - telaX))	cameraPosition[0] = mapaW - telaX;

	if (cameraPosition[1] <= 0)						cameraPosition[1] = 0;
	else if (cameraPosition[1] >= (mapaH - telaY))	cameraPosition[1] = mapaH - telaY;

}

void Jogo::getVirtualMousePosition(int &ret_virtualMouseX, int & ret_virtualMouseY) {		// a posi��o virtual � o ponto do mouse em rela��o � tela

	// cria posi��o virtual do mouse dentro editor (transformar em fun��o � parte posteriormente
	
	// mouse, window (x, y, width and height) e "virtual mouse" x's and y's *vm = posi��o zerada do mouse dentro da janela de edi��o;
	int mx, my, wx, wy, ww, wh, vmx = 0, vmy = 0;

	al_get_mouse_cursor_position(&mx, &my);
	al_get_window_position(display, &wx, &wy);
	ww = al_get_display_width(display);
	wh = al_get_display_height(display);

	if (mx <= wx || my <= wy)			vmx = 0;
	else if (mx >= wx + ww)			vmx = ww;
	else							vmx = mx - wx;

	if (my <= wy || mx <= wx)		vmy = 0;
	else if (my >= wy + wh)			vmy = wh;
	else							vmy = my - wy;


	//zera tudo se estiver fora da borda da tela
	if (mx <= wx || my <= wy || my > wy + wh || mx > wx + ww) { vmx = 0; vmy = 0; }

	ret_virtualMouseX = vmx;
	ret_virtualMouseY = vmy;

	return;

} //

void Jogo::getTilePosition(int virt_mouseX, int virt_mouseY, int cameraX, int cameraY, int &ret_tilePosX, int &ret_tilePosY) {
	
	ret_tilePosX = virt_mouseX / ((int)tile->getWidth()*(ESCALA))  + cameraX/16;
	ret_tilePosY = virt_mouseY / ((int)tile->getHeight()*(ESCALA)) + cameraY/16;

	if (virt_mouseX == 0) ret_tilePosX = 0;
	if (virt_mouseY == 0) ret_tilePosY = 0;

	return;


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
	al_destroy_bitmap(menu);
	al_destroy_bitmap(mouseTile);

	//objetos
	delete focoScroll;
	delete tile;
	delete mapa;

	//fila de eventos
	al_destroy_event_queue(queue);


	cout << "Instancia \"Jogo\" destruida!" << endl;
}
