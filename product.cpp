#include "product.h"
#include <iostream>
#include <fstream>
#include <string>

// Реализация методов класса Product
Product::Product() : id(0), name(""), price(0.0), quantity(0) {}
Product::Product(int id, const std::string& name, double price, int quantity) 
    : id(id), name(name), price(price), quantity(quantity) {}

int Product::getId() const { return id; }
std::string Product::getName() const { return name; }
double Product::getPrice() const { return price; }
int Product::getQuantity() const { return quantity; }

void Product::setId(int id) { this->id = id; }
void Product::setName(const std::string& name) { this->name = name; }
void Product::setPrice(double price) { this->price = price; }
void Product::setQuantity(int quantity) { this->quantity = quantity; }

std::ostream& operator<<(std::ostream& os, const Product& product) {
    os << product.id << " " << product.name << " " << product.price << " " << product.quantity;
    return os;
}

std::istream& operator>>(std::istream& is, Product& product) {
    is >> product.id >> product.name >> product.price >> product.quantity;
    return is;
}

// Реализация методов класса Database
Database::Database() : products(nullptr), size(0), capacity(0) {}

Database::~Database() {
    delete[] products;
}

void Database::resize() {
    int newCapacity = (capacity == 0) ? 1 : capacity * 2;
    Product* newProducts = new Product[newCapacity];

    for (int i = 0; i < size; i++) {
        newProducts[i] = products[i];
    }

    delete[] products;
    products = newProducts;
    capacity = newCapacity;
}

void Database::addProduct(const Product& product) {
    if (size >= capacity) {
        resize();
    }
    products[size++] = product;
}

void Database::removeProduct(int id) {
    for (int i = 0; i < size; i++) {
        if (products[i].getId() == id) {
            for (int j = i; j < size - 1; j++) {
                products[j] = products[j + 1];
            }
            size--;
            std::cout << "Товар удален!\n";
            return;
        }
    }
    std::cout << "Товар с таким ID не найден.\n";
}

void Database::editProduct(int id) {
    for (int i = 0; i < size; i++) {
        if (products[i].getId() == id) {
            std::cout << "Текущие данные товара:\n";
            std::cout << products[i] << "\n";

            std::string name;
            double price;
            int quantity;

            std::cout << "Введите новое название: ";
            std::getline(std::cin, name);
            products[i].setName(name);

            std::cout << "Введите новую цену: ";
            std::cin >> price;
            products[i].setPrice(price);

            std::cout << "Введите новое количество: ";
            std::cin >> quantity;
            products[i].setQuantity(quantity);

            std::cin.ignore();
            std::cout << "Товар изменен!\n";
            return;
        }
    }
    std::cout << "Товар с таким ID не найден.\n";
}

void Database::printAll() const {
    if (size == 0) {
        std::cout << "В базе нет товаров.\n";
        return;
    }

    std::cout << "ID\tНазвание\tЦена\tКоличество\n";
    for (int i = 0; i < size; i++) {
        std::cout << products[i].getId() << "\t" << products[i].getName() << "\t"
                  << products[i].getPrice() << "\t" << products[i].getQuantity() << "\n";
    }
}

void Database::searchByName(const std::string& name) const {
    bool found = false;
    for (int i = 0; i < size; i++) {
        if (products[i].getName().find(name) != std::string::npos) {
            std::cout << products[i] << "\n";
            found = true;
        }
    }
    if (!found) {
        std::cout << "Товары не найдены.\n";
    }
}

void Database::searchByPrice(double maxPrice) const {
    bool found = false;
    for (int i = 0; i < size; i++) {
        if (products[i].getPrice() <= maxPrice) {
            std::cout << products[i] << "\n";
            found = true;
        }
    }
    if (!found) {
        std::cout << "Товары не найдены.\n";
    }
}

void Database::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Файл не найден. Будет создан новый при сохранении.\n";
        return;
    }

    Product temp;
    while (file >> temp) {
        addProduct(temp);
    }
    file.close();
}

void Database::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    for (int i = 0; i < size; i++) {
        file << products[i] << "\n";
    }
    file.close();
}

// Реализация функций меню
void Menu::showMainMenu() {
    std::cout << "\nМеню управления базой данных товаров:\n";
    std::cout << "1. Показать все товары\n";
    std::cout << "2. Добавить товар\n";
    std::cout << "3. Удалить товар\n";
    std::cout << "4. Редактировать товар\n";
    std::cout << "5. Поиск по названию\n";
    std::cout << "6. Поиск по цене\n";
    std::cout << "7. Выход\n";
    std::cout << "Выберите действие: ";
}

void Menu::run() {
    Database db;
    db.loadFromFile("products.txt");

    int choice;
    do {
        showMainMenu();
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1:
                db.printAll();
                break;
            case 2: {
                int id;
                std::string name;
                double price;
                int quantity;

                std::cout << "Введите ID товара: ";
                std::cin >> id;
                std::cin.ignore();

                std::cout << "Введите название товара: ";
                std::getline(std::cin, name);

                std::cout << "Введите цену товара: ";
                std::cin >> price;

                std::cout << "Введите количество: ";
                std::cin >> quantity;
                std::cin.ignore();

                db.addProduct(Product(id, name, price, quantity));
                break;
            }
            case 3: {
                int id;
                std::cout << "Введите ID товара для удаления: ";
                std::cin >> id;
                std::cin.ignore();
                db.removeProduct(id);
                break;
            }
            case 4: {
                int id;
                std::cout << "Введите ID товара для редактирования: ";
                std::cin >> id;
                std::cin.ignore();
                db.editProduct(id);
                break;
            }
            case 5: {
                std::string name;
                std::cout << "Введите название для поиска: ";
                std::getline(std::cin, name);
                db.searchByName(name);
                break;
            }
            case 6: {
                double price;
                std::cout << "Введите максимальную цену: ";
                std::cin >> price;
                std::cin.ignore();
                db.searchByPrice(price);
                break;
            }
            case 7:
                std::cout << "Выход из программы...\n";
                break;
            default:
                std::cout << "Неверный выбор. Попробуйте снова.\n";
        }

        db.saveToFile("products.txt");
    } while (choice != 7);
}