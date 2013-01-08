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
#include "../../widgets/ContaCliente/ContaCliente.h"
#include "ClienteApp.h"
#include <cppdb/frontend.h>
#include <boost/lexical_cast.hpp>

using namespace Wt;
using namespace std;

ClienteApp::ClienteApp(WContainerWidget *parent)
  : App(parent) {
  INICIAIS = 1;
  ADICIONACLIENTE = 2;
  LISTACLIENTES = 3;
  DADOSCLIENTE = 4;
  CONTACLIENTE = 5;
  init();
}

void ClienteApp::constroiTabela(){
  //limpa vetores
  tabela.clear();
  estados.clear();

  // Estados
  adicionaEstado(INICIAIS, boost::bind(&ClienteApp::EIniciais, this));
  adicionaEstado(ADICIONACLIENTE, boost::bind(&ClienteApp::EAdicionaCliente, this));
  adicionaEstado(LISTACLIENTES, boost::bind(&ClienteApp::EListaClientes, this));
  adicionaEstado(DADOSCLIENTE, boost::bind(&ClienteApp::EDadosCliente, this));
  adicionaEstado(CONTACLIENTE, boost::bind(&ClienteApp::EContaCliente, this));  
  
  // Transicoes
  adicionaTransicao(INICIAIS, START, "back",
		    boost::bind(&ClienteApp::fazNada, this, nullptr),
		    boost::bind(&ClienteApp::guardOk, this));  
  adicionaTransicao(LISTACLIENTES, INICIAIS, "back",
		    boost::bind(&ClienteApp::fazNada, this, nullptr),
		    boost::bind(&ClienteApp::guardOk, this));  
  adicionaTransicao(CONTACLIENTE, LISTACLIENTES, "back",
		    boost::bind(&ClienteApp::fazNada, this, nullptr),
		    boost::bind(&ClienteApp::guardOk, this));  


}

void ClienteApp::init(){
  constroiTabela();
  setEstado(INICIAIS);
}

string ClienteApp::getTitulo() {
  return "PractWave - Clientes";
}

WWidget* ClienteApp::EAdicionaCliente() {
  return nullptr;
}

WWidget* ClienteApp::EIniciais() {
  WContainerWidget *container = new WContainerWidget();
  CabureApplication *app = CabureApplication::cabureApplication();
  
  WText *addButton = new WText(
			       "<div class='tile double bg-color-orange'>"
			       "   <div class='tile-content'>"
			       "     <h2>Adiciona Cliente</h2>"
			       "   </div>"
			       "</div>", Wt::XHTMLUnsafeText );
  addButton->clicked().connect(this, &ClienteApp::formAdicionaCliente);
  
  WText *searchButton = new WText(
				  "<div class='tile double bg-color-green'>"
				  "   <div class='tile-content'>"
				  "     <h2>Busca Cliente</h2>"
				  "   </div>"
				  "</div>", Wt::XHTMLUnsafeText );
  
  WTemplate *comandos = new WTemplate();
  comandos->setTemplateText(
			    "<div class='grid'>"
			    "  <div class='row'>${add}${search}</div>"
			    "</div>"
			    , Wt::XHTMLUnsafeText
			    );
  comandos->bindWidget("add", addButton);
  comandos->bindWidget("search", searchButton);
  
  container->addWidget(comandos);
  cppdb::session &db = app->db_;
  cppdb::result res = db << "select substr(upper(nome),1,1) as inicial, "
    "count(*) from cliente group by inicial order by inicial";
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
    tt->clicked().connect(boost::bind(&ClienteApp::trataEvento, this, "letra" + inicial));
    adicionaTransicao(INICIAIS, LISTACLIENTES, "letra" + inicial,
		      boost::bind(&ClienteApp::setInicialAtual, this, inicial),
		      boost::bind(&ClienteApp::guardOk, this));  
    container->addWidget(tt);
  }
  return container;
}

void ClienteApp::setInicialAtual(string ini){
  inicialAtual_ = ini;
}

WWidget* ClienteApp::EListaClientes() {
  //void ClienteApp::processInicial(string inicial) {
  CabureApplication *app = CabureApplication::cabureApplication();
  Contabilidade *contabilidade = app->contabilidade_;
  
  WContainerWidget *container = new WContainerWidget();
  cppdb::session &db = app->db_;
  cppdb::result res = db << "select id, nome, idconta from cliente where substr(upper(nome),1,1) = ? order by nome" << inicialAtual_;
  constroiTabela();
  while(res.next()) {
    int id, idconta;
    string nome;
    res >> id >> nome >> idconta;
    int saldoconta = contabilidade->getSaldoContaFolha(idconta);
    Moeda saldo(saldoconta);
    
    string buffer = "<div class='tile double bg-color-orangeDark'>"
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
    tt->clicked().connect(boost::bind(&ClienteApp::trataEvento, this, "cliente" + id));
    adicionaTransicao(LISTACLIENTES, CONTACLIENTE, "cliente" + id,
		      boost::bind(&ClienteApp::setClienteAtual, this, id, idconta),
		      boost::bind(&ClienteApp::guardOk, this));  
    container->addWidget(tt);
  }
  return container;
}

void ClienteApp::setClienteAtual(int id, int idconta){
  idCliente_ = id;
  idContaCliente_ = idconta;
}

WWidget* ClienteApp::EDadosCliente(){
  return nullptr;
}

WWidget* ClienteApp::EContaCliente(){
  WContainerWidget *molduraConta = new WContainerWidget();
  molduraConta->addWidget(new ContaCliente(molduraConta, idContaCliente_));
  return molduraConta;
}

// Formularios de edicao e adicao

void ClienteApp::formAdicionaCliente(){
  Cliente cli;
  clear();

  Wt::WAnchor *back = new Wt::WAnchor();
  back->setStyleClass("back-button big page-back");
  back->setInline(true);
  back->clicked().connect(this, &ClienteApp::init);

  WPushButton *btnSave = new WPushButton();
  btnSave->setStyleClass("btn btn-primary");
  btnSave->setText("Adicionar");
  btnSave->clicked().connect(this, &ClienteApp::adicionaCliente);
  
  WPushButton *btnCancel = new WPushButton("Cancelar");
  btnCancel->setStyleClass("btn");
  btnCancel->clicked().connect(this, &ClienteApp::init );

  WTemplate *t = new WTemplate(this);
  t->setTemplateText(
		     "<div class='page secondary'>"
		     "   <div class='page-header'>"
		     "      <div class='page-header-content'>"
		     "          <h1>${titulo}</h1>"
		     "          ${backButton}"
		     "      </div>"
		     "   </div>"
		     "   <div class='page-region'>"
		     "      <div class='page-region-content'>"
		     "      ${conteudo}"
		     "      <div class=grid>"
		     "        <div class='row'>"
		     "         ${save}${cancel}"
		     "        </div>"
		     "      </div>"
		     "      </div>"
		     "   </div>"
		     "</div>", Wt::XHTMLUnsafeText);		 
  
  t->bindString("titulo", "Cadastra novo cliente");
  t->bindWidget("save", btnSave);
  t->bindWidget("cancel", btnCancel);
  t->bindWidget("backButton", back);
  t->bindWidget("conteudo", createFormCliente(cli));
}

WWidget* ClienteApp::createFormCliente(Cliente cli) {
  editandoId = cli.id; //armazena id que podera ser salvo
  editNome_ = new Wt::WLineEdit();
  editNome_->setText(WString(cli.nome,UTF8));
  editNome_->setMaxLength(100);
  editEndereco_ = new Wt::WLineEdit();
  editEndereco_->setText(WString(cli.endereco,UTF8));
  editBairro_ = new Wt::WLineEdit();
  editBairro_->setText(WString(cli.bairro, UTF8));
  editCidade_ = new Wt::WLineEdit();
  editCidade_->setText(WString(cli.cidade,UTF8));
  editEstado_ = new Wt::WLineEdit();
  editEstado_->setText(WString(cli.estado, UTF8));
  editCEP_ = new Wt::WLineEdit();
  editCEP_->setText(WString(cli.cep,UTF8));
  editTelefone_ = new Wt::WLineEdit();
  editTelefone_->setText(WString(cli.telefone, UTF8));
  editCelular_ = new Wt::WLineEdit();
  editCelular_->setText(WString(cli.celular,UTF8));
  editEmail_ = new Wt::WLineEdit();
  editEmail_->setText(WString(cli.email,UTF8));
  editObservacao_ = new Wt::WTextArea();
  editObservacao_->setText(WString(cli.observacao, UTF8));
      
  WTemplate *t = new WTemplate();
  t->setTemplateText(
		     "<div class='grid'>"
		     "  <div class='row'>"
		     "    <div class='span2'>Nome</div>"
		     "    <div class='input-control text span6'>"
		     "        ${nome}"
		     "        <button class='helper'></button>"
		     "    </div>"
		     "    <div class=''span1>(obrigat&oacute;rio)</div>"
		     "  </div>"

		     "  <div class='row'>"
		     "    <div class='span2'>Endere&ccedil;o</div>"
		     "    <div class='input-control text span6'>"
		     "        ${endereco}"
		     "        <button class='helper'></button>"
		     "    </div>"
		     "  </div>"

		     "  <div class='row'>"
		     "    <div class='span2'>Bairro</div>"
		     "    <div class='input-control text span6'>"
		     "        ${bairro}"
		     "        <button class='helper'></button>"
		     "    </div>"
		     "  </div>"

		     "  <div class='row'>"
		     "    <div class='span2'>Cidade</div>"
		     "    <div class='input-control text span6'>"
		     "          ${cidade}"
		     "        <button class='helper'></button>"
		     "    </div>"
		     "  </div>"

		     "  <div class='row'>"
		     "    <div class='span2'>Estado</div>"
		     "    <div class='input-control text span6'>"
		     "        ${estado}"
		     "        <button class='helper'></button>"
		     "    </div>"
		     "  </div>"

		     "  <div class='row'>"
		     "    <div class='span2'>CEP</div>"
		     "    <div class='input-control text span6'>"
		     "       ${CEP}"
		     "       <button class='helper'></button>"
		     "    </div>"
		     "  </div>"		     

		     "  <div class='row'>"
		     "    <div class='span2'>Telefone</div>"
		     "    <div class='input-control text span6'>"
		     "       ${telefone}"
		     "       <button class='helper'></button>"
		     "    </div>"
		     "  </div>"
		     
		     "  <div class='row'>"
		     "    <div class='span2'>Celular</div>"
		     "    <div class='input-control text span6'>"
		     "       ${celular}"
		     "       <button class='helper'></button>"
		     "    </div>"
		     "  </div>"
		   
		     "  <div class='row'>"
		     "    <div class='span2'>E-mail</div>"
		     "    <div class='input-control text span6'>"
		     "       ${email}"
		     "       <button class='helper'></button>"
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

void ClienteApp::adicionaCliente(){
  CabureApplication *cabure = CabureApplication::cabureApplication();
  Clientes *c = cabure->clientes_;
  
  if(editNome_->text().toUTF8() != "") {
    Cliente cli(editNome_->text().toUTF8(),
		editEndereco_->text().toUTF8(),
		editBairro_->text().toUTF8(),
		editCidade_->text().toUTF8(),
		editEstado_->text().toUTF8(),
		editCEP_->text().toUTF8(),
		editTelefone_->text().toUTF8(),
		editCelular_->text().toUTF8(),
		editEmail_->text().toUTF8(),
		editObservacao_->text().toUTF8());
    
    if(c->adiciona(cli)) {
      init();
      //setMessage("Novo cliente adicionado com sucesso!", "alert-success");
    } else {
      //setMessage("Erro ao adicionar novo cliente", "alert-error");
    }
  } else {
    //setMessage("O nome do cliente &eacute; obrigat&oacute;rio!", "alert-error");
  }
}

void ClienteApp::saveCliente(){
  CabureApplication *cabure = CabureApplication::cabureApplication();
  Clientes *c = cabure->clientes_;
  
  if(editNome_->text().toUTF8() != "") {
    Cliente cli(editNome_->text().toUTF8(),
		editEndereco_->text().toUTF8(),
		editBairro_->text().toUTF8(),
		editCidade_->text().toUTF8(),
		editEstado_->text().toUTF8(),
		editCEP_->text().toUTF8(),
		editTelefone_->text().toUTF8(),
		editCelular_->text().toUTF8(),
		editEmail_->text().toUTF8(),
		editObservacao_->text().toUTF8());
    cli.id = editandoId;
    if(c->salvar(cli)) {
      init();
      //setMessage("Cliente editado com sucesso!", "alert-success");
    } else {
      //setMessage("Erro ao editar novo cliente", "alert-error");
    }
  } else {
    //setMessage("O nome do cliente &eacute; obrigat&oacute;rio!", "alert-error");
  }
}

