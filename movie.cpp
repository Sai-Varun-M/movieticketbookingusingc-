#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

class Movie {
public:
    string title;
    string director;
    int releaseYear;
    int availableSeats;

    Movie(const string& t, const string& d, int year, int seats)
        : title(t), director(d), releaseYear(year), availableSeats(seats) {}

    void display() const {
        cout << "Title: " << title << endl;
        cout << "Director: " << director << endl;
        cout << "Release Year: " << releaseYear << endl;
        cout << "Available Seats: " << availableSeats << endl;
    }
};

class Booking {
public:
    string movieTitle;
    int numTickets;
    double totalCost;

    Booking(const string& title, int tickets, double cost)
        : movieTitle(title), numTickets(tickets), totalCost(cost) {}
};

void displayMenu() {
    cout << "\n1. Display Movies\n";
    cout << "2. Book Tickets\n";
    cout << "3. View File Contents\n";  // Added option to view file contents
    cout << "4. Exit\n";
    cout << "Enter your choice: ";
}

void displayMovies(const vector<Movie>& movies) {
    cout << "\nAvailable Movies:\n";
    for (const Movie& movie : movies) {
        cout << "-------------------------\n";
        movie.display();
        cout << "-------------------------\n";
    }
}

void bookTickets(vector<Movie>& movies, vector<Booking>& bookings) {
    string movieTitle;
    int numTickets;

    cout << "Enter the movie title: ";
    cin.ignore();
    getline(cin, movieTitle);

    bool movieFound = false;
    for (Movie& movie : movies) {
        if (movie.title == movieTitle) {
            movieFound = true;

            cout << "Enter the number of tickets to book: ";
            cin >> numTickets;

            if (numTickets > 0 && numTickets <= movie.availableSeats) {
                // Calculate total cost (for simplicity, assuming a fixed cost per ticket)
                double ticketPrice = 10.0;  // Adjust this based on your pricing
                double totalCost = numTickets * ticketPrice;

                // Update available seats and add booking
                movie.availableSeats -= numTickets;
                bookings.push_back(Booking(movieTitle, numTickets, totalCost));

                // Display booking details and bill
                cout << "\nTickets booked successfully!\n";
                cout << "Booking Details:\n";
                cout << "Movie Title: " << movieTitle << endl;
                cout << "Number of Tickets: " << numTickets << endl;
                cout << fixed << setprecision(2); // Set precision for displaying double values
                cout << "Total Cost: $" << totalCost << endl;
            } else {
                cout << "Invalid number of tickets or insufficient seats.\n";
            }

            break;
        }
    }

    if (!movieFound) {
        cout << "Movie not found.\n";
    }
}

void saveMoviesToFile(const vector<Movie>& movies, const string& filename) {
    ofstream file(filename);

    if (file.is_open()) {
        for (const Movie& movie : movies) {
            file << "{\"" << movie.title << "\", \"" << movie.director << "\", " << movie.releaseYear << ", " << movie.availableSeats << "},\n";
        }

        file.close();
        cout << "Movies saved to file.\n";
    } else {
        cout << "Unable to open file.\n";
    }
}

void loadMoviesFromFile(vector<Movie>& movies, const string& filename) {
    ifstream file(filename);

    if (file.is_open()) {
        movies.clear();

        string line;
        while (getline(file, line)) {
            stringstream ss(line);

            // Assuming the format is {"title", "director", releaseYear, availableSeats},
            char discard1, discard2;  // To discard "{" and ","
            string title, director;
            int releaseYear, availableSeats;

            ss >> discard1 >> discard2;  // Read and discard "{"
            getline(ss, title, '"');  // Read until the next '"'
            ss >> discard1 >> discard2;  // Read and discard ","
            getline(ss, director, '"');  // Read until the next '"'
            ss >> discard1 >> discard2 >> discard1;  // Read and discard ", "
            ss >> releaseYear >> discard1;  // Read year and discard ","
            ss >> availableSeats >> discard1 >> discard2;  // Read seats and discard "},"

            movies.push_back(Movie(title, director, releaseYear, availableSeats));
        }

        file.close();
        cout << "Movies loaded from file.\n";
    } else {
        cout << "Unable to open file. Creating a new file.\n";
    }
}

void viewFileContents(const string& filename) {
    ifstream file(filename);

    if (file.is_open()) {
        cout << "\nFile Contents:\n";
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }

        file.close();
    } else {
        cout << "Unable to open file.\n";
    }
}

int main() {
    vector<Movie> movies = {
        {"KGF", "Prashanth Neel", 2018, 100},
        {"RRR", "S. S. Rajamouli", 2022, 150},
        {"Dhoom", "Sanjay Gadhvi", 2004, 120}
    };

    vector<Booking> bookings;
    const string filename = "movies.txt";

    loadMoviesFromFile(movies, filename);

    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            displayMovies(movies);
            break;
        case 2:
            bookTickets(movies, bookings);
            break;
        case 3:
            viewFileContents(filename);
            break;
        case 4:
            saveMoviesToFile(movies, filename);
            cout << "Exiting program.\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }

        // Clear input stream to prevent infinite loop
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

    } while (choice != 4);

    return 0;
}
