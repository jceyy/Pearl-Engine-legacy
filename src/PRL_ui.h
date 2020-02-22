#ifndef PRL_UI_INCLUDED
#define PRL_UI_INCLUDED

#include <iostream>

#include "PRL_Display.h"
#include "PRL_Collision.h"
#include "PRL_text.h"

/// Classes to implement:
/**
	1. Check box
	2. Button
		a. Button2
		b. ButtonA2
		c. ButtonA3
	3. Drop-down list
	4. List box
	5. Progress bar
	6. Slider
		a. HSlider
		b. VSlider
	7. Spin control
	8. Tabs
	9. Text boxes
	10. Tips
	11. Tree view
	12. Menu
	13. Toolbar



	Priorities:
	1. Check box
	2. Button2
	3. Slider
	4. Ui panel
**/


// Temp class
class PRL_DisplayableCluster
{
public:
    PRL_DisplayableCluster();
    ~PRL_DisplayableCluster();

private:
    std::vector <PRL_Displayable*> dspList;
};


class PRL_UiPanel;


//! @brief Abstract class to create UI elements such as buttons, sliders, check boxes, ...
class PRL_UiElement
{
	friend class PRL_UiPanel;
public:
	PRL_UiElement();
	virtual ~PRL_UiElement() = 0;

	//! @brief Set the index of the element.
	void setIndex() noexcept;
	//! @brief Get the index of the element.
	int getIndex() const noexcept;
	//! @brief Get the panel the element is attached to.
	//! @details If the element is not attached to any UI panel, the function returns nullptr.
	PRL_UiPanel const* getPanel() const noexcept;
	//! @details Tell if the element is attached to a UI panel.
	//! @details If attached, the position of the element are expressed in local coordinates of the panel.
	bool isAttached() const noexcept;
	//! @brief Get how many UI elements are currently used.
	static int getCount() noexcept;

private:
	//! @brief Index of the UI element
	//! @details Used for instance to know the order of the elements when browsing in a menu with the arrows.
	int uiIndex;
	bool uiIsPanelAttached; //!< If the elements is attached to a panel.
	PRL_UiPanel* uiPanel; //!< Panel to which the element is attached.

	static int uiCount; //!< Count of UI elements in use.
};


//! @brief Class to create UI panels.
//! @details UI elements can be attached
class PRL_UiPanel : public PRL_Displayable
{
public:
	PRL_UiPanel();
	~PRL_UiPanel();

	//! @brief Get how many UI panels are currently used.
	static int getCount() noexcept;

private:
	std::vector <PRL_UiElement*> uiElement; //!< Vector containing the UI elements attached to the panel.

	SDL_Color uiBackColor;
	PRL_Point uiSize;

	static int uiCount; //!< Count of UI panels.
};



//! @brief Class designed to create check boxes
class PRL_CheckBox : public PRL_UiElement, PRL_Displayable, PRL_Collidable
{
public:
	PRL_CheckBox();
	~PRL_CheckBox();

	bool isChecked() const noexcept;
	void check() noexcept;
	void uncheck() noexcept;
	//! @brief Get how many check boxes are currently in use.
	static int getCount() noexcept;

private:
	SDL_Color cbCheckColor;
	SDL_Color cbBackColor;
	SDL_Color cbSideColor;
	int cbCheckWidth;
	int cbSideWidth;
	int cbSize;

	bool cbChecked;
	static int cbCount; //!< Count of check boxes currently in use.
};


class PRL_2Button : public PRL_UiElement, PRL_Displayable, PRL_Collidable
{
	friend class PRL_UiPanel;
};
#endif // PRL_UI_INCLUDED
