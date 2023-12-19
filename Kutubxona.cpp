#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <ctime>

using namespace std;

const string USER_CREDENTIALS_FILE = "user_credentials.txt";
const string ADMIN_CREDENTIALS_FILE = "admin_credentials.txt";
const string BOOKS_FILE = "books.txt";
const string BORROWED_FILE = "borrowed_books.txt";

#define RED_TEXT "\033[1;31m"
#define GREEN_TEXT "\033[1;32m"
#define YELLOW_TEXT "\033[1;33m"
#define BLUE_TEXT "\033[1;34m"
#define RESET_TEXT "\033[0m"

struct User {
    string username;
    string password;
    time_t registrationTime;
};

struct Book {
    string title{};
    string author{};
    int year{};
    string genre{};
    double price{};
    bool borrowed{};
    int totalCopies{};
    int copiesBorrowed{};
    time_t borrowDate{};
    string borrowerUsername{};
};

struct BorrowedBook {
    string title{};
    string borrowerUsername{};
    time_t borrowDate{};
};

vector<User> users;
vector<Book> books;

void searchBooks();
void viewAllBooks();
void userAuthentication();
void adminAuthentication();
void userPanel(string username);
void adminPanel(string adminUsername);
void displayMenu();
void addUser();
void removeUser();
void changeUserDetails(string username);
void addBook();
void removeBook();
void updateBook();
void displayBorrowedBooks(string username);
void borrowBook(string username);
void viewAllUsers();
void viewSpecificUser();
void loadUserCredentials();
void loadBooks();
void saveUserCredentials();
void saveAdminCredentials();
void saveBooks();
void changeAdminDetails(string adminUsername);
void searchByTitle();
void searchByAuthor();
void searchByYear();
void searchByGenre();
void searchByPrice();
void displayBookDetails(const Book& book);
void registerUser();

string toLowercase(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

void displayMenu() {
    cout << GREEN_TEXT << "\n\t*********** Kutubxona Tizimi ***********\n" << "\n\t\t\t    Menyu\n";
    cout << YELLOW_TEXT << "1. Royxatdan otish\n";
    cout << YELLOW_TEXT << "2. Kitoblarni Qidirish\n";
    cout << YELLOW_TEXT << "3. Foydalanuvchi\n";
    cout << YELLOW_TEXT << "4. Admin paneli\n";
    cout << BLUE_TEXT << "5. chiqish\n";
    cout << RED_TEXT << "Biror bir variantni tanlang: ";
}

void registerUser() {
    User newUser;
    cout << BLUE_TEXT << "Yengi username kiriting: ";
    cin >> newUser.username;

    for (const User& user : users) {
        if (user.username == newUser.username) {
            cout << "Bu username mavjud boshqa username kiriting.\n";
            return;
        }
    }

    cout << BLUE_TEXT << "Yengi Parolni kiriting: ";
    cin >> newUser.password;
    newUser.password = newUser.password;

    newUser.registrationTime = time(nullptr);

    users.push_back(newUser);
    saveUserCredentials();

    tm registrationTime;
    localtime_s(&registrationTime, &newUser.registrationTime);

    cout << GREEN_TEXT << "Foydalanuvchi muvaffaqiyatli royxatdan otdi.\n";
    cout << "Royxatdan otish vaqti: " << setw(2) << setfill('0') << registrationTime.tm_mday << "/"
        << setw(2) << setfill('0') << registrationTime.tm_mon + 1 << "/"
        << registrationTime.tm_year + 1900 << " "
        << setw(2) << setfill('0') << registrationTime.tm_hour << ":"
        << setw(2) << setfill('0') << registrationTime.tm_min << ":"
        << setw(2) << setfill('0') << registrationTime.tm_sec << "\n";
}

void searchBooks() {
    int searchChoice;
    cout << GREEN_TEXT << "\n\t*********** Kitoblarni qidirish ***********\n";
    cout << YELLOW_TEXT << "1. Nomi bilan qidirish\n";
    cout << YELLOW_TEXT << "2. Muallifi bilan qidirish\n";
    cout << YELLOW_TEXT << "3. Yili bilan qidirish\n";
    cout << YELLOW_TEXT << "4. Janri bilan qidirish\n";
    cout << YELLOW_TEXT << "5. Narxi bilan qidirish\n";
    cout << YELLOW_TEXT << "6. Hamma kitoblarni korish\n";
    cout << BLUE_TEXT << "7. Menyuga qaytish\n";
    cout << RED_TEXT << "Biror bir variantni tanlang: ";
    cin >> searchChoice;

    switch (searchChoice) {
    case 1:
        searchByTitle();
        break;
    case 2:
        searchByAuthor();
        break;
    case 3:
        searchByYear();
        break;
    case 4:
        searchByGenre();
        break;
    case 5:
        searchByPrice();
        break;
    case 6:
        viewAllBooks();
        break;
    case 7:
        cout << "Asosiy menyuga qaytish...\n";
        break;
    default:
        cout << "Xato tanlov. Asosiy menyuga qaytish...\n";
    }
}

void searchByTitle() {
    string title;
    cout << "Qidirish uchun kitob nomini kiriting: ";
    cin.ignore();
    getline(cin, title);

    bool found = false;

    cout << "\nKitob nomi boyicha qidiruv natijalari \"" << title << "\":\n";
    for (const Book& book : books) {
        if (toLowercase(book.title).find(toLowercase(title)) != string::npos) {
            displayBookDetails(book);
            found = true;
        }
    }

    if (!found) {
        cout << "Bu nomga ega kitoblar topilmadi.\n";
    }
}

void searchByAuthor() {
    string author;
    cout << "Qidirish uchun kitob muallifini kiriting: ";
    cin.ignore();
    getline(cin, author);

    bool found = false;

    cout << "\nKitob muallifi boyicha qidiruv natijalari \"" << author << "\":\n";
    for (const Book& book : books) {
        if (toLowercase(book.author).find(toLowercase(author)) != string::npos) {
            displayBookDetails(book);
            found = true;
        }
    }

    if (!found) {
        cout << "Berilgan muallifning kitoblari topilmadi.\n";
    }
}

void searchByYear() {
    int year;
    cout << "Qidirish uchun kitob yilini kiriting: ";
    cin >> year;

    bool found = false;

    cout << "\nKitob yili boyicha qidiruv natijalari " << year << ":\n";
    for (const Book& book : books) {
        if (book.year == year) {
            displayBookDetails(book);
            found = true;
        }
    }

    if (!found) {
        cout << "Bu yil nashr etilgan kitoblar topilmadi.\n";
    }
}

void searchByGenre() {
    string genre;
    cout << "Qidirsh uchun kitob janrini kiriting: ";
    cin.ignore();
    getline(cin, genre);

    bool found = false;

    cout << "\nKitob janiri boyicha qidiruv natijalari \"" << genre << "\":\n";
    for (const Book& book : books) {
        if (toLowercase(book.genre).find(toLowercase(genre)) != string::npos) {
            displayBookDetails(book);
            found = true;
        }
    }

    if (!found) {
        cout << "Ushbu janrda hech qanday kitob topilmadi.\n";
    }
}

void searchByPrice() {
    double minPrice, maxPrice;
    cout << "Minimal narxni kiriting: ";
    cin >> minPrice;
    cout << "Maksimal narxni kiriting: ";
    cin >> maxPrice;

    bool found = false;

    cout << "\nKitob narxi boyicha qidiruv natijalari $" << minPrice << " to $" << maxPrice << ":\n";
    for (const Book& book : books) {
        if (book.price >= minPrice && book.price <= maxPrice) {
            displayBookDetails(book);
            found = true;
        }
    }

    if (!found) {
        cout << "Berilgan narx oralig'ida hech qanday kitob topilmadi.\n";
    }
}

void displayBookDetails(const Book& book) {
    cout << "----------------------------------------------------------------------------------\n";
    cout << "|" << setw(15) << left << "Nomi" << "|" << setw(15) << "Muallifi" << "|" << setw(5) << "Yili" << "|"
        << setw(15) << "Janri" << "|" << setw(10) << "Narxi" << "|" << setw(15) << "Jami nusxalar" << "|\n";
    cout << "----------------------------------------------------------------------------------\n";
    cout << "|" << setw(15) << left << book.title << "|" << setw(15) << book.author << "|" << setw(5) << book.year << "|"
        << setw(15) << book.genre << "|" << fixed << setprecision(2) << setw(10) << book.price << "|" << setw(15) << book.totalCopies << "|\n";
    cout << "----------------------------------------------------------------------------------\n\n";
}

void viewAllBooks() {
    cout << "\n\t*********** Hamma Kitoblar ***********\n";
    cout << "---------------------------------------------------------------------------------------------\n";
    cout << std::left << setw(25) << "Nomi""|" << setw(20) << "Muallifi""|" << setw(10) << "Yili""|" << setw(15) << "Janri""|" << setw(10) << "Narxi""|" << setw(10) << setw(15) << "Jami nusxalar""|" << "\n";
    cout << "---------------------------------------------------------------------------------------------\n";

    for (const Book& book : books) {
        cout << setw(25) << book.title << setw(20) << book.author << setw(10) << book.year << setw(15) << book.genre << setw(10) << fixed << setprecision(2) << book.price << setw(15) << book.totalCopies << "|" "\n";
    }
    cout << "---------------------------------------------------------------------------------------------\n";
}

void changeAdminDetails(string adminUsername) {
    string newUsername, newPassword;
    cout << BLUE_TEXT << "Enter your new username: ";
    cin >> newUsername;

    cout << BLUE_TEXT << "Enter your new password: ";
    cin >> newPassword;

    for (User& user : users) {
        if (user.username == adminUsername) {
            user.username = newUsername;
            user.password = newPassword;
            saveUserCredentials();
            cout << GREEN_TEXT << "Username and password changed successfully.\n";
            return;
        }
    }

    cout << "Admin not found.\n";
}

void loadUserCredentials() {
    ifstream file(USER_CREDENTIALS_FILE);
    if (file.is_open()) {
        users.clear();
        User user;
        while (file >> user.username >> user.password) {
            users.push_back(user);
        }
        file.close();
    }
}

void loadBooks() {
    ifstream file(BOOKS_FILE);
    if (file.is_open()) {
        books.clear();
        Book book;
        while (file >> book.title >> book.author >> book.year >> book.genre >> book.price >> book.totalCopies >> book.borrowDate) {
            books.push_back(book);
        }
        file.close();
    }
}

void saveUserCredentials() {
    ofstream file(USER_CREDENTIALS_FILE);
    for (const User& user : users) {
        tm registrationTime;
        localtime_s(&registrationTime, &user.registrationTime);

        file << user.username << " " << user.password << " "
            << setw(2) << setfill('0') << registrationTime.tm_mday << "/"
            << setw(2) << setfill('0') << registrationTime.tm_mon + 1 << "/"
            << registrationTime.tm_year + 1900 << " "
            << setw(2) << setfill('0') << registrationTime.tm_hour << ":"
            << setw(2) << setfill('0') << registrationTime.tm_min << ":"
            << setw(2) << setfill('0') << registrationTime.tm_sec << "\n";
    }
    file.close();
}

void saveAdminCredentials() {
    ofstream file(ADMIN_CREDENTIALS_FILE);
    const User& admin = users.back();
    file << admin.username << "\n" << admin.password;
    file.close();
}

void saveBooks() {
    ofstream file(BOOKS_FILE);
    for (const Book& book : books) {
        file << book.title << " " << book.author << " " << book.year << " " << book.genre << " "
            << fixed << setprecision(2) << book.price << " " << book.totalCopies << " " << book.borrowDate << "\n";
    }
    file.close();
}

void userAuthentication() {
    string username, password;
    cout << BLUE_TEXT << "Usernameni kiriting: ";
    cin >> username;
    cout << BLUE_TEXT << "Parolni kiriting: ";
    cin >> password;

    for (const User& user : users) {
        if (user.username == username && user.password == password) {
            userPanel(username);
            return;
        }
    }

    cout << "Foydalanuvchi nomi yoki parol notogri. Iltimos, yana bir bor urinib ko'ring.\n";
}

void adminAuthentication() {
    string adminUsername = "admin";
    string adminPassword = "123";

    string username, password;
    cout << BLUE_TEXT << "Admin usernameni kiriting: ";
    cin >> username;

    if (adminUsername == username) {
        cout << BLUE_TEXT << "Admin parolini kiriting: ";
        cin >> password;

        if (adminPassword == password) {
            adminPanel(username);
        }
        else {
            cout << "Administrator paroli notogri. Iltimos, yana bir bor urinib ko'ring.\n";
        }
    }
    else {
        cout << "Administrator username notogri. Iltimos, yana bir bor urinib ko'ring.\n";
    }
}

void userPanel(string username) {
    int choice;
    do {
        cout << GREEN_TEXT << "\n\t*********** Foydalanuvchi Paneli ***********\n";
        cout << YELLOW_TEXT << "1. Username va parolni ozgartirish\n";
        cout << YELLOW_TEXT << "2. Kitoblarni qidirish\n";
        cout << YELLOW_TEXT << "3. Kitob sotib olish\n";
        cout << YELLOW_TEXT << "4. View borrowed books\n";
        cout << BLUE_TEXT << "5. Logout\n";
        cout << RED_TEXT << "Biror bir variantni tanlang: ";
        cin >> choice;

        switch (choice) {
        case 1:
            changeUserDetails(username);
            break;
        case 2:
            searchBooks();
            break;
        case 3:
            borrowBook(username);
            break;
            //case 4:
            //displayBorrowedBooks(false, username);
             //break;
        case 5:
            cout << "Chiqish...\n";
            break;
        default:
            cout << "Yaroqsiz tanlov. Iltimos, yana bir bor urinib ko'ring.\n";
        }
    } while (choice != 5);
}

void addUser() {
    User newUser;
    cout << BLUE_TEXT << "Yengi username kiriting: ";
    cin >> newUser.username;

    for (const User& user : users) {
        if (user.username == newUser.username) {
            cout << "Bu username mavjud boshqa username kiriting.\n";
            return;
        }
    }

    cout << BLUE_TEXT << "Yengi Parolni kiriting: ";
    cin >> newUser.password;
    newUser.password = newUser.password;

    users.push_back(newUser);
    saveUserCredentials();

    cout << GREEN_TEXT << "Foydalanuvchi muvaffaqiyatli qo'shildi.\n";
}

void removeUser() {
    string username;
    cout << YELLOW_TEXT << "O'chirish uchun foydalanuvchi nomini kiriting: ";
    cin >> username;

    auto it = find_if(users.begin(), users.end(), [username](const User& user) {
        return user.username == username;
        });

    if (it != users.end()) {
        users.erase(it);
        saveUserCredentials();
        cout << GREEN_TEXT << "Foydalanuvchi muvaffaqiyatli olib tashlandi.\n";
    }
    else {
        cout << "Foydalanuvchi topilmadi.\n";
    }
}

void addBook() {
    Book newBook;
    cout << "Yangi kitobning nomini kiriting: ";
    cin.ignore();
    getline(cin, newBook.title);

    cout << "Yangi kitob muallifini kiriting: ";
    getline(cin, newBook.author);

    cout << "Nashr qilingan yilni kiriting: ";
    cin >> newBook.year;

    cout << "Yangi kitobning janrini kiriting: ";
    cin.ignore();
    getline(cin, newBook.genre);

    cout << "Yangi kitobning narxini kiriting: ";
    cin >> newBook.price;

    cout << "Mavjud nusxalarning umumiy sonini kiriting: ";
    cin >> newBook.totalCopies;
    newBook.copiesBorrowed = 0;

    newBook.borrowed = false;

    books.push_back(newBook);
    saveBooks();

    cout << GREEN_TEXT << "Kitob muvaffaqiyatli qoshildi.\n";
}

void removeBook() {
    string title;
    cout << YELLOW_TEXT << "Ochirish uchun kitob nomini kiriting: ";
    cin.ignore();
    getline(cin, title);

    auto it = find_if(books.begin(), books.end(), [title](const Book& book) {
        return book.title == title;
        });

    if (it != books.end()) {
        books.erase(it);
        saveBooks();
        cout << GREEN_TEXT << "Kitob muvaffaqiyatli olib tashlandi.\n";
    }
    else {
        cout << "Kitob topilmadi.\n";
    }
}

void updateBook() {
    string title;
    cout << "Yangilash uchun kitob nomini kiriting: ";
    cin.ignore();
    getline(cin, title);

    auto it = find_if(books.begin(), books.end(), [title](const Book& book) {
        return book.title == title;
        });

    if (it != books.end()) {
        cout << "Kitobning yangi nomini kiriting: ";
        getline(cin, it->title);

        cout << "Kitobning yangi muallifini kiriting: ";
        getline(cin, it->author);

        cout << "Yangi nashr yilini kiriting: ";
        cin >> it->year;

        cout << "Kitobning yangi janrini kiriting: ";
        cin.ignore();
        getline(cin, it->genre);

        cout << "Kitobning yangi narxini kiriting: ";
        cin >> it->price;

        saveBooks();

        cout << GREEN_TEXT << "Kitob muvaffaqiyatli yangilandi.\n";
    }
    else {
        cout << "Kitob topilmadi.\n";
    }
}

void displayBorrowedBooks(bool displayAll = false, string username = "") {
    cout << "\nKitoblar " << (displayAll ? "Foydalanuvchilar tomonidan olingan kitoblar" : "tomonidan olingan " + username) << ":\n";
    bool found = false;

    ifstream borrowedFile(BORROWED_FILE);
    if (borrowedFile.is_open()) {
        BorrowedBook borrowedBook;
        while (borrowedFile >> borrowedBook.title >> borrowedBook.borrowerUsername >> borrowedBook.borrowDate) {
            if (displayAll || borrowedBook.borrowerUsername == username) {
                tm borrowTime;
                localtime_s(&borrowTime, &borrowedBook.borrowDate);

                cout << "Nomi: " << borrowedBook.title << "\nOluvchi: " << borrowedBook.borrowerUsername
                    << "\nOlingan vaqti: " << borrowTime.tm_mday << "/" << borrowTime.tm_mon + 1 << "/" << borrowTime.tm_year + 1900
                    << "\n\n";
                found = true;
            }
        }
        borrowedFile.close();
    }
    else {
        cout << RED_TEXT << "Xato: olingan kitoblar faylini ochib bolmadi.\n";
    }

    if (!found) {
        cout << "Kitoblar yoq " << (displayAll ? "Olingan kitoblar yoq" : "Oluvchi " + username) << ".\n";
    }
}

void viewAllUsers() {
    cout << "\nBarcha foydalanuvchilar royxati:\n";
    for (const User& user : users) {
        tm registrationTime;
        localtime_s(&registrationTime, &user.registrationTime);

        cout << BLUE_TEXT << "Foydalanuvchi: " << user.username << "|"
            << "Ro'yxatdan otish vaqti: " << setw(2) << setfill('0') << registrationTime.tm_mday << "/"
            << setw(2) << setfill('0') << registrationTime.tm_mon + 1 << "/"
            << registrationTime.tm_year + 1900 << " "
            << setw(2) << setfill('0') << registrationTime.tm_hour << ":"
            << setw(2) << setfill('0') << registrationTime.tm_min << ":"
            << setw(2) << setfill('0') << registrationTime.tm_sec
            << "\n\n";
    }
}

void viewSpecificUser() {
    string username;
    cout << YELLOW_TEXT << "Foydalanuvchi nomini kiriting: ";
    cin >> username;

    auto it = find_if(users.begin(), users.end(), [username](const User& user) {
        return user.username == username;
        });

    if (it != users.end()) {
        tm registrationTime;
        localtime_s(&registrationTime, &it->registrationTime);

        cout << "\nFoydalanuvchi tafsilotlari:\n";
        cout << "Foydalanuvchi: " << it->username << "\n"
            << "Ro'yxatdan otish vaqti: " << setw(2) << setfill('0') << registrationTime.tm_mday << "/"
            << setw(2) << setfill('0') << registrationTime.tm_mon + 1 << "/"
            << registrationTime.tm_year + 1900 << " "
            << setw(2) << setfill('0') << registrationTime.tm_hour << ":"
            << setw(2) << setfill('0') << registrationTime.tm_min << ":"
            << setw(2) << setfill('0') << registrationTime.tm_sec
            << "\n\n";
    }
    else {
        cout << "Foydalanuvchi topilmadi.\n";
    }
}

void adminPanel(string adminUsername) {
    int choice;
    do {
        cout << GREEN_TEXT << "\n\t*********** Admin Paneli ***********\n";
        cout << BLUE_TEXT << "1. Kitoblarni qidirsh\n";
        cout << BLUE_TEXT << "2. Yengi foydalanuvchi qoshish\n";
        cout << BLUE_TEXT << "3. Foydalanuvchini ochirish\n";
        cout << BLUE_TEXT << "4. Barcha foydalanuvchilar korish\n";
        cout << BLUE_TEXT << "5. Yengi kitob qoshish\n";
        cout << BLUE_TEXT << "6. Kitobni ochirish\n";
        cout << BLUE_TEXT << "7. Kitobni yangilash\n";
        cout << BLUE_TEXT << "8. Foydalanuvchilar tomonidan olingan kitoblar\n";
        cout << BLUE_TEXT << "9. Change admin username and password\n";
        cout << BLUE_TEXT << "10. Foydalanuvchini ko'rish\n";
        cout << YELLOW_TEXT << "11. Chiqish\n";
        cout << RED_TEXT << "Biror bir variantni tanlang: ";
        cin >> choice;

        switch (choice) {
        case 1:
            searchBooks();
            break;
        case 2:
            addUser();
            break;
        case 3:
            removeUser();
            break;
        case 4:
            viewAllUsers();
            break;
        case 5:
            addBook();
            break;
        case 6:
            removeBook();
            break;
        case 7:
            updateBook();
            break;
        case 8:
            displayBorrowedBooks(true);
            break;
        case 9:
            changeAdminDetails(adminUsername);
            break;
        case 10:
            viewSpecificUser();
            break;
        case 11:
            cout << "Chiqish...\n";
            break;
        default:
            cout << "Yaroqsiz tanlov. Iltimos, yana bir bor urinib ko'ring.\n";
        }

    } while (choice != 11);
}

void changeUserDetails(string username) {
    string newUsername, newPassword;
    cout << "Yengi username kiriting: ";
    cin >> newUsername;

    cout << "Yengi parolni kiriting: ";
    cin >> newPassword;

    for (User& user : users) {
        if (user.username == username) {
            user.username = newUsername;
            user.password = newPassword;
            saveUserCredentials();
            cout << "Username va parol muvaffaqiyatli o'zgartirildi.\n";
            return;
        }
    }

    cout << "Foydalanuvchi topilmadi.\n";
}

void borrowBook(string username) {
    string title;
    cout << YELLOW_TEXT << "Kitobni olish uchun nomini kiriting: ";
    cin.ignore();
    getline(cin, title);

    auto it = find_if(books.begin(), books.end(), [title](const Book& book) {
        return book.title == title;
        });

    if (it != books.end()) {
        if (it->totalCopies > 0) {
            it->totalCopies--;
            it->copiesBorrowed++;
            it->borrowerUsername = username;
            it->borrowDate = time(nullptr);

            BorrowedBook borrowedBook;
            borrowedBook.title = it->title;
            borrowedBook.borrowerUsername = username;
            borrowedBook.borrowDate = it->borrowDate;

            ofstream borrowedFile(BORROWED_FILE, ios::app);
            if (borrowedFile.is_open()) {
                borrowedFile << borrowedBook.title << " " << borrowedBook.borrowerUsername << " " << borrowedBook.borrowDate << "\n";
                borrowedFile.close();
            }
            else {
                cout << RED_TEXT << "Xato: kitoblar faylini ochib boʻlmadi.\n";
            }
            saveBooks();
            cout << GREEN_TEXT << "Kitob muvaffaqiyatli olindi.\n";
        }
        else {
            cout << RED_TEXT << "Kitobning mavjud nusxalari yo'q\n";
        }
    }
    else {
        cout << "Kitob topilmadi.\n";
    }
}

int main() {
    loadUserCredentials();
    loadBooks();

    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            registerUser();
            break;
        case 2:
            searchBooks();
            break;
        case 3:
            userAuthentication();
            break;
        case 4:
            adminAuthentication();
            break;
        case 5:
            cout << "Dasturdan chiqish...\n";
            break;
        default:
            cout << "Yaroqsiz tanlov. Iltimos, yana bir bor urinib ko'ring.\n";
        }

    } while (choice != 5);

    return 0;
}