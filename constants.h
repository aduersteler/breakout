#ifndef __CONTSTANTS_H__

  #define __CONTSTANTS_H__
  #define SCREEN_WIDTH 1024
  #define SCREEN_HEIGHT 768
  #define SCREEN_RATIO 2
  #define FPS 60

  enum SceneType {
    SCENE_MAIN=1,
    SCENE_INFO=2,
    SCENE_GAME=3,
    SCENE_QUIT=4
  };

  #ifdef WINDOWS
      #include <direct.h>
      #define GetCurrentDir _getcwd
  #else
      #include <unistd.h>
      #define GetCurrentDir getcwd
  #endif

#endif