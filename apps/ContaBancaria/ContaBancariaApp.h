#ifndef __CONTABANCARIA_APP__
#define __CONTABANCARIA_APP__

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <string>

#include "../../logic/ContasBancarias.h"
#include "../App.h"
#include "../../widgets/AjusteContaWidget/AjusteContaWidget.h"

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

  void adicionaContaBancaria();
  void salvaContaBancaria();
  void retirada();
  void retiradaContaBancaria();

  AjusteContaWidget *ajuste_;
  Wt::WLineEdit *buscaEdit;
  Wt::WPushButton *buscaBtn;
  Wt::WLineEdit *editNome_;
  Wt::WLineEdit *editAgencia_;
  Wt::WLineEdit *editNumero_;
  Wt::WLineEdit *editTelefone_;
  int editandoId;

  Wt::WWidget *createFormContaBancaria (ContaBancaria cb);
  void constroiTabela();

  int idconta_;
  int codContaBancaria_;
  void entraContaBanco(); /* aloca ajusteConta e seta iddaconta */
  void backContaBanco(); /* libera ajustaConta */ 
 public:
  ContaBancariaApp(Wt::WContainerWidget *parent);
  ~ContaBancariaApp();
};

class TileContaBancaria : public Wt::WContainerWidget {
  public:
    TileContaBancaria();
};

#endif
