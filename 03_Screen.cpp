// 03_Screen.cpp
// Responsibility: one auditorium -- a screen number; owns its physical seats.
// Knows: its screen number and its list of Seats.
// Does: lets you add seats and read them back.
// Must NOT do: know about shows, bookings, or pricing logic.

class Screen {
private:
    int screenNumber;
    vector<Seat> seats; // COMPOSITION: seats are created and destroyed with the screen

public:
    Screen(int screenNumber) : screenNumber(screenNumber) {}

    int getScreenNumber() const { return screenNumber; }

    void addSeat(const Seat& seat) { seats.push_back(seat); }

    const vector<Seat>& getSeats() const { return seats; }
};
