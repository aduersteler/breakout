/*
  Description:
  Scene Controller handles the events and scenes of the game
*/

#include "scenecontroller.h"

static int currentScene;
static ALLEGRO_DISPLAY *display = NULL;
static bool *doexit;

/*
  main function. called on main.c
*/
void scenecontroller_init(ALLEGRO_DISPLAY *_display, bool *_doexit) {
  display = _display;
  doexit = _doexit;
}

/*
  opens a defined scene
*/
void scenecontroller_openScene(int scene) {
  scenecontroller_closeCurrentScene();
  switch(scene) {
    case SCENE_MAIN:
        mainscene_init(display, doexit);
        break;
    case SCENE_GAME:
        gamescene_init(display, doexit);
        break;
    case SCENE_INFO:
        infoscene_init(display, doexit);
        break;
    }
    currentScene = scene;
}

/*
  sets the current scene to default and destroyes the current scene
*/
void scenecontroller_closeCurrentScene() {
  switch(currentScene) {
    case SCENE_MAIN:
      currentScene = 0;
      mainscene_destroy();
      break;
    case SCENE_INFO:
      currentScene = 0;
      infoscene_destroy();
      break;
    case SCENE_GAME:
      currentScene = 0;
      gamescene_destroy();
      break;
  }
}

/*
  closes the current scene and stops the main loop from main.c
*/
void scenecontroller_quit() {
  scenecontroller_closeCurrentScene();
  *doexit = true;
}

/*
  the forwarded events from main.c call this function and this
  forwards the events to the current scene event handle function
*/
void scenecontroller_handleEvents(ALLEGRO_EVENT ev) {
  switch(currentScene) {
    case SCENE_MAIN:
      mainscene_handleEvents(ev);
      break;
    case SCENE_GAME:
      gamescene_handleEvents(ev);
      break;
    case SCENE_INFO:
      infoscene_handleEvents(ev);
      break;
  }
}

/*
  called from main.c and forward the function to the current 
  scene tick function
*/
void scenecontroller_tick() {
  switch(currentScene) {
    case SCENE_MAIN:
      mainscene_tick();
      break;
    case SCENE_INFO:
      infoscene_tick();
      break;
    case SCENE_GAME:
      gamescene_tick();
      break;
  }
}