#ifndef __CLIENTE_APP__
#define __CLIENTE_APP__

#include <Wt/WContainerWidget>
#include <Wt/WStackedWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WTextArea>
#include "../App.h"
#include "../../logic/Clientes.h"

class ClienteApp : public App {
 private:
  int INICIAIS = 1;
  int ADICIONACLIENTE = 2;
  int LISTACLIENTES = 3;
  int DADOSCLIENTE = 4;
  int CONTACLIENTE = 5;

  //variaveis auxiliares
  std::vector<std::string> iniciais_;
  std::string inicialAtual_;
  int idCliente_;
  int idContaCliente_;
 protected:

  void setInicialAtual(std::string ini);
  void setClienteAtual(int id, int idconta);
  void constroiTabela();

  WWidget* EIniciais();
  WWidget* EAdicionaCliente();
  WWidget* EListaClientes();
  WWidget* EDadosCliente();
  WWidget* EContaCliente();

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

  void formAdicionaCliente();
  Wt::WWidget* createFormCliente(Cliente cli);

  void adicionaCliente();
  void saveCliente();
  void buscaCliente();

 public:
  ClienteApp(Wt::WContainerWidget *parent);
};

class TileCliente : public Wt::WContainerWidget {
 public:
  TileCliente();
};

#endif

