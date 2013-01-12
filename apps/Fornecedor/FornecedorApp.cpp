#include <Wt/WTemplate>
#include <Wt/WPushButton>
#include <Wt/WAnchor>
#include <Wt/WImage>
#include <Wt/WText>
#include <Wt/WBreak>
#include <Wt/WStackedWidget>
#include <Wt/WAnimation>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <vector>

#include "../../CabureApplication.h"
#include "../../logic/Clientes.h"
#include "../../logic/Fornecedores.h"
#include "../../logic/Contabilidade.h"
#include "../../logic/Moeda.h"
#include "../../logic/Timeline.h"
#include "../../widgets/ContaWidget/ContaWidget.h"
#include "../../widgets/ContaFornecedor/ContaFornecedor.h"
#include "FornecedorApp.h"
#include <cppdb/frontend.h>
#include <boost/lexical_cast.hpp>

using namespace Wt;
using namespace std;

FornecedorApp::FornecedorApp(WContainerWidget *parent)
  : App(parent) {
  INICIAIS = 1;
  ADICIONAFORNECEDOR = 2;
  LISTAFORNECEDORES = 3;
  DADOSFORNECEDOR = 4;
  CONTAFORNECEDOR = 5;
  init();
}

void FornecedorApp::constroiTabela(){
  //limpa vetores
  tabela.clear();
  estados.clear();

  // Estados
  adicionaEstado(INICIAIS, boost::bind(&FornecedorApp::EIniciais, this));
  adicionaEstado(ADICIONAFORNECEDOR, boost::bind(&FornecedorApp::EAdicionaFornecedor, this));
  adicionaEstado(LISTAFORNECEDORES, boost::bind(&FornecedorApp::EListaFornecedores, this));
  adicionaEstado(DADOSFORNECEDOR, boost::bind(&FornecedorApp::EDadosFornecedor, this));
  adicionaEstado(CONTAFORNECEDOR, boost::bind(&FornecedorApp::EContaFornecedor, this));  
  
  // Transicoes
  adicionaTransicao(INICIAIS, START, "back",
		    boost::bind(&FornecedorApp::fazNada, this, nullptr),
		    boost::bind(&FornecedorApp::guardOk, this));  
  adicionaTransicao(LISTAFORNECEDORES, INICIAIS, "back",
		    boost::bind(&FornecedorApp::fazNada, this, nullptr),
		    boost::bind(&FornecedorApp::guardOk, this));  
  adicionaTransicao(CONTAFORNECEDOR, LISTAFORNECEDORES, "back",
		    boost::bind(&FornecedorApp::fazNada, this, nullptr),
		    boost::bind(&FornecedorApp::guardOk, this));  
  adicionaTransicao(CONTAFORNECEDOR, DADOSFORNECEDOR, "dados",
		    boost::bind(&FornecedorApp::fazNada, this, nullptr),
		    boost::bind(&FornecedorApp::guardOk, this));  
  adicionaTransicao(DADOSFORNECEDOR, CONTAFORNECEDOR, "back",
		    boost::bind(&FornecedorApp::fazNada, this, nullptr),
		    boost::bind(&FornecedorApp::guardOk, this));  
  adicionaTransicao(DADOSFORNECEDOR, CONTAFORNECEDOR, "cancel",
		    boost::bind(&FornecedorApp::fazNada, this, nullptr),
		    boost::bind(&FornecedorApp::guardOk, this));  
  adicionaTransicao(INICIAIS, ADICIONAFORNECEDOR, "adiciona",
		    boost::bind(&FornecedorApp::fazNada, this, nullptr),
		    boost::bind(&FornecedorApp::guardOk, this));  
  adicionaTransicao(ADICIONAFORNECEDOR, INICIAIS, "back",
		    boost::bind(&FornecedorApp::fazNada, this, nullptr),
		    boost::bind(&FornecedorApp::guardOk, this));  
  adicionaTransicao(ADICIONAFORNECEDOR, INICIAIS, "cancel",
		    boost::bind(&FornecedorApp::fazNada, this, nullptr),
		    boost::bind(&FornecedorApp::guardOk, this));  
  
}

void FornecedorApp::init(){
  constroiTabela();
  setEstado(INICIAIS);
}

string FornecedorApp::getTitulo() {
  return "Fornecedores";
}

WWidget* FornecedorApp::EIniciais() {
  WContainerWidget *container = new WContainerWidget();
  CabureApplication *app = CabureApplication::cabureApplication();
  
  WText *addButton = new WText(
			       "<button class='command-button default bg-color-greenDark'>"
			       "Adiciona Fornecedor"
			       "<small>Clique aqui para adicionar um novo fornecedor</small>"
			       "</button>", Wt::XHTMLUnsafeText );
  addButton->clicked().connect(boost::bind(&FornecedorApp::trataEvento, this, "adiciona"));

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
  cppdb::session &db = app->db_;
  cppdb::result res = db << "select substr(upper(nome),1,1) as inicial, "
    "count(*) from fornecedor group by inicial order by inicial";
  constroiTabela(); // constroi novamente tabela de estados e transicoes
  while(res.next()) {
    string inicial;
    int count;
    res >> inicial >> count;
    string buffer = "<div class='tile bg-color-blueDark'>"
      "   <div class='tile-content'>"
      "     <h1>" + inicial + " </h1>"
      "   </div>"
      "   <div class='brand'>"
      "     <span class='badge'>"
      + boost::lexical_cast<string>(count) +
      "     </span>"
      "   </div>"
      "</div>";
    WText *tt = new WText(WString(buffer, UTF8), Wt::XHTMLUnsafeText);
    tt->clicked().connect(boost::bind(&FornecedorApp::trataEvento, this, "letra" + inicial));
    adicionaTransicao(INICIAIS, LISTAFORNECEDORES, "letra" + inicial,
		      boost::bind(&FornecedorApp::setInicialAtual, this, inicial),
		      boost::bind(&FornecedorApp::guardOk, this));  
    container->addWidget(tt);
  }
  return container;
}

void FornecedorApp::setInicialAtual(string ini){
  inicialAtual_ = ini;
}

WWidget* FornecedorApp::EListaFornecedores() {
  CabureApplication *app = CabureApplication::cabureApplication();
  Contabilidade *contabilidade = app->contabilidade_;
  
  WContainerWidget *container = new WContainerWidget();
  cppdb::session &db = app->db_;
  cppdb::result res = db << "select id, nome, idconta from fornecedor where substr(upper(nome),1,1) = ? order by nome" << inicialAtual_;
  constroiTabela();
  while(res.next()) {
    int id, idconta;
    string nome;
    res >> id >> nome >> idconta;
    int saldoconta = contabilidade->getSaldoContaFolha(idconta);
    Moeda saldo(saldoconta);
    
    string buffer = "<div class='tile double bg-color-purple'>"
      "   <div class='tile-content'>"
      "     <h3>" + nome + " </h3>"
      "   </div>"
      "   <div class='brand'>"
      "     <span class='name'>"
      + saldo.valStr() +
      "     </span>"
      "   </div>"
      "</div>";
    WText *tt = new WText(WString(buffer, UTF8), Wt::XHTMLUnsafeText);
    tt->clicked().connect(boost::bind(&FornecedorApp::trataEvento, this, "fornecedor" + id));
    adicionaTransicao(LISTAFORNECEDORES, CONTAFORNECEDOR, "fornecedor" + id,
		      boost::bind(&FornecedorApp::setFornecedorAtual, this, id, idconta),
		      boost::bind(&FornecedorApp::guardOk, this));  
    container->addWidget(tt);
  }
  return container;
}

void FornecedorApp::setFornecedorAtual(int id, int idconta){
  idFornecedor_ = id;
  idContaFornecedor_ = idconta;
}

WWidget* FornecedorApp::EDadosFornecedor(){
  CabureApplication *app = CabureApplication::cabureApplication();
  Fornecedor f = app->fornecedores_->getFornecedorPorId(idFornecedor_);

  WPushButton *btnSave = new WPushButton("Salvar");
  btnSave->setStyleClass("fg-color-white bg-color-blue");
  btnSave->clicked().connect(this, &FornecedorApp::saveFornecedor);
  
  WPushButton *btnCancel = new WPushButton("Cancelar");
  btnCancel->setStyleClass("fg-color-white bg-color-orange");
  btnCancel->clicked().connect(boost::bind(&FornecedorApp::trataEvento, this, "cancel"));

  WTemplate *t = new WTemplate();
  t->setTemplateText(
		     "${conteudo}"
		     "<div class=grid>"
		     "  <div class='row'>"
		     "    ${save}${cancel}"
		     "  </div>"
		     "</div>", Wt::XHTMLUnsafeText);		 
  
  t->bindWidget("save", btnSave);
  t->bindWidget("cancel", btnCancel);
  t->bindWidget("conteudo", createFormFornecedor(f));

  return t;
}

WWidget* FornecedorApp::EContaFornecedor(){
  WContainerWidget *molduraConta = new WContainerWidget();
  molduraConta->addWidget(new ContaFornecedor(molduraConta, idContaFornecedor_, this/*App*/, CONTAFORNECEDOR, DADOSFORNECEDOR));
  return molduraConta;
}

// Formularios de edicao e adicao
WWidget* FornecedorApp::EAdicionaFornecedor() {
  Fornecedor f;

  WPushButton *btnSave = new WPushButton("Adicionar");
  btnSave->setStyleClass("fg-color-white bg-color-blue");
  btnSave->clicked().connect(this, &FornecedorApp::adicionaFornecedor);
  
  WPushButton *btnCancel = new WPushButton("Cancelar");
  btnCancel->setStyleClass("fg-color-white bg-color-orange");
  btnCancel->clicked().connect(boost::bind(&FornecedorApp::trataEvento, this, "cancel"));

  WTemplate *t = new WTemplate();
  t->setTemplateText(
		     "${conteudo}"
		     "<div class=grid>"
		     "  <div class='row'>"
		     "    ${save}${cancel}"
		     "  </div>"
		     "</div>", Wt::XHTMLUnsafeText);		 
  
  t->bindWidget("save", btnSave);
  t->bindWidget("cancel", btnCancel);
  t->bindWidget("conteudo", createFormFornecedor(f));

  return t;
}

WWidget* FornecedorApp::createFormFornecedor(Fornecedor f) {
  editandoId = f.id; //armazena id que podera ser salvo
  editNome_ = new Wt::WLineEdit();
  editNome_->setText(WString(f.nome,UTF8));
  editNome_->setMaxLength(100);
  editEndereco_ = new Wt::WLineEdit();
  editEndereco_->setText(WString(f.endereco,UTF8));
  editBairro_ = new Wt::WLineEdit();
  editBairro_->setText(WString(f.bairro, UTF8));
  editCidade_ = new Wt::WLineEdit();
  editCidade_->setText(WString(f.cidade,UTF8));
  editEstado_ = new Wt::WLineEdit();
  editEstado_->setText(WString(f.estado, UTF8));
  editCEP_ = new Wt::WLineEdit();
  editCEP_->setText(WString(f.cep,UTF8));
  editTelefone_ = new Wt::WLineEdit();
  editTelefone_->setText(WString(f.telefone, UTF8));
  editCelular_ = new Wt::WLineEdit();
  editCelular_->setText(WString(f.celular,UTF8));
  editEmail_ = new Wt::WLineEdit();
  editEmail_->setText(WString(f.email,UTF8));
  editObservacao_ = new Wt::WTextArea();
  editObservacao_->setText(WString(f.observacao, UTF8));
      
  WTemplate *t = new WTemplate();
  t->setTemplateText(
		     "<div class='grid'>"
		     "  <div class='row'>"
		     "    <div class='span2'>Nome</div>"
		     "    <div class='input-control text span6'>"
		     "        ${nome}"
		     "    </div>"
		     "    <div class=''span1>(obrigat&oacute;rio)</div>"
		     "  </div>"

		     "  <div class='row'>"
		     "    <div class='span2'>Endere&ccedil;o</div>"
		     "    <div class='input-control text span6'>"
		     "        ${endereco}"
		     "    </div>"
		     "  </div>"

		     "  <div class='row'>"
		     "    <div class='span2'>Bairro</div>"
		     "    <div class='input-control text span6'>"
		     "        ${bairro}"
		     "    </div>"
		     "  </div>"

		     "  <div class='row'>"
		     "    <div class='span2'>Cidade</div>"
		     "    <div class='input-control text span6'>"
		     "          ${cidade}"
		     "    </div>"
		     "  </div>"

		     "  <div class='row'>"
		     "    <div class='span2'>Estado</div>"
		     "    <div class='input-control text span6'>"
		     "        ${estado}"
		     "    </div>"
		     "  </div>"

		     "  <div class='row'>"
		     "    <div class='span2'>CEP</div>"
		     "    <div class='input-control text span6'>"
		     "       ${CEP}"
		     "    </div>"
		     "  </div>"		     

		     "  <div class='row'>"
		     "    <div class='span2'>Telefone</div>"
		     "    <div class='input-control text span6'>"
		     "       ${telefone}"
		     "    </div>"
		     "  </div>"
		     
		     "  <div class='row'>"
		     "    <div class='span2'>Celular</div>"
		     "    <div class='input-control text span6'>"
		     "       ${celular}"
		     "    </div>"
		     "  </div>"
		   
		     "  <div class='row'>"
		     "    <div class='span2'>E-mail</div>"
		     "    <div class='input-control text span6'>"
		     "       ${email}"
		     "    </div>"
		     "  </div>"
		     
		     "  <div class='row'>"
		     "    <div class='span2'>Observa&ccedil;&atilde;o</div>"
		     "    <div class='input-control textarea span6'>"
		     "       ${observacao}"		 
		     "    </div>"
		     "  </div>", Wt::XHTMLUnsafeText);

  t->bindWidget("nome", editNome_);
  t->bindWidget("endereco", editEndereco_);
  t->bindWidget("bairro", editBairro_);
  t->bindWidget("cidade", editCidade_);
  t->bindWidget("estado", editEstado_);
  t->bindWidget("CEP", editCEP_);
  t->bindWidget("telefone", editTelefone_);
  t->bindWidget("celular", editCelular_);
  t->bindWidget("email", editEmail_);
  t->bindWidget("observacao", editObservacao_);

  return t;
}

void FornecedorApp::adicionaFornecedor(){
  CabureApplication *cabure = CabureApplication::cabureApplication();
  Fornecedores *fornecedores = cabure->fornecedores_;
  
  if(editNome_->text().toUTF8() != "") {
    Fornecedor f(editNome_->text().toUTF8(),
		 editEndereco_->text().toUTF8(),
		 editBairro_->text().toUTF8(),
		 editCidade_->text().toUTF8(),
		 editEstado_->text().toUTF8(),
		 editCEP_->text().toUTF8(),
		 editTelefone_->text().toUTF8(),
		 editCelular_->text().toUTF8(),
		 editEmail_->text().toUTF8(),
		 editObservacao_->text().toUTF8());
    
    if(fornecedores->adiciona(f)) {
      setEstado(INICIAIS);
      setMessage("Novo fornecedor adicionado com sucesso!");
    } else {
      setErrorMessage("Erro ao adicionar novo fornecedor");
    }
  } else {
    setErrorMessage("O nome do fornecedor &eacute; obrigat&oacute;rio!");
  }
}

void FornecedorApp::saveFornecedor(){
  CabureApplication *cabure = CabureApplication::cabureApplication();
  Fornecedores *fornecedores = cabure->fornecedores_;
  
  if(editNome_->text().toUTF8() != "") {
    Fornecedor f(editNome_->text().toUTF8(),
		 editEndereco_->text().toUTF8(),
		 editBairro_->text().toUTF8(),
		 editCidade_->text().toUTF8(),
		 editEstado_->text().toUTF8(),
		 editCEP_->text().toUTF8(),
		 editTelefone_->text().toUTF8(),
		 editCelular_->text().toUTF8(),
		 editEmail_->text().toUTF8(),
		 editObservacao_->text().toUTF8());
    f.id = editandoId;
    if(fornecedores->salvar(f)) {
      setEstado(CONTAFORNECEDOR);      
      setMessage("Fornecedor editado com sucesso!");
    } else {
      setErrorMessage("Erro ao editar novo fornecedor");
    }
  } else {
    setErrorMessage("O nome do fornecedor &eacute; obrigat&oacute;rio!");
  }
}


