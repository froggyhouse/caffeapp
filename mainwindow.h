#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QPieSeries>
#include <QtCharts/QValueAxis>
#include <QVector>
#include "models/models.h"

QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString& role, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_searchEdit_textChanged(const QString& text);
    void on_addToCartButton_clicked();
    void on_checkoutButton_clicked();
    void on_addDishButton_clicked();
    void on_editPriceButton_clicked();
    void on_deleteDishButton_clicked();
    void on_themeToggle_clicked();
    void on_exportChartsButton_clicked();

private:
    Ui::MainWindow *ui;
    QString userRole;
    QVector<Dish> menu;
    QVector<Order> cart;
    QVector<Order> allOrders;

    void loadData();
    void applyRoleRestrictions();
    void updateMenuView(const QVector<Dish>& dishes);
    void updateCartView();
    void updateAdminTable();
    void generateCharts();
    void applyTheme(bool dark);
};

#endif // MAINWINDOW_H
