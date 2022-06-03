
Debian
====================
This directory contains files used to package pcoind/pcoin-qt
for Debian-based Linux systems. If you compile pcoind/pcoin-qt yourself, there are some useful files here.

## pcoin: URI support ##


pcoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install pcoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your pcoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/pcoin128.png` to `/usr/share/pixmaps`

pcoin-qt.protocol (KDE)

