#ifndef __AJUSTE_CONTA_WIDGET__
#define __AJUSTE_CONTA_WIDGET__

#include <string>
#include <Wt/WPushButton>
#include <Wt/WContainerWidget>

class AjusteContaWidget : public Wt::WObject {
 public:
  AjusteContaWidget(int conta);
  void setWidgetPai(Wt::WContainerWidget *c) {
    widgetPai_ = c;
  }
  Wt::WWidget *getButton();
 private:
  int conta_;
  Wt::WContainerWidget *widgetPai_;
  Wt::WLineEdit *descricao;
  Wt::WLineEdit *valor;

  void ajuste();
  void trataOk();
  void trataCancela();
  void ajusteAction(std::string descricao, int valor);
};

#endif


