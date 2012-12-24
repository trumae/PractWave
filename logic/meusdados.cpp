#include "meusdados.h"

MeusDados::MeusDados(){
}

void MeusDados::criaTabelas() {
  /*  try {
        userDB.db()
                << "CREATE TABLE IF NOT EXISTS dados_user (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
					"id_user INTEGER, email_alternativo TEXT, profissao TEXT, cidade TEXT, estado TEXT, autorizacao_email TEXT)"
                << cppdb::exec;
    } catch (...) {
        std::cerr << "Erro ao criar tabela DADOS_USER" << std::endl;
    }*/
}

bool MeusDados::updateDados() {
    try {
        userDB.db()
                << "UPDATE dados_user SET email_alternativo=?, profissao=?, cidade=?, estado=?, autorizacao_email=? "
				"WHERE id_user=?"
                << emailAlt << profissao << cidade << estado << autorizacao << idUser << cppdb::exec;
		return true;
    } catch (...) {
        std::cerr << "Erro ao atualizar dados da tabela DADOS_USER" << std::endl;
		return false;
    }
}

void MeusDados::buscaDadosUsuario() {
	cppdb::result res = userDB.db() << "SELECT email_alternativo, profissao, cidade, estado, autorizacao_email "
									"FROM dados_user WHERE id_user = ?"
									<< idUser << cppdb::row;
	
	if(!res.empty()){
		res >> emailAlt >> profissao >> cidade >> estado >> autorizacao;
	}
}
