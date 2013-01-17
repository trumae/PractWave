#ifndef __CAIXA_APP__
#define __CAIXA_APP__

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WComboBox>
#include <string>

#include "../../logic/Caixa.h"
#include "../App.h"
#include "../../widgets/AjusteContaWidget/AjusteContaWidget.h"
#include "../../widgets/ContaWidget/ContaWidget.h"

class CaixaApp : public App {
 protected:
  int CONTACAIXA;
  int RETIRADA;
  int RETIRADADEPOSITO;

  AjusteContaWidget *ajuste_;
  Wt::WLineEdit *descricao;
  Wt::WLineEdit *valor;
  Wt::WComboBox *centros;
  Wt::WComboBox *bancos;


  virtual std::string getTitulo();
  virtual void init();

  Wt::WWidget *EContaCaixa();
  Wt::WWidget *ERetirada();
  Wt::WWidget *ERetiradaDeposito();

  void TFazRetirada();
  void TFazRetiradaDeposito();

  bool CTemContasBancarias();

 public:
  CaixaApp(Wt::WContainerWidget *parent);
  ~CaixaApp();
};

class TileCaixa : public Wt::WContainerWidget {
  public:
    TileCaixa();
};

#endif
