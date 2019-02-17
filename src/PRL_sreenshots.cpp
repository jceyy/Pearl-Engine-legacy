/*#include <iostream>
#include <SDL2/SDL.h>

#include "PRL_global_variables.h"

void PRL_ExportScreenShot(SDL_Renderer *renderer)
{
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    SDL_Surface *screenshot=SDL_CreateRGBSurface(0, config_GLOBAL.resolution.x, config_GLOBAL.resolution.y, 32, 0, 0, 0, 0);

    SDL_RendererInfo infos;
    SDL_GetRendererInfo(renderer, &infos);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGB888, screenshot->pixels, screenshot->pitch); //infos.texture_formats[0]

    char title[90]="screenshots/screenshot - ", temp[100]={0};
    const char ext[]=".bmp";

    time_t timestamp = time(NULL);
    struct tm * t = localtime(&timestamp);

    sprintf(temp, "%02u.%d.%04u", t->tm_mday, t->tm_mon+1, 1900 + t->tm_year);
    strcat(title, temp);
    sprintf(temp, " at %02uh%02umin%02us ", t->tm_hour, t->tm_min, t->tm_sec);
    strcat(title, temp);
    bool done=false;
    FILE *file=NULL;
    int i;

    for (i=1; !done; i++)
    {
        char nbStr[5];
        PRL_NumberToCharString((unsigned int)i, nbStr);

        strcpy(temp, title);
        strcat(temp, nbStr);
        strcat(temp, ext);
        file=fopen(temp, "r");
        if (file==NULL)
        {
            SDL_SaveBMP(screenshot, temp);
            done=true;
        }
        else
            fclose(file);
    }
    SDL_FreeSurface(screenshot);
}
*/
