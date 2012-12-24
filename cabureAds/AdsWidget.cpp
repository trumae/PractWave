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
    decorationStyle().setBorder(WBorder(WBorder::Solid, WBorder::Thin));
    resize(WLength(250), WLength(800));
    setMinimumSize(WLength(250), WLength(800));
    setMaximumSize(WLength(250), WLength(800));
}

