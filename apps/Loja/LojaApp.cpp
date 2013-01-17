#include <Wt/WTemplate>
#include <Wt/WPushButton>
#include <Wt/WAnchor>
#include <Wt/WImage>
#include <Wt/WText>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <vector>

#include "../../core/CabureApplication.h"
#include "LojaApp.h"

using namespace Wt;
using namespace std;

LojaApp::LojaApp(WContainerWidget *parent)
    : App(parent) {
   LOJA = 1;
   init();
}

void LojaApp::init(){
  adicionaEstado(LOJA, boost::bind(&LojaApp::calculadora, this));
  
  adicionaTransicao(LOJA, START, "back",
		    boost::bind(&LojaApp::fazNada, this, nullptr),
		    boost::bind(&LojaApp::guardOk, this));
  setEstado(LOJA);
}

WWidget *LojaApp::calculadora(){
    CabureApplication *app = CabureApplication::cabureApplication();
    
	app->useStyleSheet("/apps/Loja/loja.css");
 
	app->require("http://ajax.googleapis.com/ajax/libs/jquery/1.8.3/jquery.min.js");
	app->require("/apps/Loja/loja.js");

    string corpo = 
	"<div id='conteudoLoja' class='conteudoLoja'>"
	"</div>"
	;
        	
    WText *wtext = new WText(WString(corpo, UTF8), XHTMLUnsafeText);
	app->doJavaScript("initLoja()");
    return wtext;
}

string LojaApp::getTitulo(){
   return "&Aacute;rea VIP";
}
