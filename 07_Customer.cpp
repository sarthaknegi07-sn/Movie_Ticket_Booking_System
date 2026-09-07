// 07_Customer.cpp
// Responsibility: a customer's name and phone number. Nothing else.
// Knows: its own name and phone.
// Does: exposes read-only getters.
// Must NOT do: hold bookings itself (Booking -> Customer is one-directional
//              ASSOCIATION; a customer does not own its bookings in this design).

class Customer {
private:
    string name;
    string phone;

public:
    Customer(string name, string phone) : name(name), phone(phone) {}

    string getName() const { return name; }
    string getPhone() const { return phone; }
};
