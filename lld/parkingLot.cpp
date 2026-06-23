#include <iostream>
#include <vector>
#include <string>

// ==========================================
// 1. Enums (The Rules)
// ==========================================
enum class VehicleType {
    MOTORCYCLE,
    CAR
};

// ==========================================
// 2. Vehicle Class
// ==========================================
class Vehicle {
private:
    std::string licensePlate;
    VehicleType type;

public:
    Vehicle(std::string plate, VehicleType t) : licensePlate(plate), type(t) {}

    std::string getLicensePlate() const { return licensePlate; }
    VehicleType getType() const { return type; }
};

// ==========================================
// 3. Parking Spot Class
// ==========================================
class ParkingSpot {
private:
    int spotNumber;
    VehicleType spotType;
    bool isFree;
    Vehicle* parkedVehicle; 

public:
    ParkingSpot(int number, VehicleType type) 
        : spotNumber(number), spotType(type), isFree(true), parkedVehicle(nullptr) {}

    bool isAvailable() const { return isFree; }
    VehicleType getSpotType() const { return spotType; }
    int getSpotNumber() const { return spotNumber; }

    bool assignVehicle(Vehicle* v) {
        if (!isFree || v->getType() != spotType) {
            return false; 
        }
        parkedVehicle = v;
        isFree = false;
        return true;
    }

    void removeVehicle() {
        parkedVehicle = nullptr;
        isFree = true;
    }
};

// ==========================================
// 4. Parking Lot Class (The Manager)
// ==========================================
class ParkingLot {
private:
    std::vector<ParkingSpot> spots;

public:
    ParkingLot(int numMotorcycleSpots, int numCarSpots) {
        int spotId = 1;
        for (int i = 0; i < numMotorcycleSpots; i++) {
            spots.push_back(ParkingSpot(spotId++, VehicleType::MOTORCYCLE));
        }
        for (int i = 0; i < numCarSpots; i++) {
            spots.push_back(ParkingSpot(spotId++, VehicleType::CAR));
        }
    }

    bool parkVehicle(Vehicle* v) {
        for (ParkingSpot& spot : spots) {
            if (spot.isAvailable() && spot.getSpotType() == v->getType()) {
                spot.assignVehicle(v);
                std::cout << "Parked vehicle " << v->getLicensePlate() 
                          << " at spot " << spot.getSpotNumber() << "\n";
                return true;
            }
        }
        std::cout << "Lot is full for vehicle type!\n";
        return false;
    }

    void removeVehicle(int spotNumber) {
        for (ParkingSpot& spot : spots) {
            if (spot.getSpotNumber() == spotNumber && !spot.isAvailable()) {
                spot.removeVehicle();
                std::cout << "Cleared spot " << spotNumber << "\n";
                return;
            }
        }
        std::cout << "Spot already empty or invalid.\n";
    }
};

// ==========================================
// 5. Main Execution
// ==========================================
int main() {
    // 1. Initialize lot with 1 motorcycle spot and 1 car spot
    ParkingLot myLot(1, 1);

    // 2. Create vehicles
    Vehicle myCar("ABC-123", VehicleType::CAR);
    Vehicle myBike("XYZ-999", VehicleType::MOTORCYCLE);
    Vehicle extraCar("BAD-000", VehicleType::CAR); // This one shouldn't fit

    // 3. Test parking
    myLot.parkVehicle(&myCar);     // Should succeed
    myLot.parkVehicle(&myBike);    // Should succeed
    myLot.parkVehicle(&extraCar);  // Should fail (lot is full for cars)

    // 4. Test leaving
    myLot.removeVehicle(2);        // Car leaves (Spot 2)
    myLot.parkVehicle(&extraCar);  // Should succeed now!

    return 0;
}