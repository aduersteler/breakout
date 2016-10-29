#include "stagescene.h"

static ALLEGRO_DISPLAY *display;
static ALLEGRO_BITMAP *backgroundImage = NULL;
static int displayWidth;
static int displayHeight;
static bool *doexit;
static Obstacle obstacles[157]; // todo: 157 need to be dynamic

void stagescene_init(ALLEGRO_DISPLAY *_display, bool *_doexit) {
  printf("stagescene_init\n");

  display = _display;
  doexit = _doexit;
  displayWidth = al_get_display_width(display);
  displayHeight = al_get_display_height(display);
  stagescene_loadFile("map1.txt");
  stagescene_initBackground();
}

void stagescene_destroy() {
  printf("stagescene_destroy\n");
  al_destroy_bitmap(backgroundImage);
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

void stagescene_initBackground() {
  al_set_new_bitmap_flags(ALLEGRO_MAG_LINEAR);
  char buffer[100];
  sprintf(buffer,"%s/%s",al_get_current_directory(),"background.png");

  backgroundImage = al_load_bitmap(buffer);
}

void stagescene_tick() {
  stagescene_drawBackground();
  stagescene_drawBricks();
  al_flip_display();
}

void stagescene_drawBackground() {
  
  // al_clear_to_color(al_map_rgb(0,0,0));

  al_draw_scaled_bitmap(
    backgroundImage,
    0, 
    0, 
    600, 
    375,
    0, 
    0, 
    displayWidth, 
    displayHeight, 
    0);

}

ALLEGRO_COLOR getBrickColor(Obstacle obstacle) {
  ALLEGRO_COLOR color;
  switch(obstacle.color) {
    case 'r':
      color = al_map_rgb(255,0,0);
      break;
    case 'y':
      color = al_map_rgb(255,255,0);
      break;
    case 'b':
      color = al_map_rgb(0,0,255);
      break;
    case 'g':
      color = al_map_rgb(0,255,0);
      break;
    default:
      color = al_map_rgb(144,144,144);
  }
  return color;
}

void stagescene_drawBrick(Obstacle obstacle) {

  ALLEGRO_COLOR color;

  color = getBrickColor(obstacle);

  int offsetX = 120*SCREEN_RATIO;
  int offsetY = 20*SCREEN_RATIO;
  int width = 50*SCREEN_RATIO;
  int height = 25*SCREEN_RATIO;
  int margin = 4*SCREEN_RATIO;
  int x = offsetX+(obstacle.x*(width+margin));
  int y = offsetY+(obstacle.y*(height+margin));
  
  // Background
  al_draw_filled_rectangle(x,y, x+width, y+height, color);
}

void stagescene_drawBricks() {
  for(int i = 0; i < (int)( sizeof(obstacles) / sizeof(obstacles[0]) ); i++) {
    stagescene_drawBrick(obstacles[i]);
  }
}

void stagescene_loadFile(char* filename) {
  char c;
  FILE *file;
  file = fopen("map1.txt", "r");
  int x = 0;
  int y = 0;
  int index = 1;


  if (file) {
    while ((c = getc(file)) != EOF) {
      if(c == 'r' || c == 'y' || c == 'g' || c == 'b') {
        obstacles[index].color = c;
        obstacles[index].x = x;
        obstacles[index].y = y;
        x++;
        index++;
      }
      else if('\n'){
        x=0;
        y++;
      }
      
      
      
    }
    fclose(file);

  }
}