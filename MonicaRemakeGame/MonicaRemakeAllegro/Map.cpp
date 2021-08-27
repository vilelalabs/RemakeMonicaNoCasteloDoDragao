

#include "stdafx.h"
#include "Map.h"



Map::Map() {

}


int Map::getMap(int indexLin, int indexCol)
{
	if (indexLin > this->getTamVertical() - 1 || indexLin < 0 || indexCol > this->getTamHorizontal() - 1 || indexCol < 0) {
		cout << "Erro na fun��o getMap: valor inv�lido para a fun��o!" << endl;
		return -1;
	}
	// fazer baseado nesta ideia
	// int get_element( size_t index ) { return ( a[index] ); }
	//talvez ao inv�s de retornar o array direto, se n�o funcionar, passar o valor para uma vari�vel local e retorn�-la


	return this->map[indexLin][indexCol];
}

BLOCO **Map::getBloco()
{
	return this->bloco;
}

int Map::getBlocoX(int indexLin, int indexCol)
{
	return this->bloco[indexLin][indexCol].x;
}
int Map::getBlocoY(int indexLin, int indexCol)
{
	return this->bloco[indexLin][indexCol].y;
}

TIPO_COLISAO Map::getBlocoTipoColisao(int indexLin, int indexCol)
{
	return this->bloco[indexLin][indexCol].tipo_colisao;
}


bool Map::loadMap(const char *nome_arquivo, Tile *tile) {
	
	/* ------------------- CARREGAMENTO DO ARQUIVO ------------------ */
	FILE *file = nullptr;
	ARQUIVO arq;
	errno_t err;

	//#pragma warning(suppress : 4996) - pragma para fopen()

	if (err = (fopen_s(&file, nome_arquivo, "rb"))) {
		cout << "Erro ao abrir arquivo para leitura!: "<< err << endl;
		return false;
	}

	// l� parametros iniciais no arquivo
	fread(&arq.lvl, sizeof(int), 1, file);									// ainda nao utilizado na fun��o - fazer novas variaveis para classe Map
	fread(&arq.sublvl, sizeof(int), 1, file);								// ainda nao utilizado na fun��o - fazer novas variaveis para classe Map
	fread(&arq.lin, sizeof(int), 1, file);
	fread(&arq.col, sizeof(int), 1, file);

	//aloca espa�o na mem�ria para receber o mapa, l� o arquivo e o fecha
	arq.mapa = (int **)malloc(arq.lin * sizeof(int*));
	for (int i = 0; i<arq.lin; i++)
		arq.mapa[i] = (int *)malloc(arq.col * sizeof(int));

	for (int i = 0; i<arq.lin; i++)
		for (int j = 0; j<arq.col; j++) {
			fread(&arq.mapa[i][j], sizeof(int), 1, file);
		}

	cout << "Tam do mapa na memoria RAM: " << (sizeof(int) * 4) + (sizeof(int)*arq.lin*arq.col) << endl;
	fclose(file);

	 //exibe mapa "num�rico" no Console
	/*for (int i = 0; i < arq.lin; i++) {

		for (int j = 0; j < arq.col; j++) {
			cout << arq.mapa[i][j];
			cout << ' ';
		}
		cout << endl;
	}*/

	cout << "Processo de leitura do arquivo finalizado com sucesso!" << endl;

	/* ------------------- FIM: CARREGAMENTO DO ARQUIVO ------------------ */
	
	// retorna falso se as dimens�es indicadas para o mapa n�o forem positivas
	//mas fazer com que n�o seja poss�vel esse tipo de evento antes mesmo de chamar a fun��o
	// futuramente verificar externamente para que as dimens�es de um mapa n�o sejam menores que as dimens�es da tela (para n�o ter partes pretas na tela)
	if (arq.lin <= 0 || arq.col <= 0) return false;

	tamVertical = arq.lin;
	tamHorizontal = arq.col;

	//reserva mem�ria para aloca��o do mapa (da classe Map)
	this->map = (int **)malloc(tamVertical * sizeof(int*));
	for (int i = 0; i< tamVertical; i++)
		map[i] = (int *)malloc(tamHorizontal * sizeof(int));

	//carrega mapa retirado do arquivo para vari�vel (da classe Map)
	for (int linha = 0; linha < tamVertical; linha++) {
		for (int coluna = 0; coluna < tamHorizontal; coluna++) {
			this->map[linha][coluna] = arq.mapa[linha][coluna];
		}
	}

	// limpa mem�ria utilizada para carregar o arquivo
	for (int i = 0; i<arq.lin; i++) {
		free(arq.mapa[i]);
	}
	free(arq.mapa);


	//cria BITMAP do mapa total (antiga drawMap()) -------------------------------------------------------------------------
	ALLEGRO_BITMAP *block = NULL;

	///reserva mem�ria para todos os blocos "virtuais" do mapa
	this->bloco = (BLOCO **)malloc(arq.lin * sizeof(BLOCO*));
	for (int i = 0; i<arq.lin; i++)
		this->bloco[i] = (BLOCO *)malloc(arq.col * sizeof(BLOCO));


	//define tamanho do bitmap total do mapa
	this->imagem = al_create_bitmap(this->getTamHorizontal()*tile->getWidth(), this->getTamVertical()*tile->getHeight());

	block = al_create_bitmap(tile->getWidth(), tile->getHeight());
	int larg_tilemap = al_get_bitmap_width(tile->getImagem()); // obtem a largura do tile_map 

	int tipo = 0;										// guardar� n�mero do ponto no mapa
	for (int linha = 0; linha < tamVertical; linha++) {
		for (int coluna = 0; coluna < tamHorizontal; coluna++) {

			tipo = map[linha][coluna];					// pega o valor espec�fico de um ponto
			al_set_target_bitmap(block);


			//desenha o tile correspondente no block 
			//com os comandos if..else abaixo, a fun��o s� aceita tile maps de 512x32 pixels (2 linhas de 32 tiles)
			if (tipo < larg_tilemap / tile->getWidth()) { // if(tipo <32)
				al_draw_bitmap_region(tile->getImagem(), tile->getWidth()*tipo, 0, tile->getWidth(), tile->getHeight(), 0, 0, 0); // 0 - 31
			}
			else {																 // else ... (>=32)  = segunda linhas do tile_map  
				al_draw_bitmap_region(tile->getImagem(), (tile->getWidth()*tipo) - larg_tilemap, tile->getHeight(), tile->getWidth(), tile->getHeight(), 0, 0, 0); // 32 - ...
			}

			/// seta posi��o e tipo de colis�o dos blocos-------------------
			this->bloco[linha][coluna].x = coluna * tile->getWidth(); 
			this->bloco[linha][coluna].y = linha * tile->getHeight();
			if (tipo == 1)
				this->bloco[linha][coluna].tipo_colisao = TC_PLATAFORMA;
			else if (tipo == 2 || tipo == 3 || tipo == 4 || tipo == 5 || tipo == 6) // depois otimizar  e implementar regras de tiles ou usar propriedade do bloco
				this->bloco[linha][coluna].tipo_colisao = TC_PAREDE;
			else
				this->bloco[linha][coluna].tipo_colisao = TC_SEM_COLISAO;
			///-------------------------------------------------------------
			
			//desenha o bloco an imagem total do mapa na posi��o correspondente
			al_set_target_bitmap(this->imagem);
			al_draw_bitmap(block, coluna*tile->getWidth(), linha*tile->getHeight(), 0);
		}
	}

	al_destroy_bitmap(block);
	return true;
}



//getters and setters
int Map::getTamVertical() {

	return this->tamVertical;
}
int Map::getTamHorizontal() {
	return this->tamHorizontal;
}

ALLEGRO_BITMAP * Map::getImagem()
{
	return this->imagem;
}


Map::~Map() {

	//desaloca espa�o reservado para o mapa e blocos
	for (int i = 0; i < this->getTamVertical(); i++) {
		free(bloco[i]);
		free(map[i]);
	}
	free(bloco);
	free(map);

	al_destroy_bitmap(imagem);
}
