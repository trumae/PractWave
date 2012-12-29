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
#include "../../logic/Timeline.h"
#include "TimelineApp.h"

using namespace Wt;
using namespace std;

TimelineApp::TimelineApp(WContainerWidget *parent)
    : App(parent) {
   init();
}

WWidget *TimelineApp::getConteudo(){
    CabureApplication *app = CabureApplication::cabureApplication();
    vector<ItemTimeline> vitem;
    app->timeline_->getTodosTimelineComLimite(vitem, 30);

    string tabela = "<table class='striped'>";
    for(ItemTimeline item: vitem) {
      tabela += "<tr><td>" +
	      item.descricao +
	      "</td></tr>"; 
    }
    tabela += "</table>";
        
    WText *wtext = new WText(WString(tabela, UTF8), XHTMLUnsafeText);
    return wtext;
}

string TimelineApp::getTitulo(){
   return "PractWave - Timeline";
}
