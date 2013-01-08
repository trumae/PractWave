#ifndef __TIMELINE_APP__
#define __TIMELINE_APP__

#include <Wt/WContainerWidget>
#include <string>

#include "../../logic/Timeline.h"
#include "../App.h"

class TimelineApp : public App {
 protected:
  int TIMELINE;

  virtual std::string getTitulo();
  virtual void init();
  Wt::WWidget *timeline();
 public:
  TimelineApp(Wt::WContainerWidget *parent);
};

class TileTimeline : public Wt::WContainerWidget {
  public:
    TileTimeline();
};

#endif
