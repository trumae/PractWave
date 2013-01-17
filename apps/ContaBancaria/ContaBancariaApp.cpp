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
#include "ContaBancariaApp.h"

#include "../../widgets/AjusteContaWidget/AjusteContaWidget.h"
#include "../../widgets/ContaWidget/ContaWidget.h"

using namespace Wt;
using namespace std;

ContaBancariaApp::ContaBancariaApp(WContainerWidget *parent)
    : App(parent) {
  LISTABANCO = 1;
  ADICIONABANCO = 2;
  CONTABANCO = 3;
  DADOSBANCO = 4;
  RETIRADABANCO = 5;
  RETIRADACAIXABANCO = 6;

  init();
}

ContaBancariaApp::~ContaBancariaApp(){
}

void ContaBancariaApp::constroiTabela() {
  //limpa vetores
  tabela.clear();
  estados.clear();
  
  // Estados
  adicionaEstado(LISTABANCO,         boost::bind(&ContaBancariaApp::EListaBanco, this));
  adicionaEstado(ADICIONABANCO,      boost::bind(&ContaBancariaApp::EAdicionaBanco, this));
  adicionaEstado(CONTABANCO,         boost::bind(&ContaBancariaApp::EContaBanco, this));
  adicionaEstado(DADOSBANCO,         boost::bind(&ContaBancariaApp::EDadosBanco, this));
  adicionaEstado(RETIRADABANCO,      boost::bind(&ContaBancariaApp::ERetiradaBanco, this));
  adicionaEstado(RETIRADACAIXABANCO, boost::bind(&ContaBancariaApp::ERetiradaCaixaBanco, this));  

  // Transicoes
  adicionaTransicao(LISTABANCO, START, "back",
		    boost::bind(&ContaBancariaApp::fazNada, this, nullptr),
		    boost::bind(&ContaBancariaApp::guardOk, this));
  adicionaTransicao(LISTABANCO, ADICIONABANCO, "adiciona",
		    boost::bind(&ContaBancariaApp::fazNada, this, nullptr),
		    boost::bind(&ContaBancariaApp::guardOk, this));
  adicionaTransicao(ADICIONABANCO, LISTABANCO, "back",
		    boost::bind(&ContaBancariaApp::fazNada, this, nullptr),
		    boost::bind(&ContaBancariaApp::guardOk, this));
  adicionaTransicao(ADICIONABANCO, LISTABANCO, "cancel",
		    boost::bind(&ContaBancariaApp::fazNada, this, nullptr),
		    boost::bind(&ContaBancariaApp::guardOk, this));
  adicionaTransicao(ADICIONABANCO, LISTABANCO, "salvaBanco",
		    boost::bind(&ContaBancariaApp::TAdicionaContaBancaria, this),
		    boost::bind(&ContaBancariaApp::guardOk, this));
  adicionaTransicao(CONTABANCO, LISTABANCO, "back",
		    boost::bind(&ContaBancariaApp::TBackContaBanco, this),
		    boost::bind(&ContaBancariaApp::guardOk, this));
  adicionaTransicao(CONTABANCO, RETIRADABANCO, "retirada",
		    boost::bind(&ContaBancariaApp::fazNada, this, nullptr),
		    boost::bind(&ContaBancariaApp::guardOk, this));
  adicionaTransicao(RETIRADABANCO, CONTABANCO, "back",
		    boost::bind(&ContaBancariaApp::fazNada, this, nullptr),
		    boost::bind(&ContaBancariaApp::guardOk, this));
  adicionaTransicao(CONTABANCO, RETIRADACAIXABANCO, "retiradaCaixa",
		    boost::bind(&ContaBancariaApp::fazNada, this, nullptr),
		    boost::bind(&ContaBancariaApp::guardOk, this));
  adicionaTransicao(RETIRADACAIXABANCO, CONTABANCO, "back",
		    boost::bind(&ContaBancariaApp::fazNada, this, nullptr),
		    boost::bind(&ContaBancariaApp::guardOk, this));
  adicionaTransicao(CONTABANCO, DADOSBANCO, "dados",
		    boost::bind(&ContaBancariaApp::fazNada, this, nullptr),
		    boost::bind(&ContaBancariaApp::guardOk, this));
  adicionaTransicao(DADOSBANCO, CONTABANCO, "back",
		    boost::bind(&ContaBancariaApp::fazNada, this, nullptr),
		    boost::bind(&ContaBancariaApp::guardOk, this));
  adicionaTransicao(DADOSBANCO, CONTABANCO, "cancel",
		    boost::bind(&ContaBancariaApp::fazNada, this, nullptr),
		    boost::bind(&ContaBancariaApp::guardOk, this));
  adicionaTransicao(DADOSBANCO, CONTABANCO, "salvaBanco",
		    boost::bind(&ContaBancariaApp::TSalvaContaBancaria, this),
		    boost::bind(&ContaBancariaApp::guardOk, this));
  adicionaTransicao(RETIRADABANCO, CONTABANCO, "cancel",
		    boost::bind(&ContaBancariaApp::fazNada, this, nullptr),
		    boost::bind(&ContaBancariaApp::guardOk, this));
  adicionaTransicao(RETIRADABANCO, CONTABANCO, "fazRetirada",
		    boost::bind(&ContaBancariaApp::TRetirada, this),
		    boost::bind(&ContaBancariaApp::guardOk, this));
  adicionaTransicao(RETIRADACAIXABANCO, CONTABANCO, "cancel",
		    boost::bind(&ContaBancariaApp::fazNada, this, nullptr),
		    boost::bind(&ContaBancariaApp::guardOk, this));
  adicionaTransicao(RETIRADACAIXABANCO, CONTABANCO, "fazRetiradaCaixa",
		    boost::bind(&ContaBancariaApp::TRetiradaCaixa, this),
		    boost::bind(&ContaBancariaApp::guardOk, this));
}

void ContaBancariaApp::init(){
  constroiTabela();
  setEstado(LISTABANCO);
}

WWidget *ContaBancariaApp::EListaBanco(){
    CabureApplication *app = CabureApplication::cabureApplication();
    ContasBancarias *contasBancarias = app->contasBancarias_;
    Contabilidade *contabilidade = app->contabilidade_;

    WContainerWidget *container = new WContainerWidget();

    WText *addButton = new WText(
				 "<button class='command-button default bg-color-greenDark'>"
				 "Adiciona Conta Banc&aacute;ria"
				 "<small>Clique aqui para adicionar uma nova conta banc&aacute;ria</small>"
				 "</button>", Wt::XHTMLUnsafeText );
    addButton->clicked().connect(boost::bind(&ContaBancariaApp::trataEvento, this, "adiciona"));
    
    WTemplate *comandos = new WTemplate();
    comandos->setTemplateText(
			      "<div class='grid'>"
			      "  <div class='row'>"
			      "${add}"			      
			      "</div>"
			      "</div>"
			      , Wt::XHTMLUnsafeText
			      );
    comandos->bindWidget("add", addButton);
    container->addWidget(comandos);

    std::vector < ContaBancaria > vc;
    contasBancarias->getTodosContasBancarias(vc);

    constroiTabela();
    for(ContaBancaria c : vc) {
      string conteudo = "";
      int saldoconta = contabilidade->getSaldoContaFolha(c.idconta);
      Moeda saldo(saldoconta);

      conteudo = "<div class='tile double bg-color-blue'>"
	"   <div class='tile-content'>"
	"     <h3>" + c.nome + " </h3>"
	"   </div>"
	"   <div class='brand'>"
	"     <span class='name'>"
	+ saldo.valStr() +
	"     </span>"
	"   </div>"
	"</div>";
      WText *wtext = new WText(WString(conteudo, UTF8), XHTMLUnsafeText);

      wtext->clicked().connect(boost::bind(&ContaBancariaApp::trataEvento, this, "contabanco" + c.id));
      adicionaTransicao(LISTABANCO, CONTABANCO, "contabanco" + c.id,
		      boost::bind(&ContaBancariaApp::TEntraContaBanco, this, c.id),
		      boost::bind(&ContaBancariaApp::guardOk, this));  

      container->addWidget(wtext);
    }

    return container;
}

Wt::WWidget *ContaBancariaApp::EAdicionaBanco(){
  ContaBancaria cb;

  cb_ = cb; //zera dados da conta bancaria
  WPushButton *btnSave = new WPushButton("Adicionar");
  btnSave->setStyleClass("fg-color-white bg-color-blue");
  btnSave->clicked().connect(boost::bind(&ContaBancariaApp::trataEvento, this, "salvaBanco"));
  
  WPushButton *btnCancel = new WPushButton("Cancelar");
  btnCancel->setStyleClass("fg-color-white bg-color-orange");
  btnCancel->clicked().connect(boost::bind(&ContaBancariaApp::trataEvento, this, "cancel"));

  WTemplate *t = new WTemplate();
  t->setTemplateText(
		     "${conteudo}"
		     "<div class=grid>"
		     "  <div class='row'>"
		     "    ${save}${cancel}"
		     "  </div>"
		     "</div>", Wt::XHTMLUnsafeText);		 
  
  t->bindWidget("conteudo", createFormContaBancaria());
  t->bindWidget("save", btnSave);
  t->bindWidget("cancel", btnCancel);

  return t;
}

Wt::WWidget *ContaBancariaApp::EContaBanco(){
  CabureApplication *cabure = CabureApplication::cabureApplication();

  WText *retiradaBtn = new WText(
                               "<button class='command-button bg-color-orangeDark default'>"
                               "Retirada"
                               "<small>Retirada para pagar despesas</small>"
                               "</button>",
                               Wt::XHTMLUnsafeText);
  retiradaBtn->clicked().connect(boost::bind(&ContaBancariaApp::trataEvento, this, "retirada"));

  
  WText *retiradaCaixaBtn = new WText(
                                "<button class='command-button bg-color-blueDark default'>"
                                "Retirada para Caixa"
                                "<small>Retirada de dinheiro para caixa</small>"
                                "</button>",
                                Wt::XHTMLUnsafeText);
  retiradaCaixaBtn->clicked().connect(boost::bind(&ContaBancariaApp::trataEvento, this, "retiradaCaixa"));
  
  WText *dadosBtn = new WText(
                              "<button class='command-button bg-color-greenDark default'>"
                              "Dados"
                              "<small>Veja/Edite os dados da conta banc&aacute;ria</small>"
                              "</button>",
                                Wt::XHTMLUnsafeText);
  dadosBtn->clicked().connect(boost::bind(&ContaBancariaApp::trataEvento, this, "dados"));

  WContainerWidget *container = new WContainerWidget();
  WTemplate *tt = new WTemplate();
  tt->setTemplateText("<div>"
			     "${retirada}"
			     "${retiradaCaixa}"
			     "${dados}"
                             "${ajuste}"			   
                             "</div>", XHTMLUnsafeText);
  tt->bindWidget("retirada", retiradaBtn);
  tt->bindWidget("retiradaCaixa", retiradaCaixaBtn);
  tt->bindWidget("dados", dadosBtn);
  tt->bindWidget("ajuste", ajuste_->getButtonApp());
  container->addWidget(tt);
  new ContaWidget(container, idconta_);

  return container;
}

Wt::WWidget *ContaBancariaApp::EDadosBanco(){
  WPushButton *btnSave = new WPushButton("Salvar");
  btnSave->setStyleClass("fg-color-white bg-color-blue");
  btnSave->clicked().connect(boost::bind(&ContaBancariaApp::trataEvento, this, "salvaBanco"));
  
  WPushButton *btnCancel = new WPushButton("Cancelar");
  btnCancel->setStyleClass("fg-color-white bg-color-orange");
  btnCancel->clicked().connect(boost::bind(&ContaBancariaApp::trataEvento, this, "cancel"));

  WTemplate *t = new WTemplate();
  t->setTemplateText(
		     "${conteudo}"
		     "<div class=grid>"
		     "  <div class='row'>"
		     "    ${save}${cancel}"
		     "  </div>"
		     "</div>", Wt::XHTMLUnsafeText);		 
  
  t->bindWidget("conteudo", createFormContaBancaria());
  t->bindWidget("save", btnSave);
  t->bindWidget("cancel", btnCancel);

  return t;
}

Wt::WWidget *ContaBancariaApp::ERetiradaBanco(){
  CabureApplication *cabure = CabureApplication::cabureApplication();
  Contabilidade *contabilidade = cabure->contabilidade_;
  
  descricao = new WLineEdit();
  valor = new WLineEdit();
  valor->setValidator(Moeda::newWValidator());
  
  Wt::WPushButton *ok = new WPushButton("Ok");
  ok->setStyleClass("bg-color-blue fg-color-white");
  ok->clicked().connect(boost::bind(&ContaBancariaApp::trataEvento, this, "fazRetirada"));
  
  Wt::WPushButton *cancel = new WPushButton("Cancela");
  cancel->setStyleClass("bg-color-orange fg-color-white");
  cancel->clicked().connect(boost::bind(&ContaBancariaApp::trataEvento, this, "cancel"));

  centros = new WComboBox();
  std::vector<std::string> nomes;
  std::vector<std::string>::iterator itNomes;
  contabilidade->getNomesCentrosDeCusto(nomes);
  for(itNomes = nomes.begin(); itNomes < nomes.end(); itNomes++) {
    centros->addItem(*itNomes);
  }
  
  WTemplate *t = new WTemplate();
  t->setTemplateText("<h2>Retirada</h2>"
		     "<div class='grid'>"
		     "  <div class='row'>"
		     "     <div class='span2'>Descri&ccedil;&atilde;o</div>"
		     "     <div class='input-control text span6'>"
		     "        ${descricao}"
		     "     </div>"
		     "  </div>"
		     "  <div class='row'>"
		     "     <div class='span2'>Centro de Custo</div>"
		     "     <div class='input-control text span6'>"
		     "        ${centrodecusto}"
		     "     </div>"
		     "  </div>"
		     "  <div class='row'>"
		     "     <div class='span2'>Valor</div>"
		     "     <div class='input-control text span6'>"
		     "       ${valor}"
		     "     </div>"
		     "  </div>"
		     "  <div class='row'>"
		     "     ${ok}${cancel}"
		     "  </div>"
		     "</div>", XHTMLUnsafeText);
  
  t->bindWidget("descricao", descricao);
  t->bindWidget("valor", valor);
  t->bindWidget("centrodecusto", centros);
  t->bindWidget("ok", ok);
  t->bindWidget("cancel", cancel);

  return t;
}

Wt::WWidget *ContaBancariaApp::ERetiradaCaixaBanco(){
  CabureApplication *cabure = CabureApplication::cabureApplication();
  Contabilidade *contabilidade = cabure->contabilidade_;
  
  descricao = new WLineEdit();
  valor = new WLineEdit();
  valor->setValidator(Moeda::newWValidator());
  
  Wt::WPushButton *ok = new WPushButton("Ok");
  ok->setStyleClass("bg-color-blue fg-color-white");
  ok->clicked().connect(boost::bind(&ContaBancariaApp::trataEvento, this, "fazRetiradaCaixa"));
  
  Wt::WPushButton *cancel = new WPushButton("Cancela");
  cancel->setStyleClass("bg-color-orange fg-color-white");
  cancel->clicked().connect(boost::bind(&ContaBancariaApp::trataEvento, this, "cancel"));
  
  WTemplate *t = new WTemplate();
  t->setTemplateText("<h2>Retirada para caixa</h2>"
		     "<div class='grid'>"
		     "  <div class='row'>"
		     "     <div class='span2'>Descri&ccedil;&atilde;o</div>"
		     "     <div class='input-control text span6'>"
		     "        ${descricao}"
		     "     </div>"
		     "  </div>"
		     "  <div class='row'>"
		     "     <div class='span2'>Valor</div>"
		     "     <div class='input-control text span6'>"
		     "       ${valor}"
		     "     </div>"
		     "  </div>"
		     "  <div class='row'>"
		     "     ${ok}${cancel}"
		     "  </div>"
		     "</div>", XHTMLUnsafeText);
  
  t->bindWidget("descricao", descricao);
  t->bindWidget("valor", valor);
  t->bindWidget("ok", ok);
  t->bindWidget("cancel", cancel);

  return t;
}

/////////// acoes  //////////////

void ContaBancariaApp::TAdicionaContaBancaria(){
  CabureApplication *cabure = CabureApplication::cabureApplication();
  ContasBancarias *c = cabure->contasBancarias_;
  
  if(editNome_->text().narrow() != "") {
    ContaBancaria cb(editNome_->text().toUTF8(),
		     editAgencia_->text().toUTF8(),
		     editNumero_->text().toUTF8(),
		     editTelefone_->text().toUTF8());
    
    if(c->adiciona(cb)) {
      setMessage("Nova conta banc&aacute;ria adicionada com sucesso!");
    } else {
      setErrorMessage("Erro ao adicionar nova conta banc&aacute;ria");
      estado_ = ADICIONABANCO; // retorna pro estado ADICIONABANCO
    }
  } else {
    setErrorMessage("Um nome para a conta banc&aacute;ria &eacute; obrigat&oacute;rio!");
    estado_ = ADICIONABANCO; // retorna pro estado ADICIONABANCO
  }
}

void ContaBancariaApp::TSalvaContaBancaria(){
    CabureApplication *cabure = CabureApplication::cabureApplication();
    ContasBancarias *c = cabure->contasBancarias_;

    if(editNome_->text().narrow() != "") {
        ContaBancaria cb(editNome_->text().toUTF8(),
                         editAgencia_->text().toUTF8(),
                         editNumero_->text().toUTF8(),
                         editTelefone_->text().toUTF8());
        cb.id = cb_.id;
        if(c->salvar(cb)){
            setMessage("Conta banc&aacute;ria editada com sucesso!");
	    cb_ = cb; //copia novos valores para cb_
        } else {
            setErrorMessage("Erro ao editar conta banc&aacute;ria");
        }
    } else {
        setErrorMessage("Um nome para a conta banc&aacute;ria &eacute; obrigat&oacute;rio!");
	estado_ = DADOSBANCO;
    }
}

void ContaBancariaApp::TRetirada(){
  CabureApplication *cabure = CabureApplication::cabureApplication();
  Contabilidade *contabilidade = cabure->contabilidade_;
  ContasBancarias *contaBancaria = cabure->contasBancarias_;
  if(valor->validate() == WValidator::Valid) {
    Moeda m(valor->text().narrow());
    // monta e realiza lancamento
    contaBancaria->retiradaParaCentroCusto(descricao->text().toUTF8(),
					   idconta_,
					   contabilidade->getIdPorNome(centros->currentText().toUTF8()),
					   m.valInt());
  } else {
    setErrorMessage("Formado do valor inv&aacute;lido");
    estado_ = RETIRADABANCO;
  }
}

void ContaBancariaApp::TRetiradaCaixa(){
  CabureApplication *cabure = CabureApplication::cabureApplication();
  ContasBancarias *contaBancaria = cabure->contasBancarias_;
  if(valor->validate() == WValidator::Valid) {
    Moeda m(valor->text().narrow());
    // monta e realiza lancamento
    contaBancaria->retiradaParaCaixa(
				     descricao->text().toUTF8(),
				     idconta_,
				     m.valInt());
  } else {
    setErrorMessage("Formado do valor inv&aacute;lido");
    estado_ = RETIRADACAIXABANCO;
  }
}

void ContaBancariaApp::TEntraContaBanco(int id){ /* aloca ajusteConta e seta iddaconta */
  CabureApplication *cabure = CabureApplication::cabureApplication();
  ContasBancarias *c = cabure->contasBancarias_;

  codContaBancaria_ = id;
  cb_ = c->getContaBancariaPorId(id);
  idconta_ = cb_.idconta;
  
  ajuste_ = new AjusteContaWidget(idconta_);
  ajuste_->setWidgetPai(this);
  ajuste_->setApp(this);
  ajuste_->setTituloApp("Contas Banc&aacute;rias");
  ajuste_->setEstadoRetorno(CONTABANCO);
}

void ContaBancariaApp::TBackContaBanco(){ /* libera ajustaConta */ 
  delete ajuste_;
}

WWidget *ContaBancariaApp::createFormContaBancaria (){
  editNome_ = new Wt::WLineEdit();
  editNome_->setText(WString(cb_.nome, UTF8));
  editNome_->setMaxLength(100);
  editAgencia_ = new Wt::WLineEdit();
  editAgencia_->setText(WString(cb_.agencia, UTF8));
  editNumero_ = new Wt::WLineEdit();
  editNumero_->setText(WString(cb_.numero, UTF8));
  editTelefone_ = new Wt::WLineEdit();
  editTelefone_->setText(WString(cb_.telefone, UTF8));
  
  WTemplate *t = new WTemplate();
  t->setTemplateText("<div class='grid'>"
		     "  <div class='row'>"
		     "     <div class='span2'>Nome</div>"
		     "     <div class='input-control text span6'>"
		     "        ${nome}"
		     "     </div>"
		     "     <div class='span1'>(Obrigat&oacute;rio)</div>"
		     "  </div>"
		     "  <div class='row'>"
		     "     <div class='span2'>Agencia</div>"
		     "     <div class='input-control text span6'>"
		     "        ${agencia}"
		     "     </div>"
		     "  </div>"
		     "  <div class='row'>"
		     "     <div class='span2'>N&uacute;mero</div>"
		     "     <div class='input-control text span6'>"
		     "        ${numero}"
		     "     </div>"
		     "  </div>"
		     "  <div class='row'>"
		     "     <div class='span2'>Telefone</div>"
		     "     <div class='input-control text span6'>"
		     "        ${telefone}"
		     "     </div>"
		     "  </div>"
		     "</div>", Wt::XHTMLUnsafeText);
    t->bindWidget("nome", editNome_);
    t->bindWidget("agencia", editAgencia_);
    t->bindWidget("numero", editNumero_);
    t->bindWidget("telefone", editTelefone_);
    return t;
}

string ContaBancariaApp::getTitulo(){
   return "Contas Banc&aacute;rias";
}
