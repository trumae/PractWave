#include <Wt/WDialog>
#include <Wt/WTemplate>
#include <Wt/WText>
#include <Wt/WLineEdit>
#include <Wt/WMessageBox>

#include "AjusteContaWidget.h"
#include "../../CabureApplication.h"
#include "../../logic/Moeda.h"

using namespace Wt;
using namespace std;

AjusteContaWidget::AjusteContaWidget(int conta) :
  conta_(conta) {
}

Wt::WWidget *AjusteContaWidget::getButton() {
  Wt::WText *button = new Wt::WText(
				    "<button class='command-button bg-color-pinkDark default'>"
				    "Ajuste"
				    "<small>Fa&ccedil;a ajustes no Saldo da conta</small>"
				    "</button>",
				    Wt::XHTMLUnsafeText);
  button->clicked().connect(this, &AjusteContaWidget::ajuste);
  return button;
}

void AjusteContaWidget::ajuste() {
  widgetPai_->clear();

  descricao = new WLineEdit();
  valor = new WLineEdit();
  valor->setValidator(Moeda::newWValidator());

  Wt::WPushButton *ok = new WPushButton("Ok");
  ok->setStyleClass("bg-color-blue fg-color-white");
  ok->clicked().connect(this, &AjusteContaWidget::trataOk);

  Wt::WPushButton *cancel = new WPushButton("Cancela");
  cancel->setStyleClass("bg-color-orange fg-color-white");
  cancel->clicked().connect(this, &AjusteContaWidget::trataCancela);

  WTemplate *t = new WTemplate(widgetPai_);
  t->setTemplateText(
		     "<h1>Ajuste</h1>"
		     "<div class='alerta'>"
		     "<h3>Cuidado!</h3>"
		     "O comando de ajuste deve ser usado com cuidado. "
		     "A utiliza&ccedil;&atilde;o em excesso mostra que "
		     "algumas das suas a&ccedil;&otilde;es n&atilde;o est&atilde;o "
		     "sendo registradas. "
		     "</div>"

		     "<h4>Dados do ajuste</h4>"
		     "<div class='grid'>"
		     "  <div class='row'>"
		     "      <div class='span1'>Descri&ccedil;&atilde;o</div>"
		     "      <div class='input-control text span4'>"
		     "        ${descricao}"
		     "      </div>"
		     "  </div>"
		     "  <div class='row'>"
		     "      <div class='span1'>Saldo</div>"
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
      WMessageBox::show("Erro", "Um valor para o ajuste deve ser fornecido.", Ok);
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
  contabilidade->setDescricaoLancamento("AJUSTE - " + descricao);
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
			    "AJUSTE - " + descricao +
			    " - conta: '" + nome + "' ajustada"
			    " para R$ " + moeda.valStr(),
			    "AjusteContaWidget::ajusteAction()",
			    "");
  timeline->adiciona(itemTimeline);
}

