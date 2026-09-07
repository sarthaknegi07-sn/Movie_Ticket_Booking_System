// 10_PaymentTypes.cpp
// Responsibility: how each payment method actually pays.
// Inheritance: all three derive from Payment (extends the base contract).
// this: used below just to be explicit about member vs parameter -- shown for
//       the "this keyword" OOP requirement.

class UpiPayment : public Payment {
public:
    UpiPayment(double amount) : Payment(amount) {}

    bool pay(bool simulateFailure) override {
        if (simulateFailure) {
            cout << "  [UPI] Rs." << this->amount << " payment FAILED (simulated)\n";
            return false;
        }
        cout << "  [UPI] Rs." << this->amount << " paid successfully\n";
        return true;
    }
    string methodName() const override { return "UPI"; }
};

class CardPayment : public Payment {
public:
    CardPayment(double amount) : Payment(amount) {}

    bool pay(bool simulateFailure) override {
        if (simulateFailure) {
            cout << "  [Card] Rs." << this->amount << " payment FAILED (simulated)\n";
            return false;
        }
        cout << "  [Card] Rs." << this->amount << " paid successfully\n";
        return true;
    }
    string methodName() const override { return "Card"; }
};

class CashPayment : public Payment {
public:
    CashPayment(double amount) : Payment(amount) {}

    bool pay(bool simulateFailure) override {
        // Cash is collected in person -- for this console demo we still allow
        // simulating a failure (e.g. customer could not pay exact change).
        if (simulateFailure) {
            cout << "  [Cash] Rs." << this->amount << " payment FAILED (simulated)\n";
            return false;
        }
        cout << "  [Cash] Rs." << this->amount << " received\n";
        return true;
    }
    string methodName() const override { return "Cash"; }
};

// Compile-time polymorphism (overloading): two overloads of the same factory name,
// one taking a numeric method code, one taking the method's string name.
Payment* createPayment(int methodCode, double amount) {
    if (methodCode == 1) return new UpiPayment(amount);
    if (methodCode == 2) return new CardPayment(amount);
    if (methodCode == 3) return new CashPayment(amount);
    return nullptr;
}

Payment* createPayment(const string& methodName, double amount) {
    if (methodName == "UPI") return new UpiPayment(amount);
    if (methodName == "Card") return new CardPayment(amount);
    if (methodName == "Cash") return new CashPayment(amount);
    return nullptr;
}
