/*!
 * \author  Giancarlo Villanueva
 * \date    2016-03-09
 * \file    gamedialog.cpp
 * \brief   Defines the behavior of the Game dialog.
 */

#include "gamedialog.h"
#include "ui_gamedialog.h"

GameDialog::GameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameDialog)
{
    ui->setupUi(this);
}

GameDialog::~GameDialog()
{
    delete ui;
}

void GameDialog::setGame(const Game& value)
{

}
