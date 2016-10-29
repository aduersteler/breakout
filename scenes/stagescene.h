#ifndef __STAGESCENE_H__

  #define __STAGESCENE_H__

  #include <stdio.h>
  #include <allegro5/allegro.h>
  #include <allegro5/allegro_primitives.h>
  #include <allegro5/allegro_font.h>
  #include <allegro5/allegro_ttf.h>
  #include "../constants.h"
  #include "../scenecontroller.h"

  typedef struct {
    int x;
    int y;
    char color;
  } Obstacle;

  void stagescene_init(ALLEGRO_DISPLAY *_display, bool *_doexit);
  void stagescene_initBackground();
  void stagescene_tick();
  void stagescene_loadFile(char* filename);
  void stagescene_drawBackground();
  void stagescene_drawBricks();
  void stagescene_drawBrick(Obstacle obstacle);
  void stagescene_destroy();
  void stagescene_handleEvents(ALLEGRO_EVENT ev);
  ALLEGRO_COLOR getBrickColor(Obstacle obstacle);

#endif