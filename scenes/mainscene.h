#ifndef __MAINSCENE_H__

  #define __MAINSCENE_H__

  #include <stdio.h>
  #include <string.h>
  #include <allegro5/allegro.h>
  #include <allegro5/allegro_primitives.h>
  #include <allegro5/allegro_font.h>
  #include <allegro5/allegro_ttf.h>
  #include "../constants.h"
  #include "../scenecontroller.h"

  struct Scene{
    char *text;
    int index;
    int scene;
  };

  void mainscene_init(ALLEGRO_DISPLAY *display,bool *_doexit);
  void mainscene_tick();
  void mainscene_destroy();
  void mainscene_handleEvents(ALLEGRO_EVENT ev);
  void mainscene_moveDown();
  void mainscene_moveUp();
  void mainscene_activateButton();
  void mainscene_drawButton(
    int rectX, 
    int rectY, 
    int rectWidth, 
    int rectHeight, 
    int ratio, 
    char* text, 
    bool active);

  void mainscene_initBackground();
  void mainscene_initTitle();
  void mainscene_initNavigation();

  void mainscene_drawNavigation();
  void mainscene_drawBackground();
  void mainscene_drawTitle();
#endif
