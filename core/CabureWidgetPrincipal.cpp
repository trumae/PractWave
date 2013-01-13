#include <Wt/WApplication>
#include <Wt/WText>
#include <Wt/WImage>
#include <Wt/WPanel>
#include <Wt/WMenu>
#include <Wt/WStackedWidget>
#include <Wt/WHBoxLayout>
#include <Wt/WTemplate>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WDialog>
#include <Wt/WAnchor>

#include <boost/xpressive/xpressive.hpp>
#include <iostream>

#include <cppdb/frontend.h>

#include "CabureApplication.h"
#include "CabureUserManager.h"
#include "CabureWidgetPrincipal.h"
#include "../logic/Notificacao.h"
#include "../apps/Painel/PainelApp.h"
#include "../apps/Timeline/TimelineApp.h"
#include "../apps/Cliente/ClienteApp.h"
#include "../apps/Fornecedor/FornecedorApp.h"
#include "../apps/ContaBancaria/ContaBancariaApp.h"
#include "../apps/Caixa/CaixaApp.h"
#include "../apps/Calculadora/CalculadoraApp.h"

using namespace Wt;

Wt::Auth::AuthService authServiceGoogle;
Wt::Auth::AuthService authServiceFacebook;
Wt::Auth::GoogleService *googleService = 0;
Wt::Auth::FacebookService *facebookService = 0;

CabureWidgetPrincipal::CabureWidgetPrincipal(WContainerWidget *parent)
    :WContainerWidget(parent) {
    processGoogle_ = googleService->createProcess
                     ///(googleService->authenticationScope() + ImportContactsGoogle::getScope());
                     (googleService->authenticationScope());
    WTemplate *googleggi = new WTemplate();
    googleggi->setTemplateText(
       "     <div class='tile double bg-color-green'>"
       "       <div class='tile-content'>"
       "          <h2>Google</h2>"
       "          <div class='brand'>"
       "             <img class='icon-google'/>"
       "          </div>"
       "       </div>"
       "    </div>"
    );
    googleggi->clicked().connect(processGoogle_,
                                 &Wt::Auth::OAuthProcess::startAuthenticate);
    processGoogle_->authenticated().connect(this, &CabureWidgetPrincipal::authenticatedGoogle);
    processGoogle_->authorized().connect(this, &CabureWidgetPrincipal::authorized);

    processFacebook_ = facebookService->createProcess
                       (facebookService->authenticationScope());
    WTemplate *facebookggi = new WTemplate();
    facebookggi->setTemplateText(
       "    <div class='tile double bg-color-blueDark'>"
       "       <div class='tile-content'>"
       "       <h2>Facebook</h2>"
       "       </div>"
       "    </div>"
    );
    facebookggi->clicked().connect(processFacebook_,
                                   &Wt::Auth::OAuthProcess::startAuthenticate);
    processFacebook_->authenticated().connect(this, &CabureWidgetPrincipal::authenticatedFacebook);

    WTemplate *login = new WTemplate(this);
    login->setTemplateText(
       "<div class='page  secondary'>"
       " <div class='page-header'>"
       "   <div class='page-header-content'>"
       "     <h1>Escolha a forma de entrar no PractWave</h1>"
       "   </div>"
       " </div>"						     
       " <div class='page-region'>"
       "   <div class='page-region-content'>"
       "   ${google}"
       "   ${facebook}"
       "   </div>"
       "  </div>"
       "</div>"
        , XHTMLUnsafeText);

    login->bindWidget("google",googleggi);
    login->bindWidget("facebook",facebookggi);
}

CabureWidgetPrincipal::~CabureWidgetPrincipal() {
}

void CabureWidgetPrincipal::iniciaDBUser(std::string email) {
    CabureApplication *cabure = CabureApplication::cabureApplication();
    // abre banco de dados do usuario
    cabure->db_.open("sqlite3:db=" + email + ".db");

    // cria objetos de sessao
    cabure->contabilidade_ = new Contabilidade(cabure->db_);
    cabure->timeline_ = new Timeline(cabure->db_, cabure->contabilidade_);
    cabure->clientes_ = new Clientes(cabure->db_,
                                     cabure->contabilidade_,
                                     cabure->timeline_);
    cabure->fornecedores_ = new Fornecedores(cabure->db_,
            cabure->contabilidade_,
            cabure->timeline_);
    cabure->contasBancarias_ = new ContasBancarias(
        cabure->db_, cabure->contabilidade_,
        cabure->timeline_);
    cabure->caixa_ = new Caixa(
        cabure->db_, cabure->contabilidade_,
        cabure->timeline_);
    cabure->meusDados_ = new MeusDados();
    cabure->objetosIniciados = true;

    //cria tabelas do usuario
    cabure->contabilidade_->criaTabelas();
    cabure->clientes_->criaTabelas();
    cabure->fornecedores_->criaTabelas();
    cabure->contasBancarias_->criaTabelas();
    cabure->timeline_->criaTabelas();
}

void CabureWidgetPrincipal::authorized(const Wt::Auth::OAuthAccessToken& token) {
    CabureApplication *cabure = CabureApplication::cabureApplication();
    std::cout << "Token = " << token.value() << std::endl;
    cabure->token = token.value();
}


void CabureWidgetPrincipal::authenticatedGoogle(const Wt::Auth::Identity& identity) {
    CabureApplication *app = CabureApplication::cabureApplication();
    app->tipoAuth = CabureApplication::GOOGLE;
    authenticated(identity);
}

void CabureWidgetPrincipal::authenticatedFacebook(const Wt::Auth::Identity& identity) {
    CabureApplication *app = CabureApplication::cabureApplication();
    app->tipoAuth = CabureApplication::FACEBOOK;
    authenticated(identity);
}

void CabureWidgetPrincipal::authenticated(const Wt::Auth::Identity& identity) {
    CabureApplication *cabure = CabureApplication::cabureApplication();
    //int idUsuario = -1;
    //int conviteAceito = -1;
    //cppdb::result res = userDB.db() << "select id, conviteaceito from user where email = ?"
    //                    << identity.email() << cppdb::row;

    cabure->name = identity.name().toUTF8();
    cabure->email = identity.email();
    iniciaDBUser(identity.email());

/*    if(!res.empty()) {
        res >> idUsuario >> conviteAceito;

        cabure->meusDados_->idUser = idUsuario;

        cppdb::result res2 = userDB.db() << "SELECT * FROM dados_user WHERE id_user = ?"
                             << idUsuario << cppdb::row;

        if(res2.empty()) {
            //insere novo usuario na tabela de dados do usuario
            userDB.db() << "insert into dados_user(id_user, "
                        "email_alternativo, profissao, cidade, estado) "
                        "values(?,'','','','')"
                        << idUsuario << cppdb::exec;
        }

        if(conviteAceito == 0) {
            informeSobreConvite(identity);
            return;
        }

    } else {
        //insere novo usuario
        userDB.db() << "insert into user(name, email) values(?,?)"
                    << identity.name().narrow() << identity.email() << cppdb::exec;

        cppdb::result res1 = userDB.db() << "SELECT id FROM user WHERE email = ?"
                             << identity.email() << cppdb::row;

        if(!res1.empty()) {
            res1 >> idUsuario;
            cabure->meusDados_->idUser = idUsuario;
            //insere novo usuario na tabela de dados do usuario
            userDB.db() << "insert into dados_user(id_user, "
                        "email_alternativo, profissao, cidade, estado) "
                        "values(?,'','','','')"
                        << idUsuario << cppdb::exec;
        }

        informeSobreConvite(identity);
        return;
    }
*/
    createUI();
}

bool CabureWidgetPrincipal::codigoOk(WString &s, int &promotor) {
    using namespace boost::xpressive;
    std::string st = s.narrow();
    int mes, ano;

    sregex rex = sregex::compile( "c(\\d+)\\w(\\d+)\\w(\\d\\d)(\\d\\d)(\\w\\w)" );
    smatch what;
    if( regex_match( st, what, rex ) )
    {
        std::istringstream(what[1]) >> promotor;
        std::istringstream(what[3]) >> mes;
        std::istringstream(what[4]) >> ano;
        return true;
    }

    return false;
}

void CabureWidgetPrincipal::processaCupom() {
    CabureApplication *cabure = CabureApplication::cabureApplication();
    int promotor;
    WString cod = codigo->text();

    if(codigoOk(cod, promotor)) {
        if(userDB.cupomJaUsado(cod.toUTF8())) {
            Wt::WDialog dialog("Seu cupom j&aacute; foi usado!");
            new Wt::WText("Voc&eacute; est&aacute; utilizando um cupom promocional</br>"
                          "que j&aacute; foi utilizado anteriormente.", XHTMLUnsafeText, dialog.contents());
            new Wt::WBreak(dialog.contents());
            Wt::WPushButton ok("Ok", dialog.contents());
            ok.clicked().connect(&dialog, &Wt::WDialog::accept);
            if (dialog.exec() == Wt::WDialog::Accepted)
                codigo->setText("");
        } else {
            Wt::WDialog dialog("Bem Vindo!");
            new Wt::WText("Voc&eacute; est&aacute; utilizando um cupom promocional</br>patrocinado por " +
                          userDB.getPromotor(promotor)
                          , XHTMLUnsafeText, dialog.contents());
            new Wt::WBreak(dialog.contents());
            Wt::WPushButton ok("Ok", dialog.contents());
            ok.clicked().connect(&dialog, &Wt::WDialog::accept);
            if (dialog.exec() == Wt::WDialog::Accepted) {
                userDB.registraCupomUsado(cod.toUTF8());
                userDB.aceitaConvite(cabure->email);
                Notificacao notification(cabure->email, "bemvindo.pl");
                notification.send();
                createUI();
            }
        }
    } else {
        Wt::WDialog dialog("C&oacute;digo incorreto!");
        new Wt::WText("Este c&oacute;digo promocional n&atilde;o &eacute; v&aacute;lido.</br>Verifique se voc&ecirc; n&atilde;o errou ao digita-lo.", XHTMLUnsafeText, dialog.contents());
        new Wt::WBreak(dialog.contents());
        Wt::WPushButton ok("Ok", dialog.contents());
        ok.clicked().connect(&dialog, &Wt::WDialog::accept);
        if (dialog.exec() == Wt::WDialog::Accepted)
            codigo->setText("");
    }
}

void CabureWidgetPrincipal::informeSobreConvite(const Wt::Auth::Identity& identity) {
    clear();

    WPushButton *btncodigo = new WPushButton("Resgatar");
    btncodigo->setStyleClass("btn");
    btncodigo->clicked().connect(this, &CabureWidgetPrincipal::processaCupom);

    codigo = new WLineEdit();
    codigo->setStyleClass("input-medium");

    WTemplate *mensagem = new WTemplate(this);
    mensagem->setTemplateText(
        "<link href='/css/site.css' rel='stylesheet'>"
        "<div style=\"display:inline;\">"
        "<img height=\"1\" width=\"1\" style=\"border-style:none;\" alt=\"\" src=\"http://www.googleadservices.com/pagead/conversion/999519350/?value=0,10&amp;label=eLrVCILgwgMQ9ujN3AM&amp;guid=ON&amp;script=0\"/>"
        "</div>"
        "<div id='header'>"
        "  <div class='navbar navbar-fixed-top'>"
        "	<div class='navbar-inner'>"
        "	  <div class='container'>"
        "	    <a class='btn btn-navbar' data-toggle='collapse' data-target='.nav-collapse'>"
        "	      <span class='icon-bar'></span>"
        "	      <span class='icon-bar'></span>"
        "	      <span class='icon-bar'></span>"
        "	    </a>"
        "	    <a class='brand' href='http://www.practwave.com.br/' title='PractWave' rel='homepage'>PractWave</a>"
        "	    <div class='nav'>"
        "		    <ul class='nav'>"
        "			<li>"
        "			   <a href='/index.html'>Home</a>"
        "			</li>"
        "			<li><a href='http://blog.practwave.com.br'>Blog</a></li>"
        "			<li><a href='/faq.html'>FAQ</a></li>"
        "			<li><a href='/privacidade.html'>Privacidade</a></li>"
        "			<li><a href='mailto:trumae@gmail.com'>Contato</a></li>"
        "		    </ul>"
        "	    </div>"
        "	  </div>"
        "	</div>"
        "  </div>"
        "</div><!-- end of #header -->"
        "<div class='row'>"
        "      <div class='span4 offset4 hero-unit'>"
        "      <h3>Ol&aacute; "
        + identity.name() + "! "
        " Devido &agrave; gratuidade e ao grande n&uacute;mero de novos usu&aacute;rios do PractWave estamos "
        " fazendo um controle maior dos nossos novos usu&aacute;rios. Voc&ecirc; ser&aacute; informado, "
        " atrav&eacute;s do email " + identity.email() + ", quando o uso do PractWave for liberado pra voc&ecirc;. "
        "      </h3></br></br>"
        "         <div class='well'>"
        "            <h4>Resgate seu cupom promocional:</h4></br>"
        "            <form class='form-inline'>${codigo}${btncodigo}</form>"
        "         </div>"
        "      </div>"
        "</div>", XHTMLUnsafeText);
    mensagem->bindWidget("btncodigo", btncodigo);
    mensagem->bindWidget("codigo", codigo);
}

void CabureWidgetPrincipal::createUI() {
    //CabureApplication *cabure = CabureApplication::cabureApplication();
    clear();

    //WCssDecorationStyle& decoration=cabure->root()->decorationStyle();
    //decoration.setBackgroundImage("http://br.bing.com/az/hprichbg?p=rb\%2fMidwestCardinal_ROW12109954041_1366x768.jpg");

    // modulos padrao
    TilePainel *tilePainel = new TilePainel();
    tilePainel->clicked().connect(this, &CabureWidgetPrincipal::painelApp);

    TileTimeline *tileTimeline = new TileTimeline();
    tileTimeline->clicked().connect(this, &CabureWidgetPrincipal::timelineApp);

    TileCliente *tileCliente = new TileCliente();
    tileCliente->clicked().connect(this, &CabureWidgetPrincipal::clienteApp);

    TileFornecedor *tileFornecedor = new TileFornecedor();
    tileFornecedor->clicked().connect(this, &CabureWidgetPrincipal::fornecedorApp);
   
    TileCalculadora *tileCalculadora = new TileCalculadora();
    tileCalculadora->clicked().connect(this, &CabureWidgetPrincipal::calculadoraApp);

    TileContaBancaria *tileContaBancaria = new TileContaBancaria();
    tileContaBancaria->clicked().connect(this, &CabureWidgetPrincipal::contaBancariaApp);
    
    TileCaixa *tileCaixa = new TileCaixa();
    tileCaixa->clicked().connect(this, &CabureWidgetPrincipal::caixaApp);

    WTemplate *tileLoja = new WTemplate();
    tileLoja->setTemplateText(
       "     <div class='tile icon bg-color-green'>"
       "       <div class='tile-content'>"
       "          <img class='icon' src='images/Market128.png'/>"
       "       </div>"
       "       <div class='brand'>"
       "          <span class='name'>Loja(n&atilde;o implementado)</span>"
       "       </div>"
       "    </div>"
    );
    
    WTemplate *tileMensagem = new WTemplate();
    tileMensagem->setTemplateText(
       "     <div class='tile icon bg-color-greenDark'>"
       "       <div class='tile-content'>"
       "          <img class='icon' src='images/Mail128.png'/>"
       "       </div>"
       "       <div class='brand'>"
       "          <span class='name'>Mensagem(n&atilde;o implementado)</span>"
       "          <span class='badge'>0</span>"
       "       </div>"
       "    </div>"
    );

    WTemplate *t = new WTemplate(this);
    t->setTemplateText(
     "<div class='page secondary'>"
     "  <div class='page-header'>"
     "    <div class='page-header-content'>"
     "      <h1>PractWave</h1>"
     "    </div>"
     "  </div>"
     "  <div class='page-region'>"
     "     <div class='page-region-content'>"
     "     ${painel}"
     "     ${timeline}"
     "     ${cliente}"
     "     ${fornecedor}"
     "     ${banco}"
     "     ${caixa}"
     "     ${loja}"
     "     ${mensagem}"
     "     ${calculadora}"
     "      </div>"
     //"       <h4>" + WString(cabure->name, UTF8) + "</h4>"
     "    </div>"
     "  </div>"
     "</div>"
     , Wt::XHTMLUnsafeText);

     t->bindWidget("painel", tilePainel);
     t->bindWidget("timeline", tileTimeline);
     t->bindWidget("cliente", tileCliente);
     t->bindWidget("fornecedor", tileFornecedor);
     t->bindWidget("banco", tileContaBancaria);
     t->bindWidget("caixa", tileCaixa);
     t->bindWidget("loja", tileLoja);
     t->bindWidget("mensagem", tileMensagem);
     t->bindWidget("calculadora", tileCalculadora);
}

void CabureWidgetPrincipal::viewHome(){
    createUI();
}

void CabureWidgetPrincipal::googleAnalyticsLogger(std::string url){
    doJavaScript("var pageTracker = _gat._getTracker('UA-34134276-1');"
                 "pageTracker._trackPageview('" + url + "');");
}

WContainerWidget* CabureWidgetPrincipal::showPainelApp() {
   googleAnalyticsLogger("/painel");
   clear();
   WContainerWidget *ret = new PainelApp(this);
   return ret;
}

void CabureWidgetPrincipal::painelApp(){
   showPainelApp();
}

WContainerWidget* CabureWidgetPrincipal::showTimelineApp() {
  googleAnalyticsLogger("/timeline");
  clear();
  WContainerWidget *ret = new TimelineApp(this);
  return ret;
}

void CabureWidgetPrincipal::timelineApp(){
   showTimelineApp();
}

WContainerWidget* CabureWidgetPrincipal::showClienteApp() {
  googleAnalyticsLogger("/cliente");
  clear();
  WContainerWidget *ret = new ClienteApp(this);
  return ret;
}

void CabureWidgetPrincipal::clienteApp(){
   showClienteApp();
}

WContainerWidget* CabureWidgetPrincipal::showFornecedorApp() {
  googleAnalyticsLogger("/fornecedor");
  clear();
  WContainerWidget *ret = new FornecedorApp(this);
  return ret;
}

void CabureWidgetPrincipal::fornecedorApp(){
   showFornecedorApp();
}

WContainerWidget* CabureWidgetPrincipal::showContaBancariaApp() {
  googleAnalyticsLogger("/banco");
  clear();
  WContainerWidget *ret = new ContaBancariaApp(this);
  return ret;
}

void CabureWidgetPrincipal::contaBancariaApp(){
   showContaBancariaApp();
}

WContainerWidget* CabureWidgetPrincipal::showCaixaApp() {
  googleAnalyticsLogger("/caixa");
  clear();
  WContainerWidget *ret = new CaixaApp(this);
  return ret;
}

void CabureWidgetPrincipal::caixaApp(){
   showCaixaApp();
}

WContainerWidget* CabureWidgetPrincipal::showCalculadoraApp() {
   googleAnalyticsLogger("/calculadora");
   clear();
   WContainerWidget *ret = new CalculadoraApp(this);
   return ret;
}

void CabureWidgetPrincipal::calculadoraApp(){
   showCalculadoraApp();
}

