UltraMon Wallpaper Autochanger
================

UMWP Autochanger is a little tool for [UltraMon](http://www.realtimesoft.com/ultramon/) users. I has two main features :
* randomly and periodically change the wallpaper of all your monitors,
* manage sets of wallpapers, allowing you to quickly change which images are used.

Version       : 1.5  
Release date  : 12/02/2014  
Author        : Damien "Mistic" Sorel  
Website       : http://www.strangeplanet.fr/work/umwp-autochanger  
License       : GNU General Public License Version 3  


## Requirement
* UltraMon 3.2.1 or higher
* Windows 7/8 32/64 bits


## Installation
1. Install and configure UltraMon on your computer.

2. Go to the wallpaper configuration of UltraMon and ensure that the profile "default" exists, if not, create it. You don't need to configure anything else on this screen.

3. Launch "UMWP Autochanger.exe".
    * Depending on your configuration, you might be asked to locate "UltraMonDesktop.exe".

4. Click on the plus button to add a new set. A set is basically a directory containing JPEG/PNG/BMP files. UMWP Autochanger will browse this directory recursively up to 3 levels searching for image files.

5. Once you have added your sets, click on the disk button, the wallpaper will randomly change every 60 seconds (default delay).


## Configuration
* Double-click on a set to edit it. You can change its name, its style (one image for each monitor or one image for the whole desktop) and the transformation style for images (center, tile or stretch).

* On the "Options" dialog you can choose the delay, change the behavior of the tool and configure the hotkeys.  
Hotkeys are a way to interact with the application while it is hidden in the the system bar. Hotkeys can also be defined for each individual sets.


## Troubleshouting
Q: When I launch UMWP Autochanger, the window doesn't appear.  
A: When you have already registered sets, the window automatically minimize to the system bar, double-click on the icon (or right-click => "Show") to open it. This behavior can be changed on the "Options" menu.

Q: Can I edit a set ?  
A: Yes. Double-click on it's name, you can set the name, the type and the style of wallpaper and the hotkey of the set.

Q: How hotkeys work ?  
A: There are two kind of hotkeys: the ones defined in the "Options" dialog allow to show/hide the application window, refresh the wallpaper and start/pause the timer.  
The ones defined in the set edit dialog allow to change the current set. You can assign the same hotkey to multiple sets.  
In both cases, click on the input box and press the desired keys combinination. You must at least use one modifier (Ctrl, Shift, Alt, Win) and some keys are not available (Esc, Tab, Backspace, ...). Press Esc or Backspace to clear the hotkey.

Q: Can I reorder sets ?  
A: Yes. Simply drag & drop them.


## Compiling
You can compile the application yourself with Qt Designer.  
You will need Qt 4.8.4 and libqxt 0.6.2


## Changelog
* [1.5 (12/02/2014)](http://www.strangeplanet.fr/work/umwp-autochanger/download.php?ver=1.5)
    * new: export and import XML configuration file
    * new: check UltraMon version
    * new: add logger for debuging purposes
    * new: "Active files" dialog with cliquable thumbnails for direct access to files
    * fixed: better detection of UltraMon executable
    * fixed: no more freeze when updating wallpaper

* [1.4 (06/01/2014)](http://www.strangeplanet.fr/work/umwp-autochanger/download.php?ver=1.4)
    * new: more flexibility for hot keys + new hotkeys for refresh, start/pause, show/hide
    * new: permanently save last used directory

* [1.3 (03/12/2013)](http://www.strangeplanet.fr/work/umwp-autochanger/download.php?ver=1.3)
    * new: can choose the type of wallpaper ('monitor' or 'desktop') and the style ('center', 'tile', 'stretch', 'stretch proportional')
    * new: icons for main buttons (add, [de]activate, delete)
    * change: dedicated configuration dialog
    * change: use a cache to speed up startup
    * change: random images are taken from the same (random too) set if more than one is active
    * fixed: refresh issue of the list when deleting sets

* [1.2 (09/09/2013)](http://www.strangeplanet.fr/work/umwp-autochanger/download.php?ver=1.2)
    * new: system-wide hotkeys for quick change
    * new: reorder sets with drag'n'drop
    * new: integrated help

* [1.1 (13/05/2013)](http://www.strangeplanet.fr/work/umwp-autochanger/download.php?ver=1.1)
    * fixed: use better shuffle
    * fixed: remember last used directory
    * fixed: accept files with .jpeg extension
    * new: add version checker
    * new: add quick switch menu in tray con
    * code optimizations

* [1.0 (02/03/2013)](http://www.strangeplanet.fr/work/umwp-autochanger/download.php?ver=1.0)
    * fixed: multi delete doesn't work correclty

* [1.0 alpha2 (28/02/2013)](http://www.strangeplanet.fr/work/umwp-autochanger/download.php?ver=1.0a2)
    * fixed: autorun shortcut not starting in the good working directory
    * fixed: delay spinbox no initialized correctly
    * new: multiple selection

* [1.0 alpha1 (27/02/2013)](http://www.strangeplanet.fr/work/umwp-autochanger/download.php?ver=1.0a1)
    * first version
