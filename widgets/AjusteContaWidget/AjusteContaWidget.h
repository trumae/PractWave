#ifndef __AJUSTE_CONTA_WIDGET__
#define __AJUSTE_CONTA_WIDGET__

#include <string>
#include <Wt/WPushButton>
#include <Wt/WContainerWidget>

#include "../../apps/App.h"

class AjusteContaWidget : public Wt::WObject {
 public:
  AjusteContaWidget(int conta);
  void setWidgetPai(Wt::WContainerWidget *c) {
    widgetPai_ = c;
  }
  void setApp(App *app){
    app_ = app;
  }
  void setEstadoRetorno(int est) {
    estadoRetorno_ = est;
  }
  Wt::WWidget *getButton();
  Wt::WWidget *getButtonApp();
  void setTituloApp(std::string s) {
    titulo_ = s;
  }
 private:
  int conta_;
  Wt::WContainerWidget *widgetPai_;
  App *app_;
  int estadoRetorno_;
  std::string titulo_;

  Wt::WLineEdit *descricao;
  Wt::WLineEdit *valor;

  Wt::WWidget* getAjusteConteudo();
  void ajuste();
  void ajusteApp();
  void trataOk();
  void trataCancela();
  void ajusteAction(std::string descricao, int valor);
};

#endif


