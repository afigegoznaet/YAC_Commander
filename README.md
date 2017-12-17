# YAC_Commander
I want to create yet another commander, since all Linux file managers are just very unhandy.
This is already my primary file manager in Linux/Xorg.
At the moment it performs all the basic file manager operations, like copy/move/delete/open.

Things to add:
* Some sort of Directory action if a drive is removed(maybe just notification)
* Context menu (in progress)
* chown / chmod
* soft/hard linking
* Plugins support
* Addition of custom links to mountpoints bar
* TBU

Implemented sutff:
* File/Directory move/add/delete
* tabs
* Save state on exit
* Disk / Mount points buttons
* asynchronows file/dir movement progress dialog
* pause/continue actions when copying/moving files
* possibility to abort moving
* possibility to remove files from queue
* Tab restore capability
* View file (in hex mode) -> using this project: https://github.com/virinext/QHexView
* Command line
* selection filters
* Search capability (by name, contents and file properties )
* File renaming
* drag and drop

Known issues:
* On Linux, dir doesn't refresh if some action is performed in the other tab, containing the same dir
* Intense search generates flickering and seems to block the GUI thread.

# NEED VOLUNTEERS! TESTERS, DEVELOPERS and IDEAS
