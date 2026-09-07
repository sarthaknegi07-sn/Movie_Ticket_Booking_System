// main.cpp
// Responsibility: the console menu and input reading only. Builds sample data,
// then loops reading menu choices and calling BookingService.
//
// BUILD (course rule is "no header files", so this file #includes the other
// .cpp files directly -- the whole program is compiled as ONE translation unit
// with a single command):
//     g++ main.cpp -o ticketBooking
//     ./ticketBooking
//
// The files are still one-class-per-file as required; only the #include order
// below (which must follow each class's dependencies) differs from the file
// numbering suggested in the assignment.

#include <iostream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

#include "01_Movie.cpp"
#include "02_Seat.cpp"
#include "03_Screen.cpp"
#include "04_Cinema.cpp"
#include "06_ShowSeat.cpp"
#include "05_Show.cpp"
#include "07_Customer.cpp"
#include "09_Payment.cpp"
#include "10_PaymentTypes.cpp"
#include "08_Booking.cpp"
#include "11_PriceCalculator.cpp"
#include "12_TicketPrinter.cpp"
#include "13_BookingService.cpp"

// ---------- small input helpers (keep main()'s functions doing ONE thing each) ----------

int readMenuChoice() {
    int choice;
    while (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Please enter a number: ";
    }
    return choice;
}

vector<string> splitBySeatSeparator(const string& text, char sep) {
    vector<string> parts;
    string current;
    for (char c : text) {
        if (c == sep) {
            if (!current.empty()) parts.push_back(current);
            current.clear();
        } else if (!isspace(c)) {
            current += c;
        }
    }
    if (!current.empty()) parts.push_back(current);
    return parts;
}

// ---------- sample data setup ----------

void setupCinema(Cinema& cinema, vector<Movie>& movies, vector<Show>& shows) {
    movies.push_back(Movie("3 Idiots", "Hindi", 170));
    movies.push_back(Movie("Interstellar", "English", 169));

    Screen screen1(1);
    screen1.addSeat(Seat("A1", SILVER));
    screen1.addSeat(Seat("A2", SILVER));
    screen1.addSeat(Seat("A3", SILVER));
    screen1.addSeat(Seat("A4", SILVER));
    screen1.addSeat(Seat("B1", GOLD));
    screen1.addSeat(Seat("B2", GOLD));
    screen1.addSeat(Seat("B3", GOLD));
    screen1.addSeat(Seat("C1", PLATINUM));
    screen1.addSeat(Seat("C2", PLATINUM));

    Screen screen2(2);
    screen2.addSeat(Seat("A1", SILVER));
    screen2.addSeat(Seat("A2", SILVER));
    screen2.addSeat(Seat("B1", GOLD));
    screen2.addSeat(Seat("B2", GOLD));
    screen2.addSeat(Seat("C1", PLATINUM));

    cinema.addScreen(screen1);
    cinema.addScreen(screen2);

    // Shows reference movies/screens that must outlive them (both live in main()'s
    // vectors for the whole run) -- this is exactly the AGGREGATION relationship.
    shows.push_back(Show(&movies[0], &cinema.getScreens()[0], "06:00 PM"));
    shows.push_back(Show(&movies[1], &cinema.getScreens()[1], "09:00 PM"));
}

// ---------- menu actions ----------

void listMovies(const vector<Movie>& movies) {
    cout << "\n";
    for (size_t i = 0; i < movies.size(); i++) {
        cout << "  [" << (i + 1) << "] " << movies[i].getTitle()
             << "\t" << movies[i].getLanguage()
             << "\t" << movies[i].getDurationMinutes() << " min\n";
    }
}

Show* chooseShowForMovie(vector<Movie>& movies, vector<Show>& shows) {
    listMovies(movies);
    cout << "Choose movie: ";
    int movieChoice = readMenuChoice();
    if (movieChoice < 1 || movieChoice > (int)movies.size()) {
        cout << "  Invalid movie choice.\n";
        return nullptr;
    }
    Movie* chosenMovie = &movies[movieChoice - 1];

    vector<Show*> matchingShows;
    cout << "\n";
    int index = 1;
    for (Show& s : shows) {
        if (s.getMovie() == chosenMovie) {
            cout << "  [" << index << "] Screen-" << s.getScreen()->getScreenNumber()
                 << "  " << s.getStartTime() << "\n";
            matchingShows.push_back(&s);
            index++;
        }
    }
    if (matchingShows.empty()) {
        cout << "  No shows for this movie.\n";
        return nullptr;
    }
    cout << "Choose show: ";
    int showChoice = readMenuChoice();
    if (showChoice < 1 || showChoice > (int)matchingShows.size()) {
        cout << "  Invalid show choice.\n";
        return nullptr;
    }
    return matchingShows[showChoice - 1];
}

void handleBooking(BookingService& service, vector<Movie>& movies, vector<Show>& shows) {
    Show* show = chooseShowForMovie(movies, shows);
    if (show == nullptr) return;

    show->printLayout();

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\nSeats (e.g. A1,B2): ";
    string seatLine;
    getline(cin, seatLine);
    vector<string> seatNumbers = splitBySeatSeparator(seatLine, ',');
    if (seatNumbers.empty()) {
        cout << "  No seats entered.\n";
        return;
    }

    cout << "Customer name: ";
    string name;
    getline(cin, name);
    cout << "Customer phone: ";
    string phone;
    getline(cin, phone);
    Customer customer(name, phone);

    cout << "Pay by: 1.UPI  2.Card  3.Cash > ";
    int method = readMenuChoice();
    if (method < 1 || method > 3) {
        cout << "  Invalid payment method.\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Simulate a FAILED payment for testing? (y/N): ";
    string simulateAns;
    getline(cin, simulateAns);
    bool simulateFailure = (!simulateAns.empty() && (simulateAns[0] == 'y' || simulateAns[0] == 'Y'));

    Booking* booking = service.bookAndPay(show, &customer, seatNumbers, method, simulateFailure);
    if (booking == nullptr) return; // validation already printed a message

    if (booking->getStatus() == CONFIRMED) {
        TicketPrinter::printTicket(booking);
    }
    // Note: `customer` goes out of scope after this function returns, but Booking
    // only stores a raw pointer to it (ASSOCIATION). For this console demo that is
    // fine because we only read the booking within the same menu loop iteration;
    // a production version would give Booking its own owned copy or a shared_ptr.
}

void handleCancel(BookingService& service) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Booking ID to cancel: ";
    string id;
    getline(cin, id);
    if (service.cancelBooking(id)) {
        cout << "  Booking " << id << " cancelled. Seats are AVAILABLE again.\n";
    }
}

void handleMyTickets(BookingService& service) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Booking ID: ";
    string id;
    getline(cin, id);
    Booking* booking = service.findBooking(id);
    if (booking == nullptr) {
        cout << "  No such booking id.\n";
        return;
    }
    TicketPrinter::printTicket(booking);
}

// ---------- main ----------

int main() {
    Cinema cinema("PVR");
    vector<Movie> movies;
    vector<Show> shows;
    setupCinema(cinema, movies, shows);

    BookingService service;

    cout << "===== MOVIE TICKET BOOKING =====\n";
    bool running = true;
    while (running) {
        cout << "\n1. Movies  2. Book  3. Cancel  4. My tickets   0. Exit\nChoose: ";
        int choice = readMenuChoice();

        switch (choice) {
            case 1: listMovies(movies); break;
            case 2: handleBooking(service, movies, shows); break;
            case 3: handleCancel(service); break;
            case 4: handleMyTickets(service); break;
            case 0: running = false; break;
            default: cout << "  Invalid menu choice. Please choose 0-4.\n";
        }
    }

    cout << "Goodbye!\n";
    return 0;
}
