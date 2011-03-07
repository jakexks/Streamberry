#include "utilities.h"
#include "crossplatform.h"
#include <QDateTime>
#include <QString>
#include <QDebug>


Utilities::Utilities(QString path) : execpath(path)
{
    execpath += "/";
    stylesheet = makeStylesheet();
}

int Utilities::getCurrentTimestamp()
{
    return QDateTime::fromTime_t(0).secsTo(QDateTime::currentDateTime());
}

QString Utilities::getExecutePath()
{
    return execpath;
}

QString Utilities::makeStylesheet()
{
    //top bar
    QString style = "#topBarBack {background: url(EXECPATHimages/topBarBackUp.png) no-repeat; border: none; padding:0px;}";
    style+= "#topBarBack:pressed {background: url(EXECPATHimages/topBarBackDown.png) no-repeat; border: none; padding:0px;}";
    style+="#topBarForward {background: url(EXECPATHimages/topBarForwardUp.png) no-repeat; border: none; padding:0px;}";
    style+="#topBarForward:pressed {background: url(EXECPATHimages/topBarForwardDown.png) no-repeat; border: none; padding:0px;}";
    style+="#topBarMusic {background: url(EXECPATHimages/topBarMusicVideoUp.png) no-repeat; border: none; padding:0px;}";
    //style+="#topBarMusic:pressed {background: url(EXECPATHimages/topBarMusicDown.png) no-repeat; border: none; padding:0px;}";
    style+="#topBarMusicchecked {background: url(EXECPATHimages/topBarMusicDown.png) no-repeat; border: none; padding:0px;}";
    style+="#topBarVideos {background: url(EXECPATHimages/topBarMusicVideoUp.png) no-repeat; border: none; padding:0px;}";
    style+="#topBarVideos:pressed {background: url(EXECPATHimages/topBarVideoDown.png) no-repeat; border: none; padding:0px;}";
    style+="#topBarVideos:checked {background: url(EXECPATHimages/topBarVideoDown.png) no-repeat; border: none; padding:0px;}";
    style+="#topBarWidget {background: url(EXECPATHimages/topBarBackground.png); border: none; padding:0px;}";
    style+="#topBarSeparatorRight  {background: url(EXECPATHimages/topBarPlaylistsSeparatorRightAlignLeft.png); border: none; padding:0px;}";
    //bottom bar
    style+="#bottomBarWidget {background: url(EXECPATHimages/botBarBackground.png); border: none; padding:0px;}";
    style+="#bottomBarRepeat {background: url(EXECPATHimages/botBarRepeatAllUp.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarRepeat:pressed {background: url(EXECPATHimages/botBarRepeatAllDown.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarShuffle {background: url(EXECPATHimages/botBarShuffleOnUp.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarShuffle:pressed {background: url(EXECPATHimages/botBarShuffleOnDown.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarPrevious {background: url(EXECPATHimages/botBarBackUp.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarPrevious:pressed {background: url(EXECPATHimages/botBarBackDown.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarNext {background: url(EXECPATHimages/botBarNextUp.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarNext:pressed {background: url(EXECPATHimages/botBarNextDown.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarMute {background: url(EXECPATHimages/botBarVolumeOnUp.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarMute:pressed {background: url(EXECPATHimages/botBarVolumeOnDown.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarPlaypause {background: url(EXECPATHimages/botBarPlayUp.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarPlaypause:pressed {background: url(EXECPATHimages/botBarPlayDown.png) no-repeat; border: none; padding:0px;}";
    style+="#bottomBarSongInfoLine {background-color: #222222; border: none; padding:0px;}";
    //library
    style+="#libraryTableWidget {background: #292929; border: none; padding:0px; color:#e1e1e1; selection-background-color:#8FB1BB; selection-color:#0b0b0b;}";
    style+="#albumArt {background-color: #181818; border-bottom:1px solid #121212}";
    //side bar
    style+="#sideBarMainWidget {background-color: #D7E4E8; padding:0px; border-right:1px solid #9c9c9c;}";

    style+="#sideBarPreviewText {background: url(EXECPATHimages/sideBarPreviewLabel.png) no-repeat; padding:0px; margin:0px;}";
    style+="#sideBarPlaylistsText {background: url(EXECPATHimages/topBarPlaylistsLabel.png) no-repeat; padding:0px; margin:0px;}";
    style+="#sideBarPlaylistsTitle {background: url(EXECPATHimages/topBarBackground.png); border-right:1px solid #9c9c9c; padding:0px; border-bottom:1px solid #9c9c9c;}";  //border-top:1px solid #9c9c9c;
    style+="#sideBarPreviewTitle {background: url(EXECPATHimages/topBarBackground.png);  border-right:1px solid #9c9c9c; border-top:1px solid #9c9c9c; padding:0px; border-bottom:1px solid #9c9c9c;}";

    style+="#sideBarPreviewPic {background: url(EXECPATHimages/sideBarStrawberry.png) no-repeat; padding:0px; margin:0px;}";
    style+="#sideBarVeilPic {background: url(EXECPATHimages/sideBarTimeBackground.png); padding:0px; margin:0px;}";

    style+="#sideBarPreviewPane {background-color: #000000; border-right:1px solid #9c9c9c; padding:0px; margin:0px;}";
    style+="#sideBarPlaylistCell {border-right:1px solid #9c9c9c; padding:0px; margin:0px;}";

    style+="#sideBarPlaylistTableWidget {background-color: #DAE3E7;  margin-left:33px; margin-right:2px ; selection-background-color:#8FB1BB; selection-color:#0b0b0b; padding:0px; border-left:1px solid #9DA0A2; }"; //
    style+="#sideBarTopButtons {background-color: #DAE3E7; border-right:1px solid #9c9c9c;  margin-left:15px; margin-right:3px; border: none; padding:0px; selection-background-color:#8FB1BB; selection-color:#0b0b0b;}";
    style+="#sideBarDivided {background: url(EXECPATHimages/topBarPlaylistsSeparatorLeftAlignRight.png) no-repeat; padding:0px; margin:0px;}";
    //playbutton
    style+="#songInfoLine {background:#222222; border:none; margin:0px;}";
    style+="#songInfoLabel {font-weight:bold; border:none; padding:0px;}";
    style+="#songInfo{ padding:10px;}";
    //style+="#separatorSongInfo {background:#222222; border:none; padding:0px;}"; separator-color:red;
    style+="#songInfoArea {background: url(EXECPATHimages/botBarBackground.png);  border-left:1px solid #848484; padding:10px}";
    style+="#progressback {background:url(EXECPATHimages/botBarProgress.png) no-repeat; border: none; padding:0px;}";
    style+="#playbutton {background-color:rbg(0,0,0,0); background-repeat:none; background-image:url(EXECPATHimages/botBarPlayUp.png); border: none; padding:0px;}";
    style.replace("EXECPATH", execpath);

    return style;
}

QString Utilities::getStylesheet()
{
    return stylesheet;
}

// function that specifies the colors of the background of the library rows
QString Utilities::getColor(int inputVar)
{
//dark
if(inputVar==1)
    return "#1C1C1C";
//light
if(inputVar==2)
    return "#292929";
return "#000000";
}
