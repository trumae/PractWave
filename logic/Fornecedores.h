#ifndef __CABURE__FORNECEDORES__
#define __CABURE__FORNECEDORES__

#include <string>
#include <cppdb/frontend.h>
#include <vector>

#include "Contabilidade.h"
#include "Timeline.h"
#include "ContasBancarias.h"

class Fornecedor {
public:
    Fornecedor() {}
    Fornecedor(const std::string pnome,
               const std::string pendereco,
               const std::string pbairro,
               const std::string pcidade,
               const std::string pestado,
               const std::string pcep,
               const std::string ptelefone,
               const std::string pcelular,
               const std::string pemail,
               const std::string pobservacao)
        : nome(pnome),
          endereco(pendereco),
          bairro(pbairro),
          cidade(pcidade),
          estado(pestado),
          cep(pcep),
          telefone(ptelefone),
          celular(pcelular),
          email(pemail),
          observacao(pobservacao) {}

    int id;
    std::string nome;
    std::string endereco;
    std::string bairro;
    std::string cidade;
    std::string estado;
    std::string cep;
    std::string telefone;
    std::string celular;
    std::string email;
    std::string observacao;
    int idconta;
};

class Fornecedores {
public:
    Fornecedores(cppdb::session& d, Contabilidade *c, Timeline *timeline);
    void criaTabelas();

    bool adiciona(Fornecedor &c);
    bool salvar(const Fornecedor &c);

    Fornecedor getFornecedorPorId(int i);
    Fornecedor getFornecedorPorNome(std::string s);
    void getTodosFornecedores(std::vector<Fornecedor> &v);
    void getTodosFornecedoresComLimite(std::vector<Fornecedor> &v, int limite);
    void getFornecedoresBuscaComLimite(std::vector<Fornecedor> &v, std::string padrao, int limite);

    void comprar(int idconta,
                 std::string desc,
                 long long valor);
    void pagar(int idconta,
               std::string desc,
               long long valor,
			   int origemDinheiro,
			   ContaBancaria banco);
private:
    cppdb::session db_;
    Contabilidade *contabilidade_;
    Timeline *timeline_;
};

#endif


