# Overview 

This is a "plugin" for the Video Disk Recorder (VDR).

Written by:
* Joachim Wilke [vdr (at) joachim (minus) wilke (dot) de]

Project's homepage:          https://github.com/jowi24/vdr-screenshot

Latest version available at: https://github.com/jowi24/vdr-screenshot/releases

See the file COPYING for license information.

---

# Description

With this plugin you can take still images of your screen. After installing
the plugin, a new mainmenu entry "Screenshot" will show up. Each time you
select this item, a file `/tmp/title-yyyymmdd-hhmmss.jpg` will be created,
where title is the current transmission or the recording currently replayed.

To get a screenshot with a single keypress, add a line like this to your
keymacros.conf:

...
User5    @screenshot
...

After that, pressing your user-button 5 will create a screenshot.
You can use andy User button out of User1 - User9. If you dont know, what your
user-keys are, delete the file remote.conf. After restart, VDR will guide you throgh the
reassignment of your remotecontrol.

At the moment, it is only possible to make screenshots of the tv screen withour any 
osd, because calling a plugin in vdr causes the osd to be cleared.

## New function "delayed shooting"

If enabled in the settings menu, selecting "Screenshot" in the main menu (or pressing the 
corresponding key on your remote), causes the plugin to wait for you to press the Mute-key 
twice and then takes the screenshot(s). This way, screenshots of arbitrary osd constellations 
can be taken. Without delayed shooting, this is not possible, because pressing the 
assigned remote-button for Screenshot causes the osd to be closed and calling the plugin from
the main menu disturbs the current osd either.	

## Only for vdr users, using vdr-1.3.32 or older

If you want to hide the mainmenu entry you will have to apply the 
`patch-keymacros-for-hidden-plugins.diff` in the `patch/` subdir, otherwise
the user-button will stop to work, too.

# Troubleshooting

If you experience problems when using the plugin, check

- if your image-directory exists and is writable to the vdr user

- if you have the correct device node for your video device (`/dev/video0` or similar)

When you find an error, I would be happy, if you send me a bug-report. When you expect
a crash please include a backtrace (using gdb or similar) if possible.
