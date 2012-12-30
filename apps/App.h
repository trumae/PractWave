#ifndef __APP_H__
#define __APP_H__

#include <Wt/WContainerWidget>
#include <Wt/WTemplate>
#include <Wt/WAnchor>
#include <Wt/WText>
#include <string>

#include "../CabureApplication.h"

class App : public Wt::WContainerWidget {
protected:
    virtual WWidget* getConteudo() = 0; 
    virtual std::string getTitulo() = 0;
    void init() {
        CabureApplication *app = CabureApplication::cabureApplication();
        clear();
	Wt::WAnchor *back = new Wt::WAnchor();
        back->setStyleClass("back-button big page-back");
        back->setInline(true);
        back->clicked().connect(app->principal_, &CabureWidgetPrincipal::viewHome);

	std::string tpl(
            "<img height=\"1\" width=\"1\" style=\"border-style:none;\" alt=\"\" src=\"http://www.googleadservices.com/pagead/conversion/999519350/?value=0,10&amp;label=eLrVCILgwgMQ9ujN3AM&amp;guid=ON&amp;script=0\"/>"

            "<div class='page secondary'>"
            "   <div class='page-header'>"
            "      <div class='page-header-content'>"
            "          <h1>${titulo}</h1>"
            "          ${backButton}"
            "      </div>"
            "   </div>"
            "   <div class='page-region'>"
            "      <div class='page-region-content'>"
            "      ${conteudo}<br/><br/>"
            "      </div>"
            "   </div>"
            "</div>"
	    //"<div class='nav-bar fixed-bottom bg-color-darken'>"
	    //"    <div class='nav-bar-inner'>"
            //"      <span class='element place-right'>Botoes aqui</span>"
            //"      <span class='element'>&copy; 2012 CMSoftwares</span>"
	    //"      <span class='divider'></span>"
	    //"    </div>"
	    //"</div>"
        );

	Wt::WTemplate *wtemplate = new Wt::WTemplate(this);
        wtemplate->setTemplateText(tpl);
        wtemplate->bindWidget("backButton", back);
        wtemplate->bindString("titulo", getTitulo());
        wtemplate->bindWidget("conteudo", getConteudo());
    }
  public:
    App(Wt::WContainerWidget *p) : Wt::WContainerWidget(p) {}
};

#endif

