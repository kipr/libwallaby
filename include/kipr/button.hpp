/*
 * button.hpp
 *
 *  Created on: Nov 12, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_BUTTON_HPP_
#define INCLUDE_WALLABY_BUTTON_HPP_

#include "sensor.hpp"
#include "button_ids.hpp"
#include "export.h"


class EXPORT_SYM AbstractButton : public Sensor<bool>
{
public:
	virtual ~AbstractButton();
	virtual void setPressed(bool pressed) = 0;

	inline bool isPressed() const { return value(); };

	inline bool isNotPressed() const { return !isPressed(); }

	inline bool isClicked() const
	{
		const bool ret = isPressed();
		waitUntilReleased();
		return ret;
	}

	virtual void waitUntilReleased() const;

	virtual void waitUntilPressed() const;

	virtual void waitUntilClicked() const;
};


class EXPORT_SYM AbstractTextButton : public AbstractButton
{
public:
	virtual ~AbstractTextButton();

	virtual void setText(const char * text) = 0;

	virtual const char * text() const = 0;

	virtual bool isTextDirty() const = 0;

	virtual void resetText() = 0;
};

class EXPORT_SYM IdButton : public AbstractTextButton
{
public:
	IdButton(const Button::Type::Id & id, const char * defaultText);
	~IdButton();

	virtual void setText(const char * text);
	virtual const char * text() const;
	virtual bool isTextDirty() const;
	virtual void setPressed(bool pressed);
	virtual bool value() const;
	virtual void resetText();
private:
	Button::Type::Id m_id;
	char * m_defaultText;
};

class EXPORT_SYM ExtraButtons
{
public:
	static void show();

	static void hide();

	static void setShown(bool shown);

	static bool isShown();
};

namespace Button
{
	extern EXPORT_SYM IdButton A;
	extern EXPORT_SYM IdButton B;
	extern EXPORT_SYM IdButton C;

	extern EXPORT_SYM IdButton X;
	extern EXPORT_SYM IdButton Y;
	extern EXPORT_SYM IdButton Z;

	extern EXPORT_SYM IdButton Left;
        extern EXPORT_SYM IdButton Right;

        extern EXPORT_SYM IdButton * const all[8];
};

#endif /* INCLUDE_WALLABY_BUTTON_HPP_ */
