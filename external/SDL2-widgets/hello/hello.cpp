#include "../sdl-widgets.h"

TopWin *top_win;
Button *but;

int main(int,char**) {
printf("%s:%d",__FUNCTION__,__LINE__);
  top_win=new TopWin("Hello",Rect(100,100,120,100),0,0,false,
    []() {
      top_win->clear();
      draw_title_ttf->draw_string(top_win->render,"Hello world!",Point(20,40));
    }
  );
printf("%s:%d",__FUNCTION__,__LINE__);
  but=new Button(top_win,0,Rect(5,10,60,0),"catch me",
    [](Button *b) {
      static int dy=60;
      b->hide();
      b->move(0,dy);
      b->hidden=false;
      dy= dy==60 ? -60 : 60;
    });    
printf("%s:%d",__FUNCTION__,__LINE__);
  get_events();
printf("%s:%d",__FUNCTION__,__LINE__);
  return 0;
}
