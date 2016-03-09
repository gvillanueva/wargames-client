#include "creategamedialog.h"
#include "ui_creategamedialog.h"

#include "lobbydialog.h"

/* NOTES
 * * Maybe one day, game system will determine minimum player number.
 */

CreateGameDialog::CreateGameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateGameDialog)
{
    ui->setupUi(this);
    connect(ui->bbButtons, SIGNAL(accepted()), this, SLOT(accept()));
}

CreateGameDialog::~CreateGameDialog()
{
    delete ui;
}

void CreateGameDialog::accept()
{
    LobbyDialog lobby;
    lobby.exec();
}
