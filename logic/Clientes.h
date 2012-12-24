#ifndef __CABURE__CLIENTES__
#define __CABURE__CLIENTES__

#include <string>
#include <cppdb/frontend.h>
#include <vector>

#include "Contabilidade.h"
#include "Timeline.h"

class Cliente{
 public:
  Cliente(){}
  Cliente(const std::string pnome,
	  const std::string pendereco,
	  const std::string pbairro,
	  const std::string pcidade,
	  const std::string pestado,
	  const std::string pcep,
	  const std::string ptelefone,
	  const std::string pcelular,
	  const std::string pemail,
	  const std::string pobservacao)
    : nome(pnome),
    endereco(pendereco),
    bairro(pbairro),
    cidade(pcidade),
    estado(pestado),
    cep(pcep),
    telefone(ptelefone),
    celular(pcelular),
    email(pemail),
    observacao(pobservacao){}

  int id;
  std::string nome;
  std::string endereco;
  std::string bairro; 
  std::string cidade;
  std::string estado; 
  std::string cep;
  std::string telefone;
  std::string celular;
  std::string email;
  std::string observacao;
  int idconta;
};

class Clientes {
 public:
  Clientes(cppdb::session& d, Contabilidade *c, Timeline *timeline);
  void criaTabelas();

  bool adiciona(Cliente &c);
  bool salvar(const Cliente &c);

  Cliente getClientePorId(int i);
  Cliente getClientePorNome(std::string s);
  void getTodosClientes(std::vector<Cliente> &v);
  void getTodosClientesComLimite(std::vector<Cliente> &v, int limite);
  void getClientesBuscaComLimite(std::vector<Cliente> &v, std::string padrao, int limite);

  void vender(int idconta, 
				 std::string desc, 
				 long long valor);
  void receber(int idconta,
				 std::string desc, 
				 long long valor);
 private:
  cppdb::session db_;
  Contabilidade *contabilidade_;
  Timeline *timeline_;
};

#endif


