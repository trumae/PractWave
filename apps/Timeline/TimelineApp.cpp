#include <Wt/WTemplate>
#include <Wt/WPushButton>
#include <Wt/WAnchor>
#include <Wt/WImage>
#include <Wt/WText>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <vector>

#include "../../core/CabureApplication.h"
#include "../../logic/Clientes.h"
#include "../../logic/Fornecedores.h"
#include "../../logic/Contabilidade.h"
#include "../../logic/Moeda.h"
#include "../../logic/Timeline.h"
#include "TimelineApp.h"

using namespace Wt;
using namespace std;

TimelineApp::TimelineApp(WContainerWidget *parent)
    : App(parent) {
  TIMELINE = 1;
  init();
}

void TimelineApp::init(){
  adicionaEstado(TIMELINE, boost::bind(&TimelineApp::timeline, this));
  
  adicionaTransicao(TIMELINE, START, "back",
		    boost::bind(&TimelineApp::fazNada, this, nullptr),
		    boost::bind(&TimelineApp::guardOk, this));
  setEstado(TIMELINE);
}

WWidget *TimelineApp::timeline(){
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

string TimelineApp::getTitulo(){
   return tr("tile-Timeline").toUTF8();
}
