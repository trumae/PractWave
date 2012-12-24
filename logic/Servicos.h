#ifndef __CABURE__SERVICOS__
#define __CABURE__SERVICOS__

#include <string>
#include <cppdb/frontend.h>
#include <vector>

#include "Contabilidade.h"
#include "Timeline.h"

class Servico {
public:
    Servico() {}
    Servico(const std::string pnome,
            const std::string pdescricao,
            int ppreco)
        : nome(pnome),
          descricao(pdescricao),
          preco(ppreco) {}

    int id;
    std::string nome;
    std::string descricao;
    int preco;
};

class Servicos {
public:
    Servicos(cppdb::session& d, Contabilidade *c, Timeline *timeline);
    void criaTabelas();

    bool adiciona(Servico &c);
    bool salvar(const Servico &c);

    Servico getServicoPorId(int i);
    void getTodosServicos(std::vector<Servico> &v);
    void getTodosServicosComLimite(std::vector<Servico> &v, int limite);
    void getServicosBuscaComLimite(std::vector<Servico> &v, std::string padrao, int limite);
private:
    cppdb::session db_;
    Contabilidade *contabilidade_;
    Timeline *timeline_;
};

#endif


