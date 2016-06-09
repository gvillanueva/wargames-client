/*!
 * \author  Giancarlo Villanueva
 * \date    2016-05-31
 * \file    wargamesapplication.h
 * \brief   Defines the structure of the Wargames QApplication subclass.
 */

#ifndef WARGAMESAPPLICATION_H
#define WARGAMESAPPLICATION_H

#include <QApplication>

/*!
 * \brief The WargamesApplication class subclasses QApplication to provide
 *        specific behavior for a Wargames client.
 */
class WargamesApplication : public QApplication
{
public:
    WargamesApplication(int &argc, char **argv);
};

#endif // WARGAMESAPPLICATION_H
