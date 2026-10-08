#include "mainwindow.h"
#include "authdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AuthDialog auth;
    if (auth.exec() != QDialog::Accepted)
        return 0;

    MainWindow w(auth.getUserRole());
    w.show();

    return a.exec();
}
