UltraMon Wallpaper Autochanger
================

UMWP Autochanger is a little tool for [UltraMon](http://www.realtimesoft.com/ultramon/) users. I has two main features :
* randomly and periodically change the wallpaper of all your monitors,
* manage sets of wallpapers, allowing you to quickly change which images are used.

Version       : 1.9  
Release date  : 24/02/2015  
Author        : Damien "Mistic" Sorel  
Website       : http://www.strangeplanet.fr/work/umwp-autochanger  
License       : GNU General Public License Version 3

[**Download lastest version**](http://www.strangeplanet.fr/work/umwp-autochanger/download.php)


## Requirement
* UltraMon 3.2.1 or higher
* Windows 7/8 32/64 bits


## Translation
The application is currently available in English and French. [A translation page](https://www.transifex.com/projects/p/umwp-autochanger) has been created on Transifex, feel free to register and contribute.


## Installation
1. Install and configure UltraMon on your computer.

2. Launch "UMWP Autochanger.exe".
    * Depending on your configuration, you might be asked to locate "UltraMonDesktop.exe".

3. Click on the plus button to add a new set. A set is basically a directory containing JPEG/PNG/BMP files. UMWP Autochanger will browse this directory recursively up to 3 levels searching for image files.

4. Once you have added your sets, the wallpaper will randomly change every 60 seconds (default delay).


## Configuration
* Right-click on a set to open a menu from which you can edit the set, change its state and delete it.

* On the "Options" dialog you can choose the delay, change the behavior of the tool and configure the hotkeys.  
Hotkeys are a way to interact with the application while it is hidden in the system bar. Hotkeys can also be defined for each individual sets.

* On the "Manage monitors" dialog you can disable the wallpaper on specific monitor, as well as choosing their background color.


## Troubleshouting
Q: When I launch UMWP Autochanger, the window doesn't appear.  
A: When you have already registered sets, the window automatically minimize to the system bar, double-click on the icon (or right-click => "Show") to open it. This behavior can be changed on the "Options" menu.

Q: How hotkeys work ?  
A: There are two kind of hotkeys: the ones defined in the "Options" dialog allow to show/hide the application window, refresh the wallpaper and start/pause the timer.  
The ones defined in the set edit dialog allow to change the current set. You can assign the same hotkey to multiple sets.  
In both cases, click on the input box and press the desired keys combinination. You must at least use one modifier (Ctrl, Shift, Alt, Win) and some keys are not available (Esc, Tab, Backspace, ...).

Q: Can I reorder sets ?  
A: Yes. Simply drag & drop them.


## Compiling
You can compile the application yourself with Qt Designer.  
You will need Qt 4.8.4.