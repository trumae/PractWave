#ifndef __CABURE_WIDGET_PRINCIPAL__
#define __CABURE_WIDGET_PRINCIPAL__

#include <Wt/WContainerWidget>
#include <Wt/Auth/AuthModel>
#include <Wt/Auth/AuthWidget>
#include <Wt/Auth/PasswordService>
#include <Wt/Auth/AuthService>
#include <Wt/Auth/GoogleService>
#include <Wt/Auth/FacebookService>
#include <Wt/WLineEdit>

#include "logic/Contabilidade.h"
#include "logic/Clientes.h"
#include "logic/ContasBancarias.h"
//#include "CabureMenu.h"

extern Wt::Auth::AuthService authServiceGoogle;
extern Wt::Auth::AuthService authServiceFacebook;
extern Wt::Auth::GoogleService *googleService;
extern Wt::Auth::FacebookService *facebookService;

class CabureWidgetPrincipal : public Wt::WContainerWidget {
public:
  CabureWidgetPrincipal(Wt::WContainerWidget *parent = 0);
  ~CabureWidgetPrincipal();
  
  void viewHome();
private:
  Wt::Auth::OAuthProcess* processGoogle_;
  Wt::Auth::OAuthProcess* processFacebook_;
  Wt::WLineEdit *codigo;

  Wt::WContainerWidget *content;

  void authorized(const Wt::Auth::OAuthAccessToken& token);
  void authenticatedGoogle(const Wt::Auth::Identity& identity);
  void authenticatedFacebook(const Wt::Auth::Identity& identity);
  void authenticated(const Wt::Auth::Identity& identity);
  void iniciaDBUser(std::string email);
  bool codigoOk(Wt::WString &s, int &promotor);
  void processaCupom();
  void informeSobreConvite(const Wt::Auth::Identity& identity);
  void createUI();
  void dadosContaUsuario();
  void googleAnalyticsLogger(std::string url);
  Wt::WContainerWidget* showPainelApp();
  void painelApp();
  Wt::WContainerWidget* showTimelineApp();
  void timelineApp();
  Wt::WContainerWidget* showClienteApp();
  void clienteApp();
  Wt::WContainerWidget* showFornecedorApp();
  void fornecedorApp();
  Wt::WContainerWidget* showContaBancariaApp();
  void contaBancariaApp();
  Wt::WContainerWidget* showCaixaApp();
  void caixaApp();
  Wt::WContainerWidget* showCalculadoraApp();
  void calculadoraApp();
};


#endif
