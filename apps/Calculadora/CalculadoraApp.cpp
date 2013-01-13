#include <Wt/WTemplate>
#include <Wt/WPushButton>
#include <Wt/WAnchor>
#include <Wt/WImage>
#include <Wt/WText>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <vector>

#include "../../core/CabureApplication.h"
#include "CalculadoraApp.h"

using namespace Wt;
using namespace std;

CalculadoraApp::CalculadoraApp(WContainerWidget *parent)
    : App(parent) {
   CALCULADORA = 1;
   init();
}

void CalculadoraApp::init(){
  adicionaEstado(CALCULADORA, boost::bind(&CalculadoraApp::calculadora, this));
  
  adicionaTransicao(CALCULADORA, START, "back",
		    boost::bind(&CalculadoraApp::fazNada, this, nullptr),
		    boost::bind(&CalculadoraApp::guardOk, this));
  setEstado(CALCULADORA);
}

WWidget *CalculadoraApp::calculadora(){
    CabureApplication *app = CabureApplication::cabureApplication();
    
	app->useStyleSheet("/apps/Calculadora/calculadora.css");
 
	app->require("http://ajax.googleapis.com/ajax/libs/jquery/1.8.3/jquery.min.js");
	app->require("/apps/Calculadora/calculadora.js");

    string corpo = 
	"<div id='conteudoCalculadora' class='conteudoCalculadora'>"
	"</div>"
	;
        	
    WText *wtext = new WText(WString(corpo, UTF8), XHTMLUnsafeText);
	app->doJavaScript("initCalculadora()");
    return wtext;
}

string CalculadoraApp::getTitulo(){
   return "Calculadora";
}
