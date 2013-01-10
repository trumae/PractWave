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

void ContaBancariaApp::init(){
  adicionaEstado(LISTABANCO, boost::bind(&ContaBancariaApp::listaBanco, this));
  
  adicionaTransicao(LISTABANCO, START, "back",
		    boost::bind(&ContaBancariaApp::fazNada, this, nullptr),
		    boost::bind(&ContaBancariaApp::guardOk, this));
  setEstado(LISTABANCO);
}

WWidget *ContaBancariaApp::listaBanco(){
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
