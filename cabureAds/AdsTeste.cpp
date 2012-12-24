#include <Wt/WTemplate>
#include <Wt/WPushButton>
#include "AdsTeste.h"
#include "AdsAnuncio.h"
#include "AdsWidget.h"
#include "AdsWidgetBuilder.h"

using namespace Wt;

AdsTeste::AdsTeste(){
   renderUI();
}

void AdsTeste::renderUI(){
    clear();
    WPushButton *btn = new WPushButton("Atualiza", this);
    btn->clicked().connect(this, &AdsTeste::renderUI);

    AdsWidget *a = AdsWidgetBuilder::createAdsWidget();
    
    WTemplate *t = new WTemplate(this);
    t->setTemplateText("${ads}");
    t->bindWidget("ads", a);
}
