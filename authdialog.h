#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QDialog>

// Пространство имён для UI-класса, сгенерированного Qt Designer
QT_BEGIN_NAMESPACE
namespace Ui { class AuthDialog; }
QT_END_NAMESPACE

class AuthDialog : public QDialog
{
    Q_OBJECT

public:
    // Конструктор: принимает родительский виджет (по умолчанию nullptr)
    explicit AuthDialog(QWidget *parent = nullptr);

    // Деструктор
    ~AuthDialog();

    // Геттер для получения роли пользователя после успешного входа
    // Возвращает "admin" или "client"
    QString getUserRole() const { return userRole; }

private slots:
    // Слот, вызываемый при нажатии кнопки «Войти»
    // Создаётся автоматически через "Go to slot" или пишется вручную
    void on_loginButton_clicked();

    // Слот для кнопки «Отмена» (если вы её добавили как QPushButton)
    // Если используете QDialogButtonBox — этот слот не нужен
    void on_cancelButton_clicked();

private:
    Ui::AuthDialog *ui;  // Указатель на UI-форму
    QString userRole;    // Роль: "admin" или "client"
};

#endif // AUTHDIALOG_H
