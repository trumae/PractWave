#include <sstream>
#include <Wt/WText>
#include <Wt/WTemplate>
#include <Wt/WBreak>
#include <cppdb/frontend.h>

#include "ContaWidget.h"
#include "../../CabureApplication.h"
#include "../../logic/Contabilidade.h"
#include "../../logic/Moeda.h"
#include "ItemContaWidget.h"

using namespace Wt;

ContaWidget::ContaWidget(WContainerWidget *parent, int codconta):
  WContainerWidget(parent), codconta_(codconta),
  cabDebito_("D&eacute;bito"), cabCredito_("Cr&eacute;dito"){
  viewHome();
}

void ContaWidget::viewHome(){
  CabureApplication *cabure = CabureApplication::cabureApplication();
  Contabilidade *contabilidade = cabure->contabilidade_;
  cppdb::session db = cabure->db_;

  long long saldo = contabilidade->getSaldoContaFolha(codconta_);
  Moeda msaldo(saldo);

  clear();
  WTemplate *cablistagem = new WTemplate(this);
  cablistagem->setTemplateText(
			       "<div class='grid'>"
			       "  <div class='row'>"
			       "     <div class='span6'>"
			       "        <h2>${nomeconta}</h2>"
			       "     </div>"
			       "     <div class='span4'>"
			       "        <h2>Saldo R$$ ${saldo}</h2>"
			       "     </div>"
			       "  </div>"

			       "  <div class='row'>"
			       "  <table class='striped hovered'>"
			       "   <thead>"
			       "     <th class='right'>Data</th>"
			       "     <th>Descri&ccedil;&atilde;o</th>"
			       "     <th class='right'>${cabDebito}</th>"
			       "     <th class='right'>${cabCredito}</th>"
			       "     <th></th>"
			       "   </thead>"
			       "   </tbody>", XHTMLUnsafeText);
  cablistagem->bindString("nomeconta",
			  WString(contabilidade->getNomePorId(codconta_), UTF8));
  cablistagem->bindString("saldo", msaldo.valStr());
  cablistagem->bindString("cabDebito", cabDebito_);
  cablistagem->bindString("cabCredito", cabCredito_);

  WContainerWidget *listagem = new WContainerWidget(this);
  cppdb::result res = db << 
    "select id_diario, data, descricao, debito, credito from "
    "diario, razonete where id_conta = ? and "
    "diario.id = id_diario and estornado = 0 "
    " order by data desc, id_diario desc"
			 << codconta_;
  while(res.next()){
    std::string data, descricao;
    long long debito, credito;
    int id_diario;

    res >> id_diario >> data >> descricao >> debito >> credito;
    Moeda deb(debito);
    Moeda cred(credito);
    new ItemContaWidget(listagem, this, id_diario, data, descricao, deb, cred);
  }
  WText *fimTabela = new WText("</tbody></table></div></div>", Wt::XHTMLUnsafeText, listagem);
}


