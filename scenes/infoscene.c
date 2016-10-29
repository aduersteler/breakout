#include "infoscene.h"

static ALLEGRO_DISPLAY *display;
static ALLEGRO_FONT *textFont;
static int displayWidth;
static bool *doexit_ref;

void infoscene_init(ALLEGRO_DISPLAY *_display, bool *_doexit) {
  printf("infoscene_init\n");

  display = _display;
  doexit_ref = _doexit;
  displayWidth = al_get_display_width(display);

  infoscene_initText();
  infoscene_tick();
}

void infoscene_destroy() {
  printf("infoscene_destroy\n");
  
  al_destroy_font(textFont);
}

void infoscene_handleEvents(ALLEGRO_EVENT ev) {
  if(ev.type == ALLEGRO_EVENT_KEY_UP) {
    switch(ev.keyboard.keycode) {
      case ALLEGRO_KEY_ESCAPE:
        scenecontroller_closeCurrentScene();
        scenecontroller_openScene(SCENE_MAIN);
        break;
    }
  }
}

void infoscene_tick() {
  al_clear_to_color(al_map_rgb(0,0,0));
  infoscene_drawText();
  al_flip_display();
}

void infoscene_initText() {
  char buffer[100];
  sprintf(buffer,"%s/%s",al_get_current_directory(),"Arkitech_Light.ttf");
  textFont = al_load_font(buffer, 24*SCREEN_RATIO, 1);
}

void infoscene_drawText() {
  ALLEGRO_COLOR color;
  int middle = displayWidth/2;
  int offset = 100;
  int margin = 30;

  color = al_map_rgb(255, 255, 255);
  al_clear_to_color(al_map_rgb(0,0,0));
  al_draw_text(textFont, color, middle, (0*margin+offset)*SCREEN_RATIO, 1, "Breakout Game by");
  al_draw_text(textFont, color, middle, (1*margin+offset)*SCREEN_RATIO, 1, "Albrik Dürsteler & Erik Amgwerd");
  al_draw_text(textFont, color, middle, (2*margin+offset)*SCREEN_RATIO, 1, "2016");
}
