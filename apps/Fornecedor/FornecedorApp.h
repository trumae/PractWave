#ifndef __FORNECEDOR_APP__
#define __FORNECEDOR_APP__

#include <Wt/WContainerWidget>
#include <Wt/WStackedWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WTextArea>
#include "../App.h"
#include "../../logic/Fornecedores.h"

class FornecedorApp : public App {
 private:
  int INICIAIS;
  int ADICIONAFORNECEDOR;
  int LISTAFORNECEDORES;
  int DADOSFORNECEDOR;
  int CONTAFORNECEDOR;

  //variaveis auxiliares
  std::vector<std::string> iniciais_;
  std::string inicialAtual_;
  int idFornecedor_;
  int idContaFornecedor_;
 protected:

  void setInicialAtual(std::string ini);
  void setFornecedorAtual(int id, int idconta);
  void constroiTabela();

  WWidget* EIniciais();
  WWidget* EAdicionaFornecedor();
  WWidget* EListaFornecedores();
  WWidget* EDadosFornecedor();
  WWidget* EContaFornecedor();

  virtual std::string getTitulo();
  virtual void init();
    
  Wt::WLineEdit *buscaEdit;
  Wt::WPushButton *buscaBtn;
  int editandoId;
  Wt::WLineEdit *editNome_;
  Wt::WLineEdit *editEndereco_;
  Wt::WLineEdit *editBairro_;
  Wt::WLineEdit *editCidade_;
  Wt::WLineEdit *editEstado_;
  Wt::WLineEdit *editCEP_;
  Wt::WLineEdit *editTelefone_;
  Wt::WLineEdit *editCelular_;
  Wt::WLineEdit *editEmail_;
  Wt::WTextArea *editObservacao_;

  void formAdicionaFornecedor();
  Wt::WWidget* createFormFornecedor(Fornecedor cli);

  void adicionaFornecedor();
  void saveFornecedor();
  void buscaFornecedor();

 public:
  FornecedorApp(Wt::WContainerWidget *parent);
};

class TileFornecedor : public Wt::WContainerWidget {
 public:
  TileFornecedor();
};

#endif

