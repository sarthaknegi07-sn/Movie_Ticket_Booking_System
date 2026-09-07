// 13_BookingService.cpp
// Responsibility: the orchestrator -- runs the booking flow end to end.
// Knows: the list of all bookings ever made (so it can look one up to cancel it).
// Does: validates seats, prices them, takes payment, confirms/fails/cancels bookings.
// Must NOT do: format a ticket (delegates to TicketPrinter) or read console input
//              itself (main/Cinema-menu reads input and passes plain values in).
//
// This is where FR4 and FR6 are enforced:
//   FR4 - if ANY selected seat is already BOOKED, the whole booking is rejected
//         and no seat changes state.
//   FR6 - booking is confirmed only after payment succeeds; on failure seats are
//         released (they were never marked booked in the first place here) and
//         booking status becomes FAILED.

class BookingService {
private:
    vector<Booking*> allBookings; // COMPOSITION: BookingService owns booking records

public:
    ~BookingService() {
        for (Booking* b : allBookings) delete b;
    }

    // Returns nullptr on validation failure (invalid/duplicate/already-booked seat).
    // Returns a Booking* (status FAILED or CONFIRMED) once seats validate.
    Booking* bookAndPay(Show* show, Customer* customer,
                         const vector<string>& seatNumbers,
                         int paymentMethodCode, bool simulateFailure) {
        vector<ShowSeat*> selected;

        for (const string& num : seatNumbers) {
            ShowSeat* ss = show->findSeat(num);
            if (ss == nullptr) {
                cout << "  Invalid seat number: " << num << ". Booking cancelled, nothing changed.\n";
                return nullptr;
            }
            if (!ss->isAvailable()) {
                cout << "  Seat " << num << " is already BOOKED. Booking cancelled, nothing changed.\n";
                return nullptr;
            }
            selected.push_back(ss);
        }

        double total = PriceCalculator::calculateTotal(selected);
        Booking* booking = new Booking(show, customer, selected, total);

        Payment* payment = createPayment(paymentMethodCode, total);
        bool success = payment->pay(simulateFailure);
        booking->setPayment(payment);

        if (!success) {
            booking->fail();
            cout << "  Payment failed. Booking NOT confirmed. Seats remain AVAILABLE.\n";
            allBookings.push_back(booking);
            return booking;
        }

        for (ShowSeat* ss : selected) ss->markBooked();
        booking->confirm();
        allBookings.push_back(booking);
        return booking;
    }

    bool cancelBooking(const string& bookingId) {
        Booking* booking = findBooking(bookingId);
        if (booking == nullptr) {
            cout << "  No such booking id.\n";
            return false;
        }
        if (booking->getStatus() != CONFIRMED) {
            cout << "  Only a CONFIRMED booking can be cancelled.\n";
            return false;
        }
        for (ShowSeat* ss : booking->getSeats()) ss->markAvailable();
        booking->cancel();
        return true;
    }

    Booking* findBooking(const string& bookingId) {
        for (Booking* b : allBookings) {
            if (b->getId() == bookingId) return b;
        }
        return nullptr;
    }

    vector<Booking*>& getAllBookings() { return allBookings; }
};
