#ifndef __INFOSCENE_H__

  #define __INFOSCENE_H__

  #include <stdio.h>
  #include <allegro5/allegro.h>
  #include <allegro5/allegro_primitives.h>
  #include <allegro5/allegro_font.h>
  #include <allegro5/allegro_ttf.h>
  #include "../constants.h"
  #include "../scenecontroller.h"

  void infoscene_init(ALLEGRO_DISPLAY *display, bool *_doexit);
  void infoscene_tick();
  void infoscene_initText();
  void infoscene_drawText();
  void infoscene_destroy();
  void infoscene_handleEvents(ALLEGRO_EVENT ev);
  
#endif
