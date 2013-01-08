#include <Wt/WTemplate>
#include <Wt/WPushButton>
#include <Wt/WAnchor>
#include <Wt/WImage>
#include <Wt/WText>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <vector>

#include "../../CabureApplication.h"
#include "CalculadoraApp.h"

using namespace Wt;
using namespace std;

CalculadoraApp::CalculadoraApp(WContainerWidget *parent)
    : App(parent) {
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
 
	app->require("http://ajax.googleapis.com/ajax/libs/jquery/1.8.3/jquery.min.js");
	app->require("/apps/Calculadora/calculadora.js");

    string corpo = 
	"Valor 1 <div class='input-control text'>"
	"<input id='valor1' type='text'/>"
	"<button class='helper'></button>"
	"</div>"
	"Valor 2 <div class='input-control text'>"
	"<input id='valor2' type='text'/>"
	"<button class='helper'></button>"
	"</div>"
	"Valor 3 <div class='input-control text'>"
	"<input id='valor3' type='text'/>"
	"<button class='helper'></button>"
	"</div>"
	"<input type='button' value='Calcular' onClick='calcular()'/>"
	;
        
    WText *wtext = new WText(WString(tabela, UTF8), XHTMLUnsafeText);
    return wtext;
}

string TimelineApp::getTitulo(){
   return "PractWave - Calculadora";
}
