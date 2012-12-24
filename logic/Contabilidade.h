#ifndef __CABURE__CONTABILIDADE__
#define __CABURE__CONTABILIDADE__

#include <string>
#include <cppdb/frontend.h>
#include <vector>

typedef struct _undLancamento {
  int conta;
  long long valor;
  std::string info;
} undLancamento;

class Contabilidade {
 public:
  Contabilidade(cppdb::session& d);
  
  void criaTabelas();
  
  int adicionaConta(std::string nome, 
		     std::string natureza, 
		     int pai, 
		     int centrodecusto, 
		     std::string info);

  int getIdPorNome(std::string nome);
  std::string getNomePorId(int id);
  std::string getNaturezaPorId(int id);
  int getPaiPorId(int id);
  int getCentrodecustoPorId(int id);
  std::string getInfoPorId(int id);
  void getNomesCentrosDeCusto(
	   std::vector<std::string> &nomes);


  bool temFilho(int id);
  bool existeConta(int id);

  void limpaLancamento();
  void setDataLancamento(std::string data);
  void setDescricaoLancamento(std::string desc);
  void adicionaCredito(int conta,
		       long long valor,
		       std::string info);
  void adicionaDebito(int conta,
		      long long valor,
		      std::string info);

  bool lanca();
  bool estorno(int iddiario, std::string motivo);
  bool removeLancamento(int iddiario);
  bool jaEstornado(int iddiario);


  long long getDebitoContaFolha(int idconta, std::string dataini, std::string datafim);
  long long getCreditoContaFolha(int idconta, std::string dataini, std::string datafim);
  long long getSaldoContaFolha(int idconta);

  long long getSaldo(int idconta);
 private:
  cppdb::session db_;

  std::vector<undLancamento> creditos_;
  std::vector<undLancamento> debitos_;
  std::string dataLancamento_;
  std::string descricao_;

  bool lancamentoOk();
};

#endif


