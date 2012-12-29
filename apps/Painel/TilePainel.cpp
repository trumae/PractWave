#include <Wt/WTemplate>
#include "Painel.h"

using namespace Wt;

TilePainel::TilePainel() {
    clear();
    WTemplate *tilePainel = new WTemplate(this);
    tilePainel->setTemplateText(
       "     <div class='tile bg-color-blueDark'>"
       "       <div class='tile-content'>"
       "          <h2>Painel</h2>"
       "       </div>"
       "    </div>"
    );
}

