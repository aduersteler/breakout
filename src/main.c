/*
  Main Program. Starts the game loop.
*/

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "constants.h"
#include "scenes/mainscene.h"
#include "scenes/infoscene.h"
#include "scenes/gamescene.h"
#include "scenecontroller.h"
 
int main(int argc, char **argv){

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    
    int screenWidth = SCREEN_WIDTH*SCREEN_RATIO;
    int screenHeight = SCREEN_HEIGHT*SCREEN_RATIO;

    // exit game flag
    bool doexit = false;
     
    if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
    }


    // init timer
    timer = al_create_timer(1.0 / FPS);

    if(!timer) {
      fprintf(stderr, "failed to create timer!\n");
      return -1;
    }
 
    // init display
    display = al_create_display(screenWidth, screenHeight);
    if(!display) {
      fprintf(stderr, "failed to create display!\n");
      return -1;
    }

    al_set_new_display_option(ALLEGRO_VSYNC, ALLEGRO_REQUIRE, 0);

    // init events
    event_queue = al_create_event_queue();
    if(!event_queue) {
      fprintf(stderr, "failed to create event_queue!\n");
      al_destroy_display(display);
      return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));


    // init addons

    // init keyboard
    al_install_keyboard();
    if(!al_install_audio()){
      fprintf(stderr, "failed to initialize audio!\n");
      return -1;
    }

    // init audio codec
    if(!al_init_acodec_addon()) {
        return -1;
    }

    // init font
    al_init_font_addon();
    al_init_ttf_addon();

    // init primitives
    al_init_primitives_addon();

    // init image
    al_init_image_addon();

    // register events
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    // start timer
    al_start_timer(timer);

    scenecontroller_init(display,&doexit);
    scenecontroller_openScene(SCENE_MAIN);

    // start main loop
    while(!doexit)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_TIMER) {
            // call controller tick function
            scenecontroller_tick();
        }
        else {
            // forward event to controller function
            scenecontroller_handleEvents(ev);
        }
    }

    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);

    return 0;
}
