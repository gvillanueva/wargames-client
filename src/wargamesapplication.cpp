#include "wargamesapplication.h"
#include <QSslSocket>
#include <QMessageBox>
#include <QTimer>

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

