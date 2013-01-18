#ifndef __ADS_WIDGET__BUILDER__H__
#define __ADS_WIDGET__BUILDER__H__

#include <cppdb/frontend.h>
#include "AdsWidget.h"

class AdsWidgetBuilder {
   public:
     static AdsWidget *createAdsWidget();
     static AdsAnuncio *createAdsAnuncio();
};

#endif

