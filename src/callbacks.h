/* $Id$ */
/* Copyright (c) 2010-2012 Pierre Pronchery <khorben@defora.org> */
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



#ifndef KEYBOARD_CALLBACKS_H
# define KEYBOARD_CALLBACKS_H

# include <gtk/gtk.h>
# include <Desktop.h>


/* public */
/* functions */
gboolean on_keyboard_delete_event(gpointer data);
void on_keyboard_embedded(gpointer data);
int on_keyboard_message(void * data, uint32_t value1, uint32_t value2,
		uint32_t value3);

void on_keyboard_set_layout_keypad(gpointer data);
void on_keyboard_set_layout_letters(gpointer data);
void on_keyboard_set_layout_special(gpointer data);

/* menus */
void on_file_quit(gpointer data);
void on_help_about(gpointer data);
void on_view_hide(gpointer data);

/* systray */
# if GTK_CHECK_VERSION(2, 10, 0)
void on_systray_activate(gpointer data);
void on_systray_popup_menu(GtkStatusIcon * icon, guint button, guint time,
		gpointer data);
# endif

#endif /* !KEYBOARD_CALLBACKS_H */
