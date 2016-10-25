#include "scenecontroller.h"

int currentScene;
ALLEGRO_DISPLAY *display = NULL;
bool *doexit;

void scenecontroller_init(ALLEGRO_DISPLAY *_display, bool *_doexit) {
  display = _display;
  doexit = _doexit;
}

void scenecontroller_openScene(int scene) {
  scenecontroller_closeCurrentScene();
  switch(scene) {
    case SCENE_MAIN:
        mainscene_init(display, doexit);
        break;
    case SCENE_GAME:
        stagescene_init(display, doexit);
        break;
    case SCENE_INFO:
        infoscene_init(display, doexit);
        break;
    }
    currentScene = scene;
}

void scenecontroller_closeCurrentScene() {
  currentScene = 0;
  switch(currentScene) {
    case SCENE_MAIN:
      mainscene_destroy();
      break;
    case SCENE_INFO:
      infoscene_destroy();
      break;
    case SCENE_GAME:
      stagescene_destroy();
      break;
  }
}

void scenecontroller_quit() {
  scenecontroller_closeCurrentScene();
  *doexit = true;
}

void scenecontroller_handleEvents(ALLEGRO_EVENT ev) {
  switch(currentScene) {
    case SCENE_MAIN:
      mainscene_handleEvents(ev);
      break;
    case SCENE_GAME:
      stagescene_handleEvents(ev);
      break;
    case SCENE_INFO:
      infoscene_handleEvents(ev);
      break;
  }
}

void scenecontroller_tick() {
  switch(currentScene) {
    case SCENE_MAIN:
      mainscene_tick();
      break;
    case SCENE_GAME:
      stagescene_tick();
      break;
  }
}