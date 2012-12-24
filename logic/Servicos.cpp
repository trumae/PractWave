#include "Servicos.h"

#include<iostream>
#include<sstream>
#include<exception>

#include "../CabureApplication.h"

Servicos::Servicos(cppdb::session& d, 
				Contabilidade *c, 
				Timeline *timeline)
  : db_(d),
	contabilidade_(c),	
	timeline_(timeline){
}


void Servicos::criaTabelas(){
  try{
    db_ <<   
      "create table if not exists servico ("
      "   id integer  not null primary key autoincrement, "
      "   nome text not null, "
	  "   descricao text,"
      "   preco integer) " << cppdb::exec;

    db_ << "CREATE UNIQUE INDEX if not exists idx_nome_servico_unique on servico (nome)" << cppdb::exec;

  } catch(std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

bool Servicos::adiciona(Servico &c){
  db_.begin();
  try{
    cppdb::statement stat = db_ << "insert into servico values (null,?,?,?)"
	  << c.nome << c.descricao << c.preco << cppdb::exec;    
    c.id = stat.last_insert_id();
	//adiciona entrada no timeline
	ItemTimeline itemTimeline("", 
					"Adicionado o servico " + c.nome, 
					"Servicos::adiciona(Servico &)",
					"");
	timeline_->adiciona(itemTimeline);
    db_.commit();
    return true;
  } catch (std::exception const &e) {
    db_.rollback();
    std::cerr << "ERROR: " << e.what() << std::endl;
    return false;
  }
}

bool Servicos::salvar(const Servico &c){
  try{
    db_ << "update servico set nome=?, descricao=?, preco=?  where id=?" 
	<< c.nome << c.descricao << c.preco << c.id << cppdb::exec;
	ItemTimeline itemTimeline("", 
			"O cadastro do servico '" + c.nome + "' foi editado" , 
			"Servicos::adiciona(Servico &)",
			"");
	timeline_->adiciona(itemTimeline);
    return true;
  }catch(std::exception const &e){
    std::cerr << "ERROR: " << e.what() << std::endl;
    return false;
  }
}

Servico Servicos::getServicoPorId(int i){
  Servico s;
  cppdb::result res = db_ << "select id, nome, descricao, preco"
    " from servico where id = ?" << i << cppdb::row;
  if(res.empty()){
    throw "Nao encontrei servico com id = " + i;
  } else {
    res >> s.id >> s.nome >> s.descricao >> s.preco;
  } 
  return s;
}

void Servicos::getTodosServicos(std::vector<Servico> &v){
  v.clear();
  cppdb::result res = db_ << "select id, nome, descricao, preco "
    " from servico";
  while(res.next()){
    Servico s;
    res >> s.id >> s.nome >> s.descricao >> s.preco;
    v.push_back(s);
  } 
}

void Servicos::getTodosServicosComLimite(std::vector<Servico> &v, int limite){
  v.clear();
  std::stringstream ss;
  ss << limite;
  std::string slimite = ss.str();
  cppdb::result res = db_ << "select id, nome, descricao, preco "
    " from servico limit " + slimite; 
  while(res.next()){
    Servico s;
    res >> s.id >> s.nome >> s.descricao >> s.preco;
    v.push_back(s);
  } 
}

void Servicos::getServicosBuscaComLimite(std::vector<Servico> &v, 
					 std::string padrao, int limite){
  v.clear();
  std::stringstream ss;
  ss << limite;
  std::string slimite = ss.str();
  cppdb::result res = db_ << "select id, nome, descricao, preco "
    " from servico " 
	" where nome like ?"
	" limit " + slimite << "%" + padrao + "%"; 
  while(res.next()){
    Servico s;
    res >> s.id >> s.nome >> s.descricao >> s.preco;
    v.push_back(s);
  }
}







