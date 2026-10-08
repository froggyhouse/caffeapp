#include "authdialog.h"
#include "ui_authdialog.h"                  //генерируется автоматически
#include "models/storagemanager.h"          // для cipherXOR
#include <QMessageBox>                      // для QMessageBox

AuthDialog::AuthDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AuthDialog)
{
    ui->setupUi(this);
    ui->passwordEdit->setEchoMode(QLineEdit::Password);  // скрыть пароль точками
}

AuthDialog::~AuthDialog()
{
    delete ui;
}

void AuthDialog::on_loginButton_clicked()
{
    QString login = ui->loginEdit->text();
    QString pass = StorageManager::cipherXOR(ui->passwordEdit->text());

    if (login == "admin" && pass == StorageManager::cipherXOR("admin123")) {
        userRole = "admin";
        accept();
    } else if (login == "client" && pass == StorageManager::cipherXOR("client123")) {
        userRole = "client";
        accept();
    } else {
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль!");
    }
}

void AuthDialog::on_cancelButton_clicked()
{
    reject();
}
