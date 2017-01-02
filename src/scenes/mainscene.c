/*
  Main Scene contains the main menu controlled by keyboard
*/

#include "mainscene.h"

static ALLEGRO_DISPLAY *display;
static ALLEGRO_FONT *titleFont;
static ALLEGRO_FONT *buttonFont;
static ALLEGRO_BITMAP *backgroundImage = NULL;

// current selected navigation entry
static int selected = 0;

// doexit reference from main.c
static bool *doexit;

// array of navigation entries
static Scene scenes[] = {
  {.text="New Game",.index=0,.scene=SCENE_GAME},
  {.text="Info",.index=1,.scene=SCENE_INFO},
  {.text="Quit",.index=2,.scene=SCENE_QUIT},
};

/*
  main function called by scenecontroller.c.
  prepares the required objects.
*/
void mainscene_init(ALLEGRO_DISPLAY *_display, bool *_doexit) {
  display = _display;
  doexit = _doexit;
  
  mainscene_initBackground();
  mainscene_initTitle();
  mainscene_initNavigation();
}

/*
  frees allegro objects memory
*/
void mainscene_destroy() {
  al_destroy_bitmap(backgroundImage);
  al_destroy_font(titleFont);
  al_destroy_font(buttonFont);
}

/*
  tick called by scenecontroller.c. redraw function.
*/
void mainscene_tick() {
  mainscene_drawBackground();
  mainscene_drawTitle();
  mainscene_drawNavigation();
  al_flip_display();
}

/*
  called on key pressed down. 
  increase navigation selection index.
*/
void mainscene_moveDown() {
  if(selected == 2) {
    selected=0;
  }else {
    selected++;
  }
  
}

/*
  called on key pressed up. 
  decrease navigation selection index.
*/
void mainscene_moveUp() {
  if(selected == 0) {
    selected=2;
  }else {
    selected--;
  }
}

/*
  called on key enter pressed. 
  open scene of selected scene
*/
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

/*
  gets forwarded events from scenecontroller.c. 
*/
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

/*
  init background drawing objects
*/
void mainscene_initBackground() {
  al_set_new_bitmap_flags(ALLEGRO_MAG_LINEAR);
  char buffer[FILENAME_MAX];
  sprintf(buffer,"%s/%s",al_get_current_directory(),"assets/background.png");

  backgroundImage = al_load_bitmap(buffer);
}

/*
  init title drawing objects
*/
void mainscene_initTitle() {
  char buffer[FILENAME_MAX];
  sprintf(buffer,"%s/%s",al_get_current_directory(),"assets/Arkitech_Light.ttf");
  titleFont = al_load_font(buffer, 50*SCREEN_RATIO, 1);
}

/*
  init navigation drawing objects
*/
void mainscene_initNavigation() {
  char buffer[FILENAME_MAX];
  sprintf(buffer,"%s/%s",al_get_current_directory(),"assets/Arkitech_Light.ttf");
  buttonFont = al_load_font(buffer, 30*SCREEN_RATIO, 1);
}

/*
  draw background with prepared draw objects
*/
void mainscene_drawBackground() {
  int displayWidth = al_get_display_width(display);
  int displayHeight = al_get_display_height(display);

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

/*
  draw title with prepared draw objects
*/
void mainscene_drawTitle() {
  ALLEGRO_COLOR color;
  int displayWidth = al_get_display_width(display);
  int middle = displayWidth/2;
  color = al_map_rgb(255, 255, 255);
  al_draw_text(titleFont, color, middle, 50*SCREEN_RATIO, 1, "Breakout");
}

/*
  draw a button
*/
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

/*
  draw navigation with buttons
*/
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