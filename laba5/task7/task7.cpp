#include <iostream>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <memory>

class Product {
private:
    int id, storageLife, price, weight;
    std::string name;

public:
    Product(std::string const &name, int id, int storageLife, int price, int weight) :
    name(name), id(id), storageLife(storageLife), price(price), weight(weight) {}

    Product(Product const &product) :
    Product(product.name, product.id, product.storageLife, product.price, product.weight) {}

    Product &operator = (Product const &prod) {
        name = prod.name;
        id = prod.id;
        storageLife = prod.storageLife;
        price = prod.price;
        weight = prod.weight;
        return *this;
    }

    int getId() const {
        return id;
    }

    virtual ~Product() = default;

    virtual double calculateStorageFee() const {
        return weight * 0.4;
    }

    virtual void displayInfo(std::ostream &out = std::cout) const {
        out << "Name:" << name << '\n' << "ID:" << id << '\n' << "Storage life: " << storageLife << '\n'
            << "Price: " << price << '\n' << "Weight: " << weight << std::endl;
    }
};

class PerishableProduct final : public Product {
private:
    int expirationDate;

public:
    PerishableProduct(std::string const &name, int id, int storageLife, int price, int weight, int expirationDate)
            : Product(name, id, storageLife, price, weight), expirationDate(expirationDate) {}

    double calculateStorageFee() const override {
        return Product::calculateStorageFee() + (double(100) / expirationDate);
    }

    int getExpirationDate() {
        return expirationDate;
    }
};

class ElectronicProduct final : public Product {
private:
    int warrantyPeriod, powerRating;

public:
    ElectronicProduct(std::string const &name, size_t id, size_t storageLife, size_t price, size_t weight, size_t warrantyPeriod, size_t powerRating)
            : Product(name, id, storageLife, price, weight), powerRating(powerRating), warrantyPeriod(warrantyPeriod) {}

    void displayInfo(std::ostream &out = std::cout) const override
    {
        Product::displayInfo();
        out << "Warranty period: " << warrantyPeriod << '\n'
            << "Power Rating: " << powerRating << std::endl;
    }
};

class BuildingMaterial final : public Product {
private:
    int flammability;

public:
    BuildingMaterial(std::string const &name, int id, int storageLife, int price, int weight, int flammability)
            : Product(name, id, storageLife, price, weight), flammability(flammability) {}

    double calculateStorageFee() const override {
        return Product::calculateStorageFee() + flammability * 10;
    }
};

class Warehouse final {
private:
    std::unordered_map<int, std::shared_ptr<Product>> data;

public:
    void addProduct(std::shared_ptr<Product> product) {
        if (data.find(product->getId()) != data.end())
            throw std::runtime_error("Not a unique Id");
        data[product->getId()] = product;
    }

    void deleteProduct(int id) {
        data.erase(id);
    }

    double calculateAllStorageFee() {
        double price = 0;
        for (auto [id, prod] : data) {
            price += prod->calculateStorageFee();
        }
        return price;
    }

    std::vector<PerishableProduct> getExpiringProducts(int days) const {
        std::vector<PerishableProduct> resV;
        for (auto pair : data) {
            PerishableProduct *tmp = dynamic_cast<PerishableProduct *>(pair.second.get());
            if (tmp != nullptr && tmp->getExpirationDate() <= days) {
                resV.push_back(*tmp);
            }
        }
        return resV;
    }

    std::vector<std::shared_ptr<Product>> categorySearch(std::string const &categoryName) {
        std::vector<std::shared_ptr<Product>> resV;
        if (!categoryName.compare("PerishableProduct")) {
            for (auto pair : data) {
                std::shared_ptr<PerishableProduct> tmp{std::dynamic_pointer_cast<PerishableProduct>(pair.second)};
                if (tmp != nullptr) {
                    resV.push_back(pair.second);
                }
            }
        } else if (!categoryName.compare("ElectronicProduct")) {

            for (auto pair : data) {
                std::shared_ptr<ElectronicProduct> tmp{std::dynamic_pointer_cast<ElectronicProduct>(pair.second)};
                if (tmp != nullptr) {
                    resV.push_back(pair.second);
                }
            }
        } else if (!categoryName.compare("BuildingMaterial")) {

            for (auto pair : data) {
                std::shared_ptr<BuildingMaterial> tmp{std::dynamic_pointer_cast<BuildingMaterial>(pair.second)};
                if (tmp != nullptr) {
                    resV.push_back(pair.second);
                }
            }
        } else {
            throw std::logic_error("Unknown category name");
        }

        return resV;
    }

    void displayInventory(std::ostream &out = std::cout) const {
        if (data.size() == 0)
            std::cout << "Empty warehouse" << std::endl;

        std::vector<int> perishId, electId, buildId;

        for (auto [id, prod] : data) {

            if (std::shared_ptr<BuildingMaterial> tmp {
                std::dynamic_pointer_cast<BuildingMaterial>(prod)}; tmp != nullptr) {
                buildId.push_back(id);
            } else if (std::shared_ptr<PerishableProduct> tmp {
                std::dynamic_pointer_cast<PerishableProduct>(prod)}; tmp != nullptr) {
                perishId.push_back(id);
            } else if (std::shared_ptr<ElectronicProduct> tmp {
                std::dynamic_pointer_cast<ElectronicProduct>(prod)}; tmp != nullptr) {
                electId.push_back(id);
            }
        }

        std::unordered_map<int, std::shared_ptr<Product>>::const_iterator it;
        if (!buildId.empty()) {
            std::cout << "Building material category:\n";
            for (int id : buildId) {
                out << "___________________________________________\n";
                it = data.find(id);
                it->second.get()->displayInfo(out);
                out << "___________________________________________\n";
            }
        }

        if (!electId.empty()) {
            std::cout << "Electronic product category:\n";
            for (auto id : electId) {
                out << "___________________________________________\n";
                it = data.find(id);
                it->second.get()->displayInfo(out);
                out << "___________________________________________\n";
            }
        }

        if (!perishId.empty()) {
            std::cout << "Perishable product category:\n";
            for (auto id : perishId) {
                out << "___________________________________________\n";
                it = data.find(id);
                it->second.get()->displayInfo(out);
                out << "___________________________________________\n";
            }
        }
    }

    Warehouse &operator += (std::shared_ptr<Product> product) {
        addProduct(product);
        return *this;
    }

    Warehouse &operator -= (int id) {
        deleteProduct(id);
        return *this;
    }

    std::shared_ptr<Product> operator[](int id) {
        std::unordered_map<int, std::shared_ptr<Product>>::const_iterator it{data.find(id)};
        if (it == data.end())
            return nullptr;

        return it->second;
    }

    friend std::ostream &operator << (std::ostream &out, Warehouse &wh) {
        out << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        out << "Count products: " << wh.data.size() << '\n'
            << "All storage price: " << wh.calculateAllStorageFee() << "\n";
        out << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

        for (auto [id, prod] : wh.data) {
            out << "___________________________________________\n";
            prod.get()->displayInfo(out);
            out << "___________________________________________\n";
        }
        return out;
    }
};

int main() {
    Warehouse whouse;
    whouse += std::make_shared<PerishableProduct>("Banana", 0, 10, 100, 250, 8);
    whouse += std::make_shared<PerishableProduct>("Pork", 10, 3, 999, 2000, 5);
    whouse += std::make_shared<PerishableProduct>("Lettuce", 11, 5, 250, 200, 3);

    whouse += std::make_shared<ElectronicProduct>("Drill", 110, 20, 9999, 2600, 250, 45);
    whouse += std::make_shared<ElectronicProduct>("Vacuum cleaner", 121, 19, 19999, 6500, 180, 53);
    whouse += std::make_shared<ElectronicProduct>("Hair dryer", 124, 15, 4999, 2090, 90, 20);

    whouse += std::make_shared<BuildingMaterial>("Cement", 322, 90, 5000, 20000, 0);
    whouse += std::make_shared<BuildingMaterial>("Timber", 311, 20, 10000, 10000, 100);
    whouse += std::make_shared<BuildingMaterial>("Brick", 380, 70, 9000, 15000, 0);

    std::vector<PerishableProduct> v1{whouse.getExpiringProducts(5)};
    for (auto item : v1) {
        std::cout << "--------------------\n";
        item.displayInfo();
        std::cout << "--------------------\n";
    }

    std::vector<std::shared_ptr<Product>> v2{whouse.categorySearch("ElectronicProduct")};
    std::cout << "\n\nElectronic products:\n";
    for (auto item : v2) {
        std::cout << "--------------------\n";
        item.get()->displayInfo();
        std::cout << "--------------------\n";
    }

    std::cout << whouse << std::endl;
    whouse -= 0;
    whouse -= 10;
    whouse -= 11;
    whouse.displayInventory();
    std::shared_ptr<Product> pr = whouse[110];
    pr.get()->displayInfo();
    return 0;
}