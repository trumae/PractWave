#include <Wt/WTemplate>
#include <Wt/WPushButton>
#include <Wt/WAnchor>
#include <Wt/WImage>
#include <Wt/WText>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <vector>

#include "../../core/CabureApplication.h"
#include "../../logic/Clientes.h"
#include "../../logic/Fornecedores.h"
#include "../../logic/Contabilidade.h"
#include "../../logic/Moeda.h"
#include "../../logic/ContasBancarias.h"
#include "../../logic/Caixa.h"
#include "CaixaApp.h"

using namespace Wt;
using namespace std;

CaixaApp::CaixaApp(WContainerWidget *parent)
    : App(parent) {
  CONTACAIXA = 1;
  RETIRADA = 2;
  RETIRADADEPOSITO = 3;

  init();
}

CaixaApp::~CaixaApp(){
  delete ajuste_;
}

void CaixaApp::init(){
  CabureApplication *cabure = CabureApplication::cabureApplication();
  Contabilidade *contabilidade = cabure->contabilidade_;

  adicionaEstado(CONTACAIXA, boost::bind(&CaixaApp::EContaCaixa, this));
  adicionaEstado(RETIRADA, boost::bind(&CaixaApp::ERetirada, this));
  adicionaEstado(RETIRADADEPOSITO, boost::bind(&CaixaApp::ERetiradaDeposito, this));
  
  adicionaTransicao(CONTACAIXA, START, "back",
		    boost::bind(&CaixaApp::fazNada, this, nullptr),
		    boost::bind(&CaixaApp::guardOk, this));
  adicionaTransicao(CONTACAIXA, RETIRADA, "retirada",
		    boost::bind(&CaixaApp::fazNada, this, nullptr),
		    boost::bind(&CaixaApp::guardOk, this));
  adicionaTransicao(CONTACAIXA, RETIRADADEPOSITO, "retiradaDeposito",
		    boost::bind(&CaixaApp::fazNada, this, nullptr),
		    boost::bind(&CaixaApp::CTemContasBancarias, this));
  adicionaTransicao(RETIRADA, CONTACAIXA, "back",
		    boost::bind(&CaixaApp::fazNada, this, nullptr),
		    boost::bind(&CaixaApp::guardOk, this));
  adicionaTransicao(RETIRADA, CONTACAIXA, "fazRetirada",
  		    boost::bind(&CaixaApp::TFazRetirada, this),
  		    boost::bind(&CaixaApp::guardOk, this));
  adicionaTransicao(RETIRADADEPOSITO, CONTACAIXA, "back",
		    boost::bind(&CaixaApp::fazNada, this, nullptr),
		    boost::bind(&CaixaApp::guardOk, this));
  adicionaTransicao(RETIRADADEPOSITO, CONTACAIXA, "fazRetiradaDeposito",
		    boost::bind(&CaixaApp::TFazRetiradaDeposito, this),
		    boost::bind(&CaixaApp::guardOk, this));

  ajuste_ = new AjusteContaWidget(contabilidade->getIdPorNome("CAIXA"));
  ajuste_->setWidgetPai(this);
  ajuste_->setApp(this);
  ajuste_->setTituloApp("Caixa");
  ajuste_->setEstadoRetorno(CONTACAIXA);

  setEstado(CONTACAIXA);
}

WWidget *CaixaApp::EContaCaixa(){
  CabureApplication *cabure = CabureApplication::cabureApplication();
  Contabilidade *contabilidade = cabure->contabilidade_;
  
  WText *retiradaBtn = new WText(
				 "<button class='command-button bg-color-orangeDark default'>"
				 "Retirada"
				 "<small>Retirada para pagar despesas</small>"
				 "</button>",
				 Wt::XHTMLUnsafeText);
  retiradaBtn->clicked().connect(boost::bind(&CaixaApp::trataEvento, this, "retirada"));
  
  
  WText *retiradaDepositoBtn = new WText(
				      "<button class='command-button bg-color-blueDark default'>"
				      "Retirada para dep&oacute;sito"
				      "<small>Retirada de dinheiro para deposito</small>"
				      "</button>",
				      Wt::XHTMLUnsafeText);
  retiradaDepositoBtn->clicked().connect(boost::bind(&CaixaApp::trataEvento, this, "retiradaDeposito"));
  
  WContainerWidget *container = new WContainerWidget();
  WTemplate *tt = new WTemplate();
  tt->setTemplateText("<div>"
		      "${retirada}"
		      "${retiradaDeposito}"		     
		      "${ajuste}"			   
		      "</div>", XHTMLUnsafeText);
  tt->bindWidget("retirada", retiradaBtn);
  tt->bindWidget("retiradaDeposito", retiradaDepositoBtn);
  tt->bindWidget("ajuste", ajuste_->getButtonApp());
  container->addWidget(tt);
  new ContaWidget(container, contabilidade->getIdPorNome("CAIXA"));
  
  return container;
}

Wt::WWidget *CaixaApp::ERetirada(){
  CabureApplication *cabure = CabureApplication::cabureApplication();
  Contabilidade *contabilidade = cabure->contabilidade_;
  
  descricao = new WLineEdit();
  valor = new WLineEdit();
  valor->setValidator(Moeda::newWValidator());
  
  Wt::WPushButton *ok = new WPushButton("Ok");
  ok->setStyleClass("bg-color-blue fg-color-white");
  ok->clicked().connect(boost::bind(&CaixaApp::trataEvento, this, "fazRetirada"));

  Wt::WPushButton *cancel = new WPushButton("Cancela");
  cancel->setStyleClass("bg-color-orange fg-color-white");
  cancel->clicked().connect(boost::bind(&CaixaApp::trataEvento, this, "back"));
  
  centros = new WComboBox();
  std::vector<std::string> nomes;
  std::vector<std::string>::iterator itNomes;
  contabilidade->getNomesCentrosDeCusto(nomes);
  for(itNomes = nomes.begin(); itNomes < nomes.end(); itNomes++) {
    centros->addItem(*itNomes);
  }
  WTemplate *t = new WTemplate();
    t->setTemplateText(
        "<h2>Retirada do caixa</h2>"
	"<div class='grid'>"
	"   <div class='row'>"
        "      <div class='span2'>Descri&ccedil;&atilde;o</div>"
        "      <div class='input-control text span6'>"
        "        ${descricao}"
        "      </div>"
        "   </div>"
        "   <div class='row'>"
        "      <div class='span2'>Centro de Custo</div>"
        "      <div class='input-control text span6'>"
        "         ${centrodecusto}"
        "      </div>"
        "   </div>"
        "   <div class='row'>"
        "      <div class='span2'>Valor</div>"
        "      <div class='input-control text span6'>"
        "        ${valor}"
        "      </div>"
        "   </div>"
        "   <div class='row'>"
        "      ${ok}${cancel}"
        "   </div>"
        "</div>", XHTMLUnsafeText);

    t->bindWidget("descricao", descricao);
    t->bindWidget("valor", valor);
    t->bindWidget("centrodecusto", centros);
    t->bindWidget("ok", ok);
    t->bindWidget("cancel", cancel);

    return t;
}

Wt::WWidget *CaixaApp::ERetiradaDeposito(){
  CabureApplication *cabure = CabureApplication::cabureApplication();
  ContasBancarias *contasBancarias = cabure->contasBancarias_;
  
  descricao = new WLineEdit();
  valor = new WLineEdit();
  valor->setValidator(Moeda::newWValidator());
  
  Wt::WPushButton *ok = new WPushButton("Ok");
  ok->setStyleClass("bg-color-blue fg-color-white");
  ok->clicked().connect(boost::bind(&CaixaApp::trataEvento, this, "fazRetiradaDeposito"));

  Wt::WPushButton *cancel = new WPushButton("Cancela");
  cancel->setStyleClass("bg-color-orange fg-color-white");
  cancel->clicked().connect(boost::bind(&CaixaApp::trataEvento, this, "back"));

  bancos = new WComboBox();
  std::vector<ContaBancaria> nomes;
  std::vector<ContaBancaria>::iterator itNomes;
  contasBancarias->getTodosContasBancarias(nomes);
  for(itNomes = nomes.begin(); itNomes < nomes.end(); itNomes++) {
    bancos->addItem(WString((*itNomes).nome, UTF8));
  }
  
  WTemplate *t = new WTemplate();
  t->setTemplateText(
		     "<h2>Retirada para dep&oacute;sito</h2>"
		     "<div class='grid'>"
		     "   <div class='row'>"
		     "      <div class='span2'>Descri&ccedil;&atilde;o</div>"
		     "      <div class='input-control text span6'>"
		     "        ${descricao}"
		     "      </div>"
		     "   </div>"
		     "   <div class='row'>"
		     "      <div class='span2'>Banco</div>"
		     "      <div class='input-control text span6'>"
		     "        ${bancos}"
		     "      </div>"
		     "   </div>"
		     "   <div class='row'>"
		     "      <div class='span2'>Valor</div>"
		     "      <div class='input-control text span6'>"
		     "        ${valor}"
		     "      </div>"
		     "   </div>"
		     "   <div class='row'>"
		     "      ${ok}${cancel}"
		     "   </div>"
		     "</div>", XHTMLUnsafeText);
  
  t->bindWidget("descricao", descricao);
  t->bindWidget("valor", valor);
  t->bindWidget("bancos", bancos);
  t->bindWidget("ok", ok);
  t->bindWidget("cancel", cancel);
  return t;
}

bool CaixaApp::CTemContasBancarias(){
  CabureApplication *cabure = CabureApplication::cabureApplication();
  ContasBancarias *contasBancarias = cabure->contasBancarias_;

  std::vector<ContaBancaria> nomes;
  contasBancarias->getTodosContasBancarias(nomes);
  if(nomes.size() == 0) {
    setErrorMessage("Voc&ecirc; deve ter alguma conta banc&aacute;ria cadastrada!");
    return false;
  }
  return true;
}
  
void CaixaApp::TFazRetirada(){
  CabureApplication *cabure = CabureApplication::cabureApplication();
  Contabilidade *contabilidade = cabure->contabilidade_;
  Caixa *caixa = cabure->caixa_;
  if(valor->validate() == WValidator::Valid) {
    Moeda m(valor->text().narrow());
    long long saldo = 
      contabilidade->getSaldoContaFolha(contabilidade->getIdPorNome("CAIXA"));
    if(saldo - m.valInt() >= 0) {
      // monta e realiza lancamento
      caixa->retirada(descricao->text().toUTF8(),
		      contabilidade->getIdPorNome(centros->currentText().toUTF8()),
		      m.valInt());
    } else {
      setErrorMessage("O saldo do caixa n&atilde;o pode ficar negativo!");
      estado_ = RETIRADA;
    }
  } else {
    setErrorMessage("O formato do campo valor esta incorreto!");
    estado_ = RETIRADA;
  }
}

void CaixaApp::TFazRetiradaDeposito(){
  CabureApplication *cabure = CabureApplication::cabureApplication();
  Contabilidade *contabilidade = cabure->contabilidade_;
  Caixa *caixa = cabure->caixa_;
  ContasBancarias *contasBancarias = cabure->contasBancarias_;
  if(valor->validate() == WValidator::Valid) {
    Moeda m(valor->text().narrow());
    long long saldo = 
      contabilidade->getSaldoContaFolha(contabilidade->getIdPorNome("CAIXA"));
    if(saldo - m.valInt() >= 0) {
      ContaBancaria banco =
	contasBancarias->getContaBancariaPorNome(
						 bancos->currentText().toUTF8());
      // monta e realiza lancamento
      caixa->retiradaDepositoBanco(descricao->text().toUTF8(),
				   banco.idconta, m.valInt());
    } else {
      setErrorMessage("O saldo do caixa n&atilde;o pode ficar negativo!");
      estado_ = RETIRADADEPOSITO;
    }
  } else {
    setErrorMessage("O formato do campo valor esta incorreto!");
    estado_ = RETIRADADEPOSITO;
  } 
}

string CaixaApp::getTitulo(){
   return "Caixa";
}


