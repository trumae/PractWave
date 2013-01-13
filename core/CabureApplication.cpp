#include "CabureApplication.h"

using namespace Wt;

CabureApplication::CabureApplication (const WEnvironment& env)
    : WApplication(env), objetosIniciados(false) {}

CabureApplication::~CabureApplication() {
    if(objetosIniciados) {
        delete contabilidade_;
        delete clientes_;
        delete fornecedores_;
        delete contasBancarias_;
        delete timeline_;
        delete caixa_;
	delete meusDados_;
    }
    if(db_.is_open())
        db_.close();
}
