#ifndef __CABURE__MEUS_DADOS__
#define __CABURE__MEUS_DADOS__

#include <cppdb/frontend.h>
#include <iostream>
#include "../core/CabureUserManager.h"

class MeusDados {
	public:
		MeusDados();
		int idUser;
		std::string emailAlt;
		std::string profissao;
		std::string cidade;
		std::string estado;
		int autorizacao;

		bool updateDados();
		void buscaDadosUsuario();
		void criaTabelas();
};

#endif
