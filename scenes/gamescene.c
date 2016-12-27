#include "gamescene.h"
#include <Math.h>

static ALLEGRO_DISPLAY *display;
static ALLEGRO_BITMAP *backgroundImage = NULL;
static int displayWidth;
static int displayHeight;

// doexit reference from main.c
static bool *doexit;

// obstacle bricks
static Obstacle obstacles[157]; // todo: 157 need to be dynamic

// player bar object
static Bar bar = {.width=100, .height=25, .directionX=0};

// moving ball object
static Ball ball = {.width=25, .height=25, .directionX=1, .directionY=1};

static ALLEGRO_SAMPLE *sample=NULL;
static ALLEGRO_FONT *textFont;

// is gameover flag
static int gameOver = 0;

/*
  main function called by scenecontroller.c.
  prepares the required objects.

  loads map file

  inits bar and ball position
*/
void gamescene_init(ALLEGRO_DISPLAY *_display, bool *_doexit) {
  display = _display;
  doexit = _doexit;
  displayWidth = al_get_display_width(display);
  displayHeight = al_get_display_height(display);
  char buffer[200];
    
  sprintf(buffer,"%s/%s",al_get_current_directory(),"audio.ogg");

  gamescene_initText();
  sample = al_load_sample(buffer);
  al_reserve_samples(1);
  gamescene_loadFile("map1.txt");
  gamescene_initBackground();
  gamescene_initBar();

  ball.y = (displayHeight/SCREEN_RATIO)-150*SCREEN_RATIO;
  ball.x = bar.x;

  // init values
  gameOver = 0;
  bar.directionX=0;
  ball.directionY=1;
  ball.directionX=1;
}

/*
  init bar position (center)
*/
void gamescene_initBar() {
  bar.x = 0;
  bar.y = (displayHeight/SCREEN_RATIO)-bar.height-50;
}

/*
  frees allegro objects memory
*/
void gamescene_destroy() {
  al_destroy_bitmap(backgroundImage);
  al_destroy_sample(sample);
  al_destroy_font(textFont);
}

/*
  gets forwarded events from scenecontroller.c. 
*/
void gamescene_handleEvents(ALLEGRO_EVENT ev) {
  if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
    switch(ev.keyboard.keycode) {
      
      case ALLEGRO_KEY_RIGHT:
        bar.directionX = 1;
        break;
      case ALLEGRO_KEY_LEFT:
        bar.directionX = -1;
        break;
      default:
        bar.directionX = 0;
        break;
    }
  }
  if(ev.type == ALLEGRO_EVENT_KEY_UP) {
    bar.directionX = 0;
    switch(ev.keyboard.keycode) {
      case ALLEGRO_KEY_ESCAPE:
        scenecontroller_closeCurrentScene();
        scenecontroller_openScene(SCENE_MAIN);
        break;
      case ALLEGRO_KEY_ENTER:
        if(gameOver) {
          scenecontroller_closeCurrentScene();
          scenecontroller_openScene(SCENE_MAIN);
        }
        break;
    }
  }
}

/*
  init background drawing objects
*/
void gamescene_initBackground() {
  al_set_new_bitmap_flags(ALLEGRO_MAG_LINEAR);
  char buffer[100];
  sprintf(buffer,"%s/%s",al_get_current_directory(),"background.png");

  backgroundImage = al_load_bitmap(buffer);
}

/*
  init text drawing objects
*/
void gamescene_initText() {
  char buffer[100];
  sprintf(buffer,"%s/%s",al_get_current_directory(),"Arkitech_Light.ttf");
  textFont = al_load_font(buffer, 24*SCREEN_RATIO, 1);
}

/*
  draw ball object
*/
void gamescene_drawBall() {
  ALLEGRO_COLOR color;

  color = al_map_rgb(255,255,255);

  int width = ball.width*SCREEN_RATIO;
  int height = ball.width*SCREEN_RATIO;
  int x = round(ball.x*SCREEN_RATIO);
  int y = round(ball.y*SCREEN_RATIO);
  
  // Background
  al_draw_filled_rectangle(x,y, x+width, y+height, color);
}

/*
  draw bar object
*/
void gamescene_drawBar() {
  ALLEGRO_COLOR color, colorDark, colorBright;

  color = al_map_rgb(255,0,0);
  colorDark = al_map_rgb(100,0,0);
  colorBright = al_map_rgb(255,255,255);

  int width = bar.width*SCREEN_RATIO;
  int height = bar.height*SCREEN_RATIO;
  int x = round(bar.x*SCREEN_RATIO);
  int y = round(bar.y*SCREEN_RATIO);
  
  // Background
  al_draw_filled_rectangle(x,y, x+width, y+height, color);
  al_draw_line(x,y+height,x+width,y+height,colorDark,1.0);
  al_draw_line(x+width,y,x+width,y+height,colorDark,1.0);

  al_draw_line(x,y,x+width,y,colorBright,1.0);
  al_draw_line(x,y,x,y+height,colorBright,1.0);
}

/*
  Update bar position by current speed and direction
*/
void gamescene_updateBar() {
  double moveSpeed = ((1.0/FPS)*800);

  if(bar.x+moveSpeed*bar.directionX >= 0 && (bar.x+bar.width+moveSpeed)*bar.directionX <= displayWidth/SCREEN_RATIO) {
    bar.x = bar.x+moveSpeed*bar.directionX;
  }
}

/*
  draw Game Over text
*/
void gamescene_drawGameOverText() {
  ALLEGRO_COLOR color;
  int middle = displayWidth/2;
  int offset = 100;
  int margin = 30;

  color = al_map_rgb(255, 255, 255);
  al_clear_to_color(al_map_rgb(0,0,0));
  al_draw_text(textFont, color, middle, (0*margin+offset)*SCREEN_RATIO, 1, "Game Over");
}

/*
  update ball position by current speed and direction
*/
void gamescene_updateBall() {
  double moveSpeed = ((1.0/FPS)*300);

  // BoundingCollision

  // top bounding
  if(ball.y <= 0) {
    ball.directionY = 1;
    gamescene_playSound();
  }

  // bottom bounding
  if(ball.y >= (displayHeight/SCREEN_RATIO)-ball.height) {
    // ball.directionY = -1;
    gameOver = 1;
    
  }

  // left bounding
  if(ball.x <= 0) {
    ball.directionX = 1;
    gamescene_playSound();
  }

  //right bounding
  if(ball.x >= (displayWidth/SCREEN_RATIO)-ball.width) {
    ball.directionX = -1;
    gamescene_playSound();
  }

  // Bar Collision
  if(ball.y+ball.height >= bar.y && (ball.x+ball.width) >= bar.x && ball.x < (bar.x + bar.width)) {
    ball.directionY = -1;
    gamescene_playSound();
  }

  // Obstacle Collision
  for(int i = 0; i < (int)( sizeof(obstacles) / sizeof(obstacles[0]) ); i++) {

    Obstacle *brick = &obstacles[i];
    
    if(brick->enabled) {
      
      // collision on bottom
      if( (ball.y <= brick->y+brick->height && ball.y >= brick->y && ball.x <= brick->x+brick->width && ball.x+ball.width >= brick->x) ) {
        ball.directionY = 1;
        brick->enabled = false;
        gamescene_playSound();
      }

      // collision on top
      if( (ball.y+ball.height > brick->y && ball.y+ball.height < brick->y+brick->height && ball.x < brick->x+brick->width && ball.x+ball.width > brick->x) ) {
        ball.directionY = -1;
        brick->enabled = false;
        gamescene_playSound();
      }

      // collision on left
      if( (ball.y <= brick->y+brick->height && ball.y >= brick->y && ball.x+ball.width >= brick->x && ball.x <= brick->x) ) {
        ball.directionX = -1;
        brick->enabled = false;
        gamescene_playSound();
      }

      // collision on right
      if( (ball.y < brick->y+brick->height && ball.y > brick->y && ball.x < brick->x+brick->width && ball.x+ball.width > brick->x+brick->width) ) {
        ball.directionX = 1;
        brick->enabled = false;
        gamescene_playSound();
      } 
    }

  }

  ball.x = (double) ball.x+moveSpeed*ball.directionX;
  ball.y = (double) ball.y+moveSpeed*ball.directionY;
}

/*
  play sample
*/
void gamescene_playSound() {
  al_play_sample(sample, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,0);
}

/*
  tick called by scenecontroller.c. redraw function.
*/
void gamescene_tick() {
  
  if(!gameOver) {
    gamescene_updateBar();
    gamescene_updateBall();
  }
  
  gamescene_drawBackground();
  gamescene_drawBricks();
  gamescene_drawBar();
  gamescene_drawBall();

  if(gameOver) {
    gamescene_drawGameOverText();
  } 

  al_flip_display();
}

/*
  draw background with prepared draw objects
*/
void gamescene_drawBackground() {
  
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

/*
  draw obstacle
*/
void gamescene_drawBrick(Obstacle obstacle) {

  ALLEGRO_COLOR color, colorDark, colorBright;

  color = getBrickColor(obstacle);
  colorDark = al_map_rgb(50,50,50);
  colorBright= al_map_rgb(255,255,255);

  int width = obstacle.width*SCREEN_RATIO;
  int height = obstacle.height*SCREEN_RATIO;
  double x = round(obstacle.x*SCREEN_RATIO);
  double y = round(obstacle.y*SCREEN_RATIO);
  
  
  // Background
  al_draw_filled_rectangle(x,y, x+width, y+height, color);


  al_draw_line(x,y+height,x+width,y+height,colorDark,1.0);
  al_draw_line(x+width,y,x+width,y+height,colorDark,1.0);

  al_draw_line(x,y,x+width,y,colorBright,1.0);
  al_draw_line(x,y,x,y+height,colorBright,1.0);
}

/*
  draw all obstacles
*/
void gamescene_drawBricks() {
  for(int i = 0; i < (int)( sizeof(obstacles) / sizeof(obstacles[0]) ); i++) {
    Obstacle * brick = &obstacles[i];

    if(brick->enabled) {
      gamescene_drawBrick(obstacles[i]);
    }
    
  }
}

/*
  load map file and save into obstacles array
*/
void gamescene_loadFile(char* filename) {
  char c;
  FILE *file;
  file = fopen("map2.txt", "r");
  int indexX = 0;
  int indexY = 0;
  int index = 1;


  if (file) {
    while ((c = getc(file)) != EOF) {

      // valid color? save the color
      if(c == 'r' || c == 'y' || c == 'g' || c == 'b') {

        double offsetX = 120;
        double offsetY = 20;
        int width = 50;
        int height = 25;
        int margin = 4;
        double x = offsetX+(indexX*(width+margin));
        double y = offsetY+(indexY*(height+margin));

        obstacles[index].enabled = true;
        obstacles[index].color = c;
        obstacles[index].indexX = indexX;
        obstacles[index].indexY = indexY;
        obstacles[index].width = width;
        obstacles[index].height = height;
        obstacles[index].x = x;
        obstacles[index].y = y;
        indexX++;
        index++;
      }
      // if return, make new row
      else if('\n'){
        indexX=0;
        indexY++;
      }
    }
    fclose(file);

  }
}