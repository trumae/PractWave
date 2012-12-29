#ifndef __TIMELINE_APP__
#define __TIMELINE_APP__

#include <Wt/WContainerWidget>
#include "../../logic/Timeline.h"

class TimelineApp : public Wt::WContainerWidget {
 public:
  TimelineApp(Wt::WContainerWidget *parent);
  void viewHome();
  void viewZero(){};
};

class TileTimeline : public Wt::WContainerWidget {
  public:
    TileTimeline();
};

#endif
