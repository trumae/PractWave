#ifndef __CONTA__WIDGET__
#define __CONTA__WIDGET__

#include <string>
#include <Wt/WContainerWidget>
#include "../../logic/Contabilidade.h"

class ContaWidget : public Wt::WContainerWidget {
  friend class ItemContaWidget;
 public:
  ContaWidget(Wt::WContainerWidget *parent, int codconta);
  
  void setCabecalhoDebito(std::string texto){
    cabDebito_ = texto;
  }
  void setCabecalhoCredito(std::string texto){
    cabCredito_ = texto;
  }
 private:
  int codconta_;
  std::string cabDebito_;
  std::string cabCredito_;
  
  void viewHome();
};

#endif

