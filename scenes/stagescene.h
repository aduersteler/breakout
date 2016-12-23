#ifndef __STAGESCENE_H__

  #define __STAGESCENE_H__

  #include <stdio.h>
  #include <allegro5/allegro.h>
  #include <allegro5/allegro_primitives.h>
  #include <allegro5/allegro_font.h>
  #include <allegro5/allegro_ttf.h>
  #include <allegro5/allegro_audio.h>
  #include <allegro5/allegro_acodec.h>
  #include "../constants.h"
  #include "../scenecontroller.h"

  typedef struct {
    int indexX;
    int indexY;
    int width;
    int height;
    double x;
    double y;
    bool enabled;
    char color;
  } Obstacle;

  typedef struct {
    double x;
    double y;
    int directionX;
    int width;
    int height;
  } Bar;

  typedef struct {
    double x;
    double y;
    int directionX;
    int directionY;
    int width;
    int height;
  } Ball;

  void stagescene_init(ALLEGRO_DISPLAY *_display, bool *_doexit);
  void stagescene_initBar();
  void stagescene_initBackground();
  void stagescene_initText();
  void stagescene_updateBar();
  void stagescene_playSound();
  void stagescene_drawGameOverText();
  void stagescene_updateBall();
  void stagescene_logic_tick();
  void stagescene_tick();
  void stagescene_loadFile(char* filename);
  void stagescene_drawBall();
  void stagescene_drawBackground();
  void stagescene_drawBar();
  void stagescene_drawBricks();
  void stagescene_drawBrick(Obstacle obstacle);
  void stagescene_destroy();
  void stagescene_handleEvents(ALLEGRO_EVENT ev);
  ALLEGRO_COLOR getBrickColor(Obstacle obstacle);

#endif