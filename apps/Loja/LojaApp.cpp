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
    : App(parent){
   LOJA = 1;
   init();
}

void LojaApp::init(){
  adicionaEstado(LOJA, boost::bind(&LojaApp::loja, this));
  
  adicionaTransicao(LOJA, START, "back",
		    boost::bind(&LojaApp::fazNada, this, nullptr),
		    boost::bind(&LojaApp::guardOk, this));
  setEstado(LOJA);
}

WWidget *LojaApp::loja(){
    CabureApplication *app = CabureApplication::cabureApplication();
    
	app->useStyleSheet("/apps/Loja/loja.css");
	app->require("/apps/Loja/loja.js");

	WTemplate *loja = new WTemplate();
    loja->setTemplateText("<div id='Loja' class='conteudo' >"
                          "  <div class='tile icon double bg-color-blue'' id='TileEstoque'>"
                          "       <div class='tile-content'>"
                          "          <img class='icon' src='apps/Estoque/estoque.png'/>"
                          "       </div>"
                          "       <div class='brand'>"
                          "          <span class='name'>Estoque (N&atilde;o dispon&iacute;vel )</span>"
                          "       </div>"
                          "   </div>"
                          " </div>"

/*                                                  <div class="tile icon double bg-color-orange selected" id="TileCalculadora">
                                                          <div class='tile-content'>
                                                                  <img class='icon' src='apps/Calculadora/tile.png'/>
                                                         </div>
                                                          <div class='brand'>
                                                                  <span class='name'>Calculadora</span>
                                                          </div>
                                                  </div>
                                                <div class="tile icon double bg-color-red" id="TileCheque">
                                                          <div class='tile-content'>
                                                                  <img class='icon' src='apps/Calculadora/tile.png'/>
                                                          </div>
                                                          <div class='brand'>
                                                                  <span class='name'>Cheque</span>
                                                          </div>
                                                  </div>
                                                  <div class="tile icon double bg-color-green" id="TileAgenda">
                                                          <div class='tile-content'>
                                                                  <img class='icon' src='apps/Calculadora/tile.png'/>
                                                          </div>
                                                          <div class='brand'>
                                                                  <span class='name'>Agenda</span>
                                                          </div>
                                                  </div>
                                          </div>
"*/
               , XHTMLUnsafeText);
        	
   // WText *wtext = new WText(WString(corpo, UTF8), XHTMLUnsafeText);
	app->doJavaScript("initLoja()");
    return loja;
}

string LojaApp::getTitulo(){
   return "&Aacute;rea VIP";
}
