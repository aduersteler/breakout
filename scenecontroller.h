#ifndef __SCENECONTROLLER_H__

  #define __SCENECONTROLLER_H__

  #include <stdio.h>
  #include "constants.h"
  #include "scenes/mainscene.h"
  #include "scenes/infoscene.h"
  #include "scenes/gamescene.h"
  
  void scenecontroller_init(ALLEGRO_DISPLAY *_display, bool *_doexit);
  void scenecontroller_openScene(int scene);
  void scenecontroller_handleEvents(ALLEGRO_EVENT ev);
  void scenecontroller_tick();
  void scenecontroller_quit();
  void scenecontroller_closeCurrentScene();
  
#endif