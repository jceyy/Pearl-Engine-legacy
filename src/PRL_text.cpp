#include <iostream>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

//#include "PRL_math.h"
//#include "PRL_HUD.h"
//#include "PRL_types.h"
//#include "PRL_collision.h"
//#include "PRL_input.h"
//#include "PRL_global.h"
#include "PRL_configuration.h"
#include "PRL_text.h"

using namespace std;


/** ********************************************* **/
/**                  PRL_Font                     **/
/** ********************************************* **/

int PRL_Font :: fontCount = 0;

int PRL_Font :: getCount() noexcept
{
	return fontCount;
}

PRL_Font :: PRL_Font(string const& _path, int _size) : font(nullptr), style(TTF_STYLE_NORMAL), modifIndex(0)
{
	SDL_Color c = {0, 0, 0, 255};
	color = c;
	path = _path;
	size = _size;

	if (_size <= 0)
	{
		PRL_SetError("Font size can only be positive", true);
	}

    font = TTF_OpenFont(_path.c_str(), _size);

    if (font == nullptr)
	{
		PRL_SetError(TTF_GetError(), true);
	}
	fontCount++;
}

PRL_Font :: PRL_Font(std::string const& _path, int _size, int _style, SDL_Color const& _color) : modifIndex(0)
{
	path = _path;
	size = _size;
	style = _style;
	color = _color;

	if (_size <= 0)
	{
		PRL_SetError("Font size can only be positive", true);
	}

    font = TTF_OpenFont(_path.c_str(), _size);

    if (font == nullptr)
	{
		PRL_SetError(TTF_GetError(), true);
	}

    TTF_SetFontStyle(font, style);
    fontCount++;
}

PRL_Font :: ~PRL_Font()
{
	if (font != nullptr)
		TTF_CloseFont(font);
	fontCount--;
}

void PRL_Font :: setSize(int _size)
{
	if (size != _size && _size > 0)
	{
		size = _size;
		TTF_CloseFont(font);
		font = TTF_OpenFont(path.c_str(), _size);

		if (font == nullptr)
		{
			PRL_SetError(TTF_GetError(), true);
		}

		TTF_SetFontStyle(font, style);
		modifIndex++;
	}
}

void PRL_Font :: setStyle(int _style)
{
	if (style != _style)
	{
		style = _style;
		TTF_SetFontStyle(font, style);
		modifIndex++;
	}

}

void PRL_Font :: setColor(SDL_Color const& _color) noexcept
{
	if (color.r != _color.r || color.g != _color.g || color.b != _color.b || color.a != _color.a)
	{
		color = _color;
		modifIndex++;
	}
}

/** ********************************************* **/
/**               PRL_TextLabel                   **/
/** ********************************************* **/

int PRL_TextLabel :: txtCount = 0;

int PRL_TextLabel :: getCount() noexcept
{
	return txtCount;
}

PRL_TextLabel :: PRL_TextLabel(std::string const& text_, PRL_Font const& font_) :
	fontModifIndex(0)
{
	if (font_.font == nullptr)
		PRL_SetError("Invalid PRL_Font (member TTF_Font* is nullptr)", true);
	text = text_;
	font = (PRL_Font*) &font_;
	txtCount++;
}

PRL_TextLabel :: ~PRL_TextLabel()
{
	txtCount--;
}

void PRL_TextLabel :: setText(std::string const& text_) noexcept
{
	if (text != "" && text != text_)
	{
		text = text_;
		hasChanged_ = true;
	}
}

void PRL_TextLabel :: setFont(PRL_Font const& font_)
{
	if (font_.font == nullptr)
		PRL_SetError("Invalid PRL_Font (member TTF_Font* is nullptr)", true);
	else
	{
		font = (PRL_Font*) &font_;
		hasChanged_ = true;
	}
}

void PRL_TextLabel :: setRenderStyle(int style_) noexcept
{
	if (renderStyle != style_)
	{
		renderStyle = style_;
		hasChanged_ = true;
	}
}

std::string PRL_TextLabel :: getText() const noexcept
{
	return text;
}

bool PRL_TextLabel :: hasChanged() noexcept
{
	if (font->modifIndex != fontModifIndex)
	{ // if the font has changed compared to last update
		fontModifIndex = font->modifIndex;
		return true;
	}
	return hasChanged_; // if the textLabel has changed
}

void PRL_TextLabel :: renderText()
{
	TTF_Font* ttf_font = font->font;
	SDL_Color color = font->color;
	SDL_Surface *surface = nullptr;

	if (renderStyle == PRL_TEXTSTYLE_BLENDED)
		surface = TTF_RenderUTF8_Blended(ttf_font, text.c_str(), color);
	else
		surface = TTF_RenderUTF8_Solid(ttf_font, text.c_str(), color);

	if (surface == nullptr)
		PRL_SetError(string("Text rendering failed: ") + string(SDL_GetError()), true);

    SDL_Texture *texture = nullptr;
    texture = SDL_CreateTextureFromSurface(dspRenderer, surface);

    if (texture == nullptr)
		PRL_SetError(string("Text rendering failed: ") + string(SDL_GetError()), true);

	if (dspTexture[0] != nullptr)
		SDL_DestroyTexture(dspTexture[0]);

	dspTexture[0] = texture;
	dspDst[0].w = surface->w;
	dspDst[0].h = surface->h;

	SDL_FreeSurface(surface);
	hasChanged_ = false;
}

