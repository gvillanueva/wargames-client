#include "wargamesapplication.h"
#include "logindialog.h"
#include <QSslSocket>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    WarGamesApplication wga(argc, argv);
    LoginDialog ld;
    ld.show();

    return wga.exec();
}
