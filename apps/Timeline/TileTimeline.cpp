#include <Wt/WTemplate>
#include "TimelineApp.h"

using namespace Wt;

TileTimeline::TileTimeline() {
    clear();
    WTemplate *tilePainel = new WTemplate(this);
    tilePainel->setTemplateText(
       "     <div class='tile bg-color-orange'>"
       "       <div class='tile-content'>"
       "          <h2>Timeline</h2>"
       "       </div>"
       "    </div>"
    );
}

