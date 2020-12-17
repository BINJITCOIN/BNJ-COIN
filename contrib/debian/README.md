
Debian
====================
This directory contains files used to package binjitd/binjit-qt
for Debian-based Linux systems. If you compile binjitd/binjit-qt yourself, there are some useful files here.

## binjit: URI support ##


binjit-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install binjit-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your binjit-qt binary to `/usr/bin`
and the `../../share/pixmaps/binjit128.png` to `/usr/share/pixmaps`

binjit-qt.protocol (KDE)

