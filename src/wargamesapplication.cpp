/*!
 * \author  Giancarlo Villanueva
 * \date    2016-05-31
 * \file    wargamesapplication.cpp
 * \brief   Defines the behavior of the Wargames QApplication subclass.
 */

#include "wargamesapplication.h"
#include <QSslSocket>
#include <QMessageBox>
#include <QTimer>

/*!
 * \brief WargamesApplication::WargamesApplication
 * \param argc
 * \param argv
 */
WargamesApplication::WargamesApplication(int &argc, char **argv) :
    QApplication(argc, argv)
{
    QCoreApplication::setOrganizationName("Wargames");
    QCoreApplication::setApplicationName("Wargames");
    QCoreApplication::setApplicationVersion("0.0.0.1");

    if (!QSslSocket::supportsSsl()) {
        QMessageBox::critical(NULL,
                              tr("Missing SSL Support"),
                              tr("Please install OpenSSL and try again."));
        QTimer::singleShot(0, this, SLOT(quit()));
        return;
    }
}

