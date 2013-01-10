#include <Wt/WTemplate>
#include <Wt/WPushButton>
#include <Wt/WAnchor>
#include <Wt/WImage>
#include <Wt/WText>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <vector>

#include "../../CabureApplication.h"
#include "../../logic/Clientes.h"
#include "../../logic/Fornecedores.h"
#include "../../logic/Contabilidade.h"
#include "../../logic/Moeda.h"
#include "../../logic/ContasBancarias.h"
#include "ContaBancariaApp.h"

using namespace Wt;
using namespace std;

ContaBancariaApp::ContaBancariaApp(WContainerWidget *parent)
    : App(parent) {
  LISTABANCO = 1;
  ADICIONABANCO = 2;
  CONTABANCO = 3;
  DADOSBANCO = 4;
  RETIRADABANCO = 5;
  RETIRADACAIXABANCO = 6;

  init();
}

void ContaBancariaApp::constroiTabela() {
  //limpa vetores
  tabela.clear();
  estados.clear();
  
  // Estados
  adicionaEstado(LISTABANCO,         boost::bind(&ContaBancariaApp::listaBanco, this));
  adicionaEstado(ADICIONABANCO,      boost::bind(&ContaBancariaApp::adicionaBanco, this));
  adicionaEstado(CONTABANCO,         boost::bind(&ContaBancariaApp::contaBanco, this));
  adicionaEstado(DADOSBANCO,         boost::bind(&ContaBancariaApp::dadosBanco, this));
  adicionaEstado(RETIRADABANCO,      boost::bind(&ContaBancariaApp::retiradaBanco, this));
  adicionaEstado(RETIRADACAIXABANCO,   boost::bind(&ContaBancariaApp::retiradaCaixaBanco, this));  

  // Transicoes
  adicionaTransicao(LISTABANCO, START, "back",
		    boost::bind(&ContaBancariaApp::fazNada, this, nullptr),
		    boost::bind(&ContaBancariaApp::guardOk, this));
}

void ContaBancariaApp::init(){
  constroiTabela();
  setEstado(LISTABANCO);
}

WWidget *ContaBancariaApp::listaBanco(){
    CabureApplication *app = CabureApplication::cabureApplication();
    ContasBancarias *contasBancarias = app->contasBancarias_;
    Contabilidade *contabilidade = app->contabilidade_;

    WContainerWidget *container = new WContainerWidget();

    WText *addButton = new WText(
				 "<button class='command-button default bg-color-greenDark'>"
				 "Adiciona Conta Banc&aacute;ria"
				 "<small>Clique aqui para adicionar uma nova conta banc&aacute;ria</small>"
				 "</button>", Wt::XHTMLUnsafeText );
    addButton->clicked().connect(boost::bind(&ContaBancariaApp::trataEvento, this, "adiciona"));
    
    WTemplate *comandos = new WTemplate();
    comandos->setTemplateText(
			      "<div class='grid'>"
			      "  <div class='row'>"
			      "${add}"			      
			      "</div>"
			      "</div>"
			      , Wt::XHTMLUnsafeText
			      );
    comandos->bindWidget("add", addButton);
    container->addWidget(comandos);

    std::vector < ContaBancaria > vc;
    contasBancarias->getTodosContasBancarias(vc);

    constroiTabela();
    for(ContaBancaria c : vc) {
      string conteudo = "";
      int saldoconta = contabilidade->getSaldoContaFolha(c.idconta);
      Moeda saldo(saldoconta);

      conteudo = "<div class='tile double bg-color-orange'>"
	"   <div class='tile-content'>"
	"     <h3>" + c.nome + " </h3>"
	"   </div>"
	"   <div class='brand'>"
	"     <span class='name'>"
	+ saldo.valStr() +
	"     </span>"
	"   </div>"
	"</div>";
      WText *wtext = new WText(WString(conteudo, UTF8), XHTMLUnsafeText);
      container->addWidget(wtext);
    }

    return container;
}

Wt::WWidget *ContaBancariaApp::adicionaBanco(){
  return nullptr;
}

Wt::WWidget *ContaBancariaApp::contaBanco(){
  return nullptr;
}

Wt::WWidget *ContaBancariaApp::dadosBanco(){
  return nullptr;
}

Wt::WWidget *ContaBancariaApp::retiradaBanco(){
  return nullptr;
}

Wt::WWidget *ContaBancariaApp::retiradaCaixaBanco(){
  return nullptr;
}


string ContaBancariaApp::getTitulo(){
   return "Contas Banc&aacute;rias";
}
