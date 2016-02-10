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
#include "../../core/CabureApplication.h"
#include "../../logic/Moeda.h"
#include "../../logic/Timeline.h"
#include "../AjusteContaWidget/AjusteContaWidget.h"
#include "ContaCliente.h"

using namespace Wt;

ContaCliente::ContaCliente(WContainerWidget *parent,
                           int idconta,
			   App *app,
			   int estadoRetorno,
			   int estadoDadosCliente):
  WContainerWidget(parent),
  idconta_(idconta),
  app_(app),
  estadoRetorno_(estadoRetorno),
  estadoDadosCliente_(estadoDadosCliente){
  ajuste_ = new AjusteContaWidget(idconta_);
  viewHome();
  }

ContaCliente::~ContaCliente() {
  delete ajuste_;
}

void ContaCliente::goDadosCliente(){
  app_->setEstado(estadoDadosCliente_);
}

void ContaCliente::viewHome() {
  CabureApplication *cabure = CabureApplication::cabureApplication();
  cppdb::session db = cabure->db_;

  clear();

  WText *anotarBtn = new WText(
			       "<button class='command-button bg-color-orangeDark default'>"
             + tr("sell").toUTF8() + 
			       "<small>" + tr("msg-sell").toUTF8() + "</small>"
			       "</button>",
			       Wt::XHTMLUnsafeText);
  anotarBtn->clicked().connect(this, &ContaCliente::anotar);

  WText *receberBtn = new WText(
				"<button class='command-button bg-color-blueDark default'>"
        + tr("receive").toUTF8() + 
				"<small>" + tr("msg-receive").toUTF8() + "</small>"
				"</button>",
				Wt::XHTMLUnsafeText);
  receberBtn->clicked().connect(this, &ContaCliente::receber);

  WText *dadosBtn = new WText(
			      "<button class='command-button bg-color-greenDark default'>"
            + tr("info").toUTF8() + 
			      "<small>" + tr("msg-info-customer").toUTF8() + "</small>"
			      "</button>",
				Wt::XHTMLUnsafeText);
  dadosBtn->clicked().connect(this, &ContaCliente::goDadosCliente);

  WWidget *ajusteBtn;
  if(app_ != nullptr){
    ajuste_->setWidgetPai(this);
    ajuste_->setApp(app_);
    ajuste_->setEstadoRetorno(estadoRetorno_);
    ajusteBtn = ajuste_->getButton();
  }

  WTemplate *cabecalho = new WTemplate(this);
  cabecalho->setTemplateText(string("<div>"
				    "${vender}"
				    "${receber}"
				    "${dados}") +
			     ((app_ != nullptr)?"${ajuste}":"") +
			     "</div>", XHTMLUnsafeText);
  cabecalho->bindWidget("vender", anotarBtn);
  cabecalho->bindWidget("receber", receberBtn);
  cabecalho->bindWidget("dados", dadosBtn);
  if(app_ != nullptr) cabecalho->bindWidget("ajuste", ajusteBtn);

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
  Wt::WPushButton *cancel = new WPushButton(tr("btn-cancel").toUTF8());
  cancel->setStyleClass("bg-color-orange fg-color-white");
  cancel->clicked().connect(this, &ContaCliente::trataCancela);

  WTemplate *t = new WTemplate(this);
  t->setTemplateText(
		     "<h2>" + tr("msg-sell").toUTF8() + "</h2>"
		     "<div class='grid'>"
		     "  <div class='row'>  "
		     "    <div class='span1'>" + tr("description").toUTF8() + "</div>"
		     "    <div class='input-control text span4'> <!-- descricao -->"
		     "        ${descricao}"
		     "    </div>"
		     "  </div>"
		     "  <div class='row'>  "
		     "    <div class='span1'>" + tr("value").toUTF8() + "</div>"
		     "    <div class='input-control text span4'> <!-- valor -->"
		     "      ${valor}"
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
  Wt::WPushButton *cancel = new WPushButton(tr("btn-cancel").toUTF8());
  cancel->setStyleClass("bg-color-orange fg-color-white");
  cancel->clicked().connect(this, &ContaCliente::trataCancela);

  WTemplate *t = new WTemplate(this);
  t->setTemplateText(
		     "<h2>" + tr("msg-receive").toUTF8() + "</h2>"
		     "<div class='grid'>"
		     "  <div class='row'>  "
		     "    <div class='span1'>" + tr("description").toUTF8() + "</div>"
		     "    <div class='input-control text span4'> <!-- descricao -->"
		     "        ${descricao}"
		     "    </div>"
		     "  </div>"
		     "  <div class='row'>  "
		     "    <div class='span1'>" + tr("value").toUTF8() + "</div>"
		     "    <div class='input-control text span4'> <!-- valor -->"
		     "        ${valor}"
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



