#include <Wt/WContainerWidget>
#include <Wt/WPushButton>
#include <Wt/WTemplate>
#include <Wt/WText>
#include <iostream>
#include <Wt/WMessageBox>

#include "ItemContaWidget.h"
#include "../../core/CabureApplication.h"
#include "../../logic/Contabilidade.h"
#include "../../logic/Timeline.h"

using namespace Wt;
    
ItemContaWidget::ItemContaWidget(Wt::WContainerWidget *parent,
				 ContaWidget *view,
				 int id_diario,
				 std::string data,
				 std::string descricao,
				 Moeda debito,
				 Moeda credito):
  WContainerWidget(parent), view_(view), 
  id_diario_(id_diario),
  data_(data), 
  descricao_(descricao),
  debito_(debito), 
  credito_(credito){
  setInline(true);

  WText *btnRemove = new WText("<button class='bg-color-red fg-color-white'>Remove</button>", Wt::XHTMLUnsafeText);
  btnRemove->clicked().connect(this, &ItemContaWidget::removeLancamento);

  WTemplate *t = new WTemplate(this);
  t->setTemplateText(
		     "<tr>"
		     "<td>${data}</td>"
		     "<td>${descricao}</td>"
		     "<td class='right'>${debito}</td>"
		     "<td class='right'>${credito}</td>"
		     "<td>${btnRemove}</td>"
		     "</tr>", XHTMLUnsafeText);
  t->bindString("data", data_);
  t->bindString("descricao", WString(descricao_, UTF8));
  t->bindString("debito", debito_.valStr());
  t->bindString("credito", credito_.valStr());
  t->bindWidget("btnRemove", btnRemove);
  t->setInline(true);
  }

void ItemContaWidget::removeLancamento(){
  CabureApplication *cabure = CabureApplication::cabureApplication();
  Contabilidade *contabilidade = cabure->contabilidade_;
  Timeline *timeline = cabure->timeline_;

  StandardButton result = 
    WMessageBox::show("Confirme", 
		      "Voc&ecirc; est&aacute; apagando "
		      "um lan&ccedil;amento! "
		      "Deseja continuar?",
		      Ok | Cancel);

  if(result == Ok) {
    contabilidade->removeLancamento(id_diario_);
    ItemTimeline itemTimeline("", 
			      "O lan&ccedil;amento com descri&ccedil;&atilde;o '" 
			      + descricao_ + 
			      "' foi removido." , 
			      "ItemContaWidget::removeLancamento()",
			      "");
    timeline->adiciona(itemTimeline);
  }

  view_->viewHome(); 
}



