/*
 * button.h
 *
 *  Created on: Nov 12, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_BUTTON_H_
#define INCLUDE_WALLABY_BUTTON_H_


/*!
 * \file button.h
 * \copyright KISS Institute for Practical Robotics
 * \defgroup button Buttons
 */


#ifdef __cplusplus
extern "C" {
#endif


/*!
 * Updates the A button's text.
 * \param text The text to display. Limit of 16 characters.
 * \ingroup button
 * \deprecated not planned for the Wallaby
 */
void set_a_button_text(const char * text);

/*!
 * Updates the B button's text.
 * \param text The text to display. Limit of 16 characters.
 * \ingroup button
 * \deprecated not planned for the Wallaby
 */
void set_b_button_text(const char * text);

/*!
 * Updates the C button's text.
 * \param text The text to display. Limit of 16 characters.
 * \ingroup button
 * \deprecated not planned for the Wallaby
 */
void set_c_button_text(const char * text);

/*!
 * Updates the X button's text.
 * \param text The text to display. Limit of 16 characters.
 * \ingroup button
 * \deprecated not planned for the Wallaby
 */
void set_x_button_text(const char * text);

/*!
 * Updates the Y button's text.
 * \param text The text to display. Limit of 16 characters.
 * \ingroup button
 * \deprecated not planned for the Wallaby
 */
void set_y_button_text(const char * text);

/*!
 * Updates the Z button's text.
 * \param text The text to display. Limit of 16 characters.
 * \ingroup button
 * \deprecated not planned for the Wallaby
 */
void set_z_button_text(const char * text);

/*!
 * Gets the A button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \see a_button_clicked
 * \ingroup button
 * \note Not yet implemented
 */
int a_button();

/*!
 * Gets the B button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \see a_button_clicked
 * \ingroup button
 * \note Not yet implemented
 */
int b_button();

/*!
 * Gets the C button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \see a_button_clicked
 * \ingroup button
 * \note Not yet implemented
 */
int c_button();

/*!
 * Gets the C button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \see a_button_clicked
 * \ingroup button
 * \note Not yet implemented
 */
int x_button();

/*!
 * Gets the Y button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \see a_button_clicked
 * \ingroup button
 * \note Not yet implemented
 */
int y_button();

/*!
 * Gets the Z button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \see a_button_clicked
 * \ingroup button
 * \note Not yet implemented
 */
int z_button();

/*!
 * Gets the side button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \see a_button_clicked
 * \ingroup button
 * \deprecated Not present on the Wallaby
 */
int side_button();

/*!
 * Gets the black button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \see a_button_clicked
 * \ingroup button
 * \deprecated Not present on the Wallaby
 */
int black_button();


/*!
 * Gets the ousg button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \see a_button_clicked
 * \the only physical button on the Wombat
 */
int push_button();


/*!
 * Gets the left button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \see a_button_clicked
 * \ingroup button
 */
int left_button();

/*!
 * Gets the right button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed (But returning flipped)
 * \see a_button_clicked
 * \ingroup button
 */
int right_button();

/*!
 * Gets the A button's state (pressed or not pressed.)
 * \blocksuntil the button is no longer pressed.
 * \return 1 for pressed, 0 for not pressed
 * \see a_button
 * \ingroup button
 * \note Not yet implemented
 */
int a_button_clicked();

/*!
 * Gets the A button's state (pressed or not pressed.)
 * \blocksuntil the button is no longer pressed.
 * \return 1 for pressed, 0 for not pressed
 * \see a_button
 * \ingroup button
 * \note Not yet implemented
 */
int b_button_clicked();

/*!
 * Gets the B button's state (pressed or not pressed.)
 * \blocksuntil the button is no longer pressed.
 * \return 1 for pressed, 0 for not pressed
 * \see a_button
 * \ingroup button
 * \note Not yet implemented
 */
int c_button_clicked();

/*!
 * Gets the C button's state (pressed or not pressed.)
 * \blocksuntil the button is no longer pressed.
 * \return 1 for pressed, 0 for not pressed
 * \see a_button
 * \ingroup button
 * \note Not yet implemented
 */
int x_button_clicked();

/*!
 * Gets the X button's state (pressed or not pressed.)
 * \blocksuntil the button is no longer pressed.
 * \return 1 for pressed, 0 for not pressed
 * \see a_button
 * \ingroup button
 * \note Not yet implemented
 */
int y_button_clicked();

/*!
 * Gets the Y button's state (pressed or not pressed.)
 * \blocksuntil the button is no longer pressed.
 * \return 1 for pressed, 0 for not pressed
 * \see a_button
 * \ingroup button
 * \note Not yet implemented
 */
int z_button_clicked();

/*!
 * Gets the Z button's state (pressed or not pressed.)
 * \blocksuntil the button is no longer pressed.
 * \return 1 for pressed, 0 for not pressed
 * \see a_button
 * \ingroup button
 * \note Not yet implemented
 */
int side_button_clicked();

/*!
 * Determines whether or not any of the buttons are pressed.
 * \return 1 for pressed, 0 for not pressed
 * \ingroup button
 * \note Not yet implemented
 */
int any_button();

/*!
 * Shows the X, Y, and Z buttons.
 * \see set_extra_buttons_visible
 * \ingroup button
 * \note Not yet implemented
 */
void extra_buttons_show();


/*!
 * Hides the X, Y, and Z buttons. This is the default.
 * \see set_extra_buttons_visible
 * \ingroup button
 * \note Not yet implemented
 */
void extra_buttons_hide();

/*!
 * Determines whether or not the X, Y, and Z buttons are visible.
 * \see set_extra_buttons_visible
 * \ingroup button
 * \note Not yet implemented
 */
int get_extra_buttons_visible();

/*!
 * Sets whether or not the X, Y, and Z buttons are visible.
 * \see get_extra_buttons_visible
 * \ingroup button
 * \note Not yet implemented
 */
void set_extra_buttons_visible(int visible);


#ifdef __cplusplus
}
#endif


#endif /* INCLUDE_WALLABY_BUTTON_H_ */
