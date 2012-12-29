#ifndef __TIMELINE_APP__
#define __TIMELINE_APP__

#include <Wt/WContainerWidget>
#include <string>

#include "../../logic/Timeline.h"
#include "../App.h"

class TimelineApp : public App {
 protected:
  virtual Wt::WWidget *getConteudo();
  virtual std::string getTitulo();
 public:
  TimelineApp(Wt::WContainerWidget *parent);
};

class TileTimeline : public Wt::WContainerWidget {
  public:
    TileTimeline();
};

#endif
