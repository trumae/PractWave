#include <Wt/WTemplate>
#include "CalculadoraApp.h"

using namespace Wt;

TileCalculadora::TileCalculadora() {
    clear();
    WTemplate *tileCalculadora = new WTemplate(this);
    tileCalculadora->setTemplateText(
       "     <div class='tile icon bg-color-greenLight'>"
       "       <div class='tile-content'>"
       "          <img class='icon' src='apps/Calculadora/tile.png'/>"
       "       </div>"
       "       <div class='brand'>"
       "          <span class='name'>Calculadora</span>"
       "       </div>"
       "     </div>"
    );
}

