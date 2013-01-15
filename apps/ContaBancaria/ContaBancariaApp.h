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

  Wt::WWidget *EListaBanco();
  Wt::WWidget *EAdicionaBanco();
  Wt::WWidget *EContaBanco();
  Wt::WWidget *EDadosBanco();
  Wt::WWidget *ERetiradaBanco();
  Wt::WWidget *ERetiradaCaixaBanco();

  void TAdicionaContaBancaria();
  void TSalvaContaBancaria();
  void TRetirada();
  void TRetiradaCaixa();

  AjusteContaWidget *ajuste_;
  Wt::WLineEdit *buscaEdit;
  Wt::WPushButton *buscaBtn;
  Wt::WLineEdit *editNome_;
  Wt::WLineEdit *editAgencia_;
  Wt::WLineEdit *editNumero_;
  Wt::WLineEdit *editTelefone_;

  Wt::WWidget *createFormContaBancaria ();
  void constroiTabela();

  int idconta_;
  int codContaBancaria_;
  ContaBancaria cb_;
  void TEntraContaBanco(int id); /* aloca ajusteConta e seta iddaconta */
  void TBackContaBanco(); /* libera ajustaConta */ 
 public:
  ContaBancariaApp(Wt::WContainerWidget *parent);
  ~ContaBancariaApp();
};

class TileContaBancaria : public Wt::WContainerWidget {
  public:
    TileContaBancaria();
};

#endif
