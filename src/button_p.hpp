/*
 * button_p.hpp
 *
 *  Created on: Mar 25, 2016
 *      Author: kipr
 */

#ifndef BUTTON_P_HPP_
#define BUTTON_P_HPP_


#include "wallaby/button_ids.hpp"


namespace Private
{
	struct SharedButton;

	class Button
	{
	public:
		void setPressed(const ::Button::Type::Id &id, bool pressed);
		bool isPressed(const ::Button::Type::Id &id) const;

		void setExtraShown(const bool& shown);
		bool isExtraShown() const;

		static Button *instance();
	private:
		unsigned char buttonOffset(const ::Button::Type::Id &id) const;

		Button();
		Button(const Button& rhs);

	};
}

#endif /* BUTTON_P_HPP_ */
