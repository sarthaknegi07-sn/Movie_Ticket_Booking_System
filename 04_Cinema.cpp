// 04_Cinema.cpp
// Responsibility: the theatre -- a name; owns its screens.
// Knows: its name and its list of Screens.
// Does: lets you add screens and read them back.
// Must NOT do: run the menu (that is main/Cinema-menu's job) or know about bookings.

class Cinema {
private:
    string name;
    vector<Screen> screens; // COMPOSITION: screens belong only to this cinema

public:
    Cinema(string name) : name(name) {}

    string getName() const { return name; }

    void addScreen(const Screen& screen) { screens.push_back(screen); }

    vector<Screen>& getScreens() { return screens; }
};
