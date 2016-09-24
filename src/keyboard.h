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



#ifndef KEYBOARD_KEYBOARD_H
# define KEYBOARD_KEYBOARD_H

# include <gtk/gtk.h>
# include "../include/Keyboard.h"
# include "key.h"


/* Keyboard */
/* types */
typedef struct _Keyboard Keyboard;

typedef enum _KeyboardMode
{
	KEYBOARD_MODE_WINDOWED = 0,
	KEYBOARD_MODE_DOCKED,
	KEYBOARD_MODE_EMBEDDED,
	KEYBOARD_MODE_POPUP,
	KEYBOARD_MODE_WIDGET
} KeyboardMode;

typedef struct _KeyboardPrefs
{
	int monitor;
	char const * font;
	KeyboardMode mode;
	int wait;
} KeyboardPrefs;


/* functions */
Keyboard * keyboard_new(KeyboardPrefs * prefs);
void keyboard_delete(Keyboard * keyboard);

/* accessors */
GtkWidget * keyboard_get_widget(Keyboard * keyboard);

gboolean keyboard_is_visible(Keyboard * keyboard);

/* XXX be more explicit */
void keyboard_set_layout(Keyboard * keyboard, unsigned int which);
void keyboard_set_page(Keyboard * keyboard, KeyboardPage page);

/* useful */
void keyboard_show(Keyboard * keyboard, gboolean show);
void keyboard_show_about(Keyboard * keyboard);

void keyboard_key_show(Keyboard * keyboard, KeyboardKey * key, gboolean show,
		GdkEventButton * event);

#endif /* !KEYBOARD_KEYBOARD_H */
