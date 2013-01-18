#include <Wt/WHBoxLayout>
#include <Wt/WCssDecorationStyle>
#include <Wt/WText>
#include "AdsWidget.h"
#include "AdsAnuncio.h"

using namespace Wt;

AdsWidget::AdsWidget(WContainerWidget *parent) :
   WContainerWidget(parent){
}

AdsWidget::AdsWidget(){
}

void AdsWidget::renderUI(){
  clear();
}

