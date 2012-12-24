#include "Fornecedores.h"

#include<iostream>
#include<sstream>
#include<exception>

#include "../CabureApplication.h"
#include "Moeda.h"

Fornecedores::Fornecedores(cppdb::session& d,
                           Contabilidade *c,
                           Timeline *timeline)
    : db_(d),
      contabilidade_(c),
      timeline_(timeline) {
}


void Fornecedores::criaTabelas() {
    try {
        db_ <<
            "create table if not exists fornecedor ("
            "   id integer  not null primary key autoincrement, "
            "   nome text not null, "
            "   endereco text, "
            "   bairro text, "
            "   cidade text, "
            "   estado text, "
            "   cep text, "
            "   telefone text, "
            "   celular text, "
            "   email text, "
            "   observacao text, "
            "   idconta integer not null ) " << cppdb::exec;

        db_ << "CREATE UNIQUE INDEX if not exists idx_nome_Fornecedor_unique on cliente (nome)" << cppdb::exec;

    } catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

bool Fornecedores::adiciona(Fornecedor &c) {
    CabureApplication *cabure = CabureApplication::cabureApplication();
    db_.begin();
    Contabilidade *cont = cabure->contabilidade_;
    try {
        int idconta = cont->adicionaConta("FORNECEDOR " + c.nome,
                                          "CREDORA",
                                          cont->getIdPorNome("FORNECEDORES"),
                                          0, "");
        cppdb::statement stat = db_ << "insert into fornecedor values (null,?,?,?,?,?,?,?,?,?,?,?)"
                                << c.nome << c.endereco << c.bairro << c.cidade << c.estado << c.cep
                                << c.telefone << c.celular << c.email << c.observacao
                                << idconta << cppdb::exec;
        c.idconta = idconta;
        c.id = stat.last_insert_id();
        //adiciona entrada no timeline
        ItemTimeline itemTimeline("",
                                  "Adicionado o fornecedor " + c.nome,
                                  "Fornecedores::adiciona(Fornecedor &)",
                                  "");
        timeline_->adiciona(itemTimeline);
        db_.commit();
        return true;
    } catch (std::exception const &e) {
        db_.rollback();
        std::cerr << "ERROR: " << e.what() << std::endl;
        return false;
    }
}

bool Fornecedores::salvar(const Fornecedor &c) {
    try {
        db_ << "update fornecedor set nome=?, endereco=?, bairro=?, cidade=?, "
            "estado=?, cep=?, telefone=?, celular=?, email=?, observacao=?  where id=?"
            << c.nome << c.endereco << c.bairro << c.cidade << c.estado << c.cep
            << c.telefone << c.celular << c.email << c.observacao
            << c.id << cppdb::exec;
        ItemTimeline itemTimeline("",
                                  "O cadastro do fornecedor '" + c.nome + "' foi editado" ,
                                  "Fornecedores::adiciona(Fornecedor &)",
                                  "");
        timeline_->adiciona(itemTimeline);
        return true;
    } catch(std::exception const &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return false;
    }
}

Fornecedor Fornecedores::getFornecedorPorId(int i) {
    Fornecedor c;

    cppdb::result res = db_ << "select id, nome, endereco, bairro, cidade, estado, cep, telefone, "
                        "celular, email, observacao, idconta from fornecedor where id = ?" << i << cppdb::row;
    if(res.empty()) {
        throw "Nao encontrei fornecedor com id = " + i;
    } else {
        res >> c.id >> c.nome >> c.endereco >> c.bairro >> c.cidade >> c.estado
            >> c.cep >> c.telefone >> c.celular >> c.email >> c.observacao >> c.idconta;
    }
    return c;
}

Fornecedor Fornecedores::getFornecedorPorNome(std::string s) {
    Fornecedor c;

    cppdb::result res = db_ << "select id, nome, endereco, bairro, cidade, estado, cep, telefone, "
                        "celular, email, observacao, idconta from fornecedor where nome = ?" << s << cppdb::row;
    if(res.empty()) {
        throw "Nao encontrei fornecedor com id = " + s;
    } else {
        res >> c.id >> c.nome >> c.endereco >> c.bairro >> c.cidade >> c.estado
            >> c.cep >> c.telefone >> c.celular >> c.email >> c.observacao >> c.idconta;
    }
    return c;
}

void Fornecedores::getTodosFornecedores(std::vector<Fornecedor> &v) {
    v.clear();
    cppdb::result res = db_ << "select id, nome, endereco, bairro, cidade, estado, cep, telefone, "
                        "celular, email, observacao, idconta from fornecedor";
    while(res.next()) {
        Fornecedor c;
        res >> c.id >> c.nome >> c.endereco >> c.bairro >> c.cidade >> c.estado
            >> c.cep >> c.telefone >> c.celular >> c.email >> c.observacao >> c.idconta;
        v.push_back(c);
    }
}

void Fornecedores::getTodosFornecedoresComLimite(std::vector<Fornecedor> &v, int limite) {
    v.clear();
    std::stringstream ss;
    ss << limite;
    std::string slimite = ss.str();
    cppdb::result res = db_ << "select id, nome, endereco, bairro, cidade, estado, cep, telefone, "
                        "celular, email, observacao, idconta from fornecedor limit " + slimite;
    while(res.next()) {
        Fornecedor c;
        res >> c.id >> c.nome >> c.endereco >> c.bairro >> c.cidade >> c.estado
            >> c.cep >> c.telefone >> c.celular >> c.email >> c.observacao
            >> c.idconta;
        v.push_back(c);
    }
}

void Fornecedores::getFornecedoresBuscaComLimite(std::vector<Fornecedor> &v,
        std::string padrao, int limite) {
    v.clear();
    std::stringstream ss;
    ss << limite;
    std::string slimite = ss.str();
    cppdb::result res = db_ << "select id, nome, endereco, bairro, cidade, estado, cep, telefone, "
                        "celular, email, observacao, idconta from fornecedor "
                        " where nome like ?"
                        " limit " + slimite << "%" + padrao + "%";
    while(res.next()) {
        Fornecedor c;
        res >> c.id >> c.nome >> c.endereco >> c.bairro >> c.cidade >> c.estado
            >> c.cep >> c.telefone >> c.celular >> c.email >> c.observacao
            >> c.idconta;
        v.push_back(c);
    }
}


void Fornecedores::comprar(int idconta,
                           std::string desc,
                           long long valor) {
    contabilidade_->limpaLancamento();

    Moeda mval(valor);
    contabilidade_->setDescricaoLancamento(
        "Compra - " + desc);
    contabilidade_->adicionaCredito(idconta, valor, "");
    contabilidade_->adicionaDebito(
        contabilidade_->getIdPorNome("CUSTOS PRODUTOS VENDIDOS"),
        valor, "");
    contabilidade_->lanca();

    ItemTimeline itemTimeline("",
                              "Compra de "
                              + contabilidade_->getNomePorId(idconta) +
                              " de R$ " +
                              mval.valStr() + " - "
                              + desc,
                              "Fornecedores::comprar()",
                              "");
    timeline_->adiciona(itemTimeline);
}

void Fornecedores::pagar(int idconta,
                       std::string desc,
                       long long valor,
					   int origemDinheiro,
					   ContaBancaria banco) {
    contabilidade_->limpaLancamento();

    Moeda mval(valor);

	if(origemDinheiro == 1){
    	contabilidade_->setDescricaoLancamento(
        	"Pagamento - " + desc + " - Dinheiro do banco " + banco.nome);
    	contabilidade_->adicionaCredito(
        	banco.idconta, valor, "");
    	contabilidade_->adicionaDebito(
        	idconta,
        	valor, "");
    	contabilidade_->lanca();

    	ItemTimeline itemTimeline("",
                              "Pago para "
                              + contabilidade_->getNomePorId(idconta) +
                              " R$ " +
                              mval.valStr() + " - "
                              + desc + " - Dinheiro do banco " + banco.nome ,
                              "Fornecedores::pagar()",
                              "");
    	timeline_->adiciona(itemTimeline);
	}else{
    	contabilidade_->setDescricaoLancamento(
        	"Pagamento - " + desc + " - Dinheiro do Caixa");
    	contabilidade_->adicionaCredito(
        	contabilidade_->getIdPorNome("CAIXA"), valor, "");
    	contabilidade_->adicionaDebito(
        	idconta,
        	valor, "");
    	contabilidade_->lanca();

    	ItemTimeline itemTimeline("",
                              "Pago para "
                              + contabilidade_->getNomePorId(idconta) +
                              " R$ " +
                              mval.valStr() + " - "
                              + desc + " - Dinheiro do Caixa",
                              "Fornecedores::pagar()",
                              "");
    	timeline_->adiciona(itemTimeline);
	}
}


