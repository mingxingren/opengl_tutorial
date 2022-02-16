//
// Created by MMK on 2022/2/16.
//

#ifndef YUV_SHOW_RENDER_TEST_H
#define YUV_SHOW_RENDER_TEST_H

#include <SDL.h>

class CRenderTest {
public:
    CRenderTest(SDL_Window *win) {
        this->render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    }
    ~CRenderTest() {
        SDL_DestroyTexture(this->texture);
        SDL_DestroyRenderer(this->render);
    }

    void painter(int width, int height
                 , const Uint8 *Yplane, int Ypitch
                 , const Uint8 *Uplane, int Upitch
                 , const Uint8 *Vplane, int Vpitch) {
        if (this->texture == nullptr) {
            this->texture = SDL_CreateTexture(render,
                                              SDL_PIXELFORMAT_IYUV,
                                              SDL_TEXTUREACCESS_STREAMING,
                                              width,
                                              height);
        }

        SDL_Rect area;
        area.x = 0;
        area.y = 0;
        area.w = width;
        area.h = height;
        SDL_UpdateYUVTexture(this->texture, &area,
                             Yplane, Ypitch,
                             Uplane, Upitch,
                             Vplane, Vpitch);
        SDL_RenderCopy(this->render, this->texture, NULL, &area);
        SDL_RenderPresent(this->render);
    }

private:
    SDL_Renderer * render = nullptr;
    SDL_Texture * texture = nullptr;
};


#endif //YUV_SHOW_RENDER_TEST_H
