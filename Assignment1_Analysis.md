# TCS-504 Assignment 1 — Movie Ticket Booking System
**Steps A–D and G written analysis** (code is in the separate `.cpp` files / see `main.cpp` for build & demo instructions)

---

## Step A — Requirement Analysis

### Functional Requirements

**FR1** — Listing movies: the system shall display every `Movie` currently playing, showing its title, language, and duration in minutes.

**FR2** — Listing shows: given a chosen movie, the system shall display only the `Show`s whose movie matches, showing each show's screen number and start time.

**FR3** — Seat layout: given a chosen show, the system shall display every seat for that show grouped by seat type (SILVER / GOLD / PLATINUM), each marked `[ ]` if its `ShowSeat` status is AVAILABLE or `[X]` if BOOKED.

**FR4** *(given)* — Booking: a customer selects one or more seat numbers for a show. If **any** selected seat is already BOOKED, the whole booking is rejected and no seat changes state. Booking is confirmed **only after** payment succeeds.

**FR5** — Pricing: the system shall price a booking by summing, for every selected seat, the fixed rate for its type — SILVER ₹150, GOLD ₹250, PLATINUM ₹400 — with no other charges.

**FR6** *(given)* — Payment: exactly one method (UPI / Card / Cash) per booking. If payment fails, seats are released and booking status becomes FAILED.

**FR7** — Ticket printing: once a booking's status is CONFIRMED, the system shall print a ticket containing the booking id, movie title, screen number, show start time, the list of booked seat numbers, and the total amount.

**FR8** — Cancellation: given a valid booking id whose status is CONFIRMED, the system shall set the booking's status to CANCELLED and set every seat that booking held back to AVAILABLE. A booking that is not CONFIRMED (e.g. already cancelled, or FAILED) shall not be cancellable.

### Non-Functional Requirements

**NFR1 — Modularity.** One class per file (course rule); each class has exactly one responsibility (see Step C), so a change to, say, ticket formatting touches only `TicketPrinter`.

**NFR2 — Extensibility.** Adding a new payment method (e.g. NetBanking) must require adding one new `Payment` subclass and one factory line, and must **not** require editing `BookingService`, `Booking`, or any existing `Payment` subclass.

**NFR3 — Input validation / robustness.** Every console input (menu choice, seat number, booking id) shall be validated; invalid input shall produce a clear message and let the program continue — it shall never crash or leave data in an inconsistent state.

**NFR4 — Consistency of seat state.** A seat's state changes from AVAILABLE to BOOKED (or back) only as an atomic side effect of a successful booking or a cancellation — never partially, e.g. a 2-seat booking where payment fails must leave **both** seats AVAILABLE, not one booked and one free.

---

## Step B — Noun–Verb Analysis

| Noun found | Keep as a class? | Reason |
|---|---|---|
| Movie | yes | has its own data and identity |
| Seat | yes | has number, type, price |
| "seat layout" | no | it is a view of a Show's seats, not a thing → `Show::printLayout()` |
| Screen | yes | has an identity (screen number) and owns seats |
| Cinema | yes | the theatre itself; owns screens |
| Show | yes | a movie+screen+time combination has its own identity and its own seat statuses |
| Customer | yes | has name/phone, referenced by a booking |
| Booking | yes | has an id, status, amount — a real business record |
| Payment | yes | needs to vary by method (UPI/Card/Cash) → abstraction + polymorphism |
| Ticket | no | not stored anywhere — it is only ever *printed*, so it becomes a method, `TicketPrinter::printTicket()`, not a class |
| "booking id" | no | just an attribute of Booking, not a class of its own |
| "seat number" | no | an attribute (`string`) of Seat / ShowSeat, not a class |
| Price / amount | no | a `double` computed by `PriceCalculator`, not an entity with identity |
| "show" (verb: to show seats) | — (verb) | becomes `Show::printLayout()` / `Cinema` menu action |
| book (verb) | — (verb) | becomes `BookingService::bookAndPay()` |
| pay (verb) | — (verb) | becomes `Payment::pay()` |
| cancel (verb) | — (verb) | becomes `BookingService::cancelBooking()` |
| list / display (verb) | — (verb) | becomes the `Cinema`/main menu's read-only print actions |

---

## Step C — Classes (data members, methods, access, "must not do")

> Full member lists and comments are in the `.cpp` files themselves; this table is the one-line summary the assignment asks for.

| Class | Knows (private data) | Does (public methods) | Must NOT do |
|---|---|---|---|
| `Movie` | title, language, durationMinutes | getters | know about shows/screens/pricing |
| `Seat` | seatNumber, type | getters; static type→price/string helpers | know if it's booked (that's `ShowSeat`) |
| `Screen` | screenNumber, `vector<Seat>` | addSeat, getSeats | know about shows or bookings |
| `Cinema` | name, `vector<Screen>` | addScreen, getScreens | run the menu or know about bookings |
| `Show` | movie\*, screen\*, startTime, `vector<ShowSeat>` | findSeat, printLayout | create bookings or take payments |
| `ShowSeat` | seatNumber, type, status | markBooked, markAvailable, isAvailable | calculate price or know which booking holds it |
| `Customer` | name, phone | getters | own/hold its bookings |
| `Booking` | id, show\*, customer\*, seats\*, amount, status, payment\* | confirm, fail, cancel | decide seat availability, run payment logic, or print itself |
| `Payment` (abstract) | amount | `pay()` pure virtual | know about Booking/Show/seats |
| `UpiPayment` / `CardPayment` / `CashPayment` | (inherited amount) | `pay()` override | anything outside "did this payment succeed" |
| `PriceCalculator` | nothing (stateless) | `calculateTotal(seats)` | touch seat status or create bookings |
| `TicketPrinter` | nothing (stateless) | `printTicket(booking)` | change booking/seat/payment state |
| `BookingService` | `vector<Booking*>` | `bookAndPay`, `cancelBooking`, `findBooking` | read console input or format a ticket |
| `Cinema`/main menu | sample data + service reference | the menu loop | contain business rules (seat validation, pricing) itself |

---

## Step D — Relationships (with the lifetime test)

| Pair | Choice | Justification (lifetime test) |
|---|---|---|
| Cinema — Screen | **Composition (◆)** | A Screen has no meaning outside its Cinema; if the Cinema object is destroyed, its Screens are destroyed with it (they live in a `vector<Screen>` by value inside Cinema). |
| Screen — Seat | **Composition (◆)** | A physical seat only exists as part of a specific screen; destroy the Screen and its Seats go with it (`vector<Seat>` by value). |
| Show — Movie | **Aggregation (◇)** *(given as the worked example)* | A Show *borrows* a Movie. Cancel the 6 PM show and "3 Idiots" still exists and still plays at 9 PM. Show stores a pointer and never deletes it. |
| Show — Screen | **Aggregation (◇)** | Same test: a Show borrows a Screen (a `Screen*`). Deleting one Show for Screen-1 does not delete Screen-1 — the same screen is reused by later shows. |
| Show — ShowSeat | **Composition (◆)** | A ShowSeat (a seat's status *for this show*) has no meaning outside that Show; when the Show is destroyed its `vector<ShowSeat>` is destroyed with it. |
| Booking — Customer | **Association (──▶)** | Neither owns the other's lifetime: the Customer exists (and could book again) whether or not this particular Booking exists, and the Booking simply refers to a Customer. |
| Booking — ShowSeat | **Aggregation (◇)** | Booking holds pointers to ShowSeats that live inside the Show, not inside the Booking. Cancelling/destroying the Booking does not destroy the seats — they just flip back to AVAILABLE. |
| Booking — Payment | **Composition (◆)** | A Payment record only makes sense as part of one specific Booking; Booking creates it, owns it, and its destructor deletes it — no other object holds or reuses that Payment. |
| Payment — UpiPayment | **Inheritance (──▷)** | UpiPayment *is a* Payment (extends the abstract base and implements `pay()`); this is a type relationship, not an ownership one, so the lifetime test doesn't apply — the "is-a" test does. |
| BookingService — Booking | **Composition (◆)** | BookingService creates every Booking and is the only owner; its `vector<Booking*>` is deleted in `~BookingService()`, so destroying the service destroys all booking records. |

---

## Step G — SOLID Mapping

**Single Responsibility Principle** — `Booking` does **not** print the ticket (`TicketPrinter` does) and does **not** decide seat availability (`BookingService` does). A layout/formatting change only touches `TicketPrinter`; a pricing change only touches `PriceCalculator`; neither forces a change to `Booking` itself.

**Open/Closed Principle** — Adding NetBanking needs only a new `NetBankingPayment : public Payment` class (plus one line in the `createPayment` factory). `BookingService::bookAndPay()` calls `payment->pay(...)` through the abstract `Payment` interface and never needs to change when a new payment type is added.

**Liskov Substitution Principle** — Every `Payment` subclass (`UpiPayment`, `CardPayment`, `CashPayment`) can be used anywhere a `Payment*` is expected, with no extra setup call and no subclass throwing or behaving unexpectedly — `BookingService` calls `pay()` on the base pointer with no idea which concrete class it is.

**Dependency (Inversion-flavoured) note** — `BookingService` depends on the abstract `Payment` type, not on `UpiPayment`/`CardPayment`/`CashPayment` directly for the *payment logic itself* (it calls `payment->pay(...)` polymorphically). The one place it still touches concrete types is the small `createPayment()` factory function — see below.

### One thing deliberately NOT done

We did **not** build a fully pluggable payment registry (e.g. a `map<string, function<Payment*(double)>>` that new payment types register themselves into). Instead, `createPayment()` is a small factory function with an `if` chain, so adding a new payment method still means touching one line in one factory function rather than being 100% closed to modification everywhere. We accepted this minor, contained OCP compromise because building a self-registering plugin system was out of scope for an 8-feature console assignment — `BookingService` itself never needs to change, which is the requirement's actual concern.

**Interface Segregation, applied as a judgment call:** `Payment` only declares `pay()` and `methodName()` — it deliberately does **not** declare a `refund()` method, since not all payment methods in this scope need one and forcing every subclass to implement an unused `refund()` would violate ISP.

---

## Diagrams

The class diagram and sequence diagram (Steps E and F) are provided separately as Mermaid source in `diagrams.md` — paste that code into **https://mermaid.live** (or the Mermaid plugin in draw.io / VS Code) to render and export as PNG/SVG for submission, or redraw by hand from it as the assignment allows.
