#include "Contabilidade.h"

#include<iostream>
#include<exception>

Contabilidade::Contabilidade(cppdb::session& d)
  : db_(d){
}


void Contabilidade::criaTabelas(){
  try{
    //cria e inicia dados da tabela conta
    db_ <<   
      "create table if not exists conta ("
      "   id integer  not null primary key autoincrement, "
      "   nome text not null, "
      "   natureza text not null,"
      "   pai integer not null, "
      "   centrodecusto integer, "
      "   info text ) " << cppdb::exec;
    db_ << " create index if not exists nome_idx_unique_conta on conta (nome) " << cppdb::exec;
    db_ << " create index if not exists pai_idx_conta on conta (pai) " << cppdb::exec;

    adicionaContaSeNaoExiste("ATIVO",  "DEVEDORA", -1, 0, "");   
    adicionaContaSeNaoExiste("PASSIVO",  "CREDORA", -1, 0, "");   
    adicionaContaSeNaoExiste("DISPONIVEL DE CURTO PRAZO",  "DEVEDORA", 1, 0, "");   

    adicionaContaSeNaoExiste("RECEBER",  "DEVEDORA", 1, 0, "");   
    adicionaContaSeNaoExiste("ESTOQUE",  "DEVEDORA", 1, 0, "");   
    adicionaContaSeNaoExiste("CLIENTES",  "DEVEDORA", 1, 0, "");   
    adicionaContaSeNaoExiste("PAGAR",  "CREDORA", 2, 0, "");   
    adicionaContaSeNaoExiste("PATRIMONIO LIQUIDO",  "CREDORA", 2, 0, "");   
    adicionaContaSeNaoExiste("CAIXA",  "DEVEDORA", 3, 0, "");   
    adicionaContaSeNaoExiste("BANCOS",  "DEVEDORA", 3, 0, "");   
    adicionaContaSeNaoExiste("RESULTADOS",  "DEVEDORA", 8, 0, "");   
    adicionaContaSeNaoExiste("DESPESAS",  "DEVEDORA", 11, 0, "");   
    adicionaContaSeNaoExiste("RECEITAS",  "CREDORA", 11, 0, "");   

    adicionaContaSeNaoExiste("FORNECEDORES",  "CREDORA", 2, 0, "");   
    adicionaContaSeNaoExiste("RECEITAS DE SERVICOS",  "CREDORA", 13, 0, "");   
    adicionaContaSeNaoExiste("RECEITAS DE PRODUTOS",  "CREDORA", 13, 0, "");   
    adicionaContaSeNaoExiste("RECEITAS FINANCEIRAS",  "CREDORA", 13, 0, "");   
    adicionaContaSeNaoExiste("DESPESAS FISCAIS",  "DEVEDORA", 12, 1, "");   
    adicionaContaSeNaoExiste("DESPESAS COM ALUGUEL",  "DEVEDORA", 12, 1, "");   
    adicionaContaSeNaoExiste("DESPESAS COM AGUA",  "DEVEDORA", 12, 1, "");       
    adicionaContaSeNaoExiste("DESPESAS COM LUZ",  "DEVEDORA", 12, 1, "");   
    adicionaContaSeNaoExiste("DESPESAS COM TELEFONE",  "DEVEDORA", 12, 1, "");   
    adicionaContaSeNaoExiste("DESPESAS COM INTERNET",  "DEVEDORA", 12, 1, "");   
    adicionaContaSeNaoExiste("DESPESAS COM SALARIO",  "DEVEDORA", 12, 1, "");   
    adicionaContaSeNaoExiste("DESPESAS COM CARTAO",  "DEVEDORA", 12, 1, "");   
    adicionaContaSeNaoExiste("CUSTOS PRODUTOS VENDIDOS",  "DEVEDORA", 12, 1, "");       
    adicionaContaSeNaoExiste("CUSTOS COMISSAO FUNCIONARIOS",  "DEVEDORA", 12, 1, "");   

    adicionaContaSeNaoExiste("INSUMOS PARA SERVICOS",  "DEVEDORA", 12, 1, "");   
    adicionaContaSeNaoExiste("DESPESAS GERAIS",  "DEVEDORA", 12, 1, "");   
    adicionaContaSeNaoExiste("FRETE",  "DEVEDORA", 12, 1, "");   
    adicionaContaSeNaoExiste("DESCONTOS",  "DEVEDORA", 12, 1, "");   
    adicionaContaSeNaoExiste("VENDA AVULSA",  "DEVEDORA", 6, 0, "");   
    adicionaContaSeNaoExiste("CARTAO",  "DEVEDORA", 1, 0, "");   
    adicionaContaSeNaoExiste("OBRAS",  "DEVEDORA", 1, 0, "");   
    adicionaContaSeNaoExiste("CREDIARIO",  "DEVEDORA", 1, 0, "");   
    adicionaContaSeNaoExiste("AJUSTES",  "CREDORA", 11, 1, "");   

    //cria tabela diario
    
    db_ << "CREATE TABLE if not exists diario ( "
      " id integer  not null primary key autoincrement, "
      " data datetime, "
      " descricao text, " 
      " estornado integer not null default 0)" << cppdb::exec;

    //cria tabela razonete

    db_ << "CREATE TABLE if not exists razonete ( "
      " id integer  not null primary key autoincrement, "
      " id_diario integer  not null , "
      " id_conta integer  not null , "
      " debito integer, "
      " credito integer, "
      " info text ) " << cppdb::exec;


  } catch(std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

int Contabilidade::adicionaContaSeNaoExiste(std::string nome, 
				  std::string natureza, 
				  int pai, 
				  int centrodecusto, 
				  std::string info){
  try{
    cppdb::result res = db_ << "select id from conta where nome = ?" 
			    << nome << cppdb::row;
    if(!res.empty()){
      return res.get<int>("id");
    } else {      
      cppdb::statement stat = db_ << "insert into conta values (null, ?,?,?,?,?)" 
				  << nome << natureza << pai << centrodecusto << info
				  << cppdb::exec;
      return stat.last_insert_id();
    }
  } catch(...){
    return -1;
  }
}

int Contabilidade::adicionaConta(std::string nome, 
				  std::string natureza, 
				  int pai, 
				  int centrodecusto, 
				  std::string info){
  try{
    cppdb::statement stat = db_ << "insert into conta values (null, ?,?,?,?,?)" 
				<< nome << natureza << pai << centrodecusto << info
				<< cppdb::exec;
    return stat.last_insert_id();
  } catch(...){
    return -1;
  }
}

int Contabilidade::getIdPorNome(std::string nome){
  cppdb::result res = db_ << "select id from conta where nome = ?" 
			  << nome << cppdb::row;
  if(!res.empty()){
    return res.get<int>("id");
  }
  return -1;
}

std::string Contabilidade::getNomePorId(int id) {
  cppdb::result res = db_ << "select nome from conta where id = ?" 
			  << id << cppdb::row;
  return res.get<std::string>("nome");
}

std::string Contabilidade::getNaturezaPorId(int id) {
  cppdb::result res = db_ << "select natureza from conta where id = ?" 
			  << id << cppdb::row;
  return res.get<std::string>("natureza");
}

int Contabilidade::getPaiPorId(int id){
  cppdb::result res = db_ << "select pai from conta where id = ?" 
			  << id << cppdb::row;
  return res.get<int>("nome");
}

int Contabilidade::getCentrodecustoPorId(int id){
  cppdb::result res = db_ << "select centrodecusto from conta where id = ?" 
			  << id << cppdb::row;
  return res.get<int>("centrodecusto");
}

std::string Contabilidade::getInfoPorId(int id) {
  cppdb::result res = db_ << "select info from conta where id = ?" 
			  << id << cppdb::row;
  return res.get<std::string>("info");
}

bool Contabilidade::temFilho(int id){
  cppdb::result res = 
    db_ << "select id  FROM conta  where pai = ?"
	<< id;
  if(res.empty())
    return false;
  else
    return true;
}

bool Contabilidade::existeConta(int id){
  cppdb::result res = 
    db_ << "select nome from conta where id = ?"
	<< id << cppdb::row;
  if(res.empty())
    return false;
  else
    return true;
}

void Contabilidade::limpaLancamento(){
  creditos_.clear();
  debitos_.clear();
  dataLancamento_ = "";
  descricao_ = "";
}

void Contabilidade::setDataLancamento(std::string data) {
  dataLancamento_ = data;
}

void Contabilidade::setDescricaoLancamento(std::string desc){
  descricao_ = desc;
}

void Contabilidade::adicionaCredito(int conta,
				    long long valor,
				    std::string info){
  undLancamento und = {conta, valor, info};
  creditos_.push_back(und);
}

void Contabilidade::adicionaDebito(int conta,
				   long long valor,
				   std::string info){
  undLancamento und = {conta, valor, info};
  debitos_.push_back(und);
}

bool Contabilidade::lancamentoOk(){
  std::vector<undLancamento>::iterator it;
  long long sumCreditos = 0;
  long long sumDebitos = 0;

  //verifica se todas as contas de credito existem
  for(it = creditos_.begin(); it < creditos_.end(); it++){
    if (!existeConta((*it).conta))
      return false;
    sumCreditos += (*it).valor;
  }
  
  //verifica se todas as contas de debitos existem
  for(it = debitos_.begin(); it < debitos_.end(); it++){
    if (!existeConta((*it).conta))
      return false;
    sumDebitos += (*it).valor;
  }

  if(sumCreditos != sumDebitos) 
    return false;
  
  return true;
}

bool Contabilidade::lanca(){
  cppdb::statement stat;
  std::vector<undLancamento>::iterator it;
  if(!lancamentoOk())
    return false;
  
  try{
    db_.begin();

    //lanca diario
    if(dataLancamento_.size() > 0){
      stat = db_ << "INSERT INTO diario VALUES(null,?,?, ?) " 
		 << dataLancamento_ << descricao_ << 0 << cppdb::exec;
    } else {
      stat = db_ << "INSERT INTO diario VALUES(null,date('now'),?, ?) " 
		 <<descricao_ << 0 << cppdb::exec;
    }

    int id_diario = stat.last_insert_id();
    for(it = debitos_.begin(); it < debitos_.end(); it++){
      db_ << "INSERT INTO razonete VALUES(null,?,?,?,?,?) " 
	  << id_diario << (*it).conta << (*it).valor
	  << 0 << (*it).info << cppdb::exec;
    }
    for(it = creditos_.begin(); it < creditos_.end(); it++){
      db_ << "INSERT INTO razonete VALUES(null,?,?,?,?,?) " 
	  << id_diario << (*it).conta << 0 << (*it).valor
	  << (*it).info << cppdb::exec;
    }
	limpaLancamento();
    db_.commit();
    return true;
  } catch (...) {
    db_.rollback();
    return false;
  }
}

bool Contabilidade::estorno(int iddiario, std::string motivo){
  if (jaEstornado(iddiario))
    return false;
  
  cppdb::result res;
  cppdb::statement stat;
  try{
    db_.begin();
    res = db_ << "select descricao from diario where id = ?" 
	      << iddiario << cppdb::row;
    std::string descricao = res.get<std::string>("descricao");
    descricao = "ESTORNO: " + descricao + motivo;
    
    stat = db_ << "INSERT INTO diario VALUES(null,date('now'),?,?)"
	       << descricao << 1 << cppdb::exec;
    int iddiarioestorno = stat.last_insert_id();

    res = db_ << "select debito,credito,info,id_conta from razonete where id_diario = ?"
	      << iddiario;
    
    while(res.next()){
      int debito, credito, idconta;
      std::string info;
      res >> debito >> credito >> info >> idconta;
      db_ << "INSERT INTO razonete(id,id_diario,id_conta,debito,credito,info) VALUES(null,?,?,?,?,?)"
	  << iddiarioestorno << idconta << credito << debito << info << cppdb::exec;	
    }

    db_ << "update diario set estornado = 1 where id = ?" 
	<< iddiario << cppdb::exec;

    db_.commit();
    return true;
  } catch(...) {
    db_.rollback();
    return false;
  }
}

bool Contabilidade::jaEstornado(int iddiario){
  cppdb::result res = db_ << "select estornado from diario where id = ?" 
			  << iddiario << cppdb::row;
  return (res.get<int>("estornado") == 1? true:false);
}

long long Contabilidade::getDebitoContaFolha(int idconta, std::string dataini, std::string datafim){
  cppdb::result res = db_ << 
    "select sum(credito) as soma FROM razonete inner join diario on(razonete.id_diario = diario.id)" 
    "  where id_conta = ? AND estornado = 0 AND data >= ? AND data <= ?" 
			  << idconta << dataini << datafim << cppdb::row;
  return res.get<long long>("soma");
}

long long Contabilidade::getCreditoContaFolha(int idconta, std::string dataini, std::string datafim){
  cppdb::result res = db_ << 
    "select sum(debito) as soma FROM razonete inner join diario on(razonete.id_diario = diario.id)" 
    "  where id_conta = ? AND estornado = 0 AND data >= ? AND data <= ?" 
			  << idconta << dataini << datafim << cppdb::row;
  return res.get<long long>("soma");
}

long long Contabilidade::getSaldoContaFolha(int idconta){
  cppdb::result res;
  long long ret = 0;
  std::string nat = getNaturezaPorId(idconta);
  if( nat == "DEVEDORA") {
    res = db_ <<
      "select (sum(debito) - sum(credito)) as total FROM razonete inner join diario" 
      " on(razonete.id_diario = diario.id)" 
      " where id_conta = ? AND estornado = 0" 
	      << idconta;
	res.next();
    res >> ret;
  } else {
    res = db_ <<
      "select (sum(credito) - sum(debito)) as total FROM razonete inner join diario" 
      " on(razonete.id_diario = diario.id)" 
      " where id_conta = ? AND estornado = 0" 
	      << idconta;
	res.next();
    res >> ret;
  }
  return ret;
}

long long Contabilidade::getSaldo(int idconta){
    return 0;
};

void Contabilidade::getNomesCentrosDeCusto
   (std::vector<std::string> &nomes){
   nomes.clear();
   cppdb::result res = db_ << 
	 "select nome from conta where centrodecusto = 1";
   while(res.next()){
	 std::string n;
	 res >> n;
	 nomes.push_back(n);
   }
}

bool Contabilidade::removeLancamento(int iddiario){
   try{
      db_.begin();
      db_ << "delete from diario where id = ?" 
          << iddiario << cppdb::exec;
      db_ << "delete from razonete where id_diario = ?" 
          << iddiario << cppdb::exec;
	  db_.commit();
   } catch (...) {
	  db_.rollback();
      return false;
   }
   return true;
}


