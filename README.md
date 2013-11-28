UltraMon Wallpaper Autochanger
================

UMWP Autochanger is a little tool for [UltraMon](http://www.realtimesoft.com/ultramon/) users. I has two main features :
* randomly and periodically change the wallpaper of all your monitors,
* manage sets of wallpapers, allowing you to quickly change which images are used.

Version       : 1.2.1  
Release date  : ???  
Author        : Damien "Mistic" Sorel  
Website       : http://www.strangeplanet.fr/work/umwp-autochanger  
License       : GNU General Public License Version 3  


## Requirement
* UltraMon 3.2.1 or higher
* Windows 7/8 32/64 bits

## Installation
1. Install and configure UltraMon on your computer.

2. Go to the wallpaper configuration of UltraMon, and configure the "default" wallpaper as you wish (you don't need to define an image file).

3. Launch "UMWP Autochanger.exe".
    * Depending on your configuration, you might be asked to locate  "UltraMonDesktop.exe".

4. Click on "Add" to add a new set. A set is basically a directory containing  JPEG/PNG/BMP files. UMWP Autochanger will browse this directory recursively up to 3 levels searching for image files.

5. Once you have added you sets, click on "Apply", the wallpaper will randomly change every 60 seconds (default delay).


## Troubleshouting
Q: When I launch UMWP Autochanger, the window doesn't appear.  
A: When you have already registered sets, the window automatically minimize to the system bar, double-click on the icon (or right-click => "Show") to open it. This behavior can be changed on the "Options" menu.

Q: Can I rename a set ?  
A: Yes. Double-click on it's name, you will be asked to enter the new name.

Q: How hotkeys work ?  
A: Hotkeys allows you two quickly change witch set is activated from the keyboard even when the application window is hidden. You can configure which keys must be pressed in the "Options" menu. Say you choosed Shift+Ctrl, press Shift+Ctrl+1 to activate the first set, Shift+Ctrl+2 for the second set, and so on. Only the 9 first sets can be activated from the keyboard.

Q: How can I unactivate hotkeys ?  
A: Uncheck all keys in the "Options" menu.

Q: Can I reorder sets ?  
A: Yes. Simply drag & drop them.


## Known bugs
* When changing the wallpaper, with file check activated, the software freezes for some seconds, that's because I don't use multi-threading.


## Changelog
* 1.2.1 (???)
    * new: icons for main buttons (add, [de]activate, delete)
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
