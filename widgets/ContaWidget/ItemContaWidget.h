#ifndef __VIEW__ITEM__CONTA_WIDGET__
#define __VIEW__ITEM__CONTA_WIDGET__

#include <string>
#include <cppdb/frontend.h>
#include <Wt/WContainerWidget>
#include "../../logic/Contabilidade.h"
#include "../../logic/Moeda.h"
#include "ContaWidget.h"

class ItemContaWidget : public Wt::WContainerWidget {
 public:
  ItemContaWidget(Wt::WContainerWidget *parent,
		  ContaWidget *view,
		  int id_diario,
		  std::string data,
		  std::string descricao,
		  Moeda debito,
		  Moeda credito);
 private:
  ContaWidget *view_;
  int id_diario_;
  std::string data_;
  std::string descricao_;
  Moeda debito_;
  Moeda credito_;

  void removeLancamento();
};

#endif
