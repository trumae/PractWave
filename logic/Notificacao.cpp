#include "Notificacao.h"

using namespace std;

Notificacao::Notificacao(string to, string script):
   to_(to), script_(script){
}

Notificacao::~Notificacao(){
}

void Notificacao::send(){
   cppdb::session session;
   session.open("sqlite3:db=./email/email.db");
   session << "insert into email (id, toemail, tipo, criadoem, enviado,script) values "
              "(NULL, ?, 'N', datetime('now'), 0, ?)" << to_ << script_ << cppdb::exec;
}

