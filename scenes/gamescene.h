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

  void gamescene_init(ALLEGRO_DISPLAY *_display, bool *_doexit);
  void gamescene_initBar();
  void gamescene_initBackground();
  void gamescene_initGameOverText();
  void gamescene_initScore();
  void gamescene_initWinText();
  void gamescene_updateBar();
  void gamescene_playSound();
  void gamescene_drawGameOverText();
  void gamescene_drawWinText();
  void gamescene_updateBall();
  void onBrickCollision(Obstacle *brick);
  void gamescene_logic_tick();
  void gamescene_tick();
  void gamescene_loadFile(char* filename);
  void gamescene_drawBall();
  void gamescene_drawBackground();
  void gamescene_drawBar();
  void gamescene_drawBricks();
  void gamescene_drawBrick(Obstacle obstacle);
  void gamescene_destroy();
  void gamescene_handleEvents(ALLEGRO_EVENT ev);
  ALLEGRO_COLOR getBrickColor(Obstacle obstacle);

#endif