#include "stagescene.h"
#include <Math.h>

static ALLEGRO_DISPLAY *display;
static ALLEGRO_BITMAP *backgroundImage = NULL;
static int displayWidth;
static int displayHeight;
static bool *doexit;
static Obstacle obstacles[157]; // todo: 157 need to be dynamic
static Bar bar = {.width=100, .height=25, .directionX=0};
static Ball ball = {.width=25, .height=25, .directionX=1, .directionY=1};
static ALLEGRO_SAMPLE *sample=NULL;
static ALLEGRO_FONT *textFont;
static int gameOver = 0;

void stagescene_init(ALLEGRO_DISPLAY *_display, bool *_doexit) {
  printf("stagescene_init\n");

  display = _display;
  doexit = _doexit;
  displayWidth = al_get_display_width(display);
  displayHeight = al_get_display_height(display);
  char buffer[200];
    
  sprintf(buffer,"%s/%s",al_get_current_directory(),"audio.ogg");

  stagescene_initText();
  sample = al_load_sample(buffer);
  al_reserve_samples(1);
  stagescene_loadFile("map1.txt");
  stagescene_initBackground();
  stagescene_initBar();

  ball.y = (displayHeight/SCREEN_RATIO)-150*SCREEN_RATIO;
  ball.x = bar.x;

  // init values
  gameOver = 0;
  bar.directionX=0;
  ball.directionY=1;
  ball.directionX=1;
}

void stagescene_initBar() {
  bar.x = 0;
  bar.y = (displayHeight/SCREEN_RATIO)-bar.height-50;
}

void stagescene_destroy() {
  printf("stagescene_destroy\n");
  al_destroy_bitmap(backgroundImage);
}

void stagescene_handleEvents(ALLEGRO_EVENT ev) {
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

void stagescene_initBackground() {
  al_set_new_bitmap_flags(ALLEGRO_MAG_LINEAR);
  char buffer[100];
  sprintf(buffer,"%s/%s",al_get_current_directory(),"background.png");

  backgroundImage = al_load_bitmap(buffer);
}

void stagescene_initText() {
  char buffer[100];
  sprintf(buffer,"%s/%s",al_get_current_directory(),"Arkitech_Light.ttf");
  textFont = al_load_font(buffer, 24*SCREEN_RATIO, 1);
}

void stagescene_drawBall() {
  ALLEGRO_COLOR color;

  color = al_map_rgb(255,255,255);

  int width = ball.width*SCREEN_RATIO;
  int height = ball.width*SCREEN_RATIO;
  int x = round(ball.x*SCREEN_RATIO);
  int y = round(ball.y*SCREEN_RATIO);
  
  // Background
  al_draw_filled_rectangle(x,y, x+width, y+height, color);
}

void stagescene_drawBar() {
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

void stagescene_updateBar() {
  double moveSpeed = ((1.0/FPS)*800);

  if(bar.x+moveSpeed*bar.directionX >= 0 && (bar.x+bar.width+moveSpeed)*bar.directionX <= displayWidth/SCREEN_RATIO) {
    bar.x = bar.x+moveSpeed*bar.directionX;
  }
}



void stagescene_drawGameOverText() {
  ALLEGRO_COLOR color;
  int middle = displayWidth/2;
  int offset = 100;
  int margin = 30;

  color = al_map_rgb(255, 255, 255);
  al_clear_to_color(al_map_rgb(0,0,0));
  al_draw_text(textFont, color, middle, (0*margin+offset)*SCREEN_RATIO, 1, "Game Over");
}

void stagescene_updateBall() {
  double moveSpeed = ((1.0/FPS)*300);

  // BoundingCollision

  // top bounding
  if(ball.y <= 0) {
    ball.directionY = 1;
    stagescene_playSound();
  }

  // bottom bounding
  if(ball.y >= (displayHeight/SCREEN_RATIO)-ball.height) {
    // ball.directionY = -1;
    gameOver = 1;
    
  }

  // left bounding
  if(ball.x <= 0) {
    ball.directionX = 1;
    stagescene_playSound();
  }

  //right bounding
  if(ball.x >= (displayWidth/SCREEN_RATIO)-ball.width) {
    ball.directionX = -1;
    stagescene_playSound();
  }

  // Bar Collision
  if(ball.y+ball.height >= bar.y && (ball.x+ball.width) >= bar.x && ball.x < (bar.x + bar.width)) {
    ball.directionY = -1;
    stagescene_playSound();
  }

  // Obstacle Collision
  for(int i = 0; i < (int)( sizeof(obstacles) / sizeof(obstacles[0]) ); i++) {

    Obstacle *brick = &obstacles[i];
    
    if(brick->enabled) {
      
      // collision on bottom
      if( (ball.y <= brick->y+brick->height && ball.y >= brick->y && ball.x <= brick->x+brick->width && ball.x+ball.width >= brick->x) ) {
        ball.directionY = 1;
        brick->enabled = false;
        stagescene_playSound();
      }

      // collision on top
      if( (ball.y+ball.height > brick->y && ball.y+ball.height < brick->y+brick->height && ball.x < brick->x+brick->width && ball.x+ball.width > brick->x) ) {
        ball.directionY = -1;
        brick->enabled = false;
        stagescene_playSound();
      }

      // collision on left
      if( (ball.y <= brick->y+brick->height && ball.y >= brick->y && ball.x+ball.width >= brick->x && ball.x <= brick->x) ) {
        ball.directionX = -1;
        brick->enabled = false;
        stagescene_playSound();
      }

      // collision on right
      if( (ball.y < brick->y+brick->height && ball.y > brick->y && ball.x < brick->x+brick->width && ball.x+ball.width > brick->x+brick->width) ) {
        ball.directionX = 1;
        brick->enabled = false;
        stagescene_playSound();
      } 
    }

  }

  ball.x = (double) ball.x+moveSpeed*ball.directionX;
  ball.y = (double) ball.y+moveSpeed*ball.directionY;
}

void stagescene_playSound() {
  al_play_sample(sample, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,0);
}

void stagescene_tick() {

  
  if(!gameOver) {
    stagescene_updateBar();
    stagescene_updateBall();
  }
  
  stagescene_drawBackground();
  stagescene_drawBricks();
  stagescene_drawBar();
  stagescene_drawBall();

  if(gameOver) {
    stagescene_drawGameOverText();
  } 

  al_flip_display();
}

void stagescene_drawBackground() {
  
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

void stagescene_drawBricks() {
  for(int i = 0; i < (int)( sizeof(obstacles) / sizeof(obstacles[0]) ); i++) {
    Obstacle * brick = &obstacles[i];

    if(brick->enabled) {
      stagescene_drawBrick(obstacles[i]);
    }
    
  }
}

void stagescene_loadFile(char* filename) {
  char c;
  FILE *file;
  file = fopen("map2.txt", "r");
  int indexX = 0;
  int indexY = 0;
  int index = 1;


  if (file) {
    while ((c = getc(file)) != EOF) {
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
      else if('\n'){
        indexX=0;
        indexY++;
      }
      
      
      
    }
    fclose(file);

  }
}