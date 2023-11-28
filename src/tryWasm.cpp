#include <SDL2/SDL.h>
#include <iostream>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

extern "C" {

  SDL_Window *window;
  SDL_Renderer *renderer;

  SDL_Rect rect = {.x = 10, .y = 10, .w = 150, .h = 100};

  void redraw() {
    SDL_SetRenderDrawColor(renderer, /* RGBA: black */ 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, /* RGBA: green */ 0x00, 0x80, 0x00, 0xFF);
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);
  }

  uint32_t ticksForNextKeyDown = 0;

  bool handle_events() {
    SDL_Event event;
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) {
      return false;
    }
    if (event.type == SDL_KEYDOWN) {
      uint32_t ticksNow = SDL_GetTicks();
      if (SDL_TICKS_PASSED(ticksNow, ticksForNextKeyDown)) {
        // Throttle keydown events for 10ms.
        ticksForNextKeyDown = ticksNow + 10;
        switch (event.key.keysym.sym) {
          case SDLK_UP:
            rect.y -= 10;
            break;
          case SDLK_DOWN:
            rect.y += 10;
            break;
          case SDLK_RIGHT:
            rect.x += 10;
            break;
          case SDLK_LEFT:
            rect.x -= 10;
            break;
        }
        redraw();
      }
    }
    return true;
  }

  void run_main_loop() {
  #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop([]() { handle_events(); }, 0, 0);
  #else
    while (handle_events())
      ;
  #endif
  }

  EMSCRIPTEN_KEEPALIVE void tryin() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_CreateWindowAndRenderer(500, 500, 0, &window, &renderer);
    SDL_SetWindowOpacity(window, 0.0f);

    redraw();
    run_main_loop();

    
  }

  // SDL_Window *window;
  // SDL_Renderer *renderer;
  // SDL_Surface *surface;

  // void drawRandomPixels() {
  //   if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);

  //   Uint8 * pixels = (Uint8*)surface->pixels;
    
  //   for (int i=0; i < 1048576; i++) {
  //       char randomByte = rand() % 255;
  //       pixels[i] = randomByte;
  //   }

  //   if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

  //   SDL_Texture *screenTexture = SDL_CreateTextureFromSurface(renderer, surface);

  //   SDL_RenderClear(renderer);
  //   SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
  //   SDL_RenderPresent(renderer);

  //   SDL_DestroyTexture(screenTexture);
  // }

  // EMSCRIPTEN_KEEPALIVE void tryin() {
  //   SDL_Init(SDL_INIT_VIDEO);
  //   SDL_CreateWindowAndRenderer(512, 512, 0, &window, &renderer);
  //   surface = SDL_CreateRGBSurface(0, 512, 512, 32, 0, 0, 0, 0);
    
  //   #ifdef __EMSCRIPTEN__
  //   emscripten_set_main_loop(drawRandomPixels, 0, 0);
  //   #else
  //   while(1) {        
  //       drawRandomPixels();
  //       SDL_Delay(16);
  //   }
  //   #endif 
  // }
}