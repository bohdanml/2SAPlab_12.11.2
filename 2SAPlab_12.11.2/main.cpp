#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Book {
    int udcNumber;
    string authorName;
    string title;
    int year;
    int copies;

    Book* left;
    Book* right;

    Book(int udc, const string& author, const string& t, int y, int c)
        : udcNumber(udc), authorName(author), title(t), year(y), copies(c), left(nullptr), right(nullptr) {}
};

class Library {
private:
    Book* root;

    void addBookHelper(Book*& node, int udc, const string& author, const string& title, int year, int copies);
    void removeBookHelper(Book*& node, int udc);
    void displayLibraryHelper(const Book* node) const;
    void deleteTree(Book*& node);
    void readBooksFromFileHelper(Book*& node, ifstream& file);
    void writeBooksToFileHelper(const Book* node, ofstream& file) const;

public:
    Library() : root(nullptr) {}
    ~Library() { deleteTree(root); }

    void addBook(int udc, const string& author, const string& title, int year, int copies);
    void removeBook(int udc);
    void displayLibrary() const;
    void readBooksFromFile(const string& filename);
    void writeBooksToFile(const string& filename) const;
};

void Library::addBookHelper(Book*& node, int udc, const string& author, const string& title, int year, int copies) {
    if (node == nullptr) {
        node = new Book(udc, author, title, year, copies);
    }
    else {
        if (udc < node->udcNumber) {
            addBookHelper(node->left, udc, author, title, year, copies);
        }
        else {
            addBookHelper(node->right, udc, author, title, year, copies);
        }
    }
}

void Library::addBook(int udc, const string& author, const string& title, int year, int copies) {
    addBookHelper(root, udc, author, title, year, copies);
}

void Library::removeBookHelper(Book*& node, int udc) {
    if (node == nullptr) {
        return;
    }
    if (udc < node->udcNumber) {
        removeBookHelper(node->left, udc);
    }
    else if (udc > node->udcNumber) {
        removeBookHelper(node->right, udc);
    }
    else {
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        }
        else if (node->left == nullptr) {
            Book* temp = node;
            node = node->right;
            delete temp;
        }
        else if (node->right == nullptr) {
            Book* temp = node;
            node = node->left;
            delete temp;
        }
        else {
            Book* temp = node->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            node->udcNumber = temp->udcNumber;
            removeBookHelper(node->right, temp->udcNumber);
        }
    }
}

void Library::removeBook(int udc) {
    removeBookHelper(root, udc);
}

void Library::displayLibraryHelper(const Book* node) const {
    if (node != nullptr) {
        displayLibraryHelper(node->left);
        cout << "UDC number: " << node->udcNumber << endl;
        cout << "Author: " << node->authorName << endl;
        cout << "Title: " << node->title << endl;
        cout << "Year of publication: " << node->year << endl;
        cout << "Copies available: " << node->copies << endl;
        cout << "---------------------------" << endl;
        displayLibraryHelper(node->right);
    }
}

void Library::displayLibrary() const {
    displayLibraryHelper(root);
}

void Library::deleteTree(Book*& node) {
    if (node != nullptr) {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
        node = nullptr;
    }
}

void Library::readBooksFromFileHelper(Book*& node, ifstream& file) {
    int udc;
    string author, title;
    int year, copies;
    while (file >> udc >> ws && getline(file, author) && getline(file, title) && file >> year >> copies) {
        addBookHelper(node, udc, author, title, year, copies);
    }
}

void Library::readBooksFromFile(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        readBooksFromFileHelper(root, file);
        file.close();
    }
    else {
        cout << "Unable to open file." << endl;
    }
}

void Library::writeBooksToFileHelper(const Book* node, ofstream& file) const {
    if (node != nullptr) {
        writeBooksToFileHelper(node->left, file);
        file << node->udcNumber << endl;
        file << node->authorName << endl;
        file << node->title << endl;
        file << node->year << endl;
        file << node->copies << endl;
        writeBooksToFileHelper(node->right, file);
    }
}

void Library::writeBooksToFile(const string& filename) const {
    ofstream file(filename);
    if (file.is_open()) {
        writeBooksToFileHelper(root, file);
        file.close();
    }
    else {
        cout << "Unable to open file." << endl;
    }
}

int main() {
    Library library;
    string filename;

    cout << "Enter filename: ";
    cin >> filename;

    library.readBooksFromFile(filename);

    int choice;
    do {
        cout << "1. Add book\n2. Remove book\n3. Display library\n4. Save library to file\n0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            int udc;
            string author, title;
            int year, copies;
            cout << "Enter UDC number: ";
            cin >> udc;
            cout << "Enter author's name: ";
            cin >> ws; 
            getline(cin, author);
            cout << "Enter title: ";
            getline(cin, title);
            cout << "Enter year of publication: ";
            cin >> year;
            cout << "Enter number of copies: ";
            cin >> copies;
            library.addBook(udc, author, title, year, copies);
            break;
        }
        case 2: {
            int udc;
            cout << "Enter UDC number of the book to remove: ";
            cin >> udc;
            library.removeBook(udc);
            break;
        }
        case 3:
            library.displayLibrary();
            break;
        case 4:
            library.writeBooksToFile(filename);
            cout << "Library saved to file." << endl;
            break;
        case 0:
            cout << "Exiting program." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);

    return 0;
}
