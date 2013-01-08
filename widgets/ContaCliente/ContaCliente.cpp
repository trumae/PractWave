#include <vector>
#include <string>
#include <Wt/WTemplate>
#include <Wt/WText>
#include <Wt/WLineEdit>
#include <Wt/WDialog>
#include <Wt/WPushButton>
#include <Wt/WComboBox>
#include <Wt/WValidator>
#include <Wt/WLengthValidator>
#include "../../CabureApplication.h"
#include "../../logic/Moeda.h"
#include "../../logic/Timeline.h"
#include "../AjusteContaWidget/AjusteContaWidget.h"
#include "ContaCliente.h"

using namespace Wt;

ContaCliente::ContaCliente(WContainerWidget *parent,
                           int idconta):
  WContainerWidget(parent),
  idconta_(idconta) {
  ajuste_ = new AjusteContaWidget(idconta_);
  viewHome();
  }

ContaCliente::~ContaCliente() {
  delete ajuste_;
}

void ContaCliente::viewHome() {
  CabureApplication *cabure = CabureApplication::cabureApplication();
  cppdb::session db = cabure->db_;

  clear();

  WText *anotarBtn = new WText(
			       "<button class='bg-color-orangeDark fg-color-white'>"
			       "  <span class='label'>Anotar</span>"
			       "</button>",
			       Wt::XHTMLUnsafeText);
  anotarBtn->clicked().connect(this,
			       &ContaCliente::anotar);

  WText *receberBtn = new WText(
				"<button class='bg-color-blueDark fg-color-white'>"
				"  <span class='label'>Receber</span>"
				"</button>",
				Wt::XHTMLUnsafeText);
  receberBtn->clicked().connect(this,
				&ContaCliente::receber);

  ajuste_->setWidgetPai(this);
  WWidget *ajusteBtn = ajuste_->getButton();

  WTemplate *cabecalho = new WTemplate(this);
  cabecalho->setTemplateText(
			     "<div>"
			     "${vender}"
			     "${receber}"
			     "${ajuste}"
			     "</div>", XHTMLUnsafeText);
  cabecalho->bindWidget("vender", anotarBtn);
  cabecalho->bindWidget("receber", receberBtn);
  cabecalho->bindWidget("ajuste", ajusteBtn);

  conta_ = new ContaWidget(this, idconta_);
}

void ContaCliente::trataCancela() {
  viewHome();
}

void ContaCliente::trataAnotarOk() {
  CabureApplication *cabure = CabureApplication::cabureApplication();
  Clientes *clientes = cabure->clientes_;
  if(valor->validate() == WValidator::Valid) {
    Moeda m(valor->text().narrow());
    // monta e realiza lancamento
    clientes->vender(idconta_,
		     descricao->text().toUTF8(),
		     m.valInt());
    viewHome();
  }
}

void ContaCliente::trataReceberOk() {
  CabureApplication *cabure = CabureApplication::cabureApplication();
  Clientes *clientes = cabure->clientes_;
  if(valor->validate() == WValidator::Valid) {
    Moeda m(valor->text().narrow());
    // monta e realiza lancamento
    clientes->receber(idconta_,
		      descricao->text().toUTF8(),
		      m.valInt());
    viewHome();
  }
}

void ContaCliente::anotar() {
  clear();
  descricao = new WLineEdit();
  valor = new WLineEdit();
  valor->setValidator(Moeda::newWValidator());

  Wt::WPushButton *ok = new WPushButton("Ok");
  ok->setStyleClass("bg-color-blue fg-color-white");
  ok->clicked().connect(this, &ContaCliente::trataAnotarOk);
  Wt::WPushButton *cancel = new WPushButton("Cancela");
  cancel->clicked().connect(this, &ContaCliente::trataCancela);

  WTemplate *t = new WTemplate(this);
  t->setTemplateText(
		     "<h2>Anotar para cliente</h2>"
		     "<div class='grid'>"
		     "  <div class='row'>  "
		     "    <div class='span1'>Descri&ccedil;&atilde;o</div>"
		     "    <div class='input-control text span4'> <!-- descricao -->"
		     "        ${descricao}"
		     "        <button class='helper'></button>"
		     "    </div>"
		     "  </div>"
		     "  <div class='row'>  "
		     "    <div class='span1'>Valor</div>"
		     "    <div class='input-control text span4'> <!-- valor -->"
		     "      ${valor}"
		     "      <button class='helper'></button>"
		     "    </div>"
		     "  </div>"
		     "</div>"
		     "<div>"
		     "   ${ok}${cancel}"
		     "</div>", XHTMLUnsafeText);

  t->bindWidget("descricao", descricao);
  t->bindWidget("valor", valor);
  t->bindWidget("ok", ok);
  t->bindWidget("cancel", cancel);
}

void ContaCliente::receber() {
  clear();
  descricao = new WLineEdit();
  valor = new WLineEdit();
  valor->setValidator(Moeda::newWValidator());

  Wt::WPushButton *ok = new WPushButton("Ok");
  ok->setStyleClass("bg-color-blue fg-color-white");
  ok->clicked().connect(this, &ContaCliente::trataReceberOk);
  Wt::WPushButton *cancel = new WPushButton("Cancela");
  cancel->clicked().connect(this, &ContaCliente::trataCancela);

  WTemplate *t = new WTemplate(this);
  t->setTemplateText(
		     "<h2>Receber de cliente</h2>"
		     "<div class='grid'>"
		     "  <div class='row'>  "
		     "    <div class='span1'>Descri&ccedil;&atilde;o</div>"
		     "    <div class='input-control text span4'> <!-- descricao -->"
		     "        ${descricao}"
		     "        <button class='helper'></button>"
		     "    </div>"
		     "  </div>"
		     "  <div class='row'>  "
		     "    <div class='span1'>Valor</div>"
		     "    <div class='input-control text span4'> <!-- valor -->"
		     "        ${valor}"
		     "        <button class='helper'></button>"
		     "    </div>"
		     "</div>"
		     "<div>"
		     "   ${ok}${cancel}"
		     "</div>", XHTMLUnsafeText);

  t->bindWidget("descricao", descricao);
  t->bindWidget("valor", valor);
  t->bindWidget("ok", ok);
  t->bindWidget("cancel", cancel);
}



