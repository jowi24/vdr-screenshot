/*
 * screenshot.h: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef _SCREENSHOT__H
#define _SCREENSHOT__H
 
#include <vdr/plugin.h>
#include <vdr/status.h>

static const char *VERSION        = "0.0.12";
static const char *DESCRIPTION    = "Takes screenshots";
static const char *MAINMENUENTRY  = "Screenshot";

static const char *FILEFORMATS[10] = {"pnm", "jpg"};

class cMenuScreenshot : public cOsdObject {
private:
  char tmp[20];
  const char *title;
  int showdate;
public:
  cMenuScreenshot(void);
  void Show(void);
  virtual eOSState ProcessKey(eKeys Key) { return osEnd; }
};

class cPluginScreenshot : public cPlugin, public cStatus {
private:
  bool delayActive;
  int muteCounter;
  int currentVolume;
#if VDRVERSNUM < 10338
  virtual void Replaying(const cControl *Control, const char *Name);
#else
  virtual void Replaying(const cControl *Control, const char *Name, const char *FileName, bool On);
#endif
  virtual void OsdProgramme(time_t PresentTime, const char *PresentTitle, const char *PresentSubtitle, time_t FollowingTime, const char *FollowingTitle, const char *FollowingSubtitle);
  virtual void SetVolume(int Volume, bool Absolute);
  virtual void OsdTitle(const char *Title);
  virtual void OsdClear();
				  
public:
  cPluginScreenshot(void);
  virtual ~cPluginScreenshot();
  virtual const char *Version(void) { return VERSION; }
  virtual const char *Description(void) { return tr(DESCRIPTION); }
  virtual const char *CommandLineHelp(void);
  virtual bool ProcessArgs(int argc, char *argv[]);
  virtual bool Initialize(void);
  virtual bool Start(void);
  virtual void Housekeeping(void);
  virtual const char *MainMenuEntry(void);
  virtual cOsdObject *MainMenuAction(void);
  virtual cMenuSetupPage *SetupMenu(void);
  virtual bool SetupParse(const char *Name, const char *Value);
};

class cThreadScreenshot : public cThread {
private:
public:
  virtual void Action(void);
};
  
class cMenuSetupScreenshot : public cMenuSetupPage {
private:
  char sNewPath[100];
  int  iNewFileformat;
  int  iNewQuality;
  int  iNewResX;
  int  iNewResY;
  int  iNewNoOfPics;
  int  iNewHideMenuEntry;
  int  iNewUserKey;
  int  iNewHideOsd;
  int  iNewShowDate;
  int  iNewDelayed;
protected:
  virtual void Store(void);
public:
  cMenuSetupScreenshot(void);
};

struct cScreenshotConfig {
public:
  cScreenshotConfig(void);
  char sPath[100];
  int  iFileformat;
  int  iQuality;
  int  iResX;
  int  iResY;
  int  iNoOfPics;
  int  iHideMenuEntry;
  int  iUserKey;
  int  iHideOsd;
  int  iShowDate;
  int  iDelayed;
};

extern cScreenshotConfig screenshotConfig;

struct cScreenshotData {
public:
  char title[100];
  char osd_title[100];
  cOsd *osd;
};

extern cScreenshotData screenshotData;

#endif
