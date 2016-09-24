/* $Id$ */
/* Copyright (c) 2010-2016 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Desktop Keyboard */
/* All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */



#include <string.h>
#include "keyboard.h"
#include "callbacks.h"


/* public */
/* functions */
/* on_keyboard_delete_event */
gboolean on_keyboard_delete_event(gpointer data)
{
	Keyboard * keyboard = data;

	keyboard_show(keyboard, FALSE);
	return TRUE;
}


/* on_keyboard_embedded */
void on_keyboard_embedded(gpointer data)
{
	Keyboard * keyboard = data;

	keyboard_show(keyboard, TRUE);
}


/* on_keyboard_message */
int on_keyboard_message(void * data, uint32_t value1, uint32_t value2,
		uint32_t value3)
{
	Keyboard * keyboard = data;
	KeyboardMessage message = value1;

	switch(message)
	{
		case KEYBOARD_MESSAGE_SET_PAGE:
			keyboard_set_page(keyboard, value2);
			break;
		case KEYBOARD_MESSAGE_SET_VISIBLE:
			keyboard_show(keyboard, (value2 != 0) ? TRUE : FALSE);
			break;
	}
	return 0;
}


/* on_keyboard_set_layout_keypad */
void on_keyboard_set_layout_keypad(gpointer data)
{
	Keyboard * keyboard = data;

	keyboard_set_layout(keyboard, 1); /* XXX hard-coded */
}


/* on_keyboard_set_layout_letters */
void on_keyboard_set_layout_letters(gpointer data)
{
	Keyboard * keyboard = data;

	keyboard_set_layout(keyboard, 0); /* XXX hard-coded */
}


/* on_keyboard_set_layout_special */
void on_keyboard_set_layout_special(gpointer data)
{
	Keyboard * keyboard = data;

	keyboard_set_layout(keyboard, 2); /* XXX hard-coded */
}


/* menus */
/* on_file_quit */
void on_file_quit(gpointer data)
{
	Keyboard * keyboard = data;

	keyboard_show(keyboard, FALSE);
	gtk_main_quit();
}


/* on_help_about */
void on_help_about(gpointer data)
{
	Keyboard * keyboard = data;

	keyboard_show_about(keyboard);
}


/* on_view_hide */
void on_view_hide(gpointer data)
{
	Keyboard * keyboard = data;

	keyboard_show(keyboard, FALSE);
}


#if GTK_CHECK_VERSION(2, 10, 0)
/* systray */
/* on_systray_activate */
void on_systray_activate(gpointer data)
{
	Keyboard * keyboard = data;
	gboolean visible;

	/* toggle visibility */
	visible = keyboard_is_visible(keyboard);
	keyboard_show(keyboard, visible ? FALSE : TRUE);
}


/* on_systray_popup_menu */
void on_systray_popup_menu(GtkStatusIcon * icon, guint button, guint time,
		gpointer data)
{
	GtkWidget * menu;
	GtkWidget * menuitem;

	menu = gtk_menu_new();
	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);
	g_signal_connect(menuitem, "activate", G_CALLBACK(gtk_main_quit),
			NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	gtk_widget_show_all(menu);
	gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, button, time);
}
#endif
