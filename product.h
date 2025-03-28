#ifndef PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <fstream>
#include <string>

class Product {
private:
    int id;
    std::string name;
    double price;
    int quantity;

public:
    // Конструкторы
    Product();
    Product(int id, const std::string& name, double price, int quantity);

    // Геттеры
    int getId() const;
    std::string getName() const;
    double getPrice() const;
    int getQuantity() const;

    // Сеттеры
    void setId(int id);
    void setName(const std::string& name);
    void setPrice(double price);
    void setQuantity(int quantity);

    // Перегрузка операторов ввода/вывода
    friend std::ostream& operator<<(std::ostream& os, const Product& product);
    friend std::istream& operator>>(std::istream& is, Product& product);
};

class Database {
private:
    Product* products;
    int size;
    int capacity;

    void resize();

public:
    Database();
    ~Database();

    void addProduct(const Product& product);
    void removeProduct(int id);
    void editProduct(int id);
    void printAll() const;
    void searchByName(const std::string& name) const;
    void searchByPrice(double maxPrice) const;

    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;
};

namespace Menu {
    void showMainMenu();
    void run();
}

#endif