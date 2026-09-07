// 01_Movie.cpp
// Responsibility: hold a movie's own data (title, language, duration). Nothing else.
// Knows: its own title/language/duration.
// Does: exposes read-only getters.
// Must NOT do: know about shows, screens, seats, or pricing.

class Movie {
private:
    string title;
    string language;
    int durationMinutes;

public:
    Movie(string title, string language, int durationMinutes)
        : title(title), language(language), durationMinutes(durationMinutes) {}

    string getTitle() const { return title; }
    string getLanguage() const { return language; }
    int getDurationMinutes() const { return durationMinutes; }
};
