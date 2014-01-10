********************
INFORMATION
********************
Control Panel (for World of Warcraft emulators)
Version 3.6.7
Released 5 January 2010
By Joe Wynn / ProphetX
joewynn.nz@gmail.com

********************
FIRST-TIME USERS
********************
To ensure the control panel works properly for you, follow these steps:
	1. Open "Control Panel.exe".
	2. If the Settings form doesn't appear straight away, click on "Settings..." in the menu.
	3. Click on the "Server Type" drop-down menu and select your server type.
	4. Click on the text box beside "World Server File". Find the correct file (e.g. arcemu-world.exe) and click Open. Repeat this step for each of the server modules. If you can't find the Apache or MySQL files, tick the "Enable Advanced Settings" box and try again (you can use apache.bat and mysql.bat).
	5. Click "Save". You will be returned to Control Panel.
	6. Click "Start All". If everything is set correctly, you will see that the status of your modules changes to "Running".

********************
FEATURES
********************
--Modules automatically restart--
Each server module has a checkbox next to its Start/Stop button. Tick the checkbox to enable automatic restarting for that module.

--"Clean" operation--
Control Panel can hide all of the annoying windows created by your server. To do this:
	1. Click "Stop All".
	2. Tick the "Hide process windows" checkboxbox at the bottom-left corner of Control Panel.
	3. Start your server modules again. Control Panel will indicate that your modules are running, but you will not see their "black boxes".

--Customizable interface--
Now you can completely customize your Control Panel interface by creating your own themes, or downloading pre-made themes. For more information, visit http://prophetx.freehostia.com/help.php#themes

--Accurate module status--
Control Panel will correctly determine whether each of your modules are running, even if you started them before you started Control Panel. This intelligent feature is what sets Control Panel apart from other restarters.

--Easy to switch servers--
Control Panel provides support for ArcEmu, Ascent, and MaNGOS so you don't need to download a different control panel each time you switch to a different emulator.

--Keep track of vital statistics--
The statistics page gives a reading of the uptime for each of your server modules. It also keeps track of your RAM and CPU usage.

--Track module crashes--
Control Panel can keep track of module crashes by logging each crash. Each crash log specifies which module crashed, what time it crashed, and why it crashed. You can enable crash logging by selecting [Tools] >> [View Crash Log...] and ticking the "Enable Crash Logging" checkbox. Note: crashes can only be logged if Control Panel is used to start the server modules AND it is running when they crash.

--Export Statistics as XML--
For information on how to do this, please visit [http://www.ac-web.org/forums/tutorials/78165-tutorial-how-display-server-uptime-your-website-using-control-panel.html]


********************
CHANGELOG
********************
--3.6.7--
	* Removed the "Enable Advanced Settings" checkbox, as improved coding has removed the need for it.
	* Control Panel now has the ability to export certain statistical data as XML. This allows you to post information such as server uptime on your website.
	* Fixed a bug where Control Panel couldn't find server files even if they were there!
--3.6.6--
	* Users with limited privileges are no longer affected by the error when saving settings.
	* The file dialog boxes to find your server files don't open twice.
	* Control Panel only creates a crash log file if you have enabled crash logging.
--3.6.5--
	* The config files now rely on the logon file being set correctly. This prevents problems with multi-realm servers.
	* The MySql.Data.dll reference is no longer version-specific, i.e. you can use newer or older versions of the dll if you wish.
	* Fixed a bug that sometimes prevents you from selecting the right .exe file.
	* Removed the "I support Control Panel!" option, as there are no more Google ads.
--3.6.4--
	* Fixed a bug where the context menu was disabled
	* Tidied up the tray icon, fixed a bug where some buttons weren't showing
	* Improved the algorithm which handles the status of each module. Runs a bit more smoothly now.
--3.6.3--
	* Fixed the "internal application error" bug :)
	* Removed the Google ads because Google disabled my account for the second time :(
	* Changed the behaviour of the "Automatically Restart All Modules" checkbox.
	* Re-signed the ClickOnce manifest (certificate extended until 2010) and also re-signed the assmebly. This doesn't really affect you as a user, it just prevents people from claiming my work as their own.
--3.6.2--
	* Control Panel now keeps track of how many ads you've clicked. Eventually there will be a "Top 10" list of ad-clickers :)
	* The Statistics page no longer lags while the uptimes are queried.
	* If you are not connected to the Internet, Control Panel will no longer lag when loading ads and checking for updates.
	* The ads system has been revised, it's a tiny bit less intrusive (you can still disable it though!)
	* Now if you hover over the module labels ("MySQL, "World Server", "Logon Server", "Apache") you can see whether they are ready to be started or not. This is just based on whether Control Panel can find the module file.
	* Ticking an auto-restart box will no longer start the module. (Sorry that this has taken so long to do!)
	* Added an "Automatically Restart All Modules" checkbox. Also added a pretty "restart" icon above it :D
	* Fixed a bug where the World and Logon auto-restart checkboxes un-ticked themselves if you clicked "Stop All"
--3.6.1--
	* Fixed the themes bug - Control Panel will no longer crash if no button text colour is specified (it defaults to black)
	* Still working on fixing the lag if you're not connected to the Internet when you open Control Panel
--3.6.0--
	* Users with limited permissions will no longer get an error message when opening and saving settings (this error was caused by Windows denying Control Panel access to the registry)
	* When the World and/or Logon servers crash due to MySQL being closed, the auto-restart boxes are automatically un-checked to prevent an infinite loop of error messages.
	* Added an icon to the log viewer page.
	* The crash log viewer has been changed slightly.
	* Added an option to automatically refresh the online players list (every 20 seconds).
	* The online players page no longer disables the main Control Panel page when it is opened.
	* The auto-restart function has been revised. A MySQL crash will no longer disable your world and logon servers from automatically restarting, ONLY if you have MySQL set to auto restart!
	* Crashes are handled much more efficiently - there is no longer a slight delay/lag if MySQL crashes and forces the world server to shut down.
	* I learned some Engrish and corrected a few spelling/grammar mistakes ;)
	* Added a few more tooltips to help explain some features.
	* Disabled the maximize button on the statistics and crash log viewer pages. Because I hate you. (Just kidding! It's because they look ugly maximized!)
	* I'm REALLY sorry, but I had to implement an ad system in order to continue development. It's simple: an ad pops up when you open Control Panel, you click on it, wait 15 seconds, and now you can use Control Panel :)
	* Themes can now specify which text colour to use on buttons. http://prophetx.freehostia.com/help.php#themes for more info.
--3.5.5--
	* The "Load Defaults" button now loads the default background and text color as well.
	* Fixed some tooltips that were displaying incorrectly.
	* The Configuration page now indicates whether it can find the module files or not. A red label indicates the file cannot be found, and a green label indicates it can be found.
	* Changed the way Control Panel behaves when you first open it (it only shows the welcome message once now).
	* Fixed huge bug where Control Panel threw errors at startup.
	* Changed the "Hide Process Windows" code again (lol).
--3.5.4--
	* The Initial Settings file can now be flagged for deletion once it's finished with by adding "%Delete%" on the 6th line.
	* Revised the "Hide Process Windows" code.
	* Now after clicking "Update Settings" or "Reset Settings" you are given the option of cancelling.
	* InitialSettings.cpx commands are no longer case-sensitive.
	* When you open Control Panel there is no longer a slight pause while it detects if your modules are running.
--3.5.3--
	* Introduced the new "Intial Settings" file. More information: http://www.ac-web.org/forum/tutorials/49595-setting-up-my-control-panel-your-repack.html
	* Re-wrote some small functions to improve efficiency.
--3.5.2--
	* Fixed the bug where themes don't revert when you press Cancel.
	* Changed the startup registry entry to "Control Panel by ProphetX" (may affect some people's "Run Control Panel when Windows starts" option).
	* Fixed a bug that sometimes stopped themes from working.
	* Control Panel automatically finds your servers config files. This only works if your World file is set correctly!
	* Changed the "Running/Stopped" algorithm slightly, it now checks the server modules every 600ms rather than every 500ms. This should keep Control Panel as responsive as possible while slightly reducing CPU usage.
--3.5.1--
	* Themes have been revised! Control Panel is now about 50KB smaller and you can create your own themes :) http://prophetx.freehostia.com/help.php#themes for more information.
	* MaNGOS support has been added! Unfortunately you cannot access MaNGOS config files through Control Panel yet. This will come in a later version.
	* Setting the text color no longer causes any crashes.
--3.5.0--
	* Version 3.5.0 marks the addition of the Crash Log Viewer. You are given the option of logging every crash so that you can view them later and try to track the problem.
	* Right now I'm unable to get Control Panel to return a real reason for modules closing, but it does give some information about the crash.
	* Added a few more menu items that were missing in the context menu and system tray menu (oops :P)
	* Removed some code that I wasn't even using (lol?) which should help keep file size down.
	* Added some tooltips on buttons, labels, and other stuff.
--3.4.5--
	* Control Panel detects if it is being run for the first time and asks you to set up your server modules or import previous settings.
	* Fixed crash when trying to start a module if the file is set incorrectly.
	* Revised the config editor. It is "smarter" now, but still depends on your World Server File being set correctly.
	* Added a setting to change which table Control Panel looks for the "online" field in (for online players).
	* Slightly changed the look of some parts of the Configuration page.
	* Revised the import/reset settings code.
	* Changed the look of the "Stopped" and "Running" labels.
	* Cleaned up a lot of code to try and keep file size down.
	* The function that checks whether modules are running is now much more efficient.
--3.4.4--
	* "Control Panel was unable to find the [module] file. Do you want to find it now?" - if you click yes, the file browser now opens automatically.
	* If you set a module to auto-restart but the file location is not set correctly, you no longer get an infinite loop of error messages, yay!
	* Control Panel no longer creates the C:\Program Files\Control Panel directory (sorry about that!)
	* Fixed the weird bug that made the buttons go "flat" when you first open Control Panel
	* Control Panel remembers where you positioned it last, and all of the other windows now open relative to where the main window is.
	* When you upgrade Control Panel you can import your old settings so that you don't lose them - woot
	* You can also reset your settings to default if something terrible happens ;)
	* Control Panel auto-restarts after most crashes
--3.4.3--
	* It is now impossible to have NULL values for your server module files (go on, try it - I dare you!)
	* Fixed a bug where Control Panel thought the latest version was 3.4.0 (wasn't refreshing properly)
	* Automatic restarter for World server now functions as it should
--3.4.2--
	* Introduced an "Enable Advanced Settings" checkbox. At present this enables you to select any file you wish for your server modules rather than having the control panel specify which file to look for. In later versions there will be more "advanced settings".
	* Tidied up some code
	* Fixed a bug: when control panel first starts the buttons looked "flat". This no longer happens.
--3.4.1--
	* Included Mysql.Data.dll with Control Panel (this was the cause of the "Tools" crash)
	* Fixed the World/Logon/Realm config buttons
--3.4.0--
	* Version 3.4.0 marks the THEMES milestone! Control Panel is about to be COMPLETELY customizable (soon)!
	* Background and text color controls have been revised and a "Load Defaults" button has been added.
	* The Menu Strip (Settings, Tools, Help) now has a transparent background - it looks much cleaner
	* There is now a seperate update form  (Tools > Check For Updates...)
	* Added context menu (right-click menu)
--3.3.11--
	* Text color is now customizable
	* Module-related errors are now handled better
	* Most errors are now automatically debugged and you are (hopefully) given clear instructions on how to fix the problem
	* It's a lot easier to set up your server modules
--3.3.10--
	* Control Panel no longer uses .bat files (FINALLY) and therefore does not require a root directory.
--3.3.9--
	* Root folder now acts as a sort of install directory and is defaulted to C:\Program Files\Control Panel
	* Rather than setting folders for your server modules, you now browse for the .exe files.
	* Tidied up a lot of code which improves the performance of Control Panel.
--3.3.8--
	* Control Panel now checks if your computer is connected to the Internet before attempting to connect to the update server
	* Control Panel is now developed and tested on XP, which means there will be no more XP-unfriendly releases!
	* The Online Players page no longer hangs if it can't detect MySQL
--3.3.7--
	* Added Logon folder setting
	* More small aesthetic changes
	* Added update function which can be set to automatically check for updates whenever Control Panel starts
--3.3.6--
	* Fixed tooltips on restarter checkboxes (MySQL and Apache said "Automatically restart World Server")
	* Added option to run Control Panel when Windows start
	* Small aesthetic changes
	* Revised auto restart
--3.3.5--
	* Control Panel title bar no longer displays your server name...
	* ...Instead, your server name is displayed on the Server Module box i.e. "[Server Name] Server Modules"
	* Added auto restart function. This function also starts modules automatically when the control panel is loaded.
--3.3.4--
	* Added keyboard shortcuts - [Enter] saves settings and refreshes online players, [Escape] cancels any changes and closes windows
	* Added keyboard shortcuts - (in [Settings] >> [Server]) [W] opens the world config file, [L] opens the logon config file, [R] opens the realm config file
	* Uptimes no longer reset when Control Panel is closed
--3.3.3--
	* Server name is now updated when you save settings (you no longer have to restart Control Panel)
	* Background color settings have been moved from the main screen to [Settings] >> [Control Panel]
	* Added opacity (transparency) setting
--3.3.2--
	* Control Panel now minimizes to the system tray
	* System tray icon has a context menu which allows you to start/stop modules, edit settings, etc
	* The "Configuration" page is now the "Settings" page
	* The settings page is split up into tabs
	* There is now an option to turn off the fade-out effect which has been causing crashes on some XP machines (it's turned off by default)
	* Got rid of those stupid (?) icons next to the folder path settings, now the tooltip shows up if you hover over the text boxes
	* Fixed a bug where refreshing the online players didn't refresh "There are # players online"
--3.3.1--
	* Control panel can now detect if you are running MySQL as a service
	* You can now specify the name of your server (purely aesthetic)
--3.3.0--
	* Added an "Online Players" page which tells you how many players are online and displays a list of all the online players with their name, level, race and class.
	* When the "Tools" menu is opened, the "Online Players" item shows how many players are online e.g. "Online Players (5)"
	* Added MySQL configuration (necessary for Online Players page to function correctly)
	* Control Panel now uses XP-friendly icons (no more crashes on XP)
--3.2.4--
	* Revised menu strip, added access keys
	* Control panel now creates files in '%RootFolder&\Control Panel\' - sorry for the mess it made in your %RootFolder% previously!
--3.2.3--
	* Fixed minor bugs
	* Made slight aesthetic changes
--3.2.2--
	* MySQL uptime no longer shows up as 7000 after the Start All button is pressed
	* The Start All button no longer attempts to start modules twice
--3.2.1--
	* Fixed major bug where start buttons sometimes didn't work
	* Added code to prevent multiple instances of control panel being run
	* Fixed a lot of bugs that cause crashes
--3.2.0--
	* Added uptime statistics for all server modules
	* Added system performance (RAM & CPU usage)
--3.1.0--
	* When a different server type is selected the user is asked if they also want to change the world folder (done automatically)
	* When "Start All" is pressed, the control panel will not start modules if they are already running
	* Fixed a bug where the "Stop" buttons for World and Logon Server weren't working
	* Revised some of the tooltips in the configuration form
	* Control panel is disabled while settings are being edited

--3.0.0 (First Public Release)--
	* Compatible with Ascent and ArcEmu
	* Stop/Start modules individually, or all at once
	* Real-time information on modules tells you whether they are running or not
	* Easy access to server configuration files (*-world.conf, *-logonserver.conf, *-realms.conf)
	* Run the control panel from anywhere - no need to have it in the same folder as your server
	* Choose which folder to run modules from - Game Server, MySQL, Apache can be in different folders or even different drives
	* Customize the look of the control panel by changing the background color
	* "Clean" operation allows you to run modules in the background - gets rid of ugly black boxes
	* Real-time monitoring of modules detects crashes and attempts to recover from them
	* Control panel doesn't have to be open all the time for modules to run correctly



Note: Version numbers are defined as [Major Release].[Major Upgrade].[Minor Upgrade/Bugfixes/Other Changes]