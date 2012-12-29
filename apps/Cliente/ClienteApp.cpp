#include <Wt/WTemplate>
#include <Wt/WPushButton>
#include <Wt/WAnchor>
#include <Wt/WImage>
#include <Wt/WText>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <vector>

#include "../../CabureApplication.h"
#include "../../logic/Clientes.h"
#include "../../logic/Fornecedores.h"
#include "../../logic/Contabilidade.h"
#include "../../logic/Moeda.h"
#include "../../logic/Timeline.h"
#include "ClienteApp.h"

using namespace Wt;
using namespace std;

ClienteApp::ClienteApp(WContainerWidget *parent)
    : App(parent) {
}

WWidget* ClienteApp::getConteudo() {
   return new WText("Teste"); 
}

string ClienteApp::getTitulo(){
   return "Clientes";
}
