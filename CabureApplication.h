#ifndef __CABURE_APPLICATION__
#define __CABURE_APPLICATION__

#include <Wt/WApplication>
#include <string>
#include <cppdb/frontend.h>

#include "logic/Contabilidade.h"
#include "logic/Clientes.h"
#include "logic/Fornecedores.h"
#include "logic/ContasBancarias.h"
#include "logic/Timeline.h"
#include "logic/Caixa.h"
#include "logic/meusdados.h"

#include "CabureWidgetPrincipal.h"

class CabureApplication : public Wt::WApplication {
public:
  CabureApplication(const Wt::WEnvironment& env);
  ~CabureApplication();

  static CabureApplication* cabureApplication() {
    return (CabureApplication *) WApplication::instance();
  };

  enum TipoAuth {FACEBOOK, GOOGLE};
  TipoAuth tipoAuth;  

  std::string name;
  std::string email;
  std::string token;

  CabureWidgetPrincipal *principal_;
  cppdb::session db_;
  Contabilidade *contabilidade_;
  Clientes *clientes_;
  Fornecedores *fornecedores_;
  ContasBancarias *contasBancarias_;
  Timeline *timeline_;
  Caixa *caixa_;
  MeusDados *meusDados_;

  bool objetosIniciados;
};

#endif
