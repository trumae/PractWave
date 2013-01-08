#include <Wt/WTemplate>
#include "CalculadoraApp.h"

using namespace Wt;

TileCalculadora::TileCalculadora() {
    clear();
    WTemplate *tileCalculadora = new WTemplate(this);
    tileCalculadora->setTemplateText(
       "     <div class='tile bg-color-yellow'>"
       "       <div class='tile-content'>"
       "          <h2>Calculadora</h2>"
       "       </div>"
       "    </div>"
    );
}

