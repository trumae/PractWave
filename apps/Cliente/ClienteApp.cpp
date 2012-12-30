#include <Wt/WTemplate>
#include <Wt/WPushButton>
#include <Wt/WAnchor>
#include <Wt/WImage>
#include <Wt/WText>
#include <Wt/WBreak>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <vector>

#include "../../CabureApplication.h"
#include "../../logic/Clientes.h"
#include "../../logic/Fornecedores.h"
#include "../../logic/Contabilidade.h"
#include "../../logic/Moeda.h"
#include "../../logic/Timeline.h"
#include "ClienteApp.h"
#include <cppdb/frontend.h>
#include <boost/lexical_cast.hpp>

using namespace Wt;
using namespace std;

ClienteApp::ClienteApp(WContainerWidget *parent)
    : App(parent) {
   init();
}

WWidget* ClienteApp::getConteudo() {
   WContainerWidget *container = new WContainerWidget();
   CabureApplication *app = CabureApplication::cabureApplication();

   WText *addButton = new WText(
        "<div class='tile double bg-color-orange'>"
        "   <div class='tile-content'>"
        "     <h2>Adiciona Cliente</h2>"
        "   </div>"
        "</div>", Wt::XHTMLUnsafeText );

   WText *searchButton = new WText(
        "<div class='tile double bg-color-green'>"
        "   <div class='tile-content'>"
        "     <h2>Busca Cliente</h2>"
        "   </div>"
        "</div>", Wt::XHTMLUnsafeText );

   WTemplate *comandos = new WTemplate();
   comandos->setTemplateText(
		   "<div class='grid'>"
		   "  <div class='row'>${add}${search}</div>"
		   "</div>"
		   , Wt::XHTMLUnsafeText
		   );
   comandos->bindWidget("add", addButton);
   comandos->bindWidget("search", searchButton);

   container->addWidget(comandos);
   cppdb::session &db = app->db_;
   cppdb::result res = db << "select substr(upper(nome),1,1) as inicial, "
	  "count(*) from cliente group by inicial";
   while(res.next()){
       string inicial;
       int count;
       res >> inicial >> count;
       string buffer = "<div class='tile bg-color-blueDark'>"
	         "   <div class='tile-content'>"
	         "     <h1>" + inicial + " </h1>"
	         "   </div>"
		 "   <div class='brand'>"
		 "     <span class='badge'>"
		 + boost::lexical_cast<string>(count) +
		 "     </span>"
		 "   </div>"
	         "</div>";
       WText *tt = new WText(WString(buffer, UTF8), Wt::XHTMLUnsafeText);
       container->addWidget(tt);
   } 
   return container; 
}

string ClienteApp::getTitulo(){
   return "PractWave - Clientes";
}
