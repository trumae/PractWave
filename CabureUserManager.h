#ifndef __CABURE_USER_MANAGER__
#define  __CABURE_USER_MANAGER__

#include <cppdb/frontend.h>
#include <string>

class CabureUserSessionDB {
public:
	CabureUserSessionDB();
	~CabureUserSessionDB();
	cppdb::session& db() {
		return session_;
	};

	std::string getPromotor(int i);
	bool cupomJaUsado(std::string s);
	void registraCupomUsado(std::string s);
        void aceitaConvite(std::string email);
private:
	cppdb::session session_;

	void criaTableUser();
	void criaTableDadosUser();
        void incluiColunaConviteAceito();

	//metodos para gerencia do sistema de cupons promocionais
        void criaTablesiCupom();
};

extern CabureUserSessionDB userDB;
#endif

