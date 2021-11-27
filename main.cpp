/******************************************************************************
 * @Date    : 2021-11-23 17:08:25
 * @Author  : Constantin Petra (constantin.petra@gmail.com)
 * @Link    : http://github.com/cpetra
 * @Version : $Id$
 *
******************************************************************************/
#include "dialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();
    return a.exec();
}
