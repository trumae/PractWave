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
  enum abaCliente {CONTA, DADOS} aba;

  //variaveis auxiliares
  int idAux;
  int idcontaAux;
  std::string inicialAux;
 protected:
  virtual WWidget* getConteudo();
  virtual std::string getTitulo();
  
  void processInicial(std::string inicial);
  void processCliente(int id, int idconta, std::string inicial);
  
  void showDados();
  void showConta();
  
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

