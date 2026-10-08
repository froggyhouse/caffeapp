#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include <QVector>
#include <QMap>
#include "models.h"

class OrderManager {
public:
    static void addToCart(QVector<Order>& cart, const Dish& dish, int qty = 1);
    static double calculateTotal(const QVector<Order>& cart);
    static double averageCheck(const QVector<Order>& orders);
    static Order maxOrder(const QVector<Order>& orders);
    static Order minOrder(const QVector<Order>& orders);
    static QMap<QString, double> revenueByDay(const QVector<Order>& orders);
    static QMap<QString, int> categoryStats(const QVector<Order>& orders);
};

#endif // ORDERMANAGER_H
