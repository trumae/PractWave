#include "Clientes.h"

#include<iostream>
#include<sstream>
#include<exception>
#include<Wt/WString>

#include "../core/CabureApplication.h"
#include "Moeda.h"

Clientes::Clientes(cppdb::session& d, 
				Contabilidade *c, 
				Timeline *timeline)
  : db_(d),
	contabilidade_(c),	
	timeline_(timeline){
}


void Clientes::criaTabelas(){
  try{
    db_ <<   
      "create table if not exists cliente ("
      "   id integer  not null primary key autoincrement, "
      "   nome text not null, "
      "   endereco text, "
      "   bairro text, "
      "   cidade text, "
      "   estado text, "
      "   cep text, "
      "   telefone text, "
      "   celular text, "
      "   email text, "
      "   observacao text, "
      "   idconta integer not null ) " << cppdb::exec;

    db_ << "CREATE UNIQUE INDEX if not exists idx_nome_Cliente_unique on cliente (nome)" << cppdb::exec;

  } catch(std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

bool Clientes::adiciona(Cliente &c){
  CabureApplication *cabure = CabureApplication::cabureApplication();  
  db_.begin();
  Contabilidade *cont = cabure->contabilidade_;
  try{
    int idconta = cont->adicionaConta(c.nome, 
				      "DEVEDORA",
				      cont->getIdPorNome("CLIENTES"),
				      0, "");
    cppdb::statement stat = db_ << "insert into cliente values (null,?,?,?,?,?,?,?,?,?,?,?)"
				<< c.nome << c.endereco << c.bairro << c.cidade << c.estado << c.cep
				<< c.telefone << c.celular << c.email << c.observacao 
				<< idconta << cppdb::exec;    
    c.idconta = idconta;
    c.id = stat.last_insert_id();
	//adiciona entrada no timeline
	ItemTimeline itemTimeline("", 
					Wt::WString::tr("msg-new-customer-ok").toUTF8() + " " + c.nome, 
					"Clientes::adiciona(Cliente &)",
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

bool Clientes::salvar(const Cliente &c){
  try{
    db_ << "update cliente set nome=?, endereco=?, bairro=?, cidade=?, "
      "estado=?, cep=?, telefone=?, celular=?, email=?, observacao=?  where id=?" 
	<< c.nome << c.endereco << c.bairro << c.cidade << c.estado << c.cep
	<< c.telefone << c.celular << c.email << c.observacao 
	<< c.id << cppdb::exec;
	ItemTimeline itemTimeline("", 
			Wt::WString::tr("msg-edit-customer-ok").toUTF8() + " " + c.nome, 
			"Clientes::adiciona(Cliente &)",
			"");
	timeline_->adiciona(itemTimeline);
    return true;
  }catch(std::exception const &e){
    std::cerr << "ERROR: " << e.what() << std::endl;
    return false;
  }
}

Cliente Clientes::getClientePorId(int i){
  Cliente cli;

  cppdb::result res = db_ << "select id, nome, endereco, bairro, cidade, estado, cep, telefone, "
    "celular, email, observacao, idconta from cliente where id = ?" << i << cppdb::row;
  if(res.empty()){
    throw "Nao encontrei cliente com id = " + i;
  } else {
    res >> cli.id >> cli.nome >> cli.endereco >> cli.bairro >> cli.cidade >> cli.estado 
	>> cli.cep >> cli.telefone >> cli.celular >> cli.email >> cli.observacao >> cli.idconta;
  }
  return cli;
}

Cliente Clientes::getClientePorNome(std::string s){
  Cliente cli;

  cppdb::result res = db_ << "select id, nome, endereco, bairro, cidade, estado, cep, telefone, "
    "celular, email, observacao, idconta from cliente where nome = ?" << s << cppdb::row;
  if(res.empty()){
    throw "Nao encontrei cliente com nome = " + s;
  } else {
    res >> cli.id >> cli.nome >> cli.endereco >> cli.bairro >> cli.cidade >> cli.estado 
	>> cli.cep >> cli.telefone >> cli.celular >> cli.email >> cli.observacao >> cli.idconta;
  }
  return cli;
}

void Clientes::getTodosClientes(std::vector<Cliente> &v){
  v.clear();
  cppdb::result res = db_ << "select id, nome, endereco, bairro, cidade, estado, cep, telefone, "
    "celular, email, observacao, idconta from cliente";
  while(res.next()){
    Cliente cli;
    res >> cli.id >> cli.nome >> cli.endereco >> cli.bairro >> cli.cidade >> cli.estado 
	>> cli.cep >> cli.telefone >> cli.celular >> cli.email >> cli.observacao >> cli.idconta;
    v.push_back(cli);
  }  
}

void Clientes::getTodosClientesComLimite(std::vector<Cliente> &v, int limite){
  v.clear();
  std::stringstream ss;
  ss << limite;
  std::string slimite = ss.str();
  cppdb::result res = db_ << "select id, nome, endereco, bairro, cidade, estado, cep, telefone, "
    "celular, email, observacao, idconta from cliente limit " + slimite; 
  while(res.next()){
    Cliente cli;
    res >> cli.id >> cli.nome >> cli.endereco >> cli.bairro >> cli.cidade >> cli.estado 
	>> cli.cep >> cli.telefone >> cli.celular >> cli.email >> cli.observacao 
	>> cli.idconta;
    v.push_back(cli);
  } 
}

void Clientes::getClientesBuscaComLimite(std::vector<Cliente> &v, 
					 std::string padrao, int limite){
  v.clear();
  std::stringstream ss;
  ss << limite;
  std::string slimite = ss.str();
  cppdb::result res = db_ << "select id, nome, endereco, bairro, cidade, estado, cep, telefone, "
    "celular, email, observacao, idconta from cliente " 
	" where nome like ?"
	" limit " + slimite << "%" + padrao + "%"; 
  while(res.next()){
    Cliente cli;
    res >> cli.id >> cli.nome >> cli.endereco >> cli.bairro >> cli.cidade >> cli.estado 
	>> cli.cep >> cli.telefone >> cli.celular >> cli.email >> cli.observacao 
	>> cli.idconta;
    v.push_back(cli);
  } 
}


void Clientes::vender(int idconta, 
				std::string desc, 
				long long valor){
    contabilidade_->limpaLancamento();

	Moeda mval(valor);
    contabilidade_->setDescricaoLancamento(
					Wt::WString::tr("sell").toUTF8() + " - " + desc);
	contabilidade_->adicionaDebito(idconta, valor, "");
	contabilidade_->adicionaCredito(
					contabilidade_->getIdPorNome("RECEITAS DE SERVICOS"),
				   	valor, "");
	contabilidade_->lanca();

    ItemTimeline itemTimeline("",
                Wt::WString::tr("sell-for").toUTF8() 
							  + contabilidade_->getNomePorId(idconta) + 
							  " $ " + 
							  mval.valStr() + " - " 
							  + desc,
                "Clientes::vender()",
                "");
    timeline_->adiciona(itemTimeline);
}

void Clientes::receber(int idconta,  
				std::string desc,
			   	long long valor){
    contabilidade_->limpaLancamento();

	Moeda mval(valor);
    contabilidade_->setDescricaoLancamento(
					Wt::WString::tr("receive").toUTF8() + " - " + desc);
	contabilidade_->adicionaDebito(
					contabilidade_->getIdPorNome("CAIXA"), valor, "");
	contabilidade_->adicionaCredito(
					idconta,
				   	valor, "");
	contabilidade_->lanca();

    ItemTimeline itemTimeline("",
                Wt::WString::tr("receive").toUTF8() 
							  + contabilidade_->getNomePorId(idconta) + 
							  " $ " + 
							  mval.valStr() + " - " 
							  + desc,
                "Clientes::receber()",
                "");
    timeline_->adiciona(itemTimeline);
}


