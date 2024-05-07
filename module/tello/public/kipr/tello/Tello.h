#ifndef _KIPR_TELLO_TELLO_H_
#define _KIPR_TELLO_TELLO_H_

#include <QtCore/QString>
#include <QtCore/QObject>
class Tello : public QObject
{
    Q_OBJECT
public:
    Tello(QObject *parent = nullptr);
 
};

#endif