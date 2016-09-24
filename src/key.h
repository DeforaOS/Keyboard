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



#ifndef KEYBOARD_KEY_H
# define KEYBOARD_KEY_H

# include <gtk/gtk.h>


/* Keyboard */
/* types */
typedef struct _KeyboardKey KeyboardKey;


/* functions */
KeyboardKey * keyboard_key_new(unsigned int keysym, char const * label);
void keyboard_key_delete(KeyboardKey * key);

/* accessors */
unsigned int keyboard_key_get_keysym(KeyboardKey * key);
GtkWidget * keyboard_key_get_label_widget(KeyboardKey * key);
GtkWidget * keyboard_key_get_widget(KeyboardKey * key);
unsigned int keyboard_key_get_width(KeyboardKey * key);

# if GTK_CHECK_VERSION(3, 0, 0)
void keyboard_key_set_background(KeyboardKey * key, GdkRGBA * color);
# else
void keyboard_key_set_background(KeyboardKey * key, GdkColor * color);
# endif
void keyboard_key_set_font(KeyboardKey * key, PangoFontDescription * font);
# if GTK_CHECK_VERSION(3, 0, 0)
void keyboard_key_set_foreground(KeyboardKey * key, GdkRGBA * color);
# else
void keyboard_key_set_foreground(KeyboardKey * key, GdkColor * color);
# endif
int keyboard_key_set_modifier(KeyboardKey * key, unsigned int modifier,
		unsigned int keysym, char const * label);

/* useful */
void keyboard_key_apply_modifier(KeyboardKey * key, unsigned int modifier);

#endif /* !KEYBOARD_KEY_H */
