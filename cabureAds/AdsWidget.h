#ifndef __ADS__WIDGET__H__
#define __ADS__WIDGET__H__

#include <Wt/WHBoxLayout>
#include <Wt/WContainerWidget>
#include "AdsAnuncio.h"

using namespace Wt;

class AdsWidget : public WContainerWidget {
   public:
     AdsWidget(WContainerWidget *parent);
     AdsWidget();
   private:
     void renderUI();
};

#endif

