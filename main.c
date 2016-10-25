#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include "constants.h"
#include "scenes/mainscene.h"
#include "scenes/infoscene.h"
#include "scenes/stagescene.h"
#include "scenecontroller.h"
 
int main(int argc, char **argv){

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
 
    int screenRatio = SCREEN_RATIO;
    int screenWidth = SCREEN_WIDTH*screenRatio;
    int screenHeight = SCREEN_HEIGHT*screenRatio;

    bool doexit = false;
     
    if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
    }

    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
      fprintf(stderr, "failed to create timer!\n");
      return -1;
    }
 

    display = al_create_display(screenWidth, screenHeight);
    if(!display) {
      fprintf(stderr, "failed to create display!\n");
      return -1;
    }

    event_queue = al_create_event_queue();
    if(!event_queue) {
      fprintf(stderr, "failed to create event_queue!\n");
      al_destroy_display(display);
      return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));

    // addons
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_init_image_addon();

    // events
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_start_timer(timer);

    scenecontroller_init(display,&doexit);
    scenecontroller_openScene(SCENE_MAIN);

    while(!doexit)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_TIMER) {
            scenecontroller_tick();
        }
        else {
            scenecontroller_handleEvents(ev);
        }
    }

    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);

    return 0;
}