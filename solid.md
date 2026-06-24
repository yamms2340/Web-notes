# SOLID Principles — Interview Ready (C++)

---

# 1. Single Responsibility Principle (SRP)

## Definition

> A class should have one and only one reason to change.

A class should do **one job** and do it well.

---

## Bad Example

```cpp
class User {
public:
    void updateProfile();
    void saveToDatabase();
    void sendEmail();
};
```

Responsibilities:

* User Management
* Database Operations
* Email Operations

Multiple reasons to change.

---

## Good Example

```cpp
class User {
public:
    void updateProfile();
};

class UserRepository {
public:
    void save(User& user);
};

class EmailService {
public:
    void sendEmail();
};
```

Each class has a single responsibility.

---

## Benefits

* Easier maintenance
* Easier testing
* Lower coupling
* Better readability

---

## Interview Answer

SRP states that a class should have only one reason to change. A class should encapsulate a single responsibility. For example, user management, database persistence, and email sending should be handled by separate classes rather than a single class.

---

## Quick Check

If you describe a class as:

> "This class does X and Y"

it probably violates SRP.

---

# 2. Open/Closed Principle (OCP)

## Definition

> Software entities should be open for extension but closed for modification.

We should be able to add new behavior without modifying existing code.

---

## Bad Example

```cpp
class PaymentProcessor {
public:
    void pay(string type) {
        if(type == "CreditCard")
            cout << "Credit Card Payment";
        else if(type == "UPI")
            cout << "UPI Payment";
        else if(type == "PayPal")
            cout << "PayPal Payment";
    }
};
```

Adding a new payment method requires modifying existing code.

Violation of OCP.

---

## Good Example

```cpp
class PaymentMethod {
public:
    virtual void pay() = 0;
    virtual ~PaymentMethod() = default;
};

class CreditCard : public PaymentMethod {
public:
    void pay() override {
        cout << "Credit Card Payment";
    }
};

class UPI : public PaymentMethod {
public:
    void pay() override {
        cout << "UPI Payment";
    }
};
```

Usage:

```cpp
PaymentMethod* payment = new UPI();
payment->pay();
```

Adding a new payment method:

```cpp
class PayPal : public PaymentMethod {
public:
    void pay() override {
        cout << "PayPal Payment";
    }
};
```

No existing code changes.

---

## Benefits

* Stable codebase
* Reduced bugs
* Easier feature additions

---

## Interview Answer

OCP states that classes should be open for extension but closed for modification. In C++, this is commonly achieved using abstraction and polymorphism. New functionality is added through derived classes instead of modifying existing code.

---

## Achieved Through

* Abstract Classes
* Pure Virtual Functions
* Runtime Polymorphism

---

# 3. Liskov Substitution Principle (LSP)

## Definition

> Objects of derived classes should be replaceable with objects of their base classes without affecting program correctness.

---

## Good Example

```cpp
class Bird {
public:
    virtual void eat() {}
};

class Sparrow : public Bird {};
class Pigeon : public Bird {};
```

Usage:

```cpp
Bird* bird = new Sparrow();
bird->eat();
```

Works correctly.

---

## Violation Example

```cpp
class Bird {
public:
    virtual void fly() {}
};

class Ostrich : public Bird {
public:
    void fly() override {
        throw runtime_error("Ostrich cannot fly");
    }
};
```

Usage:

```cpp
Bird* bird = new Ostrich();
bird->fly();
```

Unexpected behavior.

Violation of LSP.

---

## Better Design

```cpp
class Bird {
public:
    virtual void eat() {}
};

class FlyingBird : public Bird {
public:
    virtual void fly() = 0;
};

class Sparrow : public FlyingBird {
public:
    void fly() override {}
};

class Ostrich : public Bird {};
```

Now Ostrich is not forced to fly.

---

## Benefits

* Reliable inheritance hierarchy
* Correct polymorphism
* Predictable behavior

---

## Interview Answer

LSP states that derived classes must be substitutable for their base classes without breaking program behavior. A subclass should honor the contract established by the base class and should not introduce unexpected behavior.

---

## Quick Check

If a derived class contains:

```cpp
throw runtime_error("Not Supported");
```

or

```cpp
throw NotImplementedException;
```

there is a good chance LSP is violated.

---

# 4. Interface Segregation Principle (ISP)

## Definition

> Clients should not be forced to depend on methods they do not use.

Prefer many small interfaces over one large interface.

---

## Bad Example

```cpp
class IWorker {
public:
    virtual void work() = 0;
    virtual void eat() = 0;
};
```

Robot:

```cpp
class Robot : public IWorker {
public:
    void work() override {}

    void eat() override {
        // meaningless implementation
    }
};
```

Violation of ISP.

---

## Good Example

```cpp
class IWorkable {
public:
    virtual void work() = 0;
};

class IEatable {
public:
    virtual void eat() = 0;
};
```

Human:

```cpp
class Human : public IWorkable, public IEatable {
public:
    void work() override {}
    void eat() override {}
};
```

Robot:

```cpp
class Robot : public IWorkable {
public:
    void work() override {}
};
```

Perfect segregation.

---

## Benefits

* Smaller interfaces
* Lower coupling
* Better flexibility

---

## Interview Answer

ISP states that clients should not be forced to implement methods they do not use. Instead of large interfaces, we should design smaller and focused interfaces tailored to specific client needs.

---

## Achieved Through

* Multiple Interface Inheritance
* Pure Virtual Classes
* Composition

---

# 5. Dependency Inversion Principle (DIP)

## Definition

> High-level modules should not depend on low-level modules. Both should depend on abstractions.

Also:

> Program to interfaces, not implementations.

---

## Bad Example

```cpp
class MySQLDatabase {
public:
    void save() {}
};

class OrderService {
private:
    MySQLDatabase db;
};
```

OrderService is tightly coupled to MySQL.

Violation of DIP.

---

## Good Example

```cpp
class IDatabase {
public:
    virtual void save() = 0;
    virtual ~IDatabase() = default;
};
```

Concrete Implementations:

```cpp
class MySQLDatabase : public IDatabase {
public:
    void save() override {
        cout << "Saved in MySQL";
    }
};

class MongoDatabase : public IDatabase {
public:
    void save() override {
        cout << "Saved in MongoDB";
    }
};
```

Dependency Injection:

```cpp
class OrderService {
private:
    IDatabase* db;

public:
    OrderService(IDatabase* database)
        : db(database) {}

    void placeOrder() {
        db->save();
    }
};
```

Usage:

```cpp
IDatabase* db = new MySQLDatabase();

OrderService service(db);

service.placeOrder();
```

Now MySQL can be replaced with MongoDB without modifying OrderService.

---

## Benefits

* Loose coupling
* Easy testing
* Easy replacement of implementations
* Better scalability

---

## Interview Answer

DIP states that high-level modules should depend on abstractions rather than concrete implementations. In C++, this is achieved using interfaces and dependency injection. This reduces coupling and makes the system easier to test and extend.

---

# SOLID Principles Summary

| Principle | Meaning                                       | Achieved Through                  |
| --------- | --------------------------------------------- | --------------------------------- |
| SRP       | One class = One responsibility                | Separation of concerns            |
| OCP       | Extend without modifying                      | Abstraction + Polymorphism        |
| LSP       | Derived object should behave like base object | Proper inheritance                |
| ISP       | Don't force unused methods                    | Small focused interfaces          |
| DIP       | Depend on abstractions, not concrete classes  | Interfaces + Dependency Injection |

---

# 30-Second Interview Answer

SOLID is a set of five object-oriented design principles that help build maintainable, scalable, and loosely coupled software systems.

* SRP ensures a class has one responsibility.
* OCP allows extension without modification.
* LSP ensures derived classes can safely replace base classes.
* ISP promotes small focused interfaces.
* DIP encourages depending on abstractions rather than concrete implementations.

Together, these principles improve flexibility, testability, maintainability, and scalability of software.
