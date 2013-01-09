#include <Wt/WTemplate>
#include "FornecedorApp.h"

using namespace Wt;

TileFornecedor::TileFornecedor() {
    clear();
    WTemplate *tilePainel = new WTemplate(this);
    tilePainel->setTemplateText(
       "     <div class='tile double bg-color-purple'>"
       "       <div class='tile-content'>"
       "          <h2>Fornecedor</h2>"
       "       </div>"
       "    </div>"
    );
}

