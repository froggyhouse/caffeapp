#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include <QString>
#include <QVector>
#include "models.h"

class StorageManager {
public:
    static QVector<Dish> loadMenu(const QString& path);
    static bool saveMenu(const QString& path, const QVector<Dish>& menu);
    static QVector<Order> loadOrders(const QString& path);
    static bool saveOrders(const QString& path, const QVector<Order>& orders);
    static QString cipherXOR(const QString& input, char key = 'K');
    static bool generateReceipt(const QString& path, const QVector<Order>& cart, double total);
};

#endif // STORAGEMANAGER_H
