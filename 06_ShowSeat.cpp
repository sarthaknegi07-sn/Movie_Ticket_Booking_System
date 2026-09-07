// 06_ShowSeat.cpp
// Responsibility: the status of ONE physical seat FOR ONE show (AVAILABLE / BOOKED).
// Knows: seat number, seat type, and its own booking status for this show only.
// Does: lets BookingService mark/unmark it booked; reports availability.
// Must NOT do: calculate price, know which Booking or Customer holds it -- that is
//              Booking's job (kept this way so a seat can be freed by cancel()
//              without ShowSeat needing to know anything about Booking).

enum SeatStatus { AVAILABLE, BOOKED };

class ShowSeat {
private:
    string seatNumber;
    SeatType type;
    SeatStatus status;

public:
    ShowSeat(string seatNumber, SeatType type)
        : seatNumber(seatNumber), type(type), status(AVAILABLE) {}

    string getSeatNumber() const { return seatNumber; }
    SeatType getType() const { return type; }
    SeatStatus getStatus() const { return status; }

    bool isAvailable() const { return status == AVAILABLE; }
    void markBooked() { status = BOOKED; }
    void markAvailable() { status = AVAILABLE; }
};
