#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Define ANSI color codes
#define ANSI_RESET "\x1B[0m"
#define ANSI_RED "\x1B[91m"
#define ANSI_GREEN "\x1B[92m"
#define ANSI_YELLOW "\x1B[33m"
#define ANSI_BLUE "\x1B[94m"
#define ANSI_MAGENTA "\x1B[95m"
#define ANSI_CYAN "\x1B[96m"
using namespace std;

struct Book {
    string title;
    string author;
    int id;
    bool available;
};

class Library {
public:
    Library() {
        loadBooks();
        loadIssuedBooks();
    }

    void displayMenu() {
        int choice;
        do {
            cout << ANSI_CYAN << "Library Management System" << ANSI_RESET << "\n";
            cout << ANSI_YELLOW << "1. User\n";
            cout << "2. Admin\n";
            cout << "3. Exit\n" << ANSI_RESET;
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice) {
                case 1:
                    userMenu();
                    break;
                case 2:
                    adminMenu();
                    break;
                case 3:
                    saveBooks();
                    saveIssuedBooks();
                    cout << ANSI_GREEN << "Goodbye!" << ANSI_RESET << "\n";
                    break;
                default:
                    cout << ANSI_RED << "Invalid choice. Try again." << ANSI_RESET << "\n";
                    break;
            }
        } while (choice != 3);
    }

    void userMenu() {
        int choice;
        do {
            cout << "\nUser Menu\n";
            cout << ANSI_YELLOW << "1. View available books\n";
            cout << "2. Issue a book\n";
            cout << "3. Return a book\n";
            cout << "4. View issued books\n";
            cout << "5. Return to the main menu\n" << ANSI_RESET;
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice) {
                case 1:
                    viewAvailableBooks();
                    break;
                case 2:
                    issueBook();
                    break;
                case 3:
                    returnBook();
                    break;
                case 4:
                    viewIssuedBooks();
                    break;
                case 5:
                    return;
                default:
                    cout << ANSI_RED << "Invalid choice. Try again." << ANSI_RESET << "\n";
                    break;
            }
        } while (choice != 5);
    }

    void viewIssuedBooks() {
        if (issuedBooks.empty()) {
            cout << "You haven't issued any books.\n";
        } else {
            cout << "\nIssued Books:\n";
            for (const Book& book : issuedBooks) {
                displayBook(book);
            }
        }
    }

    void adminMenu() {
        int choice;
        do {
            cout << "\nAdmin Menu\n";
            cout << ANSI_YELLOW << "1. View all books\n";
            cout << "2. Add a book\n";
            cout << "3. Update a book\n";
            cout << "4. Delete a book\n";
            cout << "5. Return to the main menu\n" << ANSI_RESET;
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice) {
                case 1:
                    viewAllBooks();
                    break;
                case 2:
                    addBook();
                    break;
                case 3:
                    updateBook();
                    break;
                case 4:
                    deleteBook();
                    break;
                case 5:
                    return;
                default:
                    cout << ANSI_RED << "Invalid choice. Try again." << ANSI_RESET << "\n";
                    break;
            }
        } while (choice != 5);
    }

    void viewAvailableBooks() {
        cout << "\nAvailable Books:\n";
        for (const Book& book : books) {
            if (book.available) {
                displayBook(book);
            }
        }
    }

    void viewAllBooks() {
        cout << "\nAll Books:\n";
        for (const Book& book : books) {
            displayBook(book);
        }
    }

    void issueBook() {
        int id;
        cout << "Enter the book ID to issue: ";
        cin >> id;
        for (Book& book : books) {
            if (book.id == id && book.available) {
                book.available = false;
                issuedBooks.push_back(book);
                cout << "Book '" << book.title << "' issued successfully.\n";
                return;
            }
        }
        cout << ANSI_RED << "Book not found or already issued." << ANSI_RESET << "\n";
    }

    void returnBook() {
        int id;
        cout << "Enter the book ID to return: ";
        cin >> id;

        // Find the book in the issuedBooks vector and remove it
        auto it = issuedBooks.begin();
        while (it != issuedBooks.end()) {
            if (it->id == id) {
                issuedBooks.erase(it);
                break;
            }
            ++it;
        }

        for (Book& book : books) {
            if (book.id == id) {
                book.available = true;
                cout << "Book '" << book.title << "' returned successfully.\n";
                return;
            }
        }
        cout << ANSI_RED << "Book not found or not issued to you." << ANSI_RESET << "\n";
    }

    void addBook() {
        Book book;
        book.id = getNextBookID();
        cout << "Enter the title of the book: ";
        cin.ignore();
        getline(cin, book.title);
        cout << "Enter the author of the book: ";
        getline(cin, book.author);
        book.available = true;
        books.push_back(book);
        cout << ANSI_GREEN << "Book added successfully." << ANSI_RESET << "\n";
    }

    void updateBook() {
        int id;
        cout << "Enter the book ID to update: ";
        cin >> id;
        for (Book& book : books) {
            if (book.id == id) {
                cout << "Enter the new title of the book: ";
                cin.ignore();
                getline(cin, book.title);
                cout << "Enter the new author of the book: ";
                getline(cin, book.author);
                cout << ANSI_GREEN << "Book updated successfully." << ANSI_RESET << "\n";
                return;
            }
        }
        cout << ANSI_RED << "Book not found." << ANSI_RESET << "\n";
    }

    void deleteBook() {
        int id;
        cout << "Enter the book ID to delete: ";
        cin >> id;
        for (auto it = books.begin(); it != books.end(); ++it) {
            if (it->id == id) {
                books.erase(it);
                cout << ANSI_GREEN << "Book deleted successfully." << ANSI_RESET << "\n";
                return;
            }
        }
        cout << ANSI_RED << "Book not found." << ANSI_RESET << "\n";
    }

    void displayBook(const Book& book) {
        cout << "ID: " << book.id << "\n";
        cout << ANSI_CYAN << "Title: " << book.title << ANSI_RESET << "\n";
        cout << ANSI_CYAN << "Author: " << book.author << ANSI_RESET << "\n";
        cout << "Available: " << (book.available ? ANSI_GREEN "Yes" ANSI_RESET : ANSI_RED "No" ANSI_RESET) << "\n";
        cout << "----------------------------------------\n";
    }

    void loadBooks() {
        ifstream inputFile("books.txt");
        if (inputFile.is_open()) {
            Book book;
            while (inputFile >> book.id) {
                inputFile.ignore();
                getline(inputFile, book.title);
                getline(inputFile, book.author);
                inputFile >> book.available;
                books.push_back(book);
            }
            inputFile.close();
        }
    }

    void loadIssuedBooks() {
        ifstream inputFile("issued_books.txt");
        if (inputFile.is_open()) {
            Book book;
            while (inputFile >> book.id) {
                inputFile.ignore();
                getline(inputFile, book.title);
                getline(inputFile, book.author);
                inputFile >> book.available;
                issuedBooks.push_back(book);
            }
            inputFile.close();
        }
    }

    void saveBooks() {
        ofstream outputFile("books.txt");
        if (outputFile.is_open()) {
            for (const Book& book : books) {
                outputFile << book.id << "\n";
                outputFile << book.title << "\n";
                outputFile << book.author << "\n";
                outputFile << book.available << "\n";
            }
            outputFile.close();
        }
    }

    void saveIssuedBooks() {
        ofstream outputFile("issued_books.txt");
        if (outputFile.is_open()) {
            for (const Book& book : issuedBooks) {
                outputFile << book.id << "\n";
                outputFile << book.title << "\n";
                outputFile << book.author << "\n";
                outputFile << book.available << "\n";
            }
            outputFile.close();
        }
    }

    int getNextBookID() {
        int maxID = 0;
        for (const Book& book : books) {
            if (book.id > maxID) {
                maxID = book.id;
            }
        }
        return maxID + 1;
    }

private:
    vector<Book> books;
    vector<Book> issuedBooks;
};

int main() {
    Library library;
    library.displayMenu();
    return 0;
}
