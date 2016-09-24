DeforaOS Keyboard
=================

About Keyboard
--------------

This program emulates a physical keyboard by providing a virtual interface on
screen. It currently supports four modes:
 * docked, where it will occupy the bottom of the screen;
 * popup, likewise as a popup window;
 * windowed, where it can be found in the system tray (default)
 * embedded, which outputs its window ID for inclusion within another program.

Additionally, it can be piloted through its companion tool, `desktopctl(1)`.

Compiling Keyboard
------------------

The current requirements for compiling Keyboard are as follows:
 * Gtk+ 2.4 or later, or Gtk+ 3.0 or later
 * DeforaOS libDesktop
 * an implementation of `make`
 * gettext (libintl) for translations

With these installed, the following command should be enough to compile Keyboard
on most systems:

    $ make

On some systems, the Makefiles shipped can be re-generated accordingly thanks to
the DeforaOS configure tool.

The compilation process supports a number of options, such as PREFIX and DESTDIR
for packaging and portability, or OBJDIR for compilation outside of the source
tree.

Distributing Keyboard
---------------------

DeforaOS Keyboard is subject to the terms of the 2-clause BSD license. Please
see the `COPYING` file for more information.
