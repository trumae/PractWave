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
				    "<button class='bg-color-pinkDark fg-color-white'>"
				    "  <span class='label'>Ajuste</span>"
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
  ok->setStyleClass("btn btn-primary");
  ok->clicked().connect(this, &AjusteContaWidget::trataOk);

  Wt::WPushButton *cancel = new WPushButton("Cancela");
  cancel->setStyleClass("btn");
  cancel->clicked().connect(this, &AjusteContaWidget::trataCancela);

  WTemplate *t = new WTemplate(widgetPai_);
  t->setTemplateText(
		     "<h1>Ajuste</h1>"
		     "<div class='alert alert-block'>"
		     "<h3>Cuidado!</h3>"
		     "O comando de ajuste deve ser usado com cuidado. "
		     "A utiliza&ccedil;&atilde;o em excesso mostra que "
		     "algumas das suas a&ccedil;&otilde;es n&atilde;o est&atilde;o "
		     "sendo registradas. "
		     "</div>"
		     "<form class='form-horizontal'>"
		     "  <fieldset>"
		     "    <legend>Dados do ajuste</legend>"
		     "    <div class='control-group'> <!-- descricao -->"
		     "      <label class='control-label' for='nome'>"
		     "        Descri&ccedil;&atilde;o</label>"
		     "      <div class='controls'>"
		     "        ${descricao}"
		     "      </div>"
		     "    </div>"
		     "    <div class='control-group'> <!-- valor -->"
		     "      <label class='control-label' for='nome'>Saldo</label>"
		     "      <div class='controls'>"
		     "        ${valor}"
		     "      </div>"
		     "    </div>"
		     "  </fieldset>"
		     "<div class='well'>"
		     "   ${ok}${cancel}"
		     "</div>"
		     "</form>", XHTMLUnsafeText);
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
      //content_->viewHome();
    } else {
      WMessageBox::show("Erro", "Um valor para o ajuste deve ser fornecido.", Ok);
    }
  }
}

void AjusteContaWidget::trataCancela() {
  //content_->viewHome();
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

