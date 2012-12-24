#ifndef __CABURE__TIMELINE__
#define __CABURE__TIMELINE__

#include <string>
#include <cppdb/frontend.h>
#include <vector>

#include "Contabilidade.h"

class ItemTimeline{
 public:
  ItemTimeline(){}
  ItemTimeline(const std::string ptimestamp,
	  const std::string pdescricao,
	  const std::string pmetodo,
	  const std::string pextra)
    : timestamp(ptimestamp),
    descricao(pdescricao),
    metodo(pmetodo),
    extra(pextra){}

  int id;
  std::string timestamp;
  std::string descricao;
  std::string metodo; 
  std::string extra;
};

class Timeline {
 public:
  Timeline(cppdb::session& d, Contabilidade *c);
  void criaTabelas();

  bool adiciona(ItemTimeline &c);

  ItemTimeline getTimelinePorId(int i);
  void getTodosTimeline(std::vector<ItemTimeline> &v);
  void getTodosTimelineComLimite(std::vector<ItemTimeline> &v, int limite);
 private:
  cppdb::session db_;
  Contabilidade *contabilidade_;
};

#endif


