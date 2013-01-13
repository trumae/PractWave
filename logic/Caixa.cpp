#include <string>
#include "Caixa.h"
#include "Contabilidade.h"
#include "Timeline.h"

#include "../core/CabureApplication.h"
#include "Moeda.h"

Caixa::Caixa(cppdb::session& d, Contabilidade *c, Timeline* t)
    : db_(d),
      contabilidade_(c),
      timeline_(t){
}

void Caixa::retirada(std::string descricao, int codcentro, long long valor) {
    int codcaixa = contabilidade_->getIdPorNome("CAIXA");
    contabilidade_->limpaLancamento();
    contabilidade_->setDescricaoLancamento(descricao);
    contabilidade_->adicionaDebito(
        codcentro,
        valor, "");
    contabilidade_->adicionaCredito(
        codcaixa,
        valor, "");
    contabilidade_->lanca();

    Moeda mval(valor);
    ItemTimeline itemTimeline("",
                              "O lan&ccedil;amento com descri&ccedil;&atilde;o '"
                              + descricao +
                              "' de R$ " + mval.valStr() +
							  " foi adicionado no CAIXA." ,
                              "CaixaWidget::retirada()",
                              "");
    timeline_->adiciona(itemTimeline);
}

void Caixa::retiradaDepositoBanco(std::string descr, 
				int codbanco, 
				long long valor){
    int codcaixa = contabilidade_->getIdPorNome("CAIXA");
    contabilidade_->limpaLancamento();
    std::string nomebanco = contabilidade_->getNomePorId(codbanco);

	Moeda mval(valor);
    contabilidade_->setDescricaoLancamento(
			       "Retirada para banco '" + nomebanco + 
				   "' de R$ " + mval.valStr() +
				   " - " + descr);
	contabilidade_->adicionaDebito(codbanco, valor, "");
	contabilidade_->adicionaCredito(codcaixa, valor, "");
	contabilidade_->lanca();

    ItemTimeline itemTimeline("",
                              "Retirada de CAIXA para '" + nomebanco + 
							  "' de R$ " + 
							  mval.valStr() + " - " 
							  + descr,
                              "Caixa::retiradaDepositoBanco()",
                              "");
    timeline_->adiciona(itemTimeline);
}


