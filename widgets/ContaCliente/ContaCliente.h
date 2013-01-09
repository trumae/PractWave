#ifndef __CONTA__CLIENTE__WIDGET__
#define __CONTA__CLIENTE__WIDGET__

#include <string>
#include <Wt/WContainerWidget>
#include "../../logic/Contabilidade.h"
#include "../ContaWidget/ContaWidget.h"
#include "../../logic/Clientes.h"
#include "../AjusteContaWidget/AjusteContaWidget.h"
#include "../../apps/App.h"

class ContaCliente : public Wt::WContainerWidget {
 public:
  ContaCliente(Wt::WContainerWidget *parent,
	       int idconta, App* app, int estadoRetorno, int estadoDadosCliente);
  ~ContaCliente();
 private:
  ContaWidget *conta_;
  int idconta_;

  AjusteContaWidget *ajuste_;

  App *app_;
  int estadoRetorno_;
  int estadoDadosCliente_;

  Wt::WLineEdit *descricao;
  Wt::WLineEdit *valor;

  void viewHome();
  void anotar();
  void receber();
  void trataCancela();
  void trataAnotarOk();
  void trataReceberOk();

  void goDadosCliente();
};

#endif

