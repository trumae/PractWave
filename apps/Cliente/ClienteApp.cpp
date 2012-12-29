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
#include "Cliente.h"

using namespace Wt;
using namespace std;

TimelineApp::TimelineApp(WContainerWidget *parent)
    : WContainerWidget(parent) {
    viewHome();
}


void TimelineApp::viewHome() {
    CabureApplication *app = CabureApplication::cabureApplication();
    clear();
    WAnchor *back = new WAnchor();
    back->setStyleClass("back-button big page-back");
    back->setInline(true);
    back->clicked().connect(app->principal_, &CabureWidgetPrincipal::viewHome);
    
    vector<ItemTimeline> vitem;
    app->timeline_->getTodosTimelineComLimite(vitem, 30);

    string tabela = "<table class='striped'>";
    for(ItemTimeline item: vitem) {
      tabela += "<tr><td>" +
	      item.descricao +
	      "</td></tr>"; 
    }
    tabela += "</table>";

    string tpl(
        "<img height=\"1\" width=\"1\" style=\"border-style:none;\" alt=\"\" src=\"http://www.googleadservices.com/pagead/conversion/999519350/?value=0,10&amp;label=eLrVCILgwgMQ9ujN3AM&amp;guid=ON&amp;script=0\"/>"

        "<div class='page secondary'>"
        "   <div class='page-header'>"
        "      <div class='page-header-content'>"
        "          <h1>Timeline</h1>"
        "          ${backButton}"
        "      </div>"
        "   </div>"
        "   <div class='page-region'>"
        "      <div class='page-region-content'>"
	"      ${tabela}"
        "      </div>"
        "   </div>"
        "</div>"
    );
        
    WTemplate *wtemplate = new WTemplate(this);
    wtemplate->setTemplateText(tpl);
    wtemplate->bindWidget("backButton", back);
    wtemplate->bindString("tabela", WString(tabela, UTF8));

}

