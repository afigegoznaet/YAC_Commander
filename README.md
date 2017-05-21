# YAC_Commander
I want to create yet another commander, since all Linux file managers are just very unhandy.
This is already my primary file manager in Linux/Xorg.
At the moment it performs all the basic file manager operations, like copy/move/delete/open.
Things to add:
* directory refresh on change (on Linux)
* Tab persistence
* Some sort of Directory action if a drive is removed(maybe just notification)
* Context menu

* View file
* Command line
* selection filters
* chown / chmod
* soft/hard linking
* more informative progress dialog
* Search capability
* Plugins support
* hide destination column in progress dialog
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

Known issues:
* On Linux, dir doesn't refresh if some action is performed in the other tab, containing the same dir
* recursive copying of directory into itself deeper than one level is infinite

# NEED VOLUNTEERS! TESTERS, DEVELOPERS and IDEAS
