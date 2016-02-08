#include <Wt/WTemplate>
#include "ClienteApp.h"

using namespace Wt;

TileCliente::TileCliente() {
    clear();
    WTemplate *tilePainel = new WTemplate(this);
    tilePainel->setTemplateText(
       "     <div class='tile double bg-color-orangeDark'>"
       "       <div class='tile-content'>"
       "          <h2>" + tr("tile-Customers") + "</h2>"
       "       </div>"
       "    </div>"
    );
}

