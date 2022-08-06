/*
 * button_p.cpp
 *
 *  Created on: Mar 25, 2016
 *      Author: kipr
 */


#include "button_p.hpp"
#include "wallaby_p.hpp"
#include "digital_p.hpp"
#include "wallaby_regs_p.hpp"

#include <cstring>
#include <cstdio>
#include <iostream>

#include <unistd.h>
#include <fcntl.h>

using namespace Private;

static const unsigned int L_BUTTON_GPIO = 54;
static const unsigned int GPIO_DIR_IN = 0;

void gpio_export(int gpio)
{
        char buff[256];
        int fd = open("/sys/class/gpio/export", O_WRONLY);
        sprintf(buff, "%d", gpio);
        write(fd, buff, strlen(buff));
        close(fd);
}

void gpio_direction(int gpio, int direction)
{
        char buff[256];
        sprintf(buff, "/sys/class/gpio/gpio%d/direction", gpio);
        int fd = open(buff, O_WRONLY);

        if (direction)
        {
                write(fd, "out", 3);
        }
        else
        {
                write(fd, "in", 2);
        }

        close(fd);
}

void gpio_set(int gpio, int value)
{
        if (value !=0) value = 1;

        char buff[256];
        sprintf(buff, "/sys/class/gpio/gpio%d/value", gpio);
        int fd = open(buff, O_WRONLY);
        sprintf(buff, "%d", value);
        write(fd, buff, 1);
        close(fd);
}

int gpio_get(int gpio)
{
        char buff[256];
        sprintf(buff, "/sys/class/gpio/gpio%d/value", gpio);
        int fd = open(buff, O_RDONLY);
        read(fd, buff, 1);
        close(fd);

        if (buff[0] == '0')
                return 0;

        return 1;
}


void Private::Button::setPressed(const ::Button::Type::Id &id, bool pressed)
{
	const unsigned char offset = buttonOffset(id);
	if(offset > 5) return;

	unsigned char states = Private::Wallaby::instance()->readRegister8b(REG_RW_BUTTONS);

	if(pressed) states |= (1 << offset);
	else states &= ~(1 << offset);

	Private::Wallaby::instance()->writeRegister8b(REG_RW_BUTTONS, states);
}


bool Private::Button::isPressed(const ::Button::Type::Id &id) const
{
        //Physical buttons:
        if (id == ::Button::Type::Left)
        {
                return (gpio_get(L_BUTTON_GPIO) == 0 ? true : false); // active low
        }

        if (id == ::Button::Type::Right)
        {
                bool val =  Private::digital_value(13, nullptr); //Because it is active high
                return !val;
        }

	// Virtual buttons:
	const unsigned char offset = buttonOffset(id);
	if (offset > 5) return false;

	unsigned char states = Private::Wallaby::instance()->readRegister8b(REG_RW_BUTTONS);
	return (states & 1 << offset) ? true : false;
}


void Private::Button::setExtraShown(const bool& shown)
{
	unsigned char states = Private::Wallaby::instance()->readRegister8b(REG_RW_BUTTONS);
	if (shown)
	{
		states |= 0b10000000;
	}
	else
	{
		states &= 0b01111111;
	}
	Private::Wallaby::instance()->writeRegister8b(REG_RW_BUTTONS, states);
}


bool Private::Button::isExtraShown() const
{
	unsigned char states = Private::Wallaby::instance()->readRegister8b(REG_RW_BUTTONS);
	return (states & 0b10000000) ? true : false;
}


Private::Button *Private::Button::instance()
{
	static Button s_button;
	return &s_button;
}


unsigned char Private::Button::buttonOffset(const ::Button::Type::Id &id) const
{
	return (unsigned char)id;
}


Private::Button::Button()
{
        gpio_export(L_BUTTON_GPIO);
        gpio_direction(L_BUTTON_GPIO, GPIO_DIR_IN);
}


Private::Button::Button(const Button& rhs)
{
}


