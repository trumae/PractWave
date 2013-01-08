#ifndef __APP_H__
#define __APP_H__

#include <Wt/WContainerWidget>
#include <Wt/WTemplate>
#include <Wt/WAnchor>
#include <Wt/WText>
#include <string>
#include <vector>
#include <map>
#include <iostream>

#include "../CabureApplication.h"

struct Transicao {
  int current;
  int next;
  std::string event;
  boost::function<void()> action;
  boost::function<bool()> guard;
};

class App : public Wt::WContainerWidget {
 protected:
  std::vector<struct Transicao> tabela;
  std::map<int, boost::function<Wt::WWidget *()> > estados;
  int estado_;

  int START  = 0;

  Wt::WWidget *conteudo_;

  virtual void trataEvento(std::string nome) {
    int estadoantigo = estado_;
    for(Transicao trans : tabela) {
      if(trans.current == estado_ && trans.event == nome){
	if(trans.guard()) {
	  trans.action();	
	  estado_ = trans.next;
	  if(estadoantigo != estado_) {
	    if(estado_ == START ) {
	      CabureApplication *app = CabureApplication::cabureApplication();
	      app->principal_->viewHome();
	      return;
	    }
	    conteudo_ = (estados[estado_])();
	    desenha();
	  }
	  return;
	}
      }
    }
    std::cerr << "Transicao nao encontrada" << std::endl;
  };

  virtual void init() = 0;
  virtual std::string getTitulo() = 0;
  bool guardOk() { return true;}
  void fazNada(void *d) { } 
  void setEstado(int e) { 
    estado_ = e; 
    conteudo_ = (estados[estado_])();
    desenha();
  }

  void adicionaEstado(int i, boost::function <Wt::WWidget *()> fun) {
    estados[i] = fun;
  } 

  void adicionaTransicao(int current, int next, std::string event, boost::function<void()> action, boost::function<bool()> guard = &App::guardOk){
    Transicao t;
    t.current = current;
    t.next = next;
    t.event = event;
    t.action = action;
    t.guard = guard;

    tabela.push_back(t);
  }

  void desenha() {
    clear();
    Wt::WAnchor *back = new Wt::WAnchor();
    back->setStyleClass("back-button big page-back");
    back->setInline(true);
    back->clicked().connect(boost::bind(&App::trataEvento, this, "back"));
    
    std::string tpl(
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
		    );
    
    Wt::WTemplate *wtemplate = new Wt::WTemplate(this);
    wtemplate->setTemplateText(tpl);
    wtemplate->bindWidget("backButton", back);
    wtemplate->bindString("titulo", getTitulo());
    wtemplate->bindWidget("conteudo", conteudo_);
  }

 public:
 App(Wt::WContainerWidget *p) : Wt::WContainerWidget(p) {}
};

#endif

