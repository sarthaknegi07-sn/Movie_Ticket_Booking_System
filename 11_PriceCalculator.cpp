// 11_PriceCalculator.cpp
// Responsibility: turn a list of seats into a total amount. Nothing else.
// Knows: nothing (stateless) -- pure calculation.
// Does: sums Seat::priceForType() over the given seats.
// Must NOT do: touch seat status, create bookings, or print anything.

class PriceCalculator {
public:
    static double calculateTotal(const vector<ShowSeat*>& seats) {
        double total = 0.0;
        for (ShowSeat* s : seats) {
            total += Seat::priceForType(s->getType());
        }
        return total;
    }
};
