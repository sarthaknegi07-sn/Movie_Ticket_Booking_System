// 12_TicketPrinter.cpp
// Responsibility: format and print a ticket -- printing only.
// Knows: nothing of its own (stateless).
// Does: reads a CONFIRMED (or FAILED) booking and prints it in ticket format.
// Must NOT do: change booking state, touch payment, or touch seats. A layout /
//              formatting change here must never require touching Booking.

class TicketPrinter {
public:
    static void printTicket(Booking* booking) {
        cout << "\n  ================ TICKET ================\n";
        cout << "  Booking ID : " << booking->getId() << "\n";
        cout << "  Movie      : " << booking->getShow()->getMovie()->getTitle() << "\n";
        cout << "  Screen     : Screen-" << booking->getShow()->getScreen()->getScreenNumber()
             << "   " << booking->getShow()->getStartTime() << "\n";
        cout << "  Seats      : " << seatList(booking) << "\n";
        cout << "  Amount     : Rs." << booking->getTotalAmount()
             << "      Status: " << Booking::statusToString(booking->getStatus()) << "\n";
        cout << "  =========================================\n";
    }

private:
    static string seatList(Booking* booking) {
        string result;
        vector<ShowSeat*>& seats = booking->getSeats();
        for (size_t i = 0; i < seats.size(); i++) {
            result += seats[i]->getSeatNumber();
            if (i + 1 < seats.size()) result += ", ";
        }
        return result;
    }
};
