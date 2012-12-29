#ifndef __PAINELAPP_WIDGET__
#define __PAINELAPP_WIDGET__

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <vector>

#include "../../logic/Clientes.h"
#include "../../logic/Fornecedores.h"

class PainelApp : public Wt::WContainerWidget {
 public:
  PainelApp(Wt::WContainerWidget *parent);
  void viewHome();
  void viewZero(){};
 private:
  enum TipoGrafico {QUEMMEDEVE, PRAQUEMDEVO};

  std::vector<Cliente> clientes_;
  std::vector<Fornecedor> fornecedores_;

  std::string getUrlGrafico(TipoGrafico tipoGrafico);
};

class TilePainel : public Wt::WContainerWidget {
  public:
    TilePainel();
};

#endif
