/*
 * button_p.hpp
 *
 *  Created on: Nov 12, 2015
 *      Author: Joshua Southerland
 */

#ifndef SRC_BUTTON_P_HPP_
#define SRC_BUTTON_P_HPP_


#include "wallaby/button_ids.hpp"

#define MAX_BUTTON_TEXT_SIZE 16

namespace Private
{
	struct SharedButton;

	class Button
	{
	public:
		void setText(const ::Button::Type::Id & id, const char * text);
		bool isTextDirty(const ::Button::Type::Id & id) const;
		const char * text(const ::Button::Type::Id & id) const;
		void setPressed(const ::Button::Type::Id & id, bool pressed);
		bool isPressed(const ::Button::Type::Id & id) const;
		void resetButtons();

		void setExtraShown(bool shown);
		bool isExtraShown() const;

		static Button * instance();
	private:
		unsigned char buttonOffset(const ::Button::Type::Id & id) const;
		bool buttonRegs(unsigned short & start, unsigned short & end, ::Button::Type::Id id) const;

		Button();
		Button(const Button & rhs);

		mutable char m_text[6][MAX_BUTTON_TEXT_SIZE];
	};

}


#endif /* SRC_BUTTON_P_HPP_ */
