#include "mainscene.h"

ALLEGRO_DISPLAY *display;
ALLEGRO_FONT *titleFont;
ALLEGRO_FONT *buttonFont;
ALLEGRO_BITMAP *backgroundImage = NULL;
int selected = 0;
bool *doexit;

struct Scene scenes[] = {
  {.text="New Game",.index=0,.scene=SCENE_GAME},
  {.text="Info",.index=1,.scene=SCENE_INFO},
  {.text="Quit",.index=2,.scene=SCENE_QUIT},
};

void mainscene_init(ALLEGRO_DISPLAY *_display, bool *_doexit) {
  display = _display;
  doexit = _doexit;

  mainscene_initBackground();
  mainscene_initTitle();
  mainscene_initNavigation();
}

void mainscene_destroy() {
  printf("mainscene_destroy\n");
  al_destroy_bitmap(backgroundImage);
  al_destroy_font(titleFont);
  al_destroy_font(buttonFont);
}

void mainscene_tick() {
  mainscene_drawBackground();
  mainscene_drawTitle();
  mainscene_drawNavigation();
  al_flip_display();
}

void mainscene_moveDown() {
  if(selected == 2) {
    selected=0;
  }else {
    selected++;
  }
  
}
void mainscene_moveUp() {
  if(selected == 0) {
    selected=2;
  }else {
    selected--;
  }
}

void mainscene_activateButton() {
  switch(scenes[selected].scene) {
    case SCENE_QUIT:
      scenecontroller_quit();
      break;
    case SCENE_INFO:
      scenecontroller_openScene(SCENE_INFO);
      break;
    case SCENE_GAME:
      scenecontroller_openScene(SCENE_GAME);
      break;
  }
}

void mainscene_handleEvents(ALLEGRO_EVENT ev) {

  if(ev.type == ALLEGRO_EVENT_KEY_UP) {
    switch(ev.keyboard.keycode) {
      case ALLEGRO_KEY_DOWN:
         mainscene_moveDown();
         break;
      case ALLEGRO_KEY_UP:
         mainscene_moveUp();
         break;
      case ALLEGRO_KEY_ENTER:
         mainscene_activateButton();
         break;
      case ALLEGRO_KEY_ESCAPE:
         scenecontroller_quit();
         break;
    }
  }
}

void mainscene_initBackground() {
  al_set_new_bitmap_flags(ALLEGRO_MAG_LINEAR);
  char buffer[100];
  sprintf(buffer,"%s/%s",al_get_current_directory(),"background.png");

  backgroundImage = al_load_bitmap(buffer);
}

void mainscene_initTitle() {
  char buffer[100];
  sprintf(buffer,"%s/%s",al_get_current_directory(),"Arkitech_Light.ttf");
  titleFont = al_load_font(buffer, 50*SCREEN_RATIO, 1);
}

void mainscene_initNavigation() {
  char buffer[100];
  sprintf(buffer,"%s/%s",al_get_current_directory(),"Arkitech_Light.ttf");
  buttonFont = al_load_font(buffer, 30*SCREEN_RATIO, 1);
}

void mainscene_drawBackground() {
  int displayWidth = al_get_display_width(display);
  int displayHeight = al_get_display_height(display);
  al_clear_to_color(al_map_rgb(0,0,0));

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

void mainscene_drawTitle() {
  ALLEGRO_COLOR color;
  int displayWidth = al_get_display_width(display);
  int middle = displayWidth/2;
  color = al_map_rgb(255, 255, 255);
  al_draw_text(titleFont, color, middle, 50*SCREEN_RATIO, 1, "Breakout");
}

void mainscene_drawButton(
  int rectX, 
  int rectY, 
  int rectWidth, 
  int rectHeight, 
  int ratio, 
  char* text, 
  bool active) {

  ALLEGRO_COLOR color;
  int displayWidth = al_get_display_width(display);
  int padding = 14*ratio;
  ALLEGRO_COLOR ButtonBackground;
  if(active) {
    ButtonBackground = al_map_rgb(180, 180, 180);
  } else {
    ButtonBackground = al_map_rgb(144, 144, 144);
  }
  rectX = (displayWidth-rectWidth)/2;

  // Background
  color = ButtonBackground;
  al_draw_filled_rectangle(rectX, rectY, rectX+rectWidth, rectHeight+rectY, color);

  // Border
  color = al_map_rgb(0, 255, 0);
  al_draw_rectangle(rectX, rectY, rectX+rectWidth, rectHeight+rectY, color,2.0);

  // Text
  color = al_map_rgb(255, 255, 255);
  al_draw_text(buttonFont, color, rectX+(rectWidth/2), rectY+padding, 1, text); 
}

void mainscene_drawNavigation() {
  int ratio = SCREEN_RATIO;
  int rectX = 0*ratio;
  int rectY = 200*SCREEN_RATIO;
  int rectWidth = 400*ratio;
  int rectHeight = 50*ratio;
  int buttonMargin = 10*ratio;

  for(int i=0; i < (sizeof scenes / sizeof scenes[0]); i++) {
    mainscene_drawButton(rectX,rectY,rectWidth,rectHeight,ratio,scenes[i].text,(scenes[i].index == selected));
    rectY+=rectHeight+buttonMargin;
  }
 
}