#include "file_meta.h"
#include <QString>
#include <QMessageBox>

#include <iostream>


File_Meta::File_Meta()
{
    const char * const vlc_args[] = {
              "-I", "dummy", // Don't use any interface
              "--ignore-config", // Don't use VLC's config
              "--extraintf=logger", //log anything
              "--verbose=2", //be much more verbose then normal for debugging purpose
              "--plugin-path=C:\\vlc-0.9.9-win32\\plugins\\" };
    _vlcinstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    /*QMessageBox msgBox;
    msgBox.setText(libvlc_media_get_meta(_m, libvlc_meta_Title));
    msgBox.exec();*/

}

File_Meta::~File_Meta()
{
    libvlc_media_release (_m);
    libvlc_release (_vlcinstance);
}

void File_Meta::print_meta(QString file)
{
    /* Create a new LibVLC media descriptor */
    _m = libvlc_media_new_location (_vlcinstance, file.toAscii());
    //raise(&_vlcexcep);

    libvlc_media_parse (_m);
    std::cout << libvlc_media_get_meta(_m, libvlc_meta_Title) << "\n";
}

