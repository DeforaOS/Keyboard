/* $Id$ */
static char const _copyright[] =
"Copyright © 2011-2020 Pierre Pronchery <khorben@defora.org>";
/* This file is part of DeforaOS Desktop Keyboard */
static char const _license[] =
"All rights reserved.\n"
"\n"
"Redistribution and use in source and binary forms, with or without\n"
"modification, are permitted provided that the following conditions are\n"
"met:\n"
"\n"
"1. Redistributions of source code must retain the above copyright\n"
"   notice, this list of conditions and the following disclaimer.\n"
"\n"
"2. Redistributions in binary form must reproduce the above copyright\n"
"   notice, this list of conditions and the following disclaimer in the\n"
"   documentation and/or other materials provided with the distribution.\n"
"\n"
"THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS\n"
"IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED\n"
"TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A\n"
"PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT\n"
"HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,\n"
"SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED\n"
"TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR\n"
"PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF\n"
"LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING\n"
"NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS\n"
"SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */\n";
/* TODO:
 * - display "likely" keys (after modifiers) as well
 * - see if XKB could be used to define the keyboard */



#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <libintl.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#if GTK_CHECK_VERSION(3, 0, 0)
# include <gtk/gtkx.h>
#endif
#include <System.h>
#include <Desktop.h>
#define XK_LATIN1
#define XK_MISCELLANY
#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#include "callbacks.h"
#include "layout.h"
#include "keyboard.h"
#include "../config.h"
#define _(string) gettext(string)
#define N_(string) (string)

#ifndef PROGNAME_KEYBOARD
# define PROGNAME_KEYBOARD	"keyboard"
#endif


/* Keyboard */
/* private */
/* types */
struct _Keyboard
{
	/* preferences */
	KeyboardMode mode;

	KeyboardLayout ** layouts;
	size_t layouts_cnt;

	PangoFontDescription * font;
	GtkWidget * window;
#if GTK_CHECK_VERSION(2, 10, 0)
	GtkStatusIcon * icon;
#endif
	GtkWidget * ab_window;
	GdkRectangle geometry;
	int width;
	int height;
	int x;
	int y;
};

typedef struct _KeyboardKeyDefinition
{
	unsigned int row;
	unsigned int width;
	unsigned int modifier;
	unsigned int keysym;
	char const * label;
} KeyboardKeyDefinition;

typedef enum _KeyboardLayoutSection
{
	KLS_LETTERS = 0,
	KLS_KEYPAD,
	KLS_SPECIAL
} KeyboardLayoutSection;
#define KLS_LAST KLS_SPECIAL
#define KLS_COUNT (KLS_LAST + 1)

typedef struct _KeyboardLayoutDefinition
{
	char const * label;
	KeyboardKeyDefinition const * keys;
} KeyboardLayoutDefinition;

typedef enum _KeyboardLayoutType
{
	KLT_QWERTY = 0,
	KLT_QWERTZ,
	KLT_AZERTY
} KeyboardLayoutType;
#define KLT_LAST KLT_AZERTY
#define KLT_COUNT (KLT_LAST + 1)

typedef struct _KeyboardLayoutTypeName
{
	KeyboardLayoutType type;
	char const * name;
} KeyboardLayoutTypeName;


/* constants */
static char const * _authors[] =
{
	"Pierre Pronchery <khorben@defora.org>",
	NULL
};

static const KeyboardLayoutTypeName _keyboard_layout_type_name[] =
{
	{ KLT_QWERTY,	"us"	},
	{ KLT_QWERTZ,	"de"	},
	{ KLT_AZERTY,	"fr"	}
};

static const DesktopMenu _keyboard_menu_file[] =
{
	{ N_("_Quit"), G_CALLBACK(on_file_quit), GTK_STOCK_QUIT,
		GDK_CONTROL_MASK, GDK_KEY_Q },
	{ NULL, NULL, NULL, 0, 0 }
};

static const DesktopMenu _keyboard_menu_view[] =
{
	{ N_("_Hide"), G_CALLBACK(on_view_hide), NULL, GDK_CONTROL_MASK,
		GDK_KEY_H },
	{ NULL, NULL, NULL, 0, 0 }
};

static const DesktopMenu _keyboard_menu_help[] =
{
#if GTK_CHECK_VERSION(2, 6, 0)
	{ N_("_About"), G_CALLBACK(on_help_about), GTK_STOCK_ABOUT, 0, 0 },
#else
	{ N_("_About"), G_CALLBACK(on_help_about), NULL, 0, 0 },
#endif
	{ NULL, NULL, NULL, 0, 0 }
};

static const DesktopMenubar _keyboard_menubar[] =
{
	{ N_("_File"), _keyboard_menu_file },
	{ N_("_View"), _keyboard_menu_view },
	{ N_("_Help"), _keyboard_menu_help },
	{ NULL, NULL }
};


/* variables */
static KeyboardKeyDefinition const _keyboard_layout_letters_qwerty[] =
{
	{ 0, 2, 0, XK_q, "q" },
	{ 0, 0, XK_Shift_L, XK_Q, "Q" },
	{ 0, 2, 0, XK_w, "w" },
	{ 0, 0, XK_Shift_L, XK_W, "W" },
	{ 0, 2, 0, XK_e, "e" },
	{ 0, 0, XK_Shift_L, XK_E, "E" },
	{ 0, 2, 0, XK_r, "r" },
	{ 0, 0, XK_Shift_L, XK_R, "R" },
	{ 0, 2, 0, XK_t, "t" },
	{ 0, 0, XK_Shift_L, XK_T, "T" },
	{ 0, 2, 0, XK_y, "y" },
	{ 0, 0, XK_Shift_L, XK_Y, "Y" },
	{ 0, 2, 0, XK_u, "u" },
	{ 0, 0, XK_Shift_L, XK_U, "U" },
	{ 0, 2, 0, XK_i, "i" },
	{ 0, 0, XK_Shift_L, XK_I, "I" },
	{ 0, 2, 0, XK_o, "o" },
	{ 0, 0, XK_Shift_L, XK_O, "O" },
	{ 0, 2, 0, XK_p, "p" },
	{ 0, 0, XK_Shift_L, XK_P, "P" },
	{ 1, 1, 0, 0, NULL },
	{ 1, 2, 0, XK_a, "a" },
	{ 1, 0, XK_Shift_L, XK_A, "A" },
	{ 1, 2, 0, XK_s, "s" },
	{ 1, 0, XK_Shift_L, XK_S, "S" },
	{ 1, 2, 0, XK_d, "d" },
	{ 1, 0, XK_Shift_L, XK_D, "D" },
	{ 1, 2, 0, XK_f, "f" },
	{ 1, 0, XK_Shift_L, XK_F, "F" },
	{ 1, 2, 0, XK_g, "g" },
	{ 1, 0, XK_Shift_L, XK_G, "G" },
	{ 1, 2, 0, XK_h, "h" },
	{ 1, 0, XK_Shift_L, XK_H, "H" },
	{ 1, 2, 0, XK_j, "j" },
	{ 1, 0, XK_Shift_L, XK_J, "J" },
	{ 1, 2, 0, XK_k, "k" },
	{ 1, 0, XK_Shift_L, XK_K, "K" },
	{ 1, 2, 0, XK_l, "l" },
	{ 1, 0, XK_Shift_L, XK_L, "L" },
	{ 2, 2, 0, XK_Shift_L, "\xe2\x87\xa7" },
	{ 2, 2, 0, XK_z, "z" },
	{ 2, 0, XK_Shift_L, XK_Z, "Z" },
	{ 2, 2, 0, XK_x, "x" },
	{ 2, 0, XK_Shift_L, XK_X, "X" },
	{ 2, 2, 0, XK_c, "c" },
	{ 2, 0, XK_Shift_L, XK_C, "C" },
	{ 2, 2, 0, XK_v, "v" },
	{ 2, 0, XK_Shift_L, XK_V, "V" },
	{ 2, 2, 0, XK_b, "b" },
	{ 2, 0, XK_Shift_L, XK_B, "B" },
	{ 2, 2, 0, XK_n, "n" },
	{ 2, 0, XK_Shift_L, XK_N, "N" },
	{ 2, 2, 0, XK_m, "m" },
	{ 2, 0, XK_Shift_L, XK_M, "M" },
	{ 2, 2, 0, XK_comma, "," },
	{ 2, 0, XK_Shift_L, XK_comma, "<" },
	{ 2, 2, 0, XK_period, "." },
	{ 2, 0, XK_Shift_L, XK_period, ">" },
	{ 3, 3, 0, 0, NULL },
	{ 3, 3, 0, XK_Control_L, "Ctrl" },
	{ 3, 3, 0, XK_Alt_L, "Alt" },
	{ 3, 5, 0, XK_space, " " },
	{ 3, 0, XK_Shift_L, XK_space, " " },
	{ 3, 3, 0, XK_Return, "\xe2\x86\xb2" },
	{ 3, 3, 0, XK_BackSpace, "\xe2\x8c\xab" },
	{ 0, 0, 0, 0, NULL }
};

static KeyboardKeyDefinition const _keyboard_layout_letters_qwertz[] =
{
	{ 0, 2, 0, XK_q, "q" },
	{ 0, 0, XK_Shift_L, XK_Q, "Q" },
	{ 0, 2, 0, XK_w, "w" },
	{ 0, 0, XK_Shift_L, XK_W, "W" },
	{ 0, 2, 0, XK_e, "e" },
	{ 0, 0, XK_Shift_L, XK_E, "E" },
	{ 0, 2, 0, XK_r, "r" },
	{ 0, 0, XK_Shift_L, XK_R, "R" },
	{ 0, 2, 0, XK_t, "t" },
	{ 0, 0, XK_Shift_L, XK_T, "T" },
	{ 0, 2, 0, XK_z, "z" },
	{ 0, 0, XK_Shift_L, XK_Z, "Z" },
	{ 0, 2, 0, XK_u, "u" },
	{ 0, 0, XK_Shift_L, XK_U, "U" },
	{ 0, 2, 0, XK_i, "i" },
	{ 0, 0, XK_Shift_L, XK_I, "I" },
	{ 0, 2, 0, XK_o, "o" },
	{ 0, 0, XK_Shift_L, XK_O, "O" },
	{ 0, 2, 0, XK_p, "p" },
	{ 0, 0, XK_Shift_L, XK_P, "P" },
	{ 1, 1, 0, 0, NULL },
	{ 1, 2, 0, XK_a, "a" },
	{ 1, 0, XK_Shift_L, XK_A, "A" },
	{ 1, 2, 0, XK_s, "s" },
	{ 1, 0, XK_Shift_L, XK_S, "S" },
	{ 1, 2, 0, XK_d, "d" },
	{ 1, 0, XK_Shift_L, XK_D, "D" },
	{ 1, 2, 0, XK_f, "f" },
	{ 1, 0, XK_Shift_L, XK_F, "F" },
	{ 1, 2, 0, XK_g, "g" },
	{ 1, 0, XK_Shift_L, XK_G, "G" },
	{ 1, 2, 0, XK_h, "h" },
	{ 1, 0, XK_Shift_L, XK_H, "H" },
	{ 1, 2, 0, XK_j, "j" },
	{ 1, 0, XK_Shift_L, XK_J, "J" },
	{ 1, 2, 0, XK_k, "k" },
	{ 1, 0, XK_Shift_L, XK_K, "K" },
	{ 1, 2, 0, XK_l, "l" },
	{ 1, 0, XK_Shift_L, XK_L, "L" },
	{ 2, 2, 0, XK_Shift_L, "\xe2\x87\xa7" },
	{ 2, 2, 0, XK_y, "y" },
	{ 2, 0, XK_Shift_L, XK_Y, "Y" },
	{ 2, 2, 0, XK_x, "x" },
	{ 2, 0, XK_Shift_L, XK_X, "X" },
	{ 2, 2, 0, XK_c, "c" },
	{ 2, 0, XK_Shift_L, XK_C, "C" },
	{ 2, 2, 0, XK_v, "v" },
	{ 2, 0, XK_Shift_L, XK_V, "V" },
	{ 2, 2, 0, XK_b, "b" },
	{ 2, 0, XK_Shift_L, XK_B, "B" },
	{ 2, 2, 0, XK_n, "n" },
	{ 2, 0, XK_Shift_L, XK_N, "N" },
	{ 2, 2, 0, XK_m, "m" },
	{ 2, 0, XK_Shift_L, XK_M, "M" },
	{ 2, 2, 0, XK_comma, "," },
	{ 2, 0, XK_Shift_L, XK_comma, "<" },
	{ 2, 2, 0, XK_period, "." },
	{ 2, 0, XK_Shift_L, XK_period, ">" },
	{ 3, 3, 0, 0, NULL },
	{ 3, 3, 0, XK_Control_L, "Ctrl" },
	{ 3, 3, 0, XK_Alt_L, "Alt" },
	{ 3, 5, 0, XK_space, " " },
	{ 3, 0, XK_Shift_L, XK_space, " " },
	{ 3, 3, 0, XK_Return, "\xe2\x86\xb2" },
	{ 3, 3, 0, XK_BackSpace, "\xe2\x8c\xab" },
	{ 0, 0, 0, 0, NULL }
};

static KeyboardKeyDefinition const _keyboard_layout_letters_azerty[] =
{
	{ 0, 2, 0, XK_a, "a" },
	{ 0, 0, XK_Shift_L, XK_A, "A" },
	{ 0, 2, 0, XK_z, "z" },
	{ 0, 0, XK_Shift_L, XK_Z, "Z" },
	{ 0, 2, 0, XK_e, "e" },
	{ 0, 0, XK_Shift_L, XK_E, "E" },
#if 0 /* def XK_CURRENCY */
	{ 0, 0, XK_Alt_R, XK_EuroSign, "€" },
#else
	{ 0, 0, XK_Alt_R, XK_E, "€" },
#endif
	{ 0, 2, 0, XK_r, "r" },
	{ 0, 0, XK_Shift_L, XK_R, "R" },
	{ 0, 2, 0, XK_t, "t" },
	{ 0, 0, XK_Shift_L, XK_T, "T" },
	{ 0, 2, 0, XK_y, "y" },
	{ 0, 0, XK_Shift_L, XK_Y, "Y" },
	{ 0, 2, 0, XK_u, "u" },
	{ 0, 0, XK_Shift_L, XK_U, "U" },
	{ 0, 2, 0, XK_i, "i" },
	{ 0, 0, XK_Shift_L, XK_I, "I" },
	{ 0, 2, 0, XK_o, "o" },
	{ 0, 0, XK_Shift_L, XK_O, "O" },
	{ 0, 2, 0, XK_p, "p" },
	{ 0, 0, XK_Shift_L, XK_P, "P" },
	{ 0, 2, 0, XK_asciicircum, "^" },
	{ 0, 0, XK_Shift_L, XK_quotedbl, "\"" },
	{ 0, 2, 0, XK_dollar, "$" },
	{ 0, 0, XK_Shift_L, XK_sterling, "£" },
	{ 1, 1, 0, 0, NULL },
	{ 1, 2, 0, XK_q, "q" },
	{ 1, 0, XK_Shift_L, XK_Q, "Q" },
	{ 1, 2, 0, XK_s, "s" },
	{ 1, 0, XK_Shift_L, XK_S, "S" },
	{ 1, 2, 0, XK_d, "d" },
	{ 1, 0, XK_Shift_L, XK_D, "D" },
	{ 1, 2, 0, XK_f, "f" },
	{ 1, 0, XK_Shift_L, XK_F, "F" },
	{ 1, 2, 0, XK_g, "g" },
	{ 1, 0, XK_Shift_L, XK_G, "G" },
	{ 1, 2, 0, XK_h, "h" },
	{ 1, 0, XK_Shift_L, XK_H, "H" },
	{ 1, 2, 0, XK_j, "j" },
	{ 1, 0, XK_Shift_L, XK_J, "J" },
	{ 1, 2, 0, XK_k, "k" },
	{ 1, 0, XK_Shift_L, XK_K, "K" },
	{ 1, 2, 0, XK_l, "l" },
	{ 1, 0, XK_Shift_L, XK_L, "L" },
	{ 1, 2, 0, XK_m, "m" },
	{ 1, 0, XK_Shift_L, XK_M, "M" },
	{ 1, 2, 0, XK_ugrave, "ù" },
	{ 1, 0, XK_Shift_L, XK_percent, "%" },
	{ 1, 2, 0, XK_asterisk, "*" },
	{ 1, 0, XK_Shift_L, XK_mu, "µ" },
	{ 2, 2, 0, XK_Shift_L, "\xe2\x87\xa7" },
	{ 2, 2, 0, XK_less, "<" },
	{ 2, 0, XK_Shift_L, XK_less, ">" },
	{ 2, 2, 0, XK_w, "w" },
	{ 2, 0, XK_Shift_L, XK_W, "W" },
	{ 2, 2, 0, XK_x, "x" },
	{ 2, 0, XK_Shift_L, XK_X, "X" },
	{ 2, 2, 0, XK_c, "c" },
	{ 2, 0, XK_Shift_L, XK_C, "C" },
	{ 2, 2, 0, XK_v, "v" },
	{ 2, 0, XK_Shift_L, XK_V, "V" },
	{ 2, 2, 0, XK_b, "b" },
	{ 2, 0, XK_Shift_L, XK_B, "B" },
	{ 2, 2, 0, XK_n, "n" },
	{ 2, 0, XK_Shift_L, XK_N, "N" },
	{ 2, 2, 0, XK_comma, "," },
	{ 2, 0, XK_Shift_L, XK_question, "?" },
	{ 2, 2, 0, XK_semicolon, ";" },
	{ 2, 0, XK_Shift_L, XK_period, "." },
	{ 2, 2, 0, XK_colon, ":" },
	{ 2, 0, XK_Shift_L, XK_slash, "/" },
	{ 2, 2, 0, XK_exclam, "!" },
	{ 2, 0, XK_Shift_L, XK_paragraph, "§" },
	{ 3, 3, 0, 0, NULL },
	{ 3, 3, 0, XK_Control_L, "Ctrl" },
	{ 3, 3, 0, XK_Alt_L, "Alt" },
	{ 3, 7, 0, XK_space, " " },
	{ 3, 0, XK_Shift_L, XK_space, " " },
	{ 3, 3, 0, XK_Alt_R, "Alt Gr" },
	{ 3, 0, XK_Shift_L, XK_Alt_R, "Alt Gr" },
	{ 3, 3, 0, XK_Return, "\xe2\x86\xb2" },
	{ 3, 3, 0, XK_BackSpace, "\xe2\x8c\xab" },
	{ 0, 0, 0, 0, NULL }
};

static KeyboardKeyDefinition const * _keyboard_layout_letters_definition[KLT_COUNT] =
{
	_keyboard_layout_letters_qwerty,
	_keyboard_layout_letters_qwertz,
	_keyboard_layout_letters_azerty
};

static KeyboardKeyDefinition const _keyboard_layout_keypad[] =
{
	{ 0, 3, 0, XK_Num_Lock, "Num" },
	{ 0, 1, 0, 0, NULL },
	{ 0, 4, 0, XK_KP_Home, "\xe2\x86\x96" },
	{ 0, 0, XK_Num_Lock, XK_7, "7" },
	{ 0, 4, 0, XK_KP_Up, "\xe2\x86\x91" },
	{ 0, 0, XK_Num_Lock, XK_8, "8" },
	{ 0, 4, 0, XK_KP_Page_Up, "\xe2\x87\x9e" },
	{ 0, 0, XK_Num_Lock, XK_9, "9" },
	{ 0, 1, 0, 0, NULL },
	{ 0, 3, 0, XK_KP_Subtract, "-" },
	{ 1, 3, 0, XK_KP_Divide, "/" },
	{ 1, 1, 0, 0, NULL },
	{ 1, 4, 0, XK_KP_Left, "\xe2\x86\x90" },
	{ 1, 0, XK_Num_Lock, XK_4, "4" },
	{ 1, 4, 0, XK_5, "5" },
	{ 1, 0, XK_Num_Lock, XK_5, "5" },
	{ 1, 4, 0, XK_KP_Right, "\xe2\x86\x92" },
	{ 1, 0, XK_Num_Lock, XK_6, "6" },
	{ 1, 1, 0, 0, NULL },
	{ 1, 3, 0, XK_KP_Add, "+" },
	{ 2, 3, 0, XK_KP_Multiply, "*" },
	{ 2, 1, 0, 0, NULL },
	{ 2, 4, 0, XK_KP_End, "\xe2\x86\x99" },
	{ 2, 0, XK_Num_Lock, XK_1, "1" },
	{ 2, 4, 0, XK_KP_Down, "\xe2\x86\x93" },
	{ 2, 0, XK_Num_Lock, XK_2, "2" },
	{ 2, 4, 0, XK_KP_Page_Down, "\xe2\x87\x9f" },
	{ 2, 0, XK_Num_Lock, XK_3, "3" },
	{ 2, 1, 0, 0, NULL },
	{ 2, 3, 0, XK_KP_Enter, "\xe2\x86\xb2" },
	{ 3, 3, 0, 0, NULL },
	{ 3, 1, 0, 0, NULL },
	{ 3, 8, 0, XK_KP_Insert, "Ins" },
	{ 3, 0, XK_Num_Lock, XK_0, "0" },
	{ 3, 4, 0, XK_KP_Delete, "Del" },
	{ 3, 0, XK_Num_Lock, XK_KP_Decimal, "." },
	{ 3, 1, 0, 0, NULL },
	{ 3, 3, 0, XK_BackSpace, "\xe2\x8c\xab" },
	{ 0, 0, 0, 0, NULL }
};

static KeyboardKeyDefinition const _keyboard_layout_special_qwerty[] =
{
	{ 0, 3, 0, XK_Escape, "Esc" },
	{ 0, 2, 0, XK_F1, "F1" },
	{ 0, 0, XK_Shift_L, XK_F5, "F5" },
	{ 0, 2, 0, XK_F2, "F2" },
	{ 0, 0, XK_Shift_L, XK_F6, "F6" },
	{ 0, 2, 0, XK_F3, "F3" },
	{ 0, 0, XK_Shift_L, XK_F7, "F7" },
	{ 0, 2, 0, XK_F4, "F4" },
	{ 0, 0, XK_Shift_L, XK_F8, "F8" },
	{ 0, 1, 0, 0, NULL },
	{ 0, 2, 0, XK_F5, "F5" },
	{ 0, 0, XK_Shift_L, XK_F9, "F9" },
	{ 0, 2, 0, XK_F6, "F6" },
	{ 0, 0, XK_Shift_L, XK_F10, "F10" },
	{ 0, 2, 0, XK_F7, "F7" },
	{ 0, 0, XK_Shift_L, XK_F11, "F11" },
	{ 0, 2, 0, XK_F8, "F8" },
	{ 0, 0, XK_Shift_L, XK_F12, "F12" },
	{ 1, 2, 0, XK_1, "1" },
	{ 1, 0, XK_Shift_L, XK_exclam, "!" },
	{ 1, 2, 0, XK_2, "2" },
	{ 1, 0, XK_Shift_L, XK_at, "@" },
	{ 1, 2, 0, XK_3, "3" },
	{ 1, 0, XK_Shift_L, XK_numbersign, "#" },
	{ 1, 2, 0, XK_4, "4" },
	{ 1, 0, XK_Shift_L, XK_dollar, "$" },
	{ 1, 2, 0, XK_5, "5" },
	{ 1, 0, XK_Shift_L, XK_percent, "%" },
	{ 1, 2, 0, XK_6, "6" },
	{ 1, 0, XK_Shift_L, XK_asciicircum, "^" },
	{ 1, 2, 0, XK_7, "7" },
	{ 1, 0, XK_Shift_L, XK_ampersand, "&" },
	{ 1, 2, 0, XK_8, "8" },
	{ 1, 0, XK_Shift_L, XK_asterisk, "*" },
	{ 1, 2, 0, XK_9, "9" },
	{ 1, 0, XK_Shift_L, XK_parenleft, "(" },
	{ 1, 2, 0, XK_0, "0" },
	{ 1, 0, XK_Shift_L, XK_parenright, ")" },
	{ 2, 3, 0, XK_Tab, "\xe2\x86\xb9" },
	{ 2, 2, 0, XK_grave, "`" },
	{ 2, 0, XK_Shift_L, XK_asciitilde, "~" },
	{ 2, 2, 0, XK_minus, "-" },
	{ 2, 0, XK_Shift_L, XK_minus, "_" },
	{ 2, 2, 0, XK_equal, "=" },
	{ 2, 0, XK_Shift_L, XK_equal, "+" },
	{ 2, 2, 0, XK_backslash, "\\" },
	{ 2, 0, XK_Shift_L, XK_backslash, "|" },
	{ 2, 2, 0, XK_bracketleft, "[" },
	{ 2, 0, XK_Shift_L, XK_bracketleft, "{" },
	{ 2, 2, 0, XK_bracketright, "]" },
	{ 2, 0, XK_Shift_L, XK_bracketright, "}" },
	{ 2, 2, 0, XK_semicolon, ";" },
	{ 2, 0, XK_Shift_L, XK_semicolon, ":" },
	{ 2, 2, 0, XK_apostrophe, "'" },
	{ 2, 0, XK_Shift_L, XK_apostrophe, "\"" },
	{ 3, 3, 0, 0, NULL },
	{ 3, 2, 0, XK_Shift_L, "\xe2\x87\xa7" },
	{ 3, 3, 0, XK_space, " " },
	{ 3, 0, XK_Shift_L, XK_space, " " },
	{ 3, 2, 0, XK_comma, "," },
	{ 3, 0, XK_Shift_L, XK_comma, "<" },
	{ 3, 2, 0, XK_period, "." },
	{ 2, 0, XK_Shift_L, XK_period, ">" },
	{ 3, 2, 0, XK_slash, "/" },
	{ 3, 0, XK_Shift_L, XK_slash, "?" },
	{ 3, 3, 0, XK_Return, "\xe2\x86\xb2" },
	{ 3, 3, 0, XK_BackSpace, "\xe2\x8c\xab" },
	{ 0, 0, 0, 0, NULL }
};

static KeyboardKeyDefinition const _keyboard_layout_special_qwertz[] =
{
	{ 0, 3, 0, XK_Escape, "Esc" },
	{ 0, 2, 0, XK_F1, "F1" },
	{ 0, 0, XK_Shift_L, XK_F5, "F5" },
	{ 0, 2, 0, XK_F2, "F2" },
	{ 0, 0, XK_Shift_L, XK_F6, "F6" },
	{ 0, 2, 0, XK_F3, "F3" },
	{ 0, 0, XK_Shift_L, XK_F7, "F7" },
	{ 0, 2, 0, XK_F4, "F4" },
	{ 0, 0, XK_Shift_L, XK_F8, "F8" },
	{ 0, 1, 0, 0, NULL },
	{ 0, 2, 0, XK_F5, "F5" },
	{ 0, 0, XK_Shift_L, XK_F9, "F9" },
	{ 0, 2, 0, XK_F6, "F6" },
	{ 0, 0, XK_Shift_L, XK_F10, "F10" },
	{ 0, 2, 0, XK_F7, "F7" },
	{ 0, 0, XK_Shift_L, XK_F11, "F11" },
	{ 0, 2, 0, XK_F8, "F8" },
	{ 0, 0, XK_Shift_L, XK_F12, "F12" },
	{ 1, 2, 0, XK_1, "1" },
	{ 1, 0, XK_Shift_L, XK_exclam, "!" },
	{ 1, 2, 0, XK_2, "2" },
	{ 1, 0, XK_Shift_L, XK_at, "@" },
	{ 1, 2, 0, XK_3, "3" },
	{ 1, 0, XK_Shift_L, XK_numbersign, "#" },
	{ 1, 2, 0, XK_4, "4" },
	{ 1, 0, XK_Shift_L, XK_dollar, "$" },
	{ 1, 2, 0, XK_5, "5" },
	{ 1, 0, XK_Shift_L, XK_percent, "%" },
	{ 1, 2, 0, XK_6, "6" },
	{ 1, 0, XK_Shift_L, XK_asciicircum, "^" },
	{ 1, 2, 0, XK_7, "7" },
	{ 1, 0, XK_Shift_L, XK_ampersand, "&" },
	{ 1, 2, 0, XK_8, "8" },
	{ 1, 0, XK_Shift_L, XK_asterisk, "*" },
	{ 1, 2, 0, XK_9, "9" },
	{ 1, 0, XK_Shift_L, XK_parenleft, "(" },
	{ 1, 2, 0, XK_0, "0" },
	{ 1, 0, XK_Shift_L, XK_parenright, ")" },
	{ 2, 3, 0, XK_Tab, "\xe2\x86\xb9" },
	{ 2, 2, 0, XK_grave, "`" },
	{ 2, 0, XK_Shift_L, XK_asciitilde, "~" },
	{ 2, 2, 0, XK_minus, "-" },
	{ 2, 0, XK_Shift_L, XK_minus, "_" },
	{ 2, 2, 0, XK_equal, "=" },
	{ 2, 0, XK_Shift_L, XK_equal, "+" },
	{ 2, 2, 0, XK_backslash, "\\" },
	{ 2, 0, XK_Shift_L, XK_backslash, "|" },
	{ 2, 2, 0, XK_bracketleft, "[" },
	{ 2, 0, XK_Shift_L, XK_bracketleft, "{" },
	{ 2, 2, 0, XK_bracketright, "]" },
	{ 2, 0, XK_Shift_L, XK_bracketright, "}" },
	{ 2, 2, 0, XK_semicolon, ";" },
	{ 2, 0, XK_Shift_L, XK_semicolon, ":" },
	{ 2, 2, 0, XK_apostrophe, "'" },
	{ 2, 0, XK_Shift_L, XK_apostrophe, "\"" },
	{ 3, 3, 0, 0, NULL },
	{ 3, 2, 0, XK_Shift_L, "\xe2\x87\xa7" },
	{ 3, 3, 0, XK_space, " " },
	{ 3, 0, XK_Shift_L, XK_space, " " },
	{ 3, 2, 0, XK_comma, "," },
	{ 3, 0, XK_Shift_L, XK_comma, "<" },
	{ 3, 2, 0, XK_period, "." },
	{ 2, 0, XK_Shift_L, XK_period, ">" },
	{ 3, 2, 0, XK_slash, "/" },
	{ 3, 0, XK_Shift_L, XK_slash, "?" },
	{ 3, 3, 0, XK_Return, "\xe2\x86\xb2" },
	{ 3, 3, 0, XK_BackSpace, "\xe2\x8c\xab" },
	{ 0, 0, 0, 0, NULL }
};

static KeyboardKeyDefinition const _keyboard_layout_special_azerty[] =
{
	{ 0, 3, 0, XK_Escape, "Esc" },
	{ 0, 1, 0, 0, NULL },
	{ 0, 2, 0, XK_F1, "F1" },
	{ 0, 0, XK_Shift_L, XK_F5, "F5" },
	{ 0, 2, 0, XK_F2, "F2" },
	{ 0, 0, XK_Shift_L, XK_F6, "F6" },
	{ 0, 2, 0, XK_F3, "F3" },
	{ 0, 0, XK_Shift_L, XK_F7, "F7" },
	{ 0, 2, 0, XK_F4, "F4" },
	{ 0, 0, XK_Shift_L, XK_F8, "F8" },
	{ 0, 2, 0, XK_F5, "F5" },
	{ 0, 0, XK_Shift_L, XK_F9, "F9" },
	{ 0, 2, 0, XK_F6, "F6" },
	{ 0, 0, XK_Shift_L, XK_F10, "F10" },
	{ 0, 2, 0, XK_F7, "F7" },
	{ 0, 0, XK_Shift_L, XK_F11, "F11" },
	{ 0, 2, 0, XK_F8, "F8" },
	{ 0, 0, XK_Shift_L, XK_F12, "F12" },
	{ 1, 1, 0, 0, NULL },
	{ 1, 2, 0, XK_ampersand, "&" },
	{ 1, 0, XK_Shift_L, XK_1, "1" },
	{ 1, 2, 0, XK_eacute, "é" },
	{ 1, 0, XK_Shift_L, XK_2, "2" },
	{ 1, 2, 0, XK_quotedbl, "\"" },
	{ 1, 0, XK_Shift_L, XK_3, "3" },
	{ 1, 2, 0, XK_apostrophe, "'" },
	{ 1, 0, XK_Shift_L, XK_4, "4" },
	{ 1, 2, 0, XK_parenleft, "(" },
	{ 1, 0, XK_Shift_L, XK_5, "5" },
	{ 1, 2, 0, XK_minus, "-" },
	{ 1, 0, XK_Shift_L, XK_6, "6" },
	{ 1, 2, 0, XK_egrave, "è" },
	{ 1, 0, XK_Shift_L, XK_7, "7" },
	{ 1, 2, 0, XK_underscore, "_" },
	{ 1, 0, XK_Shift_L, XK_8, "8" },
	{ 1, 2, 0, XK_ccedilla, "ç" },
	{ 1, 0, XK_Shift_L, XK_9, "9" },
	{ 1, 2, 0, XK_aacute, "à" },
	{ 1, 0, XK_Shift_L, XK_0, "0" },
	{ 2, 2, 0, XK_Tab, "\xe2\x86\xb9" },
	{ 2, 2, 0, XK_twosuperior, "²" },
	{ 2, 2, 0, XK_asciitilde, "~" },
	{ 2, 2, 0, XK_numbersign, "#" },
	{ 2, 2, 0, XK_braceleft, "{" },
	{ 2, 2, 0, XK_bracketleft, "[" },
	{ 2, 2, 0, XK_bracketright, "]" },
	{ 2, 2, 0, XK_braceright, "}" },
	{ 2, 2, 0, XK_parenright, ")" },
	{ 2, 0, XK_Shift_L, XK_degree, "°" },
	{ 2, 2, 0, XK_equal, "=" },
	{ 2, 0, XK_Shift_L, XK_plus, "+" },
	{ 3, 3, 0, 0, NULL },
	{ 3, 2, 0, XK_Shift_L, "\xe2\x87\xa7" },
	{ 3, 2, 0, XK_bracketleft, "|" },
	{ 3, 2, 0, XK_grave, "`" },
	{ 3, 2, 0, XK_backslash, "\\" },
	{ 3, 2, 0, XK_asciicircum, "^" },
	{ 3, 2, 0, XK_at, "@" },
	{ 3, 3, 0, XK_Return, "\xe2\x86\xb2" },
	{ 3, 3, 0, XK_BackSpace, "\xe2\x8c\xab" },
	{ 0, 0, 0, 0, NULL }
};

static KeyboardKeyDefinition const * _keyboard_layout_special_definition[KLT_COUNT] =
{
	_keyboard_layout_special_qwerty,
	_keyboard_layout_special_qwertz,
	_keyboard_layout_special_azerty
};


/* prototypes */
static GtkWidget * _keyboard_add_layout(Keyboard * keyboard,
		KeyboardLayoutDefinition * definitions,
		size_t definitions_cnt, KeyboardLayoutSection section);

static void _keyboard_error(Keyboard * keyboard, char const * format, ...);


/* public */
/* functions */
/* keyboard_new */
static void _new_mode(Keyboard * keyboard, KeyboardMode mode);
static void _new_mode_docked(Keyboard * keyboard);
static void _new_mode_embedded(Keyboard * keyboard);
static void _new_mode_popup(Keyboard * keyboard);
static void _new_mode_widget(Keyboard * keyboard);
static void _new_mode_windowed(Keyboard * keyboard);

Keyboard * keyboard_new(KeyboardPrefs * prefs)
{
	Keyboard * keyboard;
	GtkAccelGroup * group;
	GdkScreen * screen;
	GtkWidget * vbox;
	GtkWidget * widget;
	PangoFontDescription * bold;
#if GTK_CHECK_VERSION(3, 0, 0)
	GdkRGBA gray = { 0.56, 0.56, 0.56, 1.0 };
#else
	GdkColor gray = { 0x90909090, 0x9090, 0x9090, 0x9090 };
#endif
	unsigned long id;
	KeyboardLayoutDefinition layout[KLS_COUNT] =
	{
		{ "Abc", _keyboard_layout_letters_qwerty	},
		{ "123", _keyboard_layout_keypad		},
		{ ",./", _keyboard_layout_special_qwerty	}
	};
	size_t i;
	KeyboardLayoutTypeName const * typename;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	if((keyboard = object_new(sizeof(*keyboard))) == NULL)
		return NULL;
	keyboard->mode = prefs->mode;
	keyboard->layouts = NULL;
	keyboard->layouts_cnt = 0;
	screen = gdk_screen_get_default();
	if(prefs != NULL && prefs->monitor > 0
			&& prefs->monitor < gdk_screen_get_n_monitors(screen))
		gdk_screen_get_monitor_geometry(screen, prefs->monitor,
				&keyboard->geometry);
	else
		gdk_screen_get_monitor_geometry(screen, 0, &keyboard->geometry);
	/* windows */
	_new_mode(keyboard, prefs->mode);
#if GTK_CHECK_VERSION(3, 0, 0)
	gtk_widget_override_background_color(keyboard->window,
			GTK_STATE_FLAG_NORMAL, &gray);
#else
	gtk_widget_modify_bg(keyboard->window, GTK_STATE_NORMAL, &gray);
#endif
	keyboard->icon = NULL;
	keyboard->ab_window = NULL;
	/* fonts */
	if(prefs->font != NULL)
		keyboard->font = pango_font_description_from_string(
				prefs->font);
	else
	{
		keyboard->font = pango_font_description_new();
		pango_font_description_set_weight(keyboard->font,
				PANGO_WEIGHT_BOLD);
	}
	bold = pango_font_description_new();
	pango_font_description_set_weight(bold, PANGO_WEIGHT_BOLD);
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
	gtk_container_add(GTK_CONTAINER(keyboard->window), vbox);
	/* menubar */
	if(prefs->mode == KEYBOARD_MODE_WINDOWED)
	{
		group = gtk_accel_group_new();
		gtk_window_add_accel_group(GTK_WINDOW(keyboard->window), group);
		g_object_unref(group);
		widget = desktop_menubar_create(_keyboard_menubar, keyboard,
				group);
		gtk_widget_show_all(widget);
		gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, FALSE, 0);
		/* XXX border hack */
		widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
		gtk_container_set_border_width(GTK_CONTAINER(widget), 4);
		gtk_box_pack_start(GTK_BOX(vbox), widget, TRUE, TRUE, 0);
		gtk_widget_show(vbox);
		vbox = widget;
	}
	/* layouts */
	if(prefs->layout != NULL)
	{
		for(i = 0; i < sizeof(_keyboard_layout_type_name)
				/ sizeof(*_keyboard_layout_type_name); i++)
		{
			typename = &_keyboard_layout_type_name[i];
			if(strcasecmp(prefs->layout, typename->name) == 0)
			{
				layout[KLS_LETTERS].keys = _keyboard_layout_letters_definition[typename->type];
				layout[KLS_SPECIAL].keys = _keyboard_layout_special_definition[typename->type];
				break;
			}
			typename = NULL;
		}
		if(typename == NULL)
			_keyboard_error(NULL, "%s: Unsupported layout",
					prefs->layout);
	}
	if((widget = _keyboard_add_layout(keyboard, layout, KLS_COUNT,
					KLS_LETTERS)) != NULL)
		gtk_box_pack_start(GTK_BOX(vbox), widget, TRUE, TRUE, 0);
	if((widget = _keyboard_add_layout(keyboard, layout, KLS_COUNT,
					KLS_KEYPAD)) != NULL)
		gtk_box_pack_start(GTK_BOX(vbox), widget, TRUE, TRUE, 0);
	if((widget = _keyboard_add_layout(keyboard, layout, KLS_COUNT,
					KLS_SPECIAL)) != NULL)
		gtk_box_pack_start(GTK_BOX(vbox), widget, TRUE, TRUE, 0);
	gtk_widget_show(vbox);
	if(prefs->mode == KEYBOARD_MODE_EMBEDDED)
	{
		/* print the window ID and force a flush */
		id = gtk_plug_get_id(GTK_PLUG(keyboard->window));
		printf("%lu\n", id);
		fclose(stdout);
	}
	else if(prefs->mode != KEYBOARD_MODE_WIDGET)
	{
#if GTK_CHECK_VERSION(2, 10, 0)
		/* create the systray icon */
		keyboard->icon = gtk_status_icon_new_from_icon_name(
				"input-keyboard");
# if GTK_CHECK_VERSION(2, 16, 0)
		gtk_status_icon_set_tooltip_text(keyboard->icon,
				_("Virtual keyboard"));
# endif
		g_signal_connect_swapped(keyboard->icon, "activate", G_CALLBACK(
					on_systray_activate), keyboard);
		g_signal_connect(keyboard->icon, "popup-menu", G_CALLBACK(
					on_systray_popup_menu), keyboard);
#endif
		/* show the window */
		if(prefs->wait == 0)
			gtk_widget_show(keyboard->window);
	}
	keyboard_set_layout(keyboard, KLS_LETTERS);
	pango_font_description_free(bold);
	/* messages */
	desktop_message_register(keyboard->window, KEYBOARD_CLIENT_MESSAGE,
			on_keyboard_message, keyboard);
	return keyboard;
}

static void _new_mode(Keyboard * keyboard, KeyboardMode mode)
{
	switch(mode)
	{
		case KEYBOARD_MODE_DOCKED:
			_new_mode_docked(keyboard);
			break;
		case KEYBOARD_MODE_EMBEDDED:
			_new_mode_embedded(keyboard);
			break;
		case KEYBOARD_MODE_POPUP:
			_new_mode_popup(keyboard);
			break;
		case KEYBOARD_MODE_WIDGET:
			_new_mode_widget(keyboard);
			break;
		case KEYBOARD_MODE_WINDOWED:
			_new_mode_windowed(keyboard);
			break;
	}
}

static void _new_mode_docked(Keyboard * keyboard)
{
	keyboard->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width(GTK_CONTAINER(keyboard->window), 4);
	gtk_window_set_accept_focus(GTK_WINDOW(keyboard->window), FALSE);
#if GTK_CHECK_VERSION(2, 6, 0)
	gtk_window_set_focus_on_map(GTK_WINDOW(keyboard->window), FALSE);
#endif
	gtk_window_set_type_hint(GTK_WINDOW(keyboard->window),
			GDK_WINDOW_TYPE_HINT_DOCK);
	gtk_window_stick(GTK_WINDOW(keyboard->window));
	keyboard->width = keyboard->geometry.width;
	keyboard->height = (keyboard->geometry.width / 11) * 3;
	keyboard->x = keyboard->geometry.x;
	keyboard->y = keyboard->geometry.y + keyboard->geometry.height
		- keyboard->height;
	gtk_widget_set_size_request(keyboard->window, keyboard->width,
			keyboard->height);
	gtk_window_move(GTK_WINDOW(keyboard->window), keyboard->x, keyboard->y);
	g_signal_connect_swapped(keyboard->window, "delete-event", G_CALLBACK(
				on_keyboard_delete_event), keyboard);
}

static void _new_mode_embedded(Keyboard * keyboard)
{
	keyboard->window = gtk_plug_new(0);
	keyboard->width = 0;
	keyboard->height = 0;
	keyboard->x = 0;
	keyboard->y = 0;
	g_signal_connect_swapped(keyboard->window, "embedded", G_CALLBACK(
				on_keyboard_embedded), keyboard);
}

static void _new_mode_popup(Keyboard * keyboard)
{
	keyboard->window = gtk_window_new(GTK_WINDOW_POPUP);
	gtk_container_set_border_width(GTK_CONTAINER(keyboard->window), 4);
	gtk_window_set_accept_focus(GTK_WINDOW(keyboard->window), FALSE);
	gtk_window_set_focus_on_map(GTK_WINDOW(keyboard->window), FALSE);
	keyboard->width = keyboard->geometry.width;
	keyboard->height = (keyboard->geometry.width / 11) * 3;
	keyboard->x = keyboard->geometry.x;
	keyboard->y = keyboard->geometry.y + keyboard->geometry.height
		- keyboard->height;
	gtk_window_move(GTK_WINDOW(keyboard->window), keyboard->x, keyboard->y);
	gtk_widget_set_size_request(keyboard->window, keyboard->width,
			keyboard->height);
	g_signal_connect_swapped(keyboard->window, "delete-event", G_CALLBACK(
				on_keyboard_delete_event), keyboard);
}

static void _new_mode_widget(Keyboard * keyboard)
{
	/* XXX hack */
#if GTK_CHECK_VERSION(3, 0, 0)
	keyboard->window = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
#else
	keyboard->window = gtk_hbox_new(FALSE, 4);
#endif
	keyboard->width = 0;
	keyboard->height = 0;
	keyboard->x = 0;
	keyboard->y = 0;
}

static void _new_mode_windowed(Keyboard * keyboard)
{
	keyboard->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	keyboard->width = 0;
	keyboard->height = 0;
	keyboard->x = 0;
	keyboard->y = 0;
	gtk_window_set_accept_focus(GTK_WINDOW(keyboard->window), FALSE);
	gtk_window_set_focus_on_map(GTK_WINDOW(keyboard->window), FALSE);
#if GTK_CHECK_VERSION(2, 6, 0)
	gtk_window_set_icon_name(GTK_WINDOW(keyboard->window),
			"input-keyboard");
#endif
	gtk_window_set_title(GTK_WINDOW(keyboard->window), _("Keyboard"));
	g_signal_connect_swapped(keyboard->window, "delete-event", G_CALLBACK(
				on_keyboard_delete_event), keyboard);
}


/* keyboard_delete */
void keyboard_delete(Keyboard * keyboard)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	gtk_widget_destroy(keyboard->window);
	pango_font_description_free(keyboard->font);
	object_delete(keyboard);
}


/* accessors */
/* keyboard_get_widget */
GtkWidget * keyboard_get_widget(Keyboard * keyboard)
{
	return keyboard->window;
}


/* keyboard_is_visible */
gboolean keyboard_is_visible(Keyboard * keyboard)
{
	return gtk_widget_get_visible(keyboard->window);
}


/* keyboard_set_layout */
void keyboard_set_layout(Keyboard * keyboard, unsigned int which)
{
	size_t i;
	GtkWidget * widget;

	for(i = 0; i < keyboard->layouts_cnt; i++)
		if((widget = keyboard_layout_get_widget(keyboard->layouts[i]))
				== NULL)
			continue;
		else if(i == which)
			gtk_widget_show(widget);
		else
			gtk_widget_hide(widget);
}


/* keyboard_set_modifier */
void keyboard_set_modifier(Keyboard * keyboard, unsigned int modifier)
{
	size_t i;

	for(i = 0; i < keyboard->layouts_cnt; i++)
		keyboard_layout_apply_modifier(keyboard->layouts[i], modifier);
}


/* keyboard_set_page */
void keyboard_set_page(Keyboard * keyboard, KeyboardPage page)
{
	/* FIXME really implement */
	switch(page)
	{
		case KEYBOARD_PAGE_DEFAULT:
			keyboard_set_layout(keyboard, 0);
			break;
		case KEYBOARD_PAGE_KEYPAD:
			keyboard_set_layout(keyboard, 1);
			break;
	}
}


/* useful */
/* keyboard_show */
void keyboard_show(Keyboard * keyboard, gboolean show)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(%s)\n", __func__, show ? "TRUE" : "FALSE");
#endif
	if(show == TRUE)
	{
		gtk_window_get_size(GTK_WINDOW(keyboard->window),
				&keyboard->width, &keyboard->height);
		gtk_widget_show(keyboard->window);
		gtk_window_get_position(GTK_WINDOW(keyboard->window),
				&keyboard->x, &keyboard->y);
#ifdef DEBUG
		fprintf(stderr, "DEBUG: %s() width=%d, height=%d\n", __func__,
				keyboard->width, keyboard->height);
		fprintf(stderr, "DEBUG: %s() x=%d, y=%d\n", __func__,
				keyboard->x, keyboard->y);
#endif
	}
	else if(keyboard->mode != KEYBOARD_MODE_EMBEDDED
			&& keyboard->mode != KEYBOARD_MODE_WIDGET)
		gtk_widget_hide(keyboard->window);
}


/* keyboard_show_about */
/* callbacks */
static gboolean _about_on_closex(gpointer data);

void keyboard_show_about(Keyboard * keyboard)
{
	if(keyboard->ab_window != NULL)
	{
		gtk_window_present(GTK_WINDOW(keyboard->ab_window));
		return;
	}
	keyboard->ab_window = desktop_about_dialog_new();
	gtk_window_set_transient_for(GTK_WINDOW(keyboard->ab_window),
			GTK_WINDOW(keyboard->window));
	desktop_about_dialog_set_authors(keyboard->ab_window, _authors);
	desktop_about_dialog_set_comments(keyboard->ab_window,
			_("Virtual keyboard for the DeforaOS desktop"));
	desktop_about_dialog_set_copyright(keyboard->ab_window, _copyright);
	desktop_about_dialog_set_logo_icon_name(keyboard->ab_window,
			"input-keyboard");
	desktop_about_dialog_set_license(keyboard->ab_window, _license);
	desktop_about_dialog_set_name(keyboard->ab_window, PACKAGE);
	desktop_about_dialog_set_version(keyboard->ab_window, VERSION);
	desktop_about_dialog_set_website(keyboard->ab_window,
			"https://www.defora.org/");
	g_signal_connect_swapped(keyboard->ab_window, "delete-event",
			G_CALLBACK(_about_on_closex), keyboard);
	gtk_widget_show(keyboard->ab_window);
}

/* callbacks */
static gboolean _about_on_closex(gpointer data)
{
	Keyboard * keyboard = data;

	gtk_widget_hide(keyboard->ab_window);
	return TRUE;
}


/* private */
/* keyboard_add_layout */
static void _layout_clicked(GtkWidget * widget, gpointer data);
static void _layout_selector(Keyboard * keyboard, KeyboardLayout * layout,
		KeyboardLayoutDefinition * definitions,
		size_t definitions_cnt, KeyboardLayoutSection section,
		unsigned int row, unsigned int column, unsigned width);

static GtkWidget * _keyboard_add_layout(Keyboard * keyboard,
		KeyboardLayoutDefinition * definitions,
		size_t definitions_cnt, KeyboardLayoutSection section)
{
	KeyboardLayout ** p;
	KeyboardLayout * layout;
	KeyboardKeyDefinition const * keys;
	size_t i;
	KeyboardKey * key;
	GtkWidget * widget;
#if !GTK_CHECK_VERSION(3, 0, 0)
	const GdkColor black = { 0x00000000, 0x0000, 0x0000, 0x0000 };
	const GdkColor white = { 0xffffffff, 0xffff, 0xffff, 0xffff };
	const GdkColor gray = { 0xd0d0d0d0, 0xd0d0, 0xd0d0, 0xd0d0 };
#endif

	if((p = realloc(keyboard->layouts, sizeof(*p) * (keyboard->layouts_cnt
						+ 1))) == NULL)
		return NULL;
	keyboard->layouts = p;
	if((layout = keyboard_layout_new()) == NULL)
		return NULL;
	keyboard->layouts[keyboard->layouts_cnt++] = layout;
	keys = definitions[section].keys;
	for(i = 0; keys[i].width != 0; i++)
	{
		key = keyboard_layout_add(layout, keys[i].row, keys[i].width,
				keys[i].keysym, keys[i].label);
		if(key == NULL)
			continue;
#if !GTK_CHECK_VERSION(3, 0, 0)
		keyboard_key_set_background(key, &gray);
		keyboard_key_set_foreground(key, &black);
#endif
		keyboard_key_set_font(key, keyboard->font);
		for(; keys[i + 1].width == 0 && keys[i + 1].modifier != 0; i++)
		{
#if !GTK_CHECK_VERSION(3, 0, 0)
			keyboard_key_set_background(key, &white);
#endif
			keyboard_key_set_modifier(key, keys[i + 1].modifier,
					keys[i + 1].keysym, keys[i + 1].label);
		}
	}
	_layout_selector(keyboard, layout, definitions, definitions_cnt,
			section, 3, 0, 3);
	widget = keyboard_layout_get_widget(layout);
	gtk_widget_show_all(widget);
	gtk_widget_set_no_show_all(widget, TRUE);
	gtk_widget_hide(widget);
	return widget;
}

static void _layout_clicked(GtkWidget * widget, gpointer data)
{
	Keyboard * keyboard = data;
	unsigned long d;
	KeyboardLayoutSection section;

	d = (unsigned long)g_object_get_data(G_OBJECT(widget), "layout");
	section = d;
	switch(section)
	{
		case KLS_LETTERS:
		case KLS_KEYPAD:
		case KLS_SPECIAL:
			keyboard_set_layout(keyboard, section);
			break;
	}
}

static void _layout_selector(Keyboard * keyboard, KeyboardLayout * layout,
		KeyboardLayoutDefinition * definitions,
		size_t definitions_cnt, KeyboardLayoutSection section,
		unsigned int row, unsigned int column, unsigned int width)
{
	unsigned long l;
	GtkWidget * label;
	GtkWidget * widget;
#if GTK_CHECK_VERSION(3, 0, 0)
	gboolean dark = FALSE;
	const GdkRGBA black = { 0.0, 0.0, 0.0, 1.0 };
	const GdkRGBA white = { 1.0, 1.0, 1.0, 1.0 };
#else
	const GdkColor black = { 0x00000000, 0x0000, 0x0000, 0x0000 };
	const GdkColor white = { 0xffffffff, 0xffff, 0xffff, 0xffff };
#endif

#if GTK_CHECK_VERSION(3, 0, 0)
	g_object_get(gtk_settings_get_default(),
			"gtk-application-prefer-dark-theme", &dark, NULL);
#endif
	l = (section + 1) % definitions_cnt;
	label = gtk_label_new(definitions[l].label);
#if GTK_CHECK_VERSION(3, 0, 0)
	gtk_widget_override_color(label, GTK_STATE_FLAG_NORMAL,
			dark ? &white : &black);
#else
	gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &black);
#endif
	gtk_widget_override_font(label, keyboard->font);
	widget = gtk_button_new();
#if GTK_CHECK_VERSION(3, 0, 0)
	gtk_widget_override_background_color(widget, GTK_STATE_FLAG_NORMAL,
			dark ? &black : &white);
#else
	gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, &white);
#endif
	gtk_container_add(GTK_CONTAINER(widget), label);
	g_object_set_data(G_OBJECT(widget), "layout", (void *)l);
	g_signal_connect(widget, "clicked", G_CALLBACK(_layout_clicked),
			keyboard);
	keyboard_layout_add_widget(layout, row, column, width, widget);
}


/* keyboard_error */
static void _keyboard_error(Keyboard * keyboard, char const * format, ...)
{
	va_list ap;

	/* TODO add support for error dialog messages */
	va_start(ap, format);
	fprintf(stderr, "%s: ", PROGNAME_KEYBOARD);
	vfprintf(stderr, format, ap);
	fprintf(stderr, "\n");
	va_end(ap);
}
