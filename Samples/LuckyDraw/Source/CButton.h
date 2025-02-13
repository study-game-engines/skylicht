#pragma once

#include <functional>

class CButton : public IEventReceiver
{
protected:
	std::string m_label;

	CGlyphFont *m_font;
	SFrame* m_frame;

	CGUIElement* m_element;
	CGUISprite* m_backround;
	CGUIText *m_text;

	SColor m_textColor;
	bool m_mouseHold;

public:
	std::function<void(void)> OnClick;

public:
	CButton(CGUIElement *element, SFrame* frame, const char *label, CGlyphFont *font, const SColor& textColor);

	CButton(CGUIElement *element, SFrame* frame);

	virtual ~CButton();

	virtual bool OnEvent(const SEvent& event);

	inline void setVisible(bool b)
	{
		m_element->setVisible(b);
	}

	inline bool isVisible()
	{
		return m_element->isVisible();
	}
};