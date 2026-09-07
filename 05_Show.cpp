// 05_Show.cpp
// Responsibility: one screening = a Movie on a Screen at a time; owns its ShowSeats.
// Knows: which movie, which screen, the start time, and the booking status of every
//        seat for THIS screening (via its own ShowSeat list).
// Does: builds one ShowSeat per physical seat on construction; finds a seat by number;
//       prints the seat layout.
// Must NOT do: create Bookings or take payments -- that is BookingService's job.

class Show {
private:
    Movie* movie;   // AGGREGATION: Show borrows a Movie, does not own its lifetime
    Screen* screen; // AGGREGATION: Show borrows a Screen, does not own its lifetime
    string startTime;
    vector<ShowSeat> showSeats; // COMPOSITION: these belong only to this Show

public:
    Show(Movie* movie, Screen* screen, string startTime)
        : movie(movie), screen(screen), startTime(startTime) {
        for (const Seat& s : screen->getSeats()) {
            showSeats.push_back(ShowSeat(s.getSeatNumber(), s.getType()));
        }
    }

    Movie* getMovie() const { return movie; }
    Screen* getScreen() const { return screen; }
    string getStartTime() const { return startTime; }
    vector<ShowSeat>& getShowSeats() { return showSeats; }

    ShowSeat* findSeat(const string& seatNumber) {
        for (ShowSeat& ss : showSeats) {
            if (ss.getSeatNumber() == seatNumber) return &ss;
        }
        return nullptr; // caller must handle: invalid seat number
    }

    void printLayout() const {
        cout << "\n  SCREEN-" << screen->getScreenNumber() << "  " << startTime
             << "  |  " << movie->getTitle() << "\n";
        printRow("SILVER", SILVER);
        printRow("GOLD", GOLD);
        printRow("PLATINUM", PLATINUM);
        cout << "  ( [ ] = available   [X] = booked )\n";
    }

private:
    void printRow(const string& label, SeatType type) const {
        cout << "  " << label;
        for (int i = label.size(); i < 9; i++) cout << " ";
        for (const ShowSeat& ss : showSeats) {
            if (ss.getType() == type) {
                cout << ss.getSeatNumber() << (ss.isAvailable() ? "[ ] " : "[X] ");
            }
        }
        cout << "\n";
    }
};
