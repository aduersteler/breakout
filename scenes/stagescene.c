#include "stagescene.h"

ALLEGRO_DISPLAY *display;
int displayWidth;
bool *doexit_ref;
Obstacle obstacles[200];

void stagescene_init(ALLEGRO_DISPLAY *_display, bool *_doexit) {
  printf("stagescene_init\n");

  display = _display;
  doexit_ref = _doexit;
  displayWidth = al_get_display_width(display);
  stagescene_loadFile("map1.txt");
  al_clear_to_color(al_map_rgb(255,0,0));
  stagescene_drawBricks();
  al_flip_display();
}

void stagescene_destroy() {
  printf("stagescene_destroy\n");
}

void stagescene_handleEvents(ALLEGRO_EVENT ev) {
  if(ev.type == ALLEGRO_EVENT_KEY_UP) {
    switch(ev.keyboard.keycode) {
      case ALLEGRO_KEY_ESCAPE:
        scenecontroller_closeCurrentScene();
        scenecontroller_openScene(SCENE_MAIN);
        break;
    }
  }
}

void stagescene_tick() {
  // al_clear_to_color(al_map_rgb(255,0,0));
  // stagescene_drawBricks();
  // al_flip_display();
}

void stagescene_drawBrick(Obstacle obstacle) {

  ALLEGRO_COLOR color;
  ALLEGRO_COLOR ButtonBackground = al_map_rgb(180, 180, 180);

  int offsetX = 20;
  int width = 40;
  int height = 40;
  int x = offsetX+(obstacle.x*50);
  int y = obstacle.y*50;
  

  // Background
  color = ButtonBackground;
  al_draw_filled_rectangle(x,y, x+width, y+height, color);

  // printf("x: %d, y: %d, w: %d, h: %d \n", obstacle.x, obstacle.y, 25, 5);
  // // Border
  // color = al_map_rgb(0, 255, 0);
  // al_draw_rectangle(rectX, rectY, rectX+rectWidth, rectHeight+rectY, color,2.0);
}

void stagescene_drawBricks() {
  for(int i = 0; i < (int)( sizeof(obstacles) / sizeof(obstacles[0]) ); i++) {
    stagescene_drawBrick(obstacles[i]);
  }
}

void stagescene_loadFile(char* filename) {
  int c;
  FILE *file;
  file = fopen("map1.txt", "r");
  int x = 0;
  int y = 0;
  int index = 0;


  if (file) {
    while ((c = getc(file)) != EOF) {
      obstacles[index].color = 1;
      obstacles[index].x = x;
      obstacles[index].y = y;
      x++;
      index++;
      if(c == '\n') {
        y++;
        x=0;
      }
      
    }
    fclose(file);

  }
}