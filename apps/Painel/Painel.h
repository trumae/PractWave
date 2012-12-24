#ifndef __DASHBOARD_WIDGET__
#define __DASHBOARD_WIDGET__

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <vector>

class Painel : public Wt::WContainerWidget {
 public:
  Painel(Wt::WContainerWidget *parent);
  void viewHome();
  void viewZero(){};
 private:
  enum TipoGrafico {QUEMMEDEVE, PRAQUEMDEVO};

  std::vector<Cliente> clientes_;
  std::vector<Fornecedor> fornecedores_;

  std::string getUrlGrafico(TipoGrafico tipoGrafico);
};

#endif
