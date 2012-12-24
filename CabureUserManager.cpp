#include "CabureUserManager.h"

#include <iostream>
#include <cppdb/frontend.h>

using namespace std;

CabureUserSessionDB::CabureUserSessionDB() {
    session_.open("sqlite3:db=user.db");

    criaTableUser();
    criaTableDadosUser();
    incluiColunaConviteAceito();
    criaTablesiCupom();
}

void CabureUserSessionDB::criaTableUser() {
    try {
        session_
                << "CREATE TABLE IF NOT EXISTS USER (    id   INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,    name    TEXT,   email   TEXT )  "
                << cppdb::exec;
    } catch (...) {
        std::cerr << "Erro ao criar tabela USER" << std::endl;
    }
}

void CabureUserSessionDB::incluiColunaConviteAceito() {
    try {
        session_
                << "ALTER TABLE  USER ADD COLUMN conviteaceito integer default 0 "
                << cppdb::exec;
    } catch (...) {
        std::cerr << "Erro ao adicionar a coluna 'conviteaceito' na tabela USER, provavelmente ela ja existe!" << std::endl;
    }
}

void CabureUserSessionDB::criaTablesiCupom(){
    try {
        session_
                << "CREATE TABLE IF NOT EXISTS CUPOM ( id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, codigo TEXT)"
                << cppdb::exec;
        session_
                << "CREATE TABLE IF NOT EXISTS PROMOTOR ( id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, nome TEXT)"
                << cppdb::exec;
	session_ << "create unique index if not exists promotor_unico on PROMOTOR (nome)" << cppdb::exec;
    } catch (...) {
        std::cerr << "Erro ao criar tabelas de cupom!" << std::endl;
    }
    try{
	session_ << "insert into PROMOTOR values (null, 'CM Softwares')" << cppdb::exec;
    } catch(...){
	std::cerr << "CM Software ja eh um promotor cadastrado" << std::endl;
    }
}

CabureUserSessionDB::~CabureUserSessionDB() {
    session_.close();
}

string CabureUserSessionDB::getPromotor(int i){
    cppdb::result res = session_ << "select nome from PROMOTOR where id = ? "
	    << i << cppdb::row;
    return res.get<string>("nome");
}

bool CabureUserSessionDB::cupomJaUsado(string s){
    cppdb::result res = session_ << "select * from CUPOM where codigo = ?"
	    << s << cppdb::row;
    if(res.empty()){
      return false;
    }
    return true;
}

void CabureUserSessionDB::registraCupomUsado(string s){
   session_ << "insert into CUPOM values (NULL, ?)" << s
	   << cppdb::exec;
}

void CabureUserSessionDB::aceitaConvite(string email) {
   session_ << "update user set conviteaceito = 1 where email = ?" << email << cppdb::exec;
}

CabureUserSessionDB userDB;


void CabureUserSessionDB::criaTableDadosUser() {
    try {
        session_
                << "CREATE TABLE IF NOT EXISTS dados_user (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
					"id_user INTEGER, email_alternativo TEXT, profissao TEXT, "
					"cidade TEXT, estado TEXT, autorizacao_email integer default 1)"
                << cppdb::exec;
    } catch (...) {
        std::cerr << "Erro ao criar tabela DADOS_USER" << std::endl;
    }
}

