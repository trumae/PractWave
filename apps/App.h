#ifndef __APP_H__
#define __APP_H__

#include <Wt/WContainerWidget>
#include <Wt/WTemplate>
#include <Wt/WAnchor>
#include <Wt/WText>
#include <Wt/WMessageBox>
#include <string>
#include <vector>
#include <map>
#include <iostream>

#include "../core/CabureApplication.h"

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

  int START;

  Wt::WWidget *conteudo_;
  Wt::WText *msg_;

  virtual void init() = 0;
  virtual std::string getTitulo() = 0;
  bool guardOk() { return true;}
  void fazNada(void *d) { } 

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
    
    msg_ = new Wt::WText("");

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
		    "      ${mensagem}"
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
    wtemplate->bindWidget("mensagem", msg_);
  }

  void trataEvento(std::string nome) {
    int estadoantigo = estado_;
    for(Transicao trans : tabela) {
      if(trans.current == estado_ && trans.event == nome){
	if(trans.guard()) {
	  estado_ = trans.next;
	  trans.action();	
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
  }

  void setErrorMessage(std::string msg) {
    Wt::WMessageBox::show("Erro", msg, Wt::Ok);
  }

  void setMessage(std::string msg) {
    msg_->setText(std::string("<div class='grid'>"
			      "   <div class='row'>"
			      "      <div class='mensagem span12'>")
		  + msg +
		  "</div></div></div>");
    msg_->setTextFormat(Wt::XHTMLUnsafeText);
  }

 public:
 App(Wt::WContainerWidget *p) : Wt::WContainerWidget(p) {
    START  = 0;
  }
  void setEstado(int e) { 
    estado_ = e; 
    conteudo_ = (estados[estado_])();
    desenha();
  }
};

#endif

