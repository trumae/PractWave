#include <Wt/WDialog>
#include <Wt/WTemplate>
#include <Wt/WText>
#include <Wt/WLineEdit>
#include <Wt/WMessageBox>
#include <Wt/WString>

#include "AjusteContaWidget.h"
#include "../../core/CabureApplication.h"
#include "../../logic/Moeda.h"

using namespace Wt;

AjusteContaWidget::AjusteContaWidget(int conta) :
  conta_(conta) {
  titulo_ = "";
}

Wt::WWidget *AjusteContaWidget::getButton() {
  Wt::WText *button = new Wt::WText(
				    "<button class='command-button bg-color-pinkDark default'>"
            + WString::tr("fix").toUTF8() + 
				    "<small>" + WString::tr("msg-fix").toUTF8() + "</small>"
				    "</button>",
				    Wt::XHTMLUnsafeText);
  button->clicked().connect(this, &AjusteContaWidget::ajuste);
  return button;
}

Wt::WWidget *AjusteContaWidget::getButtonApp() {
  Wt::WText *button = new Wt::WText(
				    "<button class='command-button bg-color-pinkDark default'>"
            + WString::tr("fix").toUTF8() + 
				    "<small>" + WString::tr("msg-fix").toUTF8() + "</small>"
				    "</button>",
				    Wt::XHTMLUnsafeText);
  button->clicked().connect(this, &AjusteContaWidget::ajusteApp);
  return button;
}

void AjusteContaWidget::ajuste(){
  widgetPai_->clear();
  widgetPai_->addWidget(getAjusteConteudo());
}

void AjusteContaWidget::ajusteApp(){
  widgetPai_->clear();

  Wt::WAnchor *back = new Wt::WAnchor();
  back->setStyleClass("back-button big page-back");
  back->setInline(true);
  back->clicked().connect(this, &AjusteContaWidget::trataCancela);
  
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
		  "</div>");
    
  Wt::WTemplate *wtemplate = new Wt::WTemplate(widgetPai_);
  wtemplate->setTemplateText(tpl);
  wtemplate->bindWidget("backButton", back);
  wtemplate->bindString("titulo", titulo_);
  wtemplate->bindWidget("conteudo", getAjusteConteudo());
}

WWidget* AjusteContaWidget::getAjusteConteudo() {
  descricao = new WLineEdit();
  valor = new WLineEdit();
  valor->setValidator(Moeda::newWValidator());

  Wt::WPushButton *ok = new WPushButton("Ok");
  ok->setStyleClass("bg-color-blue fg-color-white");
  ok->clicked().connect(this, &AjusteContaWidget::trataOk);

  Wt::WPushButton *cancel = new WPushButton(WString::tr("btn-cancel").toUTF8());
  cancel->setStyleClass("bg-color-orange fg-color-white");
  cancel->clicked().connect(this, &AjusteContaWidget::trataCancela);

  WTemplate *t = new WTemplate();
  t->setTemplateText(
		     "<h1>" + WString::tr("fix").toUTF8() + "</h1>"
		     "<div class='alerta'>"
		     "<h3>" + WString::tr("careful").toUTF8() + "!</h3>"
         + WString::tr("msg-careful").toUTF8() + 
		     "</div>"

		     "<h4>" + WString::tr("fix-info").toUTF8() + "</h4>"
		     "<div class='grid'>"
		     "  <div class='row'>"
		     "      <div class='span1'>" + WString::tr("description").toUTF8() + "</div>"
		     "      <div class='input-control text span4'>"
		     "        ${descricao}"
		     "      </div>"
		     "  </div>"
		     "  <div class='row'>"
		     "      <div class='span1'>" + WString::tr("balance").toUTF8() + "</div>"
		     "      <div class='input-control text span4'>"
		     "        ${valor}"
		     "      </div>"
		     "  </div>"
		     "</div>"
		     "<div'>"
		     "   ${ok}${cancel}"
		     "</div>", XHTMLUnsafeText);
  t->bindWidget("descricao", descricao);
  t->bindWidget("valor", valor);
  t->bindWidget("ok", ok);
  t->bindWidget("cancel", cancel);

  return t;
}

void AjusteContaWidget::trataOk() {
  if(valor->validate() == WValidator::Valid) {
    if(valor->text().narrow().size() > 0) {
      Moeda m(valor->text().narrow());
      // monta e realiza lancamento
      ajusteAction(descricao->text().toUTF8(),
		   m.valInt());
      app_->setEstado(estadoRetorno_);
    } else {
      WMessageBox::show("Erro", WString::tr("value-required").toUTF8(), Ok);
    }
  }
}

void AjusteContaWidget::trataCancela() {
  app_->setEstado(estadoRetorno_);
}

void AjusteContaWidget::ajusteAction(std::string descricao, int valor) {
  CabureApplication *cabure = CabureApplication::cabureApplication();
  Contabilidade *contabilidade = cabure->contabilidade_;
  Timeline *timeline = cabure->timeline_;

  string natureza = contabilidade->getNaturezaPorId(conta_);
  string nome = contabilidade->getNomePorId(conta_);
  int diff = valor - contabilidade->getSaldoContaFolha(conta_);
  int idAjuste = contabilidade->getIdPorNome("AJUSTES");
  contabilidade->limpaLancamento();
  contabilidade->setDescricaoLancamento(WString::tr("fix").toUTF8() + " - " + descricao);
  if(natureza == "DEVEDORA") {
    if(diff > 0) {
      contabilidade->adicionaDebito(conta_, diff, "");
      contabilidade->adicionaCredito(idAjuste, diff, "");
    } else {
      diff = -diff;
      contabilidade->adicionaCredito(conta_, diff, "");
      contabilidade->adicionaDebito(idAjuste, diff, "");
    }
  } else { // CREDORA
    if(diff > 0) {
      contabilidade->adicionaCredito(conta_, diff, "");
      contabilidade->adicionaDebito(idAjuste, diff, "");
    } else {
      diff = -diff;
      contabilidade->adicionaDebito(conta_, diff, "");
      contabilidade->adicionaCredito(idAjuste, diff, "");
    }
  }
  contabilidade->lanca();
  Moeda moeda(valor);
  ItemTimeline itemTimeline("",
			    WString::tr("fix").toUTF8() + " - " + descricao +
			    " - " + WString::tr("account").toUTF8() + ": '" + nome + "' "
          + WString::tr("fixed").toUTF8() + 
			    " $ " + moeda.valStr(),
			    "AjusteContaWidget::ajusteAction()",
			    "");
  timeline->adiciona(itemTimeline);
}

