#ifndef __CAIXA_APP__
#define __CAIXA_APP__

#include <Wt/WContainerWidget>
#include <string>

#include "../../logic/Caixa.h"
#include "../App.h"

class CaixaApp : public App {
 protected:
  int CONTACAIXA;
  int RETIRADA;
  int RETIRADADEPOSITO;

  virtual std::string getTitulo();
  virtual void init();

  Wt::WWidget *contaCaixa();
  Wt::WWidget *retirada();
  Wt::WWidget *retiradaDeposito();

  void fazRetirada();
  void fazRetiradaDeposito();

 public:
  CaixaApp(Wt::WContainerWidget *parent);
};

class TileCaixa : public Wt::WContainerWidget {
  public:
    TileCaixa();
};

#endif
