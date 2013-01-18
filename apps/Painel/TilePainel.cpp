#include <Wt/WTemplate>
#include <boost/lexical_cast.hpp>
#include <string>

#include "PainelApp.h"

using namespace Wt;

TilePainel::TilePainel() {
  CabureApplication *app = CabureApplication::cabureApplication();

  std::vector<Cliente> clientes;
  std::vector<Fornecedor> fornecedores;
  
  app->clientes_->getTodosClientes(clientes);
  app->fornecedores_->getTodosFornecedores(fornecedores);

  clear();
  WTemplate *tilePainel = new WTemplate(this);
  tilePainel->setTemplateText(std::string() + 
			      "<div class='tile double bg-color-blueDark' "
			      "     data-role='tile-slider' "
			      "     data-param-period='3000'>"
			      "  <div class='tile-content'>"
			      "     <h2>Painel</h2>"
			      "     <br><h4>Clientes Cadastrados: "
			      + boost::lexical_cast<std::string>(clientes.size()) +
			      "     </h4>"
			      "  </div>"
			      "  <div class='tile-content'>"
			      "     <h2>Painel</h2>"
			      "     <br><h4>Fornecedores Cadastrados: "
			      + boost::lexical_cast<std::string>(fornecedores.size()) +
			      "     </h4>"
			      "  </div>"
			      "</div>", Wt::XHTMLUnsafeText);
}




