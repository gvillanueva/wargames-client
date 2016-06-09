/*!
 * \author  Giancarlo Villanueva
 * \date    2016-05-31
 * \file    main.cpp
 * \brief   Entry point into Wargames client application.
 */

#include "wargamesapplication.h"
#include "ui/logindialog.h"
#include "ui/maindialog.h"
#include <QSslSocket>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    WargamesApplication wga(argc, argv);

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
