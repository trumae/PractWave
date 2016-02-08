#include <Wt/WTemplate>
#include "ContaBancariaApp.h"

using namespace Wt;

TileContaBancaria::TileContaBancaria() {
    clear();
    WTemplate *tilePainel = new WTemplate(this);
    tilePainel->setTemplateText(
       "     <div class='tile double bg-color-blue'>"
       "       <div class='tile-content'>"
       "          <h2>" + tr("tile-Banks") + "</h2>"
       "       </div>"
       "    </div>"
    );
}

