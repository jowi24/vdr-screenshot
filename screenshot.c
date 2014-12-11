/*
 * screenshot.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#include "screenshot.h"
#include <vdr/interface.h>

#include <vdr/status.h>
#include <vdr/recording.h>

#include <vdr/skins.h>

cScreenshotConfig screenshotConfig;

cScreenshotData screenshotData;

#ifdef FALSE
static const char *DESCRIPTION    = trNOOP("Takes screenshots");
static const char *MAINMENUENTRY  = trNOOP("Screenshot");
#endif


/*-------------------------------------------------------------------------------------------------
**************** class cThreadScreenshot **********************************************************
-------------------------------------------------------------------------------------------------*/

void cThreadScreenshot::Action(void) {
//FIX
//  cCondWait::SleepMs(500);
  const char *Title = screenshotData.osd_title[0] ? screenshotData.osd_title : 
                      screenshotData.title[0]     ? screenshotData.title     : "screenshot";
  int i = 0;
  int pnr = 0;
  time_t t = time(NULL);
  struct tm *now = localtime(&t);
  bool isOk = true;

  // take pictures
  while (isOk && i < screenshotConfig.iNoOfPics) {
    char fileName[100];
    FILE *fp;
    do {
      pnr++;
      now = localtime(&t);
      sprintf(fileName, "%s/%s-%d%02d%02d-%03d.%s", screenshotConfig.sPath, Title, \
    	    now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, \
	    pnr, FILEFORMATS[screenshotConfig.iFileformat]);
      fp = fopen(fileName, "r");
      if (fp) fclose(fp);
    } while (fp != NULL);
//FIX    
/*
    if (screenshotConfig.iShowDate) {
        Skins.Message(mtInfo, tmp);
        Skins.Flush();
    }
*/
    int	iResY = screenshotConfig.iResY;
    if( iResY == 0 )
    {
	iResY = screenshotConfig.iResX / (1.25 * Setup.OSDAspect);
    }
    isOk = cDevice::PrimaryDevice()->GrabImageFile(fileName, screenshotConfig.iFileformat, 
                                                             screenshotConfig.iQuality, 
							     screenshotConfig.iResX, 
							     iResY);
    i++;
  }

  if (screenshotData.osd) {
    delete screenshotData.osd;
    screenshotData.osd = NULL;
  }
    
  // show result
  if (isOk) {
    isyslog("screenshot: %d image%s saved", screenshotConfig.iNoOfPics, (screenshotConfig.iNoOfPics > 1) ? "s" : "");
    const char *tmp = tr(screenshotConfig.iNoOfPics > 1 ? "OK - Images saved." : "OK - Image saved.");
    Skins.QueueMessage(mtInfo, tmp);
    Skins.Flush();      
  } else {
    esyslog("screenshot: error calling GrabImage(...)");
    Skins.QueueMessage(mtError, tr("Error"));
    Skins.Flush();
  }

}

/*-------------------------------------------------------------------------------------------------
**************** class cMenuScreenshot ************************************************************
-------------------------------------------------------------------------------------------------*/

cMenuScreenshot::cMenuScreenshot(void) {

}

void cMenuScreenshot::Show(void) {

  time_t t = time(NULL);
  struct tm *now = localtime(&t);
  strftime(tmp, sizeof(tmp), "%Y.%m.%d %H:%M", now);
  if (screenshotConfig.iShowDate) {
    const cFont *fn = cFont::GetFont(fontSml);
    cOsd *osd = cOsdProvider::NewOsd(Setup.OSDLeft, Setup.OSDTop);
    tArea Areas[] = { { 0, Setup.OSDHeight - fn->Height(tmp), Setup.OSDWidth - 1, Setup.OSDHeight - 1, 4 } };
    osd->SetAreas(Areas, sizeof(Areas) / sizeof(tArea));
    osd->DrawRectangle(0, Setup.OSDHeight - fn->Height(tmp), Setup.OSDWidth - 1, Setup.OSDHeight - 1, clrTransparent);
    osd->DrawText(1, Setup.OSDHeight - fn->Height(tmp), tmp, (tColor)clrWhite, (tColor)clrTransparent, fn);
    osd->Flush();
    screenshotData.osd = osd;
  }  
  (new cThreadScreenshot())->Start();
}

/*-------------------------------------------------------------------------------------------------
**************** struct cScreenshotConfig **********************************************************
-------------------------------------------------------------------------------------------------*/

cScreenshotConfig::cScreenshotConfig(void) {
  strcpy(sPath, "/tmp");
  iFileformat    = 1;
  iQuality       = 95;
  iResX          = 768;
  iResY          = 576;
  iNoOfPics      = 1;
  iHideMenuEntry = false;
  iUserKey       = 0;
  iHideOsd       = true;
  iShowDate      = false;
  iDelayed       = false;
}
      
/*-------------------------------------------------------------------------------------------------
**************** class cMenuSetupScreenshot *******************************************************
-------------------------------------------------------------------------------------------------*/

void cMenuSetupScreenshot::Store(void) {
  strcpy(screenshotConfig.sPath, sNewPath);
  screenshotConfig.iFileformat    = iNewFileformat;
  screenshotConfig.iQuality       = iNewQuality;
  screenshotConfig.iResX          = iNewResX;
  screenshotConfig.iResY          = iNewResY;
  screenshotConfig.iNoOfPics      = iNewNoOfPics;
  screenshotConfig.iHideMenuEntry = iNewHideMenuEntry;
  screenshotConfig.iHideOsd       = iNewHideOsd;
  screenshotConfig.iShowDate      = iNewShowDate;
  screenshotConfig.iDelayed       = iNewDelayed;
  SetupStore("Path",          screenshotConfig.sPath);
  SetupStore("Fileformat",    screenshotConfig.iFileformat);
  SetupStore("Quality",       screenshotConfig.iQuality);
  SetupStore("ResX",          screenshotConfig.iResX);
  SetupStore("ResY",          screenshotConfig.iResY);
  SetupStore("NoOfPics",      screenshotConfig.iNoOfPics);
  SetupStore("HideMenuEntry", screenshotConfig.iHideMenuEntry);
  SetupStore("UserKey",       screenshotConfig.iUserKey);
  SetupStore("HideOsd",       screenshotConfig.iHideOsd);
  SetupStore("ShowDate",      screenshotConfig.iShowDate);
  SetupStore("Delayed",       screenshotConfig.iDelayed);
}
  
cMenuSetupScreenshot::cMenuSetupScreenshot(void) {
  strcpy(sNewPath, screenshotConfig.sPath);
  iNewFileformat    = screenshotConfig.iFileformat;
  iNewQuality       = screenshotConfig.iQuality;
  iNewResX          = screenshotConfig.iResX;
  iNewResY          = screenshotConfig.iResY;
  iNewNoOfPics      = screenshotConfig.iNoOfPics;
  iNewHideMenuEntry = screenshotConfig.iHideMenuEntry;
  iNewUserKey       = screenshotConfig.iUserKey;
  iNewHideOsd       = screenshotConfig.iHideOsd;
  iNewShowDate      = screenshotConfig.iShowDate;
  iNewDelayed       = screenshotConfig.iDelayed;
  char allowedChars[100];
  sprintf(allowedChars, "%s/", tr(FileNameChars));
  Add(new cMenuEditStrItem  (tr("Image directory"),         sNewPath, sizeof(sNewPath), allowedChars));
  Add(new cMenuEditStraItem (tr("Fileformat"),              &iNewFileformat, 2, FILEFORMATS));
  Add(new cMenuEditIntItem  (tr("Image quality (1-100)"),   &iNewQuality, 1, 100));
  Add(new cMenuEditIntItem  (tr("Image width (Pixel)"),     &iNewResX, 1, 768));
  Add(new cMenuEditIntItem  (tr("Image height (Pixel)"),    &iNewResY, 0, 576));
  Add(new cMenuEditIntItem  (tr("No. of pictures to take"), &iNewNoOfPics, 1, 100));
  Add(new cMenuEditBoolItem (tr("Hide mainmenu entry"),     &iNewHideMenuEntry));
//  Add(new cMenuEditBoolItem (tr("Hide OSD"),                &iNewHideOsd));
  Add(new cMenuEditBoolItem (tr("Show Date"),               &iNewShowDate));
  Add(new cMenuEditBoolItem (tr("Enable delayed shooting"), &iNewDelayed));
}

/*-------------------------------------------------------------------------------------------------
**************** class cPluginScreenshot **********************************************************
-------------------------------------------------------------------------------------------------*/

cPluginScreenshot::cPluginScreenshot(void) {
  // Initialize any member variables here.
  // DON'T DO ANYTHING ELSE THAT MAY HAVE SIDE EFFECTS, REQUIRE GLOBAL
  // VDR OBJECTS TO EXIST OR PRODUCE ANY OUTPUT!
  screenshotData.title[0]     = 0;
  screenshotData.osd_title[0] = 0;
  screenshotData.osd          = NULL;
  delayActive = false;
  muteCounter = 0;
  currentVolume = -1;
}

cPluginScreenshot::~cPluginScreenshot() {
  // Clean up after yourself!
}

const char *cPluginScreenshot::CommandLineHelp(void) {
  // Return a string that describes all known command line options.
  return NULL;
}

bool cPluginScreenshot::ProcessArgs(int argc, char *argv[]) {
  // Implement command line argument processing here if applicable.
  return true;
}

bool cPluginScreenshot::Initialize(void) {
  // Initialize any background activities the plugin shall perform.
  return true;
}

bool cPluginScreenshot::Start(void) {
  // Start any background activities the plugin shall perform.
  // Make dummy picture at startup, because the first screenshot using
  // GrabImage(...) only takes a gray image.
  cDevice::PrimaryDevice()->GrabImageFile("/dev/null", false);
  return true;
}

void cPluginScreenshot::Housekeeping(void) {
  // Perform any cleanup or other regular tasks.
}

const char *cPluginScreenshot::MainMenuEntry(void) { 
  return screenshotConfig.iHideMenuEntry ? NULL : tr(MAINMENUENTRY);
}  

cOsdObject *cPluginScreenshot::MainMenuAction(void) {
  if (screenshotConfig.iDelayed && muteCounter < 2) {
    delayActive = true;
    Skins.QueueMessage(mtInfo, tr("Delayed shooting active."));
    Skins.Flush();
  } else {
    muteCounter = 0;
    if (screenshotConfig.iDelayed)
      (new cThreadScreenshot())->Start();
    else {
      return new cMenuScreenshot();
    }
  }
  return NULL;
}

cMenuSetupPage *cPluginScreenshot::SetupMenu(void) {
  // Return a setup menu in case the plugin supports one.
  return new cMenuSetupScreenshot;
}

bool cPluginScreenshot::SetupParse(const char *Name, const char *Value) {
  // Parse your own setup parameters and store their values.
  if       (!strcasecmp(Name, "Path"))          strcpy(screenshotConfig.sPath, Value);
  else if  (!strcasecmp(Name, "Quality"))       screenshotConfig.iQuality       = atoi(Value);
  else if  (!strcasecmp(Name, "ResX"))          screenshotConfig.iResX          = atoi(Value);
  else if  (!strcasecmp(Name, "ResY"))          screenshotConfig.iResY          = atoi(Value);
  else if  (!strcasecmp(Name, "Fileformat"))    screenshotConfig.iFileformat    = atoi(Value);
  else if  (!strcasecmp(Name, "NoOfPics"))      screenshotConfig.iNoOfPics      = atoi(Value);
  else if  (!strcasecmp(Name, "HideMenuEntry")) screenshotConfig.iHideMenuEntry = atoi(Value);
  else if  (!strcasecmp(Name, "UserKey"))       screenshotConfig.iUserKey       = atoi(Value);
  else if  (!strcasecmp(Name, "HideOsd"))       screenshotConfig.iHideOsd       = atoi(Value);
  else if  (!strcasecmp(Name, "ShowDate"))      screenshotConfig.iShowDate      = atoi(Value);
  else if  (!strcasecmp(Name, "Delayed"))       screenshotConfig.iDelayed       = atoi(Value);
  else return false;
  return true;
}

void cPluginScreenshot::Replaying(const cControl *Control, const char *Name, const char *FileName, bool On) {
  if (Name)
    strncpy(screenshotData.title, Name, sizeof(screenshotData.title));
  else
    strcpy(screenshotData.title, "screenshot");
  while (char *s= strchr(screenshotData.title, '/'))
    s[0] = '-';
}

void cPluginScreenshot::OsdProgramme(time_t PresentTime, const char *PresentTitle, const char *PresentSubtitle, 
                                     time_t FollowingTime, const char *FollowingTitle, const char *FollowingSubtitle) {
  if (PresentTitle)
    strncpy(screenshotData.title, PresentTitle, sizeof(screenshotData.title));
  else
    strcpy(screenshotData.title, "screenshot");
  while (char *s= strchr(screenshotData.title, '/'))
    s[0] = '-';
}

void cPluginScreenshot::SetVolume(int Volume, bool Absolute) {
  if (delayActive) {
    if ((currentVolume == 0 && Volume > 0) || (currentVolume > 0 && Volume == 0))
      muteCounter++;
    if (muteCounter == 2) {
      delayActive = false;
      MainMenuAction();
    }
  }
  currentVolume = Volume;
}

void cPluginScreenshot::OsdTitle(const char *title) {
   strncpy(screenshotData.osd_title, title, sizeof(screenshotData.osd_title));
}  

void cPluginScreenshot::OsdClear() {
  screenshotData.osd_title[0] = 0;
}

VDRPLUGINCREATOR(cPluginScreenshot); // Don't touch this!
