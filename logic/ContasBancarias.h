#ifndef __CABURE__CONTABANCARIA__
#define __CABURE__CONTABANCARIA__

#include <string>
#include <cppdb/frontend.h>
#include <vector>

#include "Contabilidade.h"
#include "Timeline.h"

class ContaBancaria{
 public:
  ContaBancaria(){}
  ContaBancaria(const std::string pnome,
	  const std::string pagencia,
	  const std::string pnumero,
	  const std::string ptelefone)
    : nome(pnome),
	  agencia(pagencia),
	  numero(pnumero),
	  telefone(ptelefone)
  {}

  int id;
  std::string nome;
  std::string agencia; 
  std::string numero;
  std::string telefone;
  int idconta;
};

class ContasBancarias {
 public:
  ContasBancarias(cppdb::session& d, Contabilidade *c, Timeline *timeline);
  void criaTabelas();

  bool adiciona(ContaBancaria &c);
  bool salvar(const ContaBancaria &c);

  ContaBancaria getContaBancariaPorId(int i);
  ContaBancaria getContaBancariaPorNome(std::string n);
  void getTodosContasBancarias(std::vector<ContaBancaria> &v);
  void getTodosContasBancariasComLimite(
				  std::vector<ContaBancaria> &v, 
				  int limite);
  void getContasBancariasBuscaComLimite(std::vector<ContaBancaria> &v, 
				  std::string padrao, int limite);

  void retiradaParaCentroCusto(std::string descr, int codcontabanco,
				  int centro, long long valor);
  void retiradaParaCaixa(std::string descr, int codcontabanco, 
				  long long valor);
 private:
  cppdb::session db_;
  Contabilidade *contabilidade_;
  Timeline *timeline_;
};

#endif


