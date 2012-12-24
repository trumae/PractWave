#include "Clientes.h"

#include<iostream>
#include<sstream>
#include<exception>

#include "../CabureApplication.h"
#include "ContasBancarias.h"
#include "Moeda.h"

ContasBancarias::ContasBancarias(cppdb::session& d,
                                 Contabilidade *c,
                                 Timeline *timeline)
    : db_(d),
      contabilidade_(c),
      timeline_(timeline) {
}


void ContasBancarias::criaTabelas() {
    try {
        db_ <<
            "create table if not exists contabancaria ("
            "   id integer  not null primary key autoincrement, "
            "   nome text not null, "
            "   agencia text, "
            "   numero text, "
            "   telefone text, "
            "   idconta integer not null ) " << cppdb::exec;
        db_ << "CREATE UNIQUE INDEX if not exists idx_nome_ContaBancaria_unique on contabancaria (nome)" << cppdb::exec;
    } catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

bool ContasBancarias::adiciona(ContaBancaria &c) {
    CabureApplication *cabure = CabureApplication::cabureApplication();
    db_.begin();
    Contabilidade *cont = cabure->contabilidade_;
    try {
        int idconta = cont->adicionaConta("BANCO " + c.nome,
                                          "DEVEDORA",
                                          cont->getIdPorNome("BANCOS"),
                                          0, "");
        cppdb::statement stat = db_ << "insert into contabancaria values (null,?,?,?,?,?)"
                                << c.nome << c.agencia << c.numero
                                << c.telefone << idconta << cppdb::exec;
        c.idconta = idconta;
        c.id = stat.last_insert_id();
        //adiciona entrada no timeline
        ItemTimeline itemTimeline("",
                                  "Adicionado o conta bancaria " + c.nome,
                                  "ContasBancarias::adiciona(ContaBancaria &)",
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

bool ContasBancarias::salvar(const ContaBancaria &c) {
    try {
        db_ << "update contabancaria set nome=?, "
            " agencia=?, numero=?, telefone=? "
            "where id=?"
            << c.nome << c.agencia << c.numero
            << c.telefone << c.id << cppdb::exec;
        ItemTimeline itemTimeline("",
                                  "O cadastro da conta bancaria '" + c.nome + "' foi editado" ,
                                  "ContasBancarias::adiciona(ContaBancaria &)",
                                  "");
        timeline_->adiciona(itemTimeline);
        return true;
    } catch(std::exception const &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return false;
    }
}

ContaBancaria ContasBancarias::getContaBancariaPorId(int i) {
    ContaBancaria c;

    cppdb::result res = db_ << "select id,nome,agencia,numero,telefone,"
                        "idconta from contabancaria where id = ?" << i << cppdb::row;
    if(res.empty()) {
        throw "Nao encontrei conta bancaria com id = " + i;
    } else {
        res >> c.id >> c.nome >> c.agencia >> c.numero
            >> c.telefone >> c.idconta;
    }
    return c;
}

ContaBancaria ContasBancarias::getContaBancariaPorNome(std::string n) {
    ContaBancaria c;

    cppdb::result res = db_ << "select id,nome,agencia,numero,telefone,"
                        "idconta from contabancaria where nome = ?"
                        << n << cppdb::row;
    if(res.empty()) {
        throw "Nao encontrei conta bancaria com nome = " + n;
    } else {
        res >> c.id >> c.nome >> c.agencia >> c.numero
            >> c.telefone >> c.idconta;
    }
    return c;
}

void ContasBancarias::getTodosContasBancarias(std::vector<ContaBancaria> &v) {
    v.clear();
    cppdb::result res = db_ << "select id,nome,agencia,numero,telefone, "
                        " idconta from contabancaria";
    while(res.next()) {
        ContaBancaria c;
        res >> c.id >> c.nome >> c.agencia
            >> c.numero >> c.telefone >> c.idconta;
        v.push_back(c);
    }
}

void ContasBancarias::getTodosContasBancariasComLimite(std::vector<ContaBancaria> &v, int limite) {
    v.clear();
    std::stringstream ss;
    ss << limite;
    std::string slimite = ss.str();
    cppdb::result res = db_ << "select id,nome,agencia,numero,telefone, "
                        " idconta from contabancaria limit " + slimite;
    while(res.next()) {
        ContaBancaria c;
        res >> c.id >> c.nome >> c.agencia
            >> c.numero >> c.telefone >> c.idconta;
        v.push_back(c);
    }
}

void ContasBancarias::getContasBancariasBuscaComLimite(
    std::vector<ContaBancaria> &v,
    std::string padrao, int limite) {
    v.clear();
    std::stringstream ss;
    ss << limite;
    std::string slimite = ss.str();
    cppdb::result res = db_ << "select id,nome,agencia,numero,telefone,"
                        " idconta from contabancaria "
                        " where nome like ?"
                        " limit " + slimite << "%" + padrao + "%";
    while(res.next()) {
        ContaBancaria c;
        res >> c.id >> c.nome >> c.agencia
            >> c.numero >> c.telefone >> c.idconta;
        v.push_back(c);
    }
}

void ContasBancarias::retiradaParaCentroCusto(
    std::string descr, int codcontabanco, int centro,long long valor) {
    contabilidade_->limpaLancamento();
    std::string nomebanco = contabilidade_->getNomePorId(codcontabanco);
    std::string nomecentro = contabilidade_->getNomePorId(centro);

    contabilidade_->setDescricaoLancamento("Retirada para '" +
                                           nomecentro + "'");
    contabilidade_->adicionaDebito(centro, valor, "");
    contabilidade_->adicionaCredito(codcontabanco, valor, "");
    contabilidade_->lanca();

    Moeda mval(valor);
    ItemTimeline itemTimeline("",
                              "Retirada de '" + nomebanco + "' para '"
                              + nomecentro + "' de R$ " +
                              mval.valStr() + " - " + descr,
                              "ContasBancarias::retiradaParaCentroCusto()",
                              "");
    timeline_->adiciona(itemTimeline);
}

void ContasBancarias::retiradaParaCaixa(
    std::string descr, int codcontabanco,long long valor) {
    contabilidade_->limpaLancamento();
    std::string nomebanco = contabilidade_->getNomePorId(codcontabanco);

    Moeda mval(valor);
    contabilidade_->setDescricaoLancamento(
        "Retirada para caixa - " + descr);
    contabilidade_->adicionaDebito(
        contabilidade_->getIdPorNome("CAIXA"), valor, "");
    contabilidade_->adicionaCredito(codcontabanco, valor, "");
    contabilidade_->lanca();

    ItemTimeline itemTimeline("",
                              "Retirada de '" + nomebanco +
                              "' para CAIXA de R$ " +
                              mval.valStr() + " - "
                              + descr,
                              "ContasBancarias::retiradaParaCaixa()",
                              "");
    timeline_->adiciona(itemTimeline);
}





