// 08_Booking.cpp
// Responsibility: booking id, which show, which seats, total amount, status.
// Knows: its own id, its Show, its Customer, the ShowSeats it holds, the amount,
//        its status, and its Payment record.
// Does: transitions between PENDING -> CONFIRMED / FAILED, and CONFIRMED -> CANCELLED.
// Must NOT do: decide seat availability or run payment logic itself -- BookingService
//              orchestrates that and just tells Booking what happened. Booking also
//              must NOT print the ticket (that is TicketPrinter's single job -- see
//              SOLID / Single Responsibility notes).
//
// Static member: nextId is shared by every Booking to hand out unique ids.

enum BookingStatus { PENDING, CONFIRMED, FAILED, CANCELLED };

class Booking {
private:
    static int nextId;

    string bookingId;
    Show* show;                 // AGGREGATION: booking refers to a show, doesn't own it
    Customer* customer;         // ASSOCIATION: neither owns the other
    vector<ShowSeat*> seats;    // AGGREGATION: these seats live inside Show, not Booking
    double totalAmount;
    BookingStatus status;
    Payment* payment;           // COMPOSITION: the payment record belongs to this booking

public:
    Booking(Show* show, Customer* customer, vector<ShowSeat*> seats, double totalAmount)
        : show(show), customer(customer), seats(seats),
          totalAmount(totalAmount), status(PENDING), payment(nullptr) {
        nextId++;
        bookingId = "BK" + to_string(1000 + nextId);
    }

    ~Booking() { delete payment; }

    void setPayment(Payment* p) { payment = p; }
    void confirm() { status = CONFIRMED; }
    void fail() { status = FAILED; }
    void cancel() { status = CANCELLED; }

    string getId() const { return bookingId; }
    Show* getShow() const { return show; }
    Customer* getCustomer() const { return customer; }
    vector<ShowSeat*>& getSeats() { return seats; }
    double getTotalAmount() const { return totalAmount; }
    BookingStatus getStatus() const { return status; }
    Payment* getPayment() const { return payment; }

    static string statusToString(BookingStatus s) {
        if (s == PENDING) return "PENDING";
        if (s == CONFIRMED) return "CONFIRMED";
        if (s == FAILED) return "FAILED";
        return "CANCELLED";
    }
};

int Booking::nextId = 0;
