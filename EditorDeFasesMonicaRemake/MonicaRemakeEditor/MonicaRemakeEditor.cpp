// MonicaRemakeEditor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "allegro5\allegro.h"
#include "allegro5\allegro_image.h"
#include <allegro5\allegro_primitives.h>
#include <iostream>

#include <windows.h>

using namespace ::std;


#include "Jogo.h"

Jogo* jogo = new Jogo(); // inicializa instância do jogo

int main()
{
	// para visualizar o console de acompanhamento em monitor diferente do jogo
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos(consoleWindow, HWND_TOP, 1000, -750, 0, 0, SWP_NOSIZE | SWP_NOZORDER);


	//faz inicializações do jogo
	jogo->init();					// inicializa o jogo (geral, tela, inputs, audio)

	
	//loop principal do jogo
	while (jogo->isRunning()) {		// verifica se jogo ainda está ativo

		jogo->monitoraEventos();	//recebe os eventos a cada loopping para operar nas funções seguintes
		jogo->input();				// verifica entradas
		jogo->update();				// atualiza status dos objetos e da tela
		jogo->render();				// mostra tudo na tela
	}

	delete jogo;					//finaliza instância


	//finaliza programa e aguarda para visualizar resultados
	cout << "Jogo Finalizado" << endl;
	for (int i = 40; i > 0; i--) {
		Sleep(i);
		cout << ".";
	}
	
    return 0;
}

