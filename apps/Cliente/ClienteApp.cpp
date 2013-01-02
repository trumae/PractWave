#include <Wt/WTemplate>
#include <Wt/WPushButton>
#include <Wt/WAnchor>
#include <Wt/WImage>
#include <Wt/WText>
#include <Wt/WBreak>
#include <Wt/WStackedWidget>
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
    init();
}

WWidget* ClienteApp::getConteudo() {
    WContainerWidget *container = new WContainerWidget();
    CabureApplication *app = CabureApplication::cabureApplication();

    WText *addButton = new WText(
        "<div class='tile double bg-color-orange'>"
        "   <div class='tile-content'>"
        "     <h2>Adiciona Cliente</h2>"
        "   </div>"
        "</div>", Wt::XHTMLUnsafeText );

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
        tt->clicked().connect(boost::bind(&ClienteApp::processInicial, this, inicial));
        container->addWidget(tt);
    }
    return container;
}

string ClienteApp::getTitulo() {
    return "PractWave - Clientes";
}

void ClienteApp::processInicial(string inicial) {
    CabureApplication *app = CabureApplication::cabureApplication();
    Contabilidade *contabilidade = app->contabilidade_;
    clear();
    Wt::WAnchor *back = new Wt::WAnchor();
    back->setStyleClass("back-button big page-back");
    back->setInline(true);
    back->clicked().connect(this, &ClienteApp::init);

    WContainerWidget *container = new WContainerWidget();
    cppdb::session &db = app->db_;
    cppdb::result res = db << "select id, nome, idconta from cliente where substr(upper(nome),1,1) = ? order by nome" << inicial;
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
        tt->clicked().connect(boost::bind(&ClienteApp::processCliente, this, id, idconta, inicial));
        container->addWidget(tt);
    }

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
    "      </div>"
    "   </div>"
    "</div>", Wt::XHTMLUnsafeText);

    t->bindWidget("backButton", back);
    t->bindString("titulo", getTitulo());
    t->bindWidget("conteudo", container);
}

void ClienteApp::processCliente(int id, int idconta, string inicial) {
    CabureApplication *app = CabureApplication::cabureApplication();
    Contabilidade *contabilidade = app->contabilidade_;
    clear();
    
	stack = new WStackedWidget();
    stack->addWidget(new ContaCliente(this, idconta));
    stack->addWidget(new WText("Teste"));

    Wt::WAnchor *back = new Wt::WAnchor();
    back->setStyleClass("back-button big page-back");
    back->setInline(true);
    back->clicked().connect(boost::bind(&ClienteApp::processInicial, this, inicial));

    WText *conta = new WText("<li class='sticker sticker-color-blue'><a><i class='icon-clipboard-2'></i>Conta</a></li>", Wt::XHTMLUnsafeText);
    conta->setInline(true);
	conta->clicked().connect(this, &ClienteApp::showConta);


	WText *dados = new WText("<li class='sticker sticker-color-orange'><a><i class='icon-user'></i>Dados</a></li>", Wt::XHTMLUnsafeText);
	dados->setInline(true);
	dados->clicked().connect(this, &ClienteApp::showDados);

    WTemplate *t = new WTemplate(this);
    t->setTemplateText(
    "<div class='page secondary with-sidebar'>"
    "   <div class='page-header'>"
    "      <div class='page-header-content'>"
    "          <h1>${titulo}</h1>"
    "          ${backButton}"
    "      </div>"
    "   </div>"
    "   <div class='page-sidebar'>"
	"    ${conta} ${dados}"
    "   </div>"
    "   <div class='page-region'>"
    "      <div class='page-region-content'>"
    "      ${conteudo}"
    "      </div>"
    "   </div>"
    "</div>", Wt::XHTMLUnsafeText);

    t->bindWidget("backButton", back);
    t->bindWidget("conta", conta);
    t->bindWidget("dados", dados);
    t->bindString("titulo", getTitulo());

    t->bindWidget("conteudo", stack);
}

void ClienteApp::showDados(){
    stack->setCurrentIndex(1);
}

void ClienteApp::showConta(){
    stack->setCurrentIndex(0);
}



