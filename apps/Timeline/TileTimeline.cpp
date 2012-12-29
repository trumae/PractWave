#include <Wt/WTemplate>
#include "Timeline.h"

using namespace Wt;

TileTimeline::TileTimeline() {
    clear();
    WTemplate *tilePainel = new WTemplate(this);
    tilePainel->setTemplateText(
       "     <div class='tile bg-color-yellow'>"
       "       <div class='tile-content'>"
       "          <h2>Timeline</h2>"
       "       </div>"
       "    </div>"
    );
}

