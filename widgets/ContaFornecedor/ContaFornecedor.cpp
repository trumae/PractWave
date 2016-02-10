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
#include "ContaFornecedor.h"

using namespace Wt;

ContaFornecedor::ContaFornecedor(WContainerWidget *parent,
				 int idconta,
				 App *app,
				 int estadoRetorno,
				 int estadoDadosFornecedor):
  WContainerWidget(parent),
  idconta_(idconta),
  app_(app),
  estadoRetorno_(estadoRetorno),
  estadoDadosFornecedor_(estadoDadosFornecedor){
  ajuste_ = new AjusteContaWidget(idconta_);
  viewHome();
  }

ContaFornecedor::~ContaFornecedor() {
  delete ajuste_;
}

void ContaFornecedor::goDadosFornecedor(){
  app_->setEstado(estadoDadosFornecedor_);
}

void ContaFornecedor::viewHome() {
  CabureApplication *cabure = CabureApplication::cabureApplication();
  cppdb::session db = cabure->db_;

  clear();

  WText *anotarBtn = new WText(
			       "<button class='command-button bg-color-orangeDark default'>"
             + tr("buy").toUTF8() +			      
			       "<small>" + tr("msg-buy").toUTF8() + "</small>"
			       "</button>",
			       Wt::XHTMLUnsafeText);
  anotarBtn->clicked().connect(this, &ContaFornecedor::comprar);

  WText *receberBtn = new WText(
				"<button class='command-button bg-color-blueDark default'>"
        + tr("pay").toUTF8() + 
				"<small>" + tr("msg-pay").toUTF8() + "</small>"
				"</button>",
				Wt::XHTMLUnsafeText);
  receberBtn->clicked().connect(this, &ContaFornecedor::pagar);

  WText *dadosBtn = new WText(
			      "<button class='command-button bg-color-greenDark default'>"
			      + tr("info").toUTF8() +  
			      "<small>" + tr("msg-info-supplier").toUTF8() + "</small>"
			      "</button>",
				Wt::XHTMLUnsafeText);
  dadosBtn->clicked().connect(this, &ContaFornecedor::goDadosFornecedor);

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

void ContaFornecedor::trataCancela() {
  viewHome();
}

void ContaFornecedor::trataComprarOk() {
  CabureApplication *cabure = CabureApplication::cabureApplication();
  Fornecedores *fornecedores = cabure->fornecedores_;
  if(valor->validate() == WValidator::Valid) {
    Moeda m(valor->text().narrow());
    // monta e realiza lancamento
    fornecedores->comprar(idconta_,
			  descricao->text().toUTF8(),
			  m.valInt());
    viewHome();
  }
}

void ContaFornecedor::trataPagarOk() {
  CabureApplication *cabure = CabureApplication::cabureApplication();
  Contabilidade *contabilidade = cabure->contabilidade_;
  Fornecedores *fornecedores = cabure->fornecedores_;
  ContasBancarias *contasBancarias = cabure->contasBancarias_;
  ContaBancaria banco;
  
  if(valor->validate() == WValidator::Valid) {
    Moeda m(valor->text().narrow());
    long long saldo = contabilidade->getSaldoContaFolha(
							contabilidade->getIdPorNome("CAIXA"));
    if(saldo - m.valInt() < 0 && origemDinheiro->currentIndex() == 0) {
      WMessageBox::show("Error", tr("msg-not-negative-balance").toUTF8(), Ok);
    } else {
      if(bancos->currentText() != "")
	banco = contasBancarias->getContaBancariaPorNome(bancos->currentText().toUTF8());
      // monta e realiza lancamento
      fornecedores->pagar(idconta_,
                          descricao->text().toUTF8(),
                          m.valInt(),
			  origemDinheiro->currentIndex(),
			  banco);
      viewHome();
    }
  }
}

void ContaFornecedor::comprar() {
  clear();
  descricao = new WLineEdit();
  valor = new WLineEdit();
  valor->setValidator(Moeda::newWValidator());

  Wt::WPushButton *ok = new WPushButton("Ok");
  ok->setStyleClass("bg-color-blue fg-color-white");
  ok->clicked().connect(this, &ContaFornecedor::trataComprarOk);
  Wt::WPushButton *cancel = new WPushButton(tr("btn-cancel").toUTF8());
  cancel->setStyleClass("bg-color-orange fg-color-white");
  cancel->clicked().connect(this, &ContaFornecedor::trataCancela);

  WTemplate *t = new WTemplate(this);
  t->setTemplateText(
		     "<h2>" + tr("msg-buy").toUTF8() + "</h2>"
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

void ContaFornecedor::pagar() {
  CabureApplication *cabure = CabureApplication::cabureApplication();
  ContasBancarias *contasBancarias = cabure->contasBancarias_;

  clear();
  descricao = new WLineEdit();
  valor = new WLineEdit();
  valor->setValidator(Moeda::newWValidator());

  Wt::WPushButton *ok = new WPushButton("Ok");
  ok->setStyleClass("bg-color-blue fg-color-white");
  ok->clicked().connect(this, &ContaFornecedor::trataPagarOk);
  Wt::WPushButton *cancel = new WPushButton(tr("btn-cancel").toUTF8());
  cancel->setStyleClass("bg-color-orange fg-color-white");
  cancel->clicked().connect(this, &ContaFornecedor::trataCancela);

  bancos = new WComboBox();
  std::vector<ContaBancaria> nomes;
  std::vector<ContaBancaria>::iterator itNomes;
  contasBancarias->getTodosContasBancarias(nomes);
  for(itNomes = nomes.begin(); itNomes < nomes.end(); itNomes++) {
    bancos->addItem(WString((*itNomes).nome, UTF8));
  }
  
  origemDinheiro = new WComboBox();
  origemDinheiro->addItem(tr("cash").toUTF8());
  if(nomes.size() > 0)
    origemDinheiro->addItem(tr("bank").toUTF8());
  origemDinheiro->sactivated().connect(this, &ContaFornecedor::exibirEsconderBancos);
  
  bancosTemplate = new WTemplate(this);
  bancosTemplate->setTemplateText("<div class='row'>"
				  "  <div class='span2'>Banco</div>"
				  "  <div class='input-control text span6'>"
				  "      ${bancos}"
				  "  </div>"
				  "</div>", XHTMLUnsafeText);
  bancosTemplate->bindWidget("bancos", bancos);

  WTemplate *t = new WTemplate(this);
  t->setTemplateText(
		     "<h2>" + tr("msg-pay").toUTF8() + "</h2>"
		     "<div class='grid'>"
		     "  <div class='row'>  "
		     "    <div class='span2'>" + tr("description").toUTF8() + "</div>"
		     "    <div class='input-control text span4'> <!-- descricao -->"
		     "        ${descricao}"
		     "    </div>"
		     "  </div>"
		     "  <div class='row'>"
		     "    <div class='span2'>" + tr("money-from").toUTF8() + "</div>"
		     "    <div class='input-control text span6'>"
		     "        ${origemDinheiro}"
		     "    </div>"
		     "  </div>"
		     "  ${bancosTemplate}"
		     "  <div class='row'>  "
		     "    <div class='span2'>" + tr("value").toUTF8() + "</div>"
		     "    <div class='input-control text span4'> <!-- valor -->"
		     "        ${valor}"
		     "    </div>"
		     "</div>"
		     "<div>"
		     "   ${ok}${cancel}"
		     "</div>", XHTMLUnsafeText);

  t->bindWidget("origemDinheiro", origemDinheiro);
  t->bindWidget("bancosTemplate", bancosTemplate);
  t->bindWidget("descricao", descricao);
  t->bindWidget("valor", valor);
  t->bindWidget("ok", ok);
  t->bindWidget("cancel", cancel);

  bancosTemplate->setHidden(true);
}

void ContaFornecedor::exibirEsconderBancos(){
  if(origemDinheiro->currentIndex() == 1)
    bancosTemplate->setHidden(false);
  else
    bancosTemplate->setHidden(true);
}



