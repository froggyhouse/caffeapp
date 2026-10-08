#ifndef MODELS_H
#define MODELS_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>

// Сущность "Блюдо"
struct Dish {
    int id;
    QString name;
    QString category;   // "Супы", "Горячее", "Напитки", "Десерты"
    double price;
    QString imagePath;
    bool available;

    Dish() : id(0), price(0.0), available(true) {}
    Dish(int id, QString name, QString cat, double price, QString img, bool avail = true)
        : id(id), name(name), category(cat), price(price), imagePath(img), available(avail) {}

    Dish& operator=(const Dish& other) {
        if (this != &other) {
            id = other.id;
            name = other.name;
            category = other.category;
            price = other.price;
            imagePath = other.imagePath;
            available = other.available;
        }
        return *this;
    }

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["id"] = id;
        obj["name"] = name;
        obj["category"] = category;
        obj["price"] = price;
        obj["imagePath"] = imagePath;
        obj["available"] = available;
        return obj;
    }

    static Dish fromJson(const QJsonObject& obj) {
        return Dish(
            obj["id"].toInt(),
            obj["name"].toString(),
            obj["category"].toString(),
            obj["price"].toDouble(),
            obj["imagePath"].toString(),
            obj["available"].toBool(true)
        );
    }
};

// Сущность "Заказ"
struct Order {
    QString date;
    int dishId;
    QString dishName;
    double price;
    QString category;
    int quantity;

    Order() : dishId(0), price(0.0), quantity(1) {}
    Order(QString date, int id, QString name, double price, QString cat, int qty = 1)
        : date(date), dishId(id), dishName(name), price(price), category(cat), quantity(qty) {}

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["date"] = date;
        obj["dishId"] = dishId;
        obj["dishName"] = dishName;
        obj["price"] = price;
        obj["category"] = category;
        obj["quantity"] = quantity;
        return obj;
    }

    static Order fromJson(const QJsonObject& obj) {
        return Order(
            obj["date"].toString(),
            obj["dishId"].toInt(),
            obj["dishName"].toString(),
            obj["price"].toDouble(),
            obj["category"].toString(),
            obj["quantity"].toInt(1)
        );
    }
};

struct User {
    QString login;
    QString passwordHash;
    QString role;
};

#endif // MODELS_H
