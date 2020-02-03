#ifndef PRL_TEXT_H_INCLUDED
#define PRL_TEXT_H_INCLUDED

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <thread>
#include "PRL_display.h"
//#include "PRL_input.h"
/*
#define PRL_DEFAULT_TEXT_COLOR {255, 255, 255, 255}
#define PRL_DEFAULT_TEXT_INPUT_BKGD_COLOR {255, 255, 255, 128}
#define PRL_HUD_MAX_BORDER_SIZE 200 // in pixels
#define PRL_HUD_SPACE_BETWEEN_BORDER_AND_TEXT 5 // in pixels

/// Destroy and fill the provided texture with the appropriate rectangle background with a border.
SDL_Texture* PRL_CreateRectangle(SDL_Renderer *renderer, int w, int h, SDL_Color const& bkgd, int borderSz, SDL_Color const& border);
/// Destroy and fill the provided texture with the appropriate rectangle background without border.
SDL_Texture* PRL_CreateRectangle(SDL_Renderer *renderer, int w, int h, SDL_Color const& bkgd);
*/

//! @brief Specify which kind of text rendering is desired.
//! @details This enumeration's members are directly linked to SDL_TTF rendering functions.
enum PRL_TextRenderStyle
{
    PRL_TEXTSTYLE_SOLID, PRL_TEXTSTYLE_BLENDED, PRL_TEXTSTYLE_DEFAULT = PRL_TEXTSTYLE_BLENDED
};

//! @brief This class is a container for a TTF_font, simplifying its use.
class PRL_Font
{
	friend class PRL_TextLabel;
public:
	/// Load a font specifying the path to the .ttf file and the point size (at 72 DPI).
	PRL_Font(std::string const& path, int size);
	//! @brief Load a font specifying the path to the .ttf file, the point size (at 72 DPI), the style and the color.
	//! @details The style can be any combination of the flags TTF_STYLE_BOLD, TTF_STYLE_ITALIC, TTF_STYLE_UNDERLINE, TTF_STLE_STRIKETHROUGH, or simply TTF_STYLE_NORMAL.
	PRL_Font(std::string const& path, int size, int style, SDL_Color const& color);
	~PRL_Font();

	//! @brief Change the size (in points for 72 DPI) of the font.
	//! @details Note that this operation requires to close and reopen the TTF_Font pointer object. Creating several different fonts won't hurt the memory.
	void setSize(int size);
	//! @brief Set the font's style.
	//! @details The style can be any combination of the flags TTF_STYLE_BOLD, TTF_STYLE_ITALIC, TTF_STYLE_UNDERLINE, TTF_STLE_STRIKETHROUGH, or simply TTF_STYLE_NORMAL.
	void setStyle(int style);
	//! @brief Set the font's color.
	void setColor(SDL_Color const& color) noexcept;
	//! @brief Get total count of opened fonts.
	static int getCount() noexcept;

private:
	PRL_Font(const PRL_Font&); //!< Private copy constructor to prevent copying.
	PRL_Font& operator=(const PRL_Font&); //!< Private copy constructor to prevent copying.

	TTF_Font* font; //!< Pointer on the font.
	std::string path; //!< Path to the opened font, used to reopen when size is changed.
	SDL_Color color; //!< Color of the font.
	int style; //!< Style of the font.
	int size; //!< Size of the font, in points for 72 DPI.
	int modifIndex; //!< Counts the modifications (size, color, style) to the font.

	static int fontCount; //!< Total count of opened fonts.
};



class PRL_TextLabel : public PRL_Displayable
{
	friend class PRL_Handler;
public:
	PRL_TextLabel(std::string const& text, PRL_Font const& font);
	~PRL_TextLabel();

	void setText(std::string const& text) noexcept;
	void setFont(PRL_Font const& font);
	void setRenderStyle(int style) noexcept;

	std::string getText() const noexcept;
	bool hasChanged() noexcept;

	static int getCount() noexcept;

private:
    void renderText();

    std::string text;
	PRL_Font* font;
	int renderStyle;
	bool hasChanged_;
	int fontModifIndex;

	static int txtCount; //!< Total count of text labels.
};

/*enum PRL_TextRenderStyle
{
    PRL_TEXTSTYLE_SOLID, PRL_TEXTSTYLE_BLENDED, PRL_TEXTSTYLE_DEFAULT = PRL_TEXTSTYLE_BLENDED
};



class PRL_TextLabel : public PRL_Displayable
{
public:
    PRL_TextLabel();
    PRL_TextLabel(std::string txt);
    PRL_TextLabel(const char *txt);
    ~PRL_TextLabel();

    ///Prepare the font needed by providing the font path, font size (in pt for 72 DPI) and the line's max length (in pxls)
    int setFont(const char *fontPath, int fontSize);
    /// Set the input to use if displacement is allowed
    void setInput(PRL_Input *in);
    /// Set text as a string
    void setText(std::string txt);
    /// Set text as a C like char string
    void setText(const char *txt);
    /// Set whether to use the solid (PRL_TEXTRENDER_SOLID, rough) or blended (PRL_TEXTRENDER_BLENDED, smoothed) text rendering method
    void setTextRenderMethod(PRL_TextRenderMethod method);
    /// Set the text color. By default PRL_DEFAULT_TEXT_COLOR
    void setTextColor(SDL_Color color);
    /// Set font style between TTF_STYLE_BOLD, TTF_STYLE_ITALIC, TTF_STYLE_STRIKETHROUGH, TTF_STYLE_UNDERLINE and TTF_STYLE_NORMAL. By default TTF_STYLE_NORMAL
    void setFontStyle(int style);
    /// Set a background color and start using it
    void useBackground(SDL_Color bkgd);
    /// Set a background color with borders and start using it
    void useBackground(SDL_Color bkgd, SDL_Color border, int borderSize);
    /// Stop using a background for the text box
    void stopUsingBackground();
    /// Allow or not the text box to be moved around
    void allowDisplacement(bool b);
    /// Sets the default distance between the border and the text in pixels. By default PRL_HUD_SPACE_BETWEEN_BORDER_AND_TEXT
    void setSpaceBetweenBorderAndText(int pxl);
    /// Sets the reference renderer. This has to be done before loading the font or it may result in no effect
    void setReferenceRenderer(int x, int y);
    /// Render text and update position if displacement is allowed
    void update();

private:
    PRL_Input *input;
    TTF_Font *font;
    SDL_Color fontColor;
    SDL_Texture *texture, *bkgdtarget;
    SDL_Surface *surface;
    char *path; // file path of the font
    std::string text;
    int fontSz;
    int fontStyle;

    bool useBkgd; // use background or not
    bool allowDisplacement_, beingMoved;
    PRL_FPoint relativeClickDistance; // used when moving the text label

    SDL_Color bkgdColor, borderColor, bkgdTempColor;
    int borderSz, spaceBetweenBorderAndText;
    PRL_TextRenderMethod renderingMethod;

    PRL_Point referenceRenderer;

    #if PRL_MULTITHREADING == 1
    bool isFirstRenderingDone;
    std::thread t1;
    #endif // PRL_MULTITHREADING

    int loadFont(const char *fontPath);
    void setFontSize(int sz);
    void renderText();
    void renderText_surfaceManipulation();
};*/




/*
/// Simple line text box without visual cursor handling
class PRL_TextBoxSimple : public PRL_Displayable
{
public:
    PRL_TextBoxSimple();
    ~PRL_TextBoxSimple();

    ///Prepare the font needed by providing the font path, font size (in pt for 72 DPI) and the line's max length (in pxls)
    int setFont(const char *fontPath, int fontSize, int lineMaxLength);
    /// Set the input to use
    void setInput(PRL_Input *in);
    /// Set text as a string
    void setText(std::string txt);
    /// Set text as a C like char string
    void setText(const char *txt);
    /// Set whether to use the solid (PRL_TEXTRENDER_SOLID, rough) or blended (PRL_TEXTRENDER_BLENDED, smoothed) text rendering method
    void setTextRenderMethod(PRL_TextRenderMethod method);
    /// Set the text color. By default PRL_DEFAULT_TEXT_COLOR
    void setTextColor(SDL_Color color);
    /// When using a background, set the background color used on text input
    void setTextInputBkgColor(SDL_Color color);
    /// Set font style between TTF_STYLE_BOLD, TTF_STYLE_ITALIC, TTF_STYLE_STRIKETHROUGH, TTF_STYLE_UNDERLINE and TTF_STYLE_NORMAL. By default TTF_STYLE_NORMAL
    void setFontStyle(int style);
    /// Set the line max length in pixels
    void setLineMaxLength(int lenth);
    /// Set a background color and start using it
    void useBackground(SDL_Color bkgd);
    /// Set a background color with borders and start using it
    void useBackground(SDL_Color bkgd, SDL_Color border, int borderSize);
    /// Stop using a background for the text box
    void stopUsingBackground();
    /// Start tracking text input. NB: the pointer on a PRL_Input class has to be already provided (or use void startTextInputTracking(PRL_Input *in))
    void startTextInputTracking();
    /// Set the input class to use and start tracking text input
    void startTextInputTracking(PRL_Input *in);
    ///Stop tracking text input
    void stopTextInputTracking();
    /// Allow or not the text box to be moved around
    void allowDisplacement(bool b);
    /// Sets the default distance between the border and the text in pixels. By default PRL_HUD_SPACE_BETWEEN_BORDER_AND_TEXT
    void setSpaceBetweenBorderAndText(int pxl);
    /// Sets the reference renderer. This has to be done before loading the font or it may result in no effect
    void setReferenceRenderer(int x, int y);
    /// If tracking text input, update the text and render the text. Otherwise it just renders the text.
    void update();


private:
    PRL_Input *input;
    TTF_Font *font;
    SDL_Color fontColor;
    SDL_Texture *texture, *bkgdtarget;
    SDL_Surface *surface;
    char *path; // file path of the font
    std::string text;
    int fontSz;
    int fontStyle;
    int maxLineLength; // in pixels

    bool isTrackingInput, allowTracking;
    bool useBkgd; // use background or not
    bool allowDisplacement_, beingMoved;
    PRL_FPoint relativeClickDistance; // used when moving the text box

    SDL_Color bkgdColor, bkgdTextInputColor, borderColor, bkgdTempColor;
    int borderSz, spaceBetweenBorderAndText;
    PRL_TextRenderMethod renderingMethod;

    PRL_Point referenceRenderer;

    #if PRL_MULTITHREADING == 1
    bool isFirstRenderingDone;
    std::thread t1;
    #endif // PRL_MULTITHREADING

    int loadFont(const char *fontPath);
    void setFontSize(int sz);
    void renderText();
    void renderText_surfaceManipulation();
};

*/
/*
#define PRL_DEFAULT_LINE_SPACING 5 // in pixels
#define PRL_DEFAULT_SELECTED_TEXT_COLOR {220, 220, 220, 220}

class PRL_TextBox : public PRL_Displayable
{
public:
    PRL_TextBox();
    ~PRL_TextBox();

    ///Prepare the font needed by providing the font path, font size (in pt for 72 DPI) and the line's max length (in pxls)
    int setFont(const char *fontPath, int fontSize, int lineMaxLength, int maxLinesNb);
    /// Set the input to use
    void setInput(PRL_Input *in);
    /// Set text as a string
    void setText(std::string txt);
    /// Set text as a C like char string
    void setText(const char *txt);
    /// Set whether to use the solid (PRL_TEXTRENDER_SOLID, rough) or blended (PRL_TEXTRENDER_BLENDED, smoothed) text rendering method
    void setTextRenderMethod(PRL_TextRenderMethod method);
    /// Set the text color. By default PRL_DEFAULT_TEXT_COLOR
    void setTextColor(SDL_Color color);
    /// When using a background, set the background color used on text input
    void setTextInputBkgColor(SDL_Color color);
    /// Color behind the text when selected. By default PRL_DEFAULT_SELECTED_TEXT_COLOR
    void setSelectedTextColor(SDL_Color color);
    /// Set font style between TTF_STYLE_BOLD, TTF_STYLE_ITALIC, TTF_STYLE_STRIKETHROUGH, TTF_STYLE_UNDERLINE and TTF_STYLE_NORMAL. By default TTF_STYLE_NORMAL
    void setFontStyle(int style);

    void setLineMaxLength(int lenth);

    void setMaxLinesNb(int nb);
    /// Set a background color and start using it
    void useBackground(SDL_Color bkgd);
    /// Set a background color with borders and start using it
    void useBackground(SDL_Color bkgd, SDL_Color border, int borderSize);
    /// Stop using a background for the text box
    void stopUsingBackground();
    /// Start tracking text input. NB: the pointer on a PRL_Input class has to be already provided (or use void startTextInputTracking(PRL_Input *in))
    void startTextInputTracking();
    /// Set the input class to use and start tracking text input
    void startTextInputTracking(PRL_Input *in);
    ///Stop tracking text input
    void stopTextInputTracking();
    /// Allow or not the text box to be moved around
    void allowDisplacement(bool b);
    /// Sets the default distance between the border and the text in pixels. By default PRL_HUD_SPACE_BETWEEN_BORDER_AND_TEXT
    void setSpaceBetweenBorderAndText(int pxl);
    /// Sets the reference renderer. This has to be done before loading the font or it may result in no effect
    void setReferenceRenderer(int x, int y);
    /// If tracking text input, update the text and render the text. Otherwise it just renders the text.
    void update(); // TO BECOME INT TYPE


private:
    PRL_Input *input;
    TTF_Font *font;
    SDL_Color fontColor;
    SDL_Texture **texture, *bkgdtarget;
    SDL_Surface **surface; // dynamic array of surfaces to allow multi-threading
    bool *lineHasChanged;
    std::string *textLine;
    SDL_Surface *sampleText; // store a simple space in current
    char *path; // file path of the font
    std::string text, selectedText;
    int fontSz;
    int fontStyle;
    int maxLineLength; // in pixels
    int currentLine, linesNumber, maxLinesNumber, lineSpacing;

    bool isTrackingInput, allowTracking;
    bool useBkgd; // use background or not
    bool allowDisplacement_, beingMoved;
    PRL_FPoint relativeClickDistance; // used when moving the text box

    SDL_Color bkgdColor, bkgdTextInputColor, borderColor, bkgdTempColor, selectedColor;
    int borderSz, spaceBetweenBorderAndText;
    PRL_TextRenderMethod renderingMethod;

    PRL_Point referenceRenderer;

    bool isFirstRenderingDone; // no multi-threading define to reduce code complexity
    std::thread **t;

    int loadFont(const char *fontPath);
    void setFontSize(int sz);
    int renderText();
    void renderText_surfaceManipulation(int lineNb);
    void moveLastWordToNextLine(int workingLine);
};


*/


#endif // PRL_TEXT_H_INCLUDED
