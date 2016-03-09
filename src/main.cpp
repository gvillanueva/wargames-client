#include "wargamesapplication.h"
#include "ui/logindialog.h"
#include "ui/maindialog.h"
#include <QSslSocket>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    WarGamesApplication wga(argc, argv);

    // Show login dialog
    LoginDialog ld;
    ld.exec();
    if (ld.result() != QDialog::Accepted)
        return 0;

    // Show main dialog
    MainDialog md;
    md.show();

    return wga.exec();
}
