#include "storagemanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTextStream>
#include <QDateTime>

QVector<Dish> StorageManager::loadMenu(const QString& path) {
    QVector<Dish> menu;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return menu;
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    for (const auto& item : doc.array()) {
        menu.push_back(Dish::fromJson(item.toObject()));
    }
    return menu;
}

bool StorageManager::saveMenu(const QString& path, const QVector<Dish>& menu) {
    QJsonArray arr;
    for (const auto& d : menu) arr.append(d.toJson());
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) return false;
    file.write(QJsonDocument(arr).toJson());
    return true;
}

QVector<Order> StorageManager::loadOrders(const QString& path) {
    QVector<Order> orders;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return orders;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    for (const auto& item : doc.array()) {
        orders.push_back(Order::fromJson(item.toObject()));
    }
    return orders;
}

bool StorageManager::saveOrders(const QString& path, const QVector<Order>& orders) {
    QJsonArray arr;
    for (const auto& o : orders) arr.append(o.toJson());
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) return false;
    file.write(QJsonDocument(arr).toJson());
    return true;
}

QString StorageManager::cipherXOR(const QString& input, char key) {
    QString result = input;
    for (int i = 0; i < result.size(); ++i) {
        result[i] = QChar(result[i].unicode() ^ key);
    }
    return result;
}

bool StorageManager::generateReceipt(const QString& path, const QVector<Order>& cart, double total) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    QTextStream out(&file);
    out << "========================================\n";
    out << "       КАФЕ \"БЫСТРЫЙ ОБЕД\"\n";
    out << "========================================\n";
    out << "Дата: " << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm") << "\n\n";
    for (const auto& item : cart) {
        out << QString("%1 x%2 ......... %3 руб.\n")
                   .arg(item.dishName).arg(item.quantity).arg(item.price * item.quantity);
    }
    out << "\n----------------------------------------\n";
    out << "ИТОГО: " << total << " руб.\n";
    out << "========================================\n";
    out << "Спасибо за заказ!\n";
    file.close();
    return true;
}
