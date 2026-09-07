// 02_Seat.cpp
// Responsibility: one physical seat -- its number and type. Nothing else.
// Knows: its own seat number and type.
// Does: exposes read-only getters; static helpers for type<->price/string (Encapsulation).
// Must NOT do: know whether it is booked -- that belongs to ShowSeat, not Seat.

enum SeatType { SILVER, GOLD, PLATINUM };

// Constants instead of magic numbers (clean-code checklist).
const double SILVER_PRICE = 150.0;
const double GOLD_PRICE = 250.0;
const double PLATINUM_PRICE = 400.0;

class Seat {
private:
    string seatNumber; // e.g. "A1"
    SeatType type;

public:
    Seat(string seatNumber, SeatType type)
        : seatNumber(seatNumber), type(type) {}

    string getSeatNumber() const { return seatNumber; }
    SeatType getType() const { return type; }

    static string typeToString(SeatType t) {
        if (t == SILVER) return "SILVER";
        if (t == GOLD) return "GOLD";
        return "PLATINUM";
    }

    static double priceForType(SeatType t) {
        if (t == SILVER) return SILVER_PRICE;
        if (t == GOLD) return GOLD_PRICE;
        return PLATINUM_PRICE;
    }
};
