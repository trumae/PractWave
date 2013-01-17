#include <Wt/WTemplate>
#include "LojaApp.h"

using namespace Wt;

TileLoja::TileLoja() {
    clear();
    WTemplate *tileLoja = new WTemplate(this);
    tileLoja->setTemplateText(
       "     <div class='tile icon bg-color-green'>"
       "       <div class='tile-content'>"
       "          <img class='icon' src='apps/Loja/tile.png'/>"
       "       </div>"
       "       <div class='brand'>"
       "          <span class='name'>&Aacute;rea VIP</span>"
       "       </div>"
       "     </div>"
    );
}

