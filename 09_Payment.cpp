// 09_Payment.cpp
// Responsibility: the payment CONTRACT only -- pay() is pure virtual.
// Knows: the amount to be paid.
// Does: nothing by itself -- forces subclasses to implement pay().
// Must NOT do: know about Booking, Show, or seats. It only knows an amount.
//
// Abstraction: this class can never be instantiated directly (pure virtual pay()).
// Runtime polymorphism: a Payment* / Payment& at BookingService calls pay() and the
// correct UpiPayment/CardPayment/CashPayment::pay() runs, decided at run time.

class Payment {
protected:
    double amount;

public:
    Payment(double amount) : amount(amount) {}

    // simulateFailure lets the demo exercise the "failed payment" edge case (F6)
    // without needing a real payment gateway.
    virtual bool pay(bool simulateFailure) = 0;
    virtual string methodName() const = 0;

    double getAmount() const { return amount; }

    virtual ~Payment() {}
};
