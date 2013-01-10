#ifndef __CONTABANCARIA_APP__
#define __CONTABANCARIA_APP__

#include <Wt/WContainerWidget>
#include <string>

#include "../../logic/ContasBancarias.h"
#include "../App.h"

class ContaBancariaApp : public App {
 protected:
  int LISTABANCO;
  int ADICIONABANCO;
  int CONTABANCO;
  int DADOSBANCO;
  int RETIRADABANCO;
  int RETIRADACAIXABANCO;

  virtual std::string getTitulo();
  virtual void init();

  Wt::WWidget *listaBanco();
  Wt::WWidget *adicionaBanco();
  Wt::WWidget *contaBanco();
  Wt::WWidget *dadosBanco();
  Wt::WWidget *retiradaBanco();
  Wt::WWidget *retiradaCaixaBanco();

 public:
  ContaBancariaApp(Wt::WContainerWidget *parent);
};

class TileContaBancaria : public Wt::WContainerWidget {
  public:
    TileContaBancaria();
};

#endif
