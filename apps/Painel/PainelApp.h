#ifndef __PAINELAPP_WIDGET__
#define __PAINELAPP_WIDGET__

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <vector>

#include "../../logic/Clientes.h"
#include "../../logic/Fornecedores.h"
#include "../App.h"

class PainelApp : public App {
 protected:

  // Estados
  int PAINEL;

  virtual std::string getTitulo();

  void init();
  Wt::WWidget *Painel();
 public:
  PainelApp(Wt::WContainerWidget *parent);
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
