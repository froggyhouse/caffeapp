#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "models/storagemanager.h"
#include "models/ordermanager.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QDate>
#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

MainWindow::MainWindow(const QString& role, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), userRole(role) {
    ui->setupUi(this);
    loadData();
    applyRoleRestrictions();
    updateMenuView(menu);
    updateCartView();
    updateAdminTable();
    generateCharts();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::loadData() {
    menu = StorageManager::loadMenu("C:/QtProjects/caffeapp/data/menu.json");
    allOrders = StorageManager::loadOrders("C:/QtProjects/caffeapp/data/orders.json");
}

void MainWindow::applyRoleRestrictions() {
    if (userRole == "client") {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->adminTab));
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->statsTab));
    }
}

void MainWindow::updateMenuView(const QVector<Dish>& dishes) {
    ui->menuList->clear();
    for (const auto& d : dishes) {
        if (!d.available) continue;
        QListWidgetItem* item = new QListWidgetItem(
            QIcon(d.imagePath),
            QString("%1\n%2 | %3 руб.").arg(d.name).arg(d.category).arg(d.price)
        );
        item->setData(Qt::UserRole, d.id);
        ui->menuList->addItem(item);
    }
}

void MainWindow::on_searchEdit_textChanged(const QString& text) {
    QVector<Dish> filtered;
    for (const auto& d : menu) {
        if (d.name.contains(text, Qt::CaseInsensitive)) filtered.push_back(d);
    }
    updateMenuView(filtered);
}

void MainWindow::on_addToCartButton_clicked() {
    auto item = ui->menuList->currentItem();
    if (!item) return;
    int id = item->data(Qt::UserRole).toInt();
    for (const auto& d : menu) {
        if (d.id == id) {
            OrderManager::addToCart(cart, d);
            break;
        }
    }
    updateCartView();
}

void MainWindow::updateCartView() {
    ui->cartList->clear();
    for (const auto& o : cart) {
        ui->cartList->addItem(QString("%1 x%2 = %3 руб.")
            .arg(o.dishName).arg(o.quantity).arg(o.price * o.quantity));
    }
    double total = OrderManager::calculateTotal(cart);
    ui->totalLabel->setText(QString("Итого: %1 руб.").arg(total));
}

void MainWindow::on_checkoutButton_clicked() {
    if (cart.isEmpty()) {
        QMessageBox::warning(this, "Корзина", "Корзина пуста!");
        return;
    }
    double total = OrderManager::calculateTotal(cart);
    for (const auto& item : cart) allOrders.push_back(item);
    StorageManager::saveOrders("data/orders.json", allOrders);
    StorageManager::generateReceipt("receipt.txt", cart, total);
    QMessageBox::information(this, "Заказ оформлен", QString("Сумма: %1 руб.\nЧек сохранён в receipt.txt").arg(total));
    cart.clear();
    updateCartView();
    generateCharts();
}

void MainWindow::on_addDishButton_clicked() {
    bool ok;
    QString name = QInputDialog::getText(this, "Добавить блюдо", "Название:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;
    double price = QInputDialog::getDouble(this, "Цена", "Введите цену:", 0, 0, 100000, 2, &ok);
    if (!ok) return;
    int newId = 1;
    for (const auto& d : menu) newId = std::max(newId, d.id + 1);
    Dish newDish(newId, name, "Горячее", price, "C:/QtProjects/caffeapp/images/default.png");
    menu.push_back(newDish);
    StorageManager::saveMenu("data/menu.json", menu);
    updateMenuView(menu);
    updateAdminTable();
}

void MainWindow::on_editPriceButton_clicked() {
    int row = ui->adminTable->currentRow();
    if (row < 0) return;
    bool ok;
    double newPrice = QInputDialog::getDouble(this, "Редактирование", "Новая цена:", menu[row].price, 0, 100000, 2, &ok);
    if (!ok) return;
    menu[row].price = newPrice;
    StorageManager::saveMenu("data/menu.json", menu);
    updateMenuView(menu);
    updateAdminTable();
}

void MainWindow::on_deleteDishButton_clicked() {
    int row = ui->adminTable->currentRow();
    if (row < 0) return;
    if (QMessageBox::question(this, "Удаление", "Удалить блюдо?") == QMessageBox::Yes) {
        menu.remove(row);
        StorageManager::saveMenu("data/menu.json", menu);
        updateMenuView(menu);
        updateAdminTable();
    }
}

void MainWindow::updateAdminTable() {
    ui->adminTable->setRowCount(menu.size());
    ui->adminTable->setColumnCount(4);
    ui->adminTable->setHorizontalHeaderLabels({"ID", "Название", "Категория", "Цена"});
    for (int i = 0; i < menu.size(); ++i) {
        ui->adminTable->setItem(i, 0, new QTableWidgetItem(QString::number(menu[i].id)));
        ui->adminTable->setItem(i, 1, new QTableWidgetItem(menu[i].name));
        ui->adminTable->setItem(i, 2, new QTableWidgetItem(menu[i].category));
        ui->adminTable->setItem(i, 3, new QTableWidgetItem(QString::number(menu[i].price)));
    }
}

void MainWindow::generateCharts() {
    // Столбчатая диаграмма
    auto revenue = OrderManager::revenueByDay(allOrders);
    QBarSet* set = new QBarSet("Выручка");
    for (auto it = revenue.begin(); it != revenue.end(); ++it) *set << it.value();
    QBarSeries* series = new QBarSeries();
    series->append(set);
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Выручка по дням");
    chart->createDefaultAxes();
    ui->salesChartView->setChart(chart);

    // Круговая диаграмма
    auto cats = OrderManager::categoryStats(allOrders);
    QPieSeries* pie = new QPieSeries();
    for (auto it = cats.begin(); it != cats.end(); ++it) pie->append(it.key(), it.value());
    QChart* pieChart = new QChart();
    pieChart->addSeries(pie);
    pieChart->setTitle("Популярность категорий");
    ui->categoryChartView->setChart(pieChart);

    // Текстовая аналитика
    double avg = OrderManager::averageCheck(allOrders);
    Order maxO = OrderManager::maxOrder(allOrders);
    Order minO = OrderManager::minOrder(allOrders);
    ui->statsLabel->setText(QString("Средний чек: %1 руб.\nСамое дорогое: %2 (%3 руб.)\nСамое дешёвое: %4 (%5 руб.)\nВсего заказов: %6")
        .arg(avg).arg(maxO.dishName).arg(maxO.price).arg(minO.dishName).arg(minO.price).arg(allOrders.size()));
}

void MainWindow::on_exportChartsButton_clicked() {
    ui->salesChartView->grab().save("sales_chart.jpeg");
    ui->categoryChartView->grab().save("category_chart.jpeg");
    QMessageBox::information(this, "Экспорт", "Графики сохранены:\nsales_chart.jpeg\ncategory_chart.jpeg");
}

void MainWindow::on_themeToggle_clicked() {
    static bool dark = false;
    dark = !dark;
    applyTheme(dark);
}

void MainWindow::applyTheme(bool dark) {
    if (dark) {
        qApp->setStyleSheet("QWidget { background-color: #2b2b2b; color: #ffffff; }"
                            "QPushButton { background-color: #3c3c3c; border: 1px solid #555; padding: 5px; }"
                            "QLineEdit, QListWidget, QTableWidget { background-color: #1e1e1e; color: #fff; }");
    } else {
        qApp->setStyleSheet("");
    }
}
