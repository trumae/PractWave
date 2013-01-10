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
#include "../../logic/Caixa.h"
#include "CaixaApp.h"

using namespace Wt;
using namespace std;

CaixaApp::CaixaApp(WContainerWidget *parent)
    : App(parent) {
  CONTACAIXA = 1;
  RETIRADA = 2;
  RETIRADADEPOSITO = 3;

  init();
}

void CaixaApp::init(){
  adicionaEstado(CONTACAIXA, boost::bind(&CaixaApp::contaCaixa, this));
  
  adicionaTransicao(CONTACAIXA, START, "back",
		    boost::bind(&CaixaApp::fazNada, this, nullptr),
		    boost::bind(&CaixaApp::guardOk, this));
  setEstado(CONTACAIXA);
}

WWidget *CaixaApp::contaCaixa(){
    CabureApplication *app = CabureApplication::cabureApplication();
    vector<ItemTimeline> vitem;
    app->timeline_->getTodosTimelineComLimite(vitem, 30);

    string tabela = 
      "<div class='grid'>"
      "<div class='row'>"
      "<table class='span12 striped'>";
    for(ItemTimeline item: vitem) {
      tabela += "<tr><td>" +
	      item.descricao +
	      "</td></tr>"; 
    }
    tabela += 
      "</div></div></table>";
        
    WText *wtext = new WText(WString(tabela, UTF8), XHTMLUnsafeText);
    return wtext;
}

Wt::WWidget *CaixaApp::retirada(){
  return nullptr;
}

Wt::WWidget *CaixaApp::retiradaDeposito(){
  return nullptr;
}

string CaixaApp::getTitulo(){
   return "Caixa";
}
