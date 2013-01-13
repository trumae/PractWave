#include "Timeline.h"

#include<iostream>
#include<sstream>
#include<exception>

#include "../core/CabureApplication.h"

Timeline::Timeline(cppdb::session& d, Contabilidade *c)
  : db_(d), 
    contabilidade_(c){
}


void Timeline::criaTabelas(){
  try{
    db_ <<   
      "create table if not exists timeline ("
      "   id integer  not null primary key autoincrement, "
	  "   timestamp datetime, "
	  "   descricao text, "
	  "   metodo text, "
      "   extra integer) " << cppdb::exec;

    db_ << "CREATE INDEX if not exists idx_timestamp_timeline on timeline (timestamp)" << cppdb::exec;

  } catch(std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

bool Timeline::adiciona(ItemTimeline &c){
  try{
    cppdb::statement stat;
    if(c.timestamp.empty()){
       stat = 
			db_ << "insert into timeline values (null,date('now'),?,?,?)"
				<< c.descricao << c.metodo 
				<< c.extra << cppdb::exec;    
	} else {
       stat = 
			db_ << "insert into timeline values (null,?,?,?,?)"
				<< c.timestamp << c.descricao
			    << c.metodo << c.extra << cppdb::exec;    
	}
    c.id = stat.last_insert_id();
    return true;
  } catch (std::exception const &e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return false;
  }
}

ItemTimeline Timeline::getTimelinePorId(int i){
  ItemTimeline item;

  cppdb::result res = db_ << "select id, timestamp, descricao, metodo, extra "
    " from timeline where id = ?" << i << cppdb::row;
  if(res.empty()){
    throw "Nao encontrei Item no Timeline com id = " + i;
  } else {
    res >> item.timestamp >> item.descricao >> item.metodo
		>> item.extra;
  }
  return item;
}

void Timeline::getTodosTimeline(std::vector<ItemTimeline> &v){
  v.clear();
  cppdb::result res = db_ << "select id, timestamp, descricao, "
		  " metodo, extra "
          " from timeline";
  while(res.next()){
    ItemTimeline item;
    res >> item.id >> item.timestamp >> item.descricao
	    >> item.metodo >> item.extra;	
    v.push_back(item);
  }  
}

void Timeline::getTodosTimelineComLimite(std::vector<ItemTimeline> &v, 
				int limite){
  v.clear();
  std::stringstream ss;
  ss << limite;
  std::string slimite = ss.str();
  cppdb::result res = db_ << "select id, timestamp, descricao, "
    " metodo, extra from timeline order by timestamp desc limit " 
	+ slimite; 
  while(res.next()){
    ItemTimeline item;
    res >> item.id >> item.timestamp >> item.descricao
	    >> item.metodo >> item.extra;	
    v.push_back(item);
  } 
}




