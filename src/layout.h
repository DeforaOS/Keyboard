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



#ifndef KEYBOARD_LAYOUT_H
# define KEYBOARD_LAYOUT_H

# include <gtk/gtk.h>
# include "key.h"


/* Keyboard */
/* types */
typedef struct _KeyboardLayout KeyboardLayout;


/* functions */
KeyboardLayout * keyboard_layout_new(void);
void keyboard_layout_delete(KeyboardLayout * layout);

/* accessors */
GtkWidget * keyboard_layout_get_widget(KeyboardLayout * layout);

/* useful */
KeyboardKey * keyboard_layout_add(KeyboardLayout * layout, unsigned int row,
		unsigned int width, unsigned int keysym, char const * label);
void keyboard_layout_add_widget(KeyboardLayout * layout, unsigned int row,
		unsigned int column, unsigned int width, GtkWidget * widget);
void keyboard_layout_apply_modifier(KeyboardLayout * layout,
		unsigned int modifier);

#endif /* !KEYBOARD_LAYOUT_H */
