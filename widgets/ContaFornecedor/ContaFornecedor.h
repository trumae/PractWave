#ifndef __CONTA__FORNECEDOR__WIDGET__
#define __CONTA__FORNECEDOR__WIDGET__

#include <string>
#include <Wt/WContainerWidget>
#include "../../logic/Contabilidade.h"
#include "../ContaWidget/ContaWidget.h"
#include "../../logic/Fornecedores.h"
#include "../AjusteContaWidget/AjusteContaWidget.h"
#include "../../apps/App.h"

class ContaFornecedor : public Wt::WContainerWidget {
 public:
  ContaFornecedor(Wt::WContainerWidget *parent,
	       int idconta, App* app, int estadoRetorno, int estadoDadosFornecedor);
  ~ContaFornecedor();
 private:
  ContaWidget *conta_;
  int idconta_;

  AjusteContaWidget *ajuste_;

  App *app_;
  int estadoRetorno_;
  int estadoDadosFornecedor_;

  Wt::WLineEdit *descricao;
  Wt::WLineEdit *valor;

  void viewHome();
  void comprar();
  void pagar();
  void trataCancela();
  void trataComprarOk();
  void trataPagarOk();

  void goDadosFornecedor();
};

#endif

