/*
  Game Scene. Contains the main game: moving ball, 
  movable bar (player) and obstacles (bricks).
*/

#include "gamescene.h"
#include <Math.h>

static ALLEGRO_DISPLAY *display;
static ALLEGRO_BITMAP *backgroundImage = NULL;
static int displayWidth;
static int displayHeight;
static int score = 0;

// doexit reference from main.c
static bool *doexit;

// obstacle bricks
static Obstacle obstacles[MAX_BRICKS]; // todo: 157 need to be dynamic

// player bar object
static Bar bar = {.width=100, .height=25, .directionX=0};

// moving ball object
static Ball ball = {.width=25, .height=25, .directionX=1, .directionY=1};

static ALLEGRO_SAMPLE *sample=NULL;
static ALLEGRO_FONT *gameOverFont;
static ALLEGRO_FONT *scoreFont;
static ALLEGRO_FONT *winFont;

// is gameover flag
static int gameOver = 0;

// is game win flag
static int gameWon = 0;

// disabled bricks count
static int disabledBricksCount = 0;

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
  char audioFilename[FILENAME_MAX];
  char mapFilename[FILENAME_MAX];

  sprintf(audioFilename,"%s/%s",al_get_current_directory(),"assets/audio.ogg");
  sprintf(mapFilename,"%s/%s",al_get_current_directory(),"maps/map2.txt");

  sample = al_load_sample(audioFilename);
  al_reserve_samples(1);
  gamescene_loadFile(mapFilename);

  gamescene_initBackground();
  gamescene_initBar();
  gamescene_initGameOverText();
  gamescene_initScore();
  gamescene_initWinText();

  ball.y = (displayHeight/SCREEN_RATIO)-150*SCREEN_RATIO;
  ball.x = bar.x;

  // init values
  gameOver = 0;
  gameWon = 0;
  disabledBricksCount = 0;

  score = 0;
  bar.directionX=0;
  ball.directionY=1;
  ball.directionX=1;
}

void gamescene_initScore() {
  char buffer[FILENAME_MAX];
  sprintf(buffer,"%s/%s",al_get_current_directory(),"assets/Arkitech_Light.ttf");
  scoreFont = al_load_font(buffer, 24*SCREEN_RATIO, 1);
}

void gamescene_initWinText() {
  char buffer[FILENAME_MAX];
  sprintf(buffer,"%s/%s",al_get_current_directory(),"assets/Arkitech_Light.ttf");
  winFont = al_load_font(buffer, 24*SCREEN_RATIO, 1);
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
  al_destroy_font(gameOverFont);
  al_destroy_font(scoreFont);
  al_destroy_font(winFont);
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
        if(gameOver || gameWon) {
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
  char buffer[FILENAME_MAX];
  sprintf(buffer,"%s/%s",al_get_current_directory(),"assets/background.png");

  backgroundImage = al_load_bitmap(buffer);
}

/*
  init text drawing objects
*/
void gamescene_initGameOverText() {
  char buffer[FILENAME_MAX];
  sprintf(buffer,"%s/%s",al_get_current_directory(),"assets/Arkitech_Light.ttf");
  gameOverFont = al_load_font(buffer, 24*SCREEN_RATIO, 1);
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
  al_draw_text(gameOverFont, color, middle, (0*margin+offset)*SCREEN_RATIO, 1, "Game Over");
}

/*
  draw Win Text
*/
void gamescene_drawWinText() {
  ALLEGRO_COLOR color;
  int middle = displayWidth/2;
  int offset = 100;
  int margin = 30;

  color = al_map_rgb(255, 255, 255);
  al_clear_to_color(al_map_rgb(0,0,0));
  al_draw_text(winFont, color, middle, (0*margin+offset)*SCREEN_RATIO, 1, "You Win!");
}

/*
  draw Score
*/
void gamescene_drawScore() {
  ALLEGRO_COLOR color;
  int middle = displayWidth/2;

  color = al_map_rgb(255, 255, 255);
  char scoreText[1024];
  sprintf(scoreText, "%d", score);
  al_draw_text(scoreFont, color, middle, (displayHeight/SCREEN_RATIO-40)*SCREEN_RATIO, 1, scoreText);
}

/*
  Called if brick has collision
*/
void onBrickCollision(Obstacle *brick) {
  
  gamescene_playSound();
  brick->enabled = false;
  disabledBricksCount++;

  switch(brick->color) {
    case 'y':
      score += 20;
      break;
    default:
      score += 5;
      break;
  }
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

  int bricksCount = (int)(sizeof(obstacles) / sizeof(obstacles[0]));

  // Obstacle Collision
  for(int i = 0; i < bricksCount; i++) {

    Obstacle *brick = &obstacles[i];
    int hasCollision = 0;
    
    if(brick->enabled) {
      // collision on bottom
      if( (ball.y <= brick->y+brick->height && ball.y >= brick->y && ball.x <= brick->x+brick->width && ball.x+ball.width >= brick->x) ) {
        ball.directionY = 1;
        hasCollision = 1;
      }

      // collision on top
      if( (ball.y+ball.height > brick->y && ball.y+ball.height < brick->y+brick->height && ball.x < brick->x+brick->width && ball.x+ball.width > brick->x) ) {
        ball.directionY = -1;
        hasCollision = 1;
      }

      // collision on left
      if( (ball.y <= brick->y+brick->height && ball.y >= brick->y && ball.x+ball.width >= brick->x && ball.x <= brick->x) ) {
        ball.directionX = -1;
        hasCollision = 1;
      }

      // collision on right
      if( (ball.y < brick->y+brick->height && ball.y > brick->y && ball.x < brick->x+brick->width && ball.x+ball.width > brick->x+brick->width) ) {
        ball.directionX = 1;
        hasCollision = 1;
      } 

      if(hasCollision) {
        onBrickCollision(brick);
      }
    }

    if(disabledBricksCount >= bricksCount) {
      gameWon = 1;
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


  
  if(!gameOver && !gameWon) {
    gamescene_updateBar();
    gamescene_updateBall();
  }
  
  gamescene_drawBackground();
  gamescene_drawBricks();
  gamescene_drawBar();
  gamescene_drawBall();
  gamescene_drawScore();

  if(gameOver) {
    gamescene_drawGameOverText();
  }

  if(gameWon) {
    gamescene_drawWinText();
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
  printf("%s\n",filename);
  char c;
  FILE *file;
  file = fopen(filename, "r");
  int indexX = 0;
  int indexY = 0;
  int index = 0;


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

        obstacles[index].enabled = 1;
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