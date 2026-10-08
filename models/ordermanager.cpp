#include "ordermanager.h"
#include <QDate>
#include <algorithm>

void OrderManager::addToCart(QVector<Order>& cart, const Dish& dish, int qty) {
    for (auto& item : cart) {
        if (item.dishId == dish.id) {
            item.quantity += qty;
            return;
        }
    }
    cart.push_back(Order(QDate::currentDate().toString("yyyy-MM-dd"),
                         dish.id, dish.name, dish.price, dish.category, qty));
}

double OrderManager::calculateTotal(const QVector<Order>& cart) {
    double total = 0;
    for (const auto& item : cart) total += item.price * item.quantity;
    return total;
}

double OrderManager::averageCheck(const QVector<Order>& orders) {
    if (orders.isEmpty()) return 0.0;
    double sum = 0;
    for (const auto& o : orders) sum += o.price * o.quantity;
    return sum / orders.size();
}

Order OrderManager::maxOrder(const QVector<Order>& orders) {
    if (orders.isEmpty()) return Order();
    return *std::max_element(orders.begin(), orders.end(),
        [](const Order& a, const Order& b) { return a.price < b.price; });
}

Order OrderManager::minOrder(const QVector<Order>& orders) {
    if (orders.isEmpty()) return Order();
    return *std::min_element(orders.begin(), orders.end(),
        [](const Order& a, const Order& b) { return a.price < b.price; });
}

QMap<QString, double> OrderManager::revenueByDay(const QVector<Order>& orders) {
    QMap<QString, double> result;
    for (const auto& o : orders) result[o.date] += o.price * o.quantity;
    return result;
}

QMap<QString, int> OrderManager::categoryStats(const QVector<Order>& orders) {
    QMap<QString, int> result;
    for (const auto& o : orders) result[o.category] += o.quantity;
    return result;
}
