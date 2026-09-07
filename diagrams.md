# Step E — Class Diagram & Step F — Sequence Diagram (Mermaid source)

Paste each block below into **https://mermaid.live** (or the Mermaid plugin for
draw.io / VS Code) to render, then export as PNG/SVG for your submission —
or redraw the same structure by hand on paper, which the assignment also allows.

---

## Step E — Class Diagram

```mermaid
classDiagram
    class Movie {
        -string title
        -string language
        -int durationMinutes
        +getTitle() string
        +getLanguage() string
        +getDurationMinutes() int
    }

    class Seat {
        -string seatNumber
        -SeatType type
        +getSeatNumber() string
        +getType() SeatType
        +priceForType(SeatType)$ double
    }

    class Screen {
        -int screenNumber
        -Seat[] seats
        +addSeat(Seat)
        +getSeats() Seat[]
    }

    class Cinema {
        -string name
        -Screen[] screens
        +addScreen(Screen)
        +getScreens() Screen[]
    }

    class Show {
        -Movie movie
        -Screen screen
        -string startTime
        -ShowSeat[] showSeats
        +findSeat(string) ShowSeat
        +printLayout()
    }

    class ShowSeat {
        -string seatNumber
        -SeatType type
        -SeatStatus status
        +isAvailable() bool
        +markBooked()
        +markAvailable()
    }

    class Customer {
        -string name
        -string phone
        +getName() string
        +getPhone() string
    }

    class Booking {
        -string bookingId
        -Show show
        -Customer customer
        -ShowSeat[] seats
        -double totalAmount
        -BookingStatus status
        -Payment payment
        +confirm()
        +fail()
        +cancel()
    }

    class Payment {
        <<abstract>>
        #double amount
        +pay(bool)* bool
        +methodName()* string
    }

    class UpiPayment {
        +pay(bool) bool
        +methodName() string
    }
    class CardPayment {
        +pay(bool) bool
        +methodName() string
    }
    class CashPayment {
        +pay(bool) bool
        +methodName() string
    }

    class PriceCalculator {
        +calculateTotal(ShowSeat[])$ double
    }

    class TicketPrinter {
        +printTicket(Booking)$
    }

    class BookingService {
        -Booking[] allBookings
        +bookAndPay(Show, Customer, string[], int, bool) Booking
        +cancelBooking(string) bool
        +findBooking(string) Booking
    }

    Cinema "1" *-- "1..*" Screen
    Screen "1" *-- "1..*" Seat
    Show "1" o-- "1" Movie
    Show "1" o-- "1" Screen
    Show "1" *-- "1..*" ShowSeat
    Booking "many" --> "1" Customer
    Booking "1" o-- "1..*" ShowSeat
    Booking "1" *-- "1" Payment
    Payment <|-- UpiPayment
    Payment <|-- CardPayment
    Payment <|-- CashPayment
    BookingService "1" *-- "0..*" Booking
    BookingService ..> PriceCalculator : uses
```

**Notation key used above:** `*--` = composition (◆), `o--` = aggregation (◇),
`-->` = association, `<|--` = inheritance, `<<abstract>>` on `Payment` marks
the abstract base, `$` marks a static method, `#` marks protected.

---

## Step F — Sequence Diagram: "customer books 1 seat and pays by UPI"

```mermaid
sequenceDiagram
    actor Customer
    participant BookingService
    participant Show
    participant ShowSeat
    participant PriceCalculator
    participant Payment
    participant Booking
    participant TicketPrinter

    Customer->>+BookingService: bookAndPay(show, customer, ["A1"], UPI, false)
    BookingService->>+Show: findSeat("A1")
    Show-->>-BookingService: showSeat
    BookingService->>+ShowSeat: isAvailable()
    ShowSeat-->>-BookingService: true

    BookingService->>+PriceCalculator: calculateTotal([showSeat])
    PriceCalculator-->>-BookingService: total = 150

    BookingService->>+Booking: «create» Booking(show, customer, [showSeat], 150)
    Booking-->>-BookingService: booking

    BookingService->>+Payment: «create» UpiPayment(150)
    Payment-->>-BookingService: payment

    BookingService->>+Payment: pay(simulateFailure = false)
    Payment-->>-BookingService: true

    BookingService->>+ShowSeat: markBooked()
    ShowSeat-->>-BookingService: (done)

    BookingService->>+Booking: confirm()
    Booking-->>-BookingService: (done)

    BookingService-->>-Customer: booking (status = CONFIRMED)

    Customer->>+TicketPrinter: printTicket(booking)
    TicketPrinter-->>-Customer: ticket printed to console
```
