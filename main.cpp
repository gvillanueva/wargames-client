#include "wargamesapplication.h"
#include "logindialog.h"
#include "maindialog.h"
#include <QSslSocket>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    WarGamesApplication wga(argc, argv);
    LoginDialog ld;

    ld.exec();
    MainDialog md;
    if (ld.result() == QDialog::Accepted)
    {
        md.show();
    }

    return wga.exec();
}
