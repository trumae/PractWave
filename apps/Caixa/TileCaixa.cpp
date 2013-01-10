#include <Wt/WTemplate>
#include "CaixaApp.h"

using namespace Wt;

TileCaixa::TileCaixa() {
    clear();
    WTemplate *tilePainel = new WTemplate(this);
    tilePainel->setTemplateText(
       "     <div class='tile double bg-color-pink'>"
       "       <div class='tile-content'>"
       "          <h2>Caixa</h2>"
       "       </div>"
       "    </div>"
    );
}

