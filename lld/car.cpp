#include <iostream>
#include <string>
#include <vector>

// ==========================================
// 1. Car Class
// ==========================================
class Car {
private:
    std::string licensePlate;
    std::string make;
    bool isAvailable;

public:
    // Constructor using an initializer list (C++ best practice)
    Car(std::string plate, std::string make) 
        : licensePlate(plate), make(make), isAvailable(true) {}

    void rentOut() {
        isAvailable = false;
    }

    void returnCar() {
        isAvailable = true;
    }

    // Getters marked as 'const' because they don't modify the object
    std::string getLicensePlate() const { return licensePlate; }
    std::string getMake() const { return make; }
    bool getIsAvailable() const { return isAvailable; }
};

// ==========================================
// 2. Customer Class
// ==========================================
class Customer {
private:
    std::string customerId;
    std::string name;

public:
    Customer(std::string id, std::string name) 
        : customerId(id), name(name) {}

    std::string getId() const { return customerId; }
    std::string getName() const { return name; }
};

// ==========================================
// 3. Rental Class
// ==========================================
class Rental {
private:
    // Using pointers (*) because a Rental references an existing Car/Customer, 
    // it doesn't create new ones.
    Car* car;         
    Customer* customer;
    std::string rentalDate;
    std::string returnDate;

public:
    Rental(Car* c, Customer* cust, std::string rDate) 
        : car(c), customer(cust), rentalDate(rDate), returnDate("") {
        
        car->rentOut(); // Using arrow (->) to access pointer methods
    }

    void completeRental(std::string rDate) {
        this->returnDate = rDate;
        car->returnCar();
    }

    Car* getCar() const { return car; }
};

// ==========================================
// 4. CarRentalSystem Class
// ==========================================
class CarRentalSystem {
private:
    std::vector<Car*> cars;
    std::vector<Rental*> rentals;

public:
    // Destructor to clean up dynamically allocated memory
    ~CarRentalSystem() {
        for (Rental* rental : rentals) {
            delete rental; 
        }
        // Assuming cars are managed elsewhere, otherwise we'd delete them too.
    }

    void addCar(Car* car) {
        cars.push_back(car);
    }

    Rental* rentCar(std::string licensePlate, Customer* customer, std::string rentalDate) {
        Car* car = findAvailableCar(licensePlate);
        
        if (car != nullptr) { // Equivalent to Java's 'null'
            // Dynamically allocate a new Rental object using 'new'
            Rental* rental = new Rental(car, customer, rentalDate);
            rentals.push_back(rental);
            return rental;
        }
        return nullptr;
    }

private:
    // Helper method to find a car
    Car* findAvailableCar(std::string licensePlate) {
        // Standard range-based for loop (easier to read than Java Streams in C++)
        for (Car* c : cars) {
            if (c->getLicensePlate() == licensePlate && c->getIsAvailable()) {
                return c;
            }
        }
        return nullptr; 
    }
};

int main() {
    Car car1("UP32AB1234", "Toyota");

    Customer cust1("C101", "Yamini");

    CarRentalSystem system;
    system.addCar(&car1);

    Rental* rental = system.rentCar(
        "UP32AB1234",
        &cust1,
        "23-06-2026"
    );

    if (rental != nullptr) {
        std::cout << "Car rented successfully\n";

        rental->completeRental("25-06-2026");
        std::cout << "Car returned successfully\n";
    } else {
        std::cout << "Car not available\n";
    }

    return 0;
}