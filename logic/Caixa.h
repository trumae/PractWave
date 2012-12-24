#ifndef __CAIXA__H__
#define __CAIXA__H__

#include <string>
#include <cppdb/frontend.h>
#include <vector>
#include "Contabilidade.h"
#include "Timeline.h"

class Caixa {
 public:
  Caixa(cppdb::session& d, Contabilidade *c, Timeline *t);

  void retirada(std::string descricao, int codcentro, long long valor);
  void retiradaDepositoBanco(std::string descr, 
				  int codbanco,
				  long long valor);
 private:
  cppdb::session db_;
  Contabilidade *contabilidade_;
  Timeline *timeline_;
};

#endif


