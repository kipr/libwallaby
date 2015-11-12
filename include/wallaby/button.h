/*
 * button.h
 *
 *  Created on: Nov 12, 2015
 *      Author: Joshua Southerland
 */

#ifndef INCLUDE_WALLABY_BUTTON_H_
#define INCLUDE_WALLABY_BUTTON_H_


#ifdef __cplusplus
extern "C" {
#endif

void set_a_button_text(const char * text);

void set_b_button_text(const char * text);

void set_c_button_text(const char * text);

void set_x_button_text(const char * text);

void set_y_button_text(const char * text);

void set_z_button_text(const char * text);

int a_button();

int b_button();

int c_button();

int x_button();

int y_button();

int z_buton();

int side_button();

int black_button();

int a_button_clicked();

int b_button_clicked();

int c_button_clicked();

int x_button_clicked();

int y_button_clicked();

int z_button_clicked();

int side_button_clicked();

int any_button();

void extra_buttons_show();

void extra_buttons_hide();

int get_extra_buttons_visible();

void set_extra_buttons_visible(int visible);


#ifdef __cplusplus
}
#endif


#endif /* INCLUDE_WALLABY_BUTTON_H_ */
