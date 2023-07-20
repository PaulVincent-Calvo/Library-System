#include <iostream>
#include <vector>
using namespace std;

// class for student accounts -- to be used for linked list
class student_account 
{
    public:
        string name, username, password;
        int srcode;
        student_account* next;

        // constructor that initializes every next pointer to nullptr whenever an object is created from the class
        student_account():next(nullptr){};  
};

// struct for books that are inside the shelf
struct book
{
    string book_code;    
    string book_title;
    string author;
    bool availability;
};

// class for shelf in the library
class shelf 
{
    public:
        int number_of_rows = 5;
        int size_of_row = 2;
        vector<vector<book>> rows;
};

enum request_type
{
    borrowing,
    returning
};

class request_sheet
{
    public:
        student_account* requester_ptr;  
        int srcode; 
        string name;
        string book_code;
        request_type type;

        request_sheet(student_account* requester, string book_code, request_type type) : requester_ptr(requester), type (type)
        {
            srcode = requester_ptr->srcode;
            name = requester_ptr->name;
            this->book_code = book_code;
        }
};

// arrrays for admin accounts -- to be used for binary search or linear search
string admin_accounts_username[3] = {"paulvincent-calvo", "trevor-marcos","kevinhans-mirabel"};
string admin_accounts_password[3] = {"libraryadmin1", "libraryadmin2", "libraryadmin3"};

// for aesthetics only -- used to either make the ui look good, the code shorter, or to avoid nesting
void header();
void line_printer();
int exit_program();
void login_failed();
int input_checker();

// login verification for admins and students
student_account* student_login_verification (student_account* head, string username, string password);
bool binarysearch_admin_login(string username, string password);
bool linearsearch_admin_login(string username, string password);

// main
void main_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves);

// new functions
void add_remove_books_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves);
void displaybooks(const shelf &shelf_to_display); // for admins -- shows every book in the library even if unavailable
void admin_displaybooks_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves);
void student_available_books_menu(student_account*& head, student_account* current_user, vector<request_sheet> &all_requests, vector<shelf> &library_shelves);
void display_available_books(const shelf &shelf_to_display); // for students -- only shows books that are available
void display_unavailable_books(const shelf &shelf_to_display);
void borrow_book_menu(student_account*& head, student_account* current_user, vector<request_sheet> &all_requests,vector<shelf> &library_shelves); 
void return_book_menu(student_account*& head, student_account* current_user, vector<request_sheet> &all_requests,vector<shelf> &library_shelves);
void admin_process_requests_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves);
void all_requests_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves);
void process_requests_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves);
bool remove_book_from_shelf(shelf& target_shelf, const string& book_code);
void remove_books_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves);
bool add_book_to_shelf(shelf& target_shelf, const string& book_code, const string& book_title, const string& author, bool availability);
void add_books_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves);
// functions for admin accounts
void library_admin_login_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves);
void display_accounts_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves);
void search_student_acc_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves);
void library_admin_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves);

void display_all_student_accounts(const student_account* head);
const student_account* find_student_acc(const student_account* head, int srcode);
bool borrowing_check_book_code(string book_code, const shelf &shelf_to_display);
bool returning_check_book_code(string book_code, const shelf &shelf_to_display);

// functions for student accounts  
student_account* register_student_account(student_account* head, int srcode, string name, string username, string password);
void student_menu(student_account*& head,  student_account* current_user, vector<request_sheet> &all_requests, vector<shelf> &library_shelves);
void student_login_page (student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves);
void student_login_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves);
void student_registration_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelvess);

int main()
{        
    vector<shelf> library_shelves;
    vector<request_sheet>all_requests;

    // built in shelves in the system
    shelf programming_books;
    shelf science_books;
    shelf math_books;
    shelf engineering_books;

    programming_books.rows = {
        {{"PRG-001", "Clean Code", "Robert C. Martin", false}, {"PRG-002", "Code Complete", "Steve McConnell", true}},
        {{"PRG-003", "Cracking the Coding Interview", "Gayle Laakmann McDowell", true}, {"PRG-004", "Design Patterns", "Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides", true}},
        {{"PRG-005", "Effective Modern C++", "Scott Meyers", false}, {"PRG-006", "Head First Java", "Kathy Sierra, Bert Bates", false}},
        {{"PRG-007", "Introduction to Algorithms", "Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, Clifford Stein", true}, {"PRG-008", "Python Crash Course", "Eric Matthes", true}},
        {{"PRG-009", "The C++ Programming Language", "Bjarne Stroustrup", true}, {"PRG-010", "The Pragmatic Programmer", "Andrew Hunt, David Thomas", true}},
    };

    science_books.rows = {
        {{"SCI-001", "A Brief History of Time", "Stephen Hawking", true}, {"SCI-002", "Cosmos", "Carl Sagan", true}},
        {{"SCI-003", "Gödel, Escher, Bach: An Eternal Golden Braid", "Douglas R. Hofstadter", true}, {"SCI-004", "Sapiens: A Brief History of Humankind", "Yuval Noah Harari", true}},
        {{"SCI-005", "The Code Book", "Simon Singh", false}, {"SCI-006", "The Double Helix", "James D. Watson", true}},
        {{"SCI-007", "The Elegant Universe", "Brian Greene", true}, {"SCI-008", "The Origin of Species", "Charles Darwin", true}},
        {{"SCI-009", "The Selfish Gene", "Richard Dawkins", true}, {"SCI-010", "The Structure of Scientific Revolutions", "Thomas S. Kuhn", false}},
    };
    
    math_books.rows = {
        {{"MTH-001", "A Mathematician's Apology", "G. H. Hardy", true}, {"MTH-002", "Fermat's Enigma: The Epic Quest to Solve the World's Greatest Mathematical Problem", "Simon Singh", false}},
        {{"MTH-003", "Flatland: A Romance of Many Dimensions", "Edwin A. Abbott", true}, {"MTH-004", "How Not to Be Wrong: The Power of Mathematical Thinking", "Jordan Ellenberg", true}},
        {{"MTH-005", "Journey through Genius: The Great Theorems of Mathematics", "William Dunham", true}, {"MTH-006", "Mathematics: From the Birth of Numbers", "Jan Gullberg", true}},
        {{"MTH-007", "The Art of Mathematics: Coffee Time in Memphis", "Béla Bollobás", true}, {"MTH-008", "The Joy of x: A Guided Tour of Math, from One to Infinity", "Steven H. Strogatz", true}},
        {{"MTH-009", "The Princeton Companion to Mathematics", "Timothy Gowers", true}, {"MTH-010", "Zero: The Biography of a Dangerous Idea", "Charles Seife", true}},
    };

    engineering_books.rows = {
        {{"ENG-001", "Engineering Economy", "William G. Sullivan, Elin M. Wicks, C. Patrick Koelling", true}, {"ENG-002", "Engineering Fundamentals: An Introduction to Engineering", "Saeed Moaveni", true}},
        {{"ENG-003", "Engineering Mechanics: Dynamics", "J.L. Meriam, L.G. Kraige", true}, {"ENG-004", "Engineering Mechanics: Statics", "J.L. Meriam, L.G. Kraige", true}},
        {{"ENG-005", "Introduction to Electric Circuits", "James A. Svoboda, Richard C. Dorf", true}, {"ENG-006", "Introduction to Flight", "John D. Anderson Jr.", true}},
        {{"ENG-007", "Introduction to Mechatronics and Measurement Systems", "David G. Alciatore, Michael B. Histand", true}, {"ENG-008", "Introduction to Robotics: Mechanics and Control", "John J. Craig", true}},
        {{"ENG-009", "Machine Design", "Robert L. Norton", true}}
    };

    library_shelves.push_back(programming_books);
    library_shelves.push_back(science_books);
    library_shelves.push_back(math_books);
    library_shelves.push_back(engineering_books);

    student_account* head = nullptr;
    head = register_student_account(head, 1, "John Doe", "johndoe", "johndoepassword");
    head = register_student_account(head, 2, "Jane Smith", "janesmith", "janesmithpassword");

    // pre-made requests
    request_sheet new_request1(head, "SCI-001", request_type::borrowing);
    all_requests.push_back(new_request1);

    request_sheet new_request2(head, "ENG-005", request_type::returning);
    all_requests.push_back(new_request2);

    main_menu(head, all_requests, library_shelves);

    return 0;
}

void main_menu(student_account*& head, vector<request_sheet> &all_requests,vector<shelf> &library_shelves)
{    
    int menu_choice;
    system("cls");
    header();
    cout << "                                           Main Menu" << endl;
    line_printer();

    cout << endl << endl << "Plase choose an action." << endl;
    cout << "[1] Log in as Admin" << endl << "[2] Log in as Student" << endl << "[3] Exit" << endl;
    
    while (true)
    {
        menu_choice = input_checker();
        if (menu_choice > 3) 
        {
            cout << "Please input an integer from the options given." << endl;
        }
        else
        {
            break;
        }
    }    

    if (menu_choice == 1)
    {
        library_admin_login_menu(head, all_requests,library_shelves);
    }
    else if (menu_choice == 2)
    {
        student_login_menu(head, all_requests,library_shelves);
    }
    else if (menu_choice == 3)
    {
        exit_program();
    }
}

// ----------------------------- for styling the ui or the code itself
void header()
{
    for (int i = 0; i < 100; i++) // i
    {
        cout << "=";
    }

    cout << endl << "                            Batangas State University Library Services" << endl; 

    for (int j = 0; j < 100; j++) // j
    {
        cout << "=";
    }

    cout << endl;
}

void line_printer()
{
    for (int j = 0; j < 100; j++) 
    {
        cout << "="; 
    }
}

int exit_program()
{
    system("cls");
    header();
    cout << "                                           Exit Page" << endl;
    line_printer();

    cout << endl << endl << "Thank you for using the program.";

    return 0; 
}

void login_failed()
{
    // when user login fails, they are given the same choices; try again or exit the program
    system("cls");
    header();
    cout << "                                            Login Failed" << endl;
    line_printer();

    cout << endl << endl << "Login attempt unsuccessful. Incorrect username and/or password.";
    cout << endl << endl << "Please choose an option"; 
    cout << endl << "[1] Try again" << endl << "[2] Exit the program";
}

int input_checker()
{
    // since we always ask for an integer from the user when they want to do something, this function will always be used to get an integer from them
    // after getting an integer, we can just check to see if the integer that the user provided is valid based on the number(the variable "choice") 
    // that this function will return
    string user_choice_placeholder;
    int user_choice;

    while (true)
    {   
        try 
        {
            cout << endl << "Please input your choice: ";
            cin >> user_choice_placeholder;
            user_choice = stoi(user_choice_placeholder);
            break;
        }
        catch (const exception&)
        {
            cout << "Please input a valid option." << endl;
        }
    }
    return user_choice;
}

// ---------------------------------- for login verification
student_account* student_login_verification (student_account* head, string username, string password)
{
    student_account* current = head;
    while (current != nullptr) 
    {
        if (current->username == username && current->password == password) 
        {
            // return pointer if the username and password matches
            return current;                  
        }
        current = current->next;
    }
    // return null if the username/password is incorrect
    return nullptr;  
}

bool binarysearch_admin_login(string username, string password)
{
    int count = sizeof(admin_accounts_username) / sizeof(admin_accounts_username[0]);
    int min = 0;
    int max = count - 1;
    int current_index;
    bool username_accepted = false, password_accepted = false; 

    // binary search to confirm that the username is valid
    while (min <= max && !username_accepted)
    {
        current_index = (min+max)/2;
        if (username.compare(admin_accounts_username[current_index]) < 0)
        {
            max = current_index - 1;
        }
        else if(username.compare(admin_accounts_username[current_index]) > 1)
        {
            min = current_index + 1;
        }
        else 
        {
            username_accepted = true;    
            break;
        }
    }

    if (username_accepted)
    {
        // Check if the password is valid for the corresponding username
        if (password == admin_accounts_password[current_index])
        {
            password_accepted = true;
        }
    }
    
    // return true if both username and password are valid, else, false
    return (username_accepted && password_accepted);
}

bool linearsearch_admin_login(string username, string password)
{
    int count = sizeof(admin_accounts_username) / sizeof(admin_accounts_username[0]);
    bool username_accepted = false, password_accepted = false; 

    for (int current_index = 0; current_index < count; current_index++)
    {
        // checks if the username is valid from the array of admin_acounts_username using linear search
        if (username == admin_accounts_username[current_index])
        {
            username_accepted = true;
            // checks if the password is valid on the same index that the username was found
            if (password == admin_accounts_password[current_index])
            {
                password_accepted = true;
                break;
            }
        }
    }

    // return true if both username and password are valid, else, false
    return (username_accepted && password_accepted);
}

// ----------------------------------- for admin menus
void library_admin_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves)
{
    int lib_admin_menu_choice;

    system("cls");
    header();
    cout << "                                             Admin Menu" << endl;
    line_printer();

    cout << endl << endl << "Please choose an action." << endl; 
    cout << "[1] Display Books" << endl <<  "[2] Add/Remove Book" << endl << "[3] Process Student Requests" << endl << "[4] Display all student account information" << endl << "[5] Search for a student account " << endl << "[6] Return to Main Menu" << endl <<"[7] Exit the program" << endl;

    while (true)
    {
        lib_admin_menu_choice = input_checker();

        if (lib_admin_menu_choice > 6 || lib_admin_menu_choice < 1)
        {
            // for when the user enters an integer that is not on the options
            cout << "Please input an integer from the options given." << endl; 
        }
        else
        {
            break;
        }
    }

    if (lib_admin_menu_choice == 1)
    {
        admin_displaybooks_menu(head, all_requests,library_shelves);
    }
    else if (lib_admin_menu_choice == 2)
    {
        add_remove_books_menu(head, all_requests, library_shelves);
    }
    else if (lib_admin_menu_choice == 3)
    {
        admin_process_requests_menu(head, all_requests, library_shelves);
    }
    else if (lib_admin_menu_choice == 4)
    {
        display_accounts_menu(head, all_requests,library_shelves);
    }
    else if (lib_admin_menu_choice == 5)
    {
        search_student_acc_menu(head, all_requests,library_shelves);
    } 
    else if (lib_admin_menu_choice == 6)
    {
        main_menu(head, all_requests,library_shelves);
    }
    else if (lib_admin_menu_choice == 7)
    {
        exit_program();
    }
}

void displaybooks(const shelf &shelf_to_display)
{
    for (int row = 0; row < shelf_to_display.number_of_rows; row++)
    {
        for (const book& b : shelf_to_display.rows[row])
        {
            cout << "Book Code: " << b.book_code << endl;
            cout << "Book Title: " << b.book_title << endl;
            cout << "Author: " << b.author << endl;
            cout << "Availability: " << (b.availability ? "Available" : "Not Available") << endl;
            cout << endl;
        }
    }
}

void admin_displaybooks_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves)
{
    int displaybooks_choice, displaybooks_again;

    system("cls");
    header();
    cout << "                                         Display Books Menu" << endl;
    line_printer();

    cout << endl << endl << "Please choose an action." << endl; 
    cout << "[1] Display All Books" << endl << "[2] Display Programming Books" << endl << "[3] Display Science Books" << endl << "[4] Display Math Books" << endl << "[5] Display Engineering Books" << endl << "[6] Return to Admin Menu" << endl << "[7] Return to Main Menu" << endl << "[8] Exit Program" << endl;

    while (true)
    {
        displaybooks_choice = input_checker();

        if (displaybooks_choice > 8 || displaybooks_choice < 1)
        {
            // for when the user enters an integer that is not on the options
            cout << "Please input an integer from the options given." << endl; 
        }
        else
        {
            break;
        }
    }

    if (displaybooks_choice == 1)
    {
        system("cls");
        header();
        cout << "                                         Display Books Menu" << endl;
        line_printer();

        cout << endl << endl;

        for (size_t i = 0; i < library_shelves.size(); ++i)
        {
            if (i == 0)
            {
                line_printer();
                cout << endl;
                cout << "                                           Programming Books" << endl;
                line_printer();
                cout << endl;
            }
            else if (i == 1)
            {
                line_printer();
                cout << endl;
                cout << "                                           Science Books" << endl;
                line_printer();
                cout << endl;
            }
            else if (i == 2)
            {
                line_printer();
                cout << endl;
                cout << "                                             Math Books" << endl;
                line_printer();
                cout << endl;
            }
            else if (i == 3)
            {
                line_printer();
                cout << endl;
                cout << "                                         Engineering Books" << endl;
                line_printer();
                cout << endl;
            }
            displaybooks(library_shelves[i]);
        }
    }
    else if (displaybooks_choice == 2)
    {
        system("cls");
        header();
        cout << "                                           Programming Books" << endl;
        line_printer();
        cout << endl;
        displaybooks(library_shelves[0]);
    }
    else if (displaybooks_choice == 3)
    {
        system("cls");
        header();
        cout << "                                           Science Books" << endl;
        line_printer();
        cout << endl;
        displaybooks(library_shelves[1]);
    }
    else if (displaybooks_choice == 4)
    {
        system("cls");
        header();
        cout << "                                             Math Books" << endl;
        line_printer();
        cout << endl;
        displaybooks(library_shelves[2]);
    }
    else if (displaybooks_choice == 5)
    {
        system("cls");
        header();
        cout << "                                         Engineering Books" << endl;
        line_printer();
        cout << endl;
        displaybooks(library_shelves[3]);
    }
    else if (displaybooks_choice == 6)
    {
        library_admin_menu(head, all_requests, library_shelves);
    }
    else if (displaybooks_choice == 7)
    {
        main_menu(head, all_requests, library_shelves);
    }
    else if (displaybooks_choice == 8)
    {
        exit_program();
    }
    
    // if they don't choose to return to other menus or exit the program and they chose to display books
    if (displaybooks_choice !=8)
    {
        line_printer();
        cout << endl << endl << "Please choose an action." << endl; 
        cout << "[1] Return to Display Books Menu" << endl << "[2] Return to Admin Menu" << endl << "[3] Return to Main Menu" << endl << "[4] Exit Program" << endl;

        while (true)
        {
            displaybooks_again = input_checker();

            if (displaybooks_again > 4 || displaybooks_again < 1)
            {
                // for when the user enters an integer that is not on the options
                cout << "Please input an integer from the options given." << endl; 
            }
            else
            {
                break;
            }
        }

        if (displaybooks_again == 1)
        {
            admin_displaybooks_menu(head, all_requests, library_shelves);
        }
        else if (displaybooks_again == 2)
        {
            library_admin_menu(head, all_requests, library_shelves);
        }
        else if (displaybooks_again == 3)
        {
            main_menu(head, all_requests, library_shelves);
        }
        else if (displaybooks_again == 4)
        {
            exit_program();
        }
    }
}

bool add_book_to_shelf(shelf& target_shelf, const string& book_code, const string& book_title, const string& author, bool availability) 
{
    book new_book;
    new_book.book_code = book_code;
    new_book.book_title = book_title;
    new_book.author = author;
    new_book.availability = availability;

    for (int row = 0; row < target_shelf.number_of_rows; row++) 
    {
        if (target_shelf.rows[row].size() < target_shelf.size_of_row) 
        {
            // There is space in the row, push_back the book to the row
            target_shelf.rows[row].push_back(new_book);
            return true;
        }
    }

    return false;
}

void add_books_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves)
{
    int shelf, add_books_menu_choice;
    string book_code, book_title, author;
    bool add_book_successful;

    system("cls");
    header();
    cout << "                                          Add Books Menu" << endl;
    line_printer();

    cin.ignore();
    cout << endl << endl << "Please enter the details of the book you want to add." ;
    cout << endl << "Book Code: ";
    getline(cin, book_code);
    cout << endl << "Book Title: ";
    getline(cin, book_title);
    cout << endl << "Author/s: ";
    getline(cin, author);
    
    cout << endl << endl << "Please enter the number of the shelf in which the book is located." << endl;
    cout << "[1] Programming Shelf" << endl << "[2] Science Shelf" << endl << "[3] Math Shelf" << endl << "[4] Engineering Shelf" << endl; 

    while (true)
    {
        shelf = input_checker();

        if (shelf > 4 || shelf < 1)
        {
            // for when the user enters an integer that is not on the options
            cout << "Please input an integer from the options given." << endl; 
        }
        else
        {
            break;
        }
    }

    if (shelf == 1)
    {
        add_book_successful = add_book_to_shelf(library_shelves[0], book_code, book_title, author, true);
    }
    else if (shelf == 2)
    {
        add_book_successful = add_book_to_shelf(library_shelves[1], book_code, book_title, author, true);
    }
    else if (shelf == 3)
    {
        add_book_successful = add_book_to_shelf(library_shelves[2], book_code, book_title, author, true);
    }
    else if (shelf == 4)
    {
        add_book_successful = add_book_to_shelf(library_shelves[3], book_code, book_title, author, true);
    }

    if (add_book_successful == true)
    {
        cout << "Successfuly added " << book_title << " to the library."<< endl;
    }
    else
    {
        cout << "The addition of " << book_title << " has failed." << endl;
    }

    cout << "[1] Try Again" << endl << "[2] Return to Add/Remove Books Menu" << endl << "[3] Return to Admin Menu" << endl << "[4] Return to Main Menu" << endl << "[5] Exit Program" << endl;
    
    while (true)
    {
        add_books_menu_choice = input_checker();

        if (add_books_menu_choice > 5 || add_books_menu_choice < 1)
        {
            // for when the user enters an integer that is not on the options
            cout << "Please input an integer from the options given." << endl; 
        }
        else
        {
            break;
        }
    }

    if (add_books_menu_choice == 1)
    {
        add_books_menu(head, all_requests, library_shelves);
    }
    else if (add_books_menu_choice == 2)
    {
        add_remove_books_menu(head, all_requests, library_shelves);
    }
    else if (add_books_menu_choice == 3)
    {
        library_admin_menu(head, all_requests, library_shelves);
    }
    else if (add_books_menu_choice == 4)
    {
        main_menu(head, all_requests, library_shelves);
    }
    else if (add_books_menu_choice == 5)
    {
        exit_program();
    }
}

bool remove_book_from_shelf(shelf& target_shelf, const string& book_code) 
{
    bool book_found = false;
    for (int row = 0; row < target_shelf.number_of_rows; row++) 
    {
        for (int col = 0; col < target_shelf.size_of_row; col++) 
        {
            if (target_shelf.rows[row][col].book_code == book_code) 
            {
                target_shelf.rows[row].erase(target_shelf.rows[row].begin() + col);
                book_found = true;
                break; 
            }
        }
        if (book_found) 
        {
            break; 
        }
    }
    return book_found;
}

void remove_books_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves)
{
    string remove_book_code;
    int remove_book_menu_choice, shelf;
    bool remove_books_successful;

    system("cls");
    header();
    cout << "                                         Remove Books Menu" << endl;
    line_printer();

    cout << endl << endl;
    for (size_t i = 0; i < library_shelves.size(); ++i)
    {
        if (i == 0)
        {
            line_printer();
            cout << endl;
            cout << "                                           Programming Books" << endl;
            line_printer();
            cout << endl;
        }
        else if (i == 1)
        {
            line_printer();
            cout << endl;
            cout << "                                           Science Books" << endl;
            line_printer();
            cout << endl;
        }
        else if (i == 2)
        {
            line_printer();
            cout << endl;
            cout << "                                             Math Books" << endl;
            line_printer();
            cout << endl;
        }
        else if (i == 3)
        {
            line_printer();
            cout << endl;
            cout << "                                         Engineering Books" << endl;
            line_printer();
            cout << endl;
        }
        displaybooks(library_shelves[i]);
    }

    cout << endl << "Please enter the book code of the book you want to remove." << endl;
    cout << "Book Code: ";
    cin >> remove_book_code;

    cout << endl << endl << "Please enter the number of the shelf in which the book is located." << endl;
    cout << "[1] Programming Shelf" << endl << "[2] Science Shelf" << endl << "[3] Math Shelf" << endl << "[4] Engineering Shelf" << endl; 
    
    while (true)
    {
        shelf = input_checker();

        if (shelf > 4 || shelf < 1)
        {
            // for when the user enters an integer that is not on the options
            cout << "Please input an integer from the options given." << endl; 
        }
        else
        {
            break;
        }
    }
    
    if (shelf == 1)
    {
        remove_books_successful = remove_book_from_shelf(library_shelves[0], remove_book_code);
    }
    else if (shelf == 2)
    {
        remove_books_successful = remove_book_from_shelf(library_shelves[1], remove_book_code);
    }
    else if (shelf == 3)
    {
        remove_books_successful = remove_book_from_shelf(library_shelves[2], remove_book_code);
    }
    else if (shelf == 4)
    {
        remove_books_successful = remove_book_from_shelf(library_shelves[3], remove_book_code);
    }

    if (remove_books_successful == true)
    {
        cout << "Successfuly removed " << remove_book_code << " from the library."<< endl;
    }
    else
    {
        cout << "The removal of " << remove_book_code << " has failed." << endl;
    }

    cout << endl << endl << "Please choose an action." << endl;
    cout << "[1] Try Again" << endl << "[2] Return to Add/Remove Books Menu" << endl << "[3] Return to Admin Menu" << endl << "[4] Return to Main Menu" << endl << "[5] Exit Program" << endl; 

    while (true)
    {
        remove_book_menu_choice = input_checker();

        if (remove_book_menu_choice > 5 || remove_book_menu_choice < 1)
        {
            // for when the user enters an integer that is not on the options
            cout << "Please input an integer from the options given." << endl; 
        }
        else
        {
            break;
        }
    }

    if (remove_book_menu_choice == 1)
    {
        remove_books_menu(head, all_requests, library_shelves);
    }
    else if (remove_book_menu_choice == 2)
    {
        add_remove_books_menu(head, all_requests, library_shelves);
    }
    else if (remove_book_menu_choice == 3)
    {
        library_admin_menu(head, all_requests, library_shelves);
    }
    else if (remove_book_menu_choice == 4)
    {
        main_menu(head, all_requests, library_shelves);
    }
    else if (remove_book_menu_choice == 5)
    {
        exit_program();
        return;
    }
}

void add_remove_books_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves)
{
    int add_remove_menu_choice, add_remove_again; 
    system("cls");
    header();
    cout << "                                          Add/Remove Books" << endl;
    line_printer();

    cout << endl << endl << "Please choose an action." << endl; 
    cout << "[1] Display Books" << endl << "[2] Add Books" << endl << "[3] Remove Books" << endl << "[4] Return to Admin Menu" << endl << "[5] Return to Main Menu" << endl << "[6] Exit Program" << endl;

    while (true)
    {
        add_remove_menu_choice = input_checker();

        if (add_remove_menu_choice > 6 || add_remove_menu_choice < 1)
        {
            // for when the user enters an integer that is not on the options
            cout << "Please input an integer from the options given." << endl; 
        }
        else
        {
            break;
        }
    }

    if (add_remove_menu_choice == 1)
    {
        system("cls");
        header();
        cout << "                                          Add/Remove Books" << endl;
        line_printer();
        cout << endl << endl;
        for (size_t i = 0; i < library_shelves.size(); ++i)
        {
            if (i == 0)
            {
                line_printer();
                cout << endl;
                cout << "                                           Programming Books" << endl;
                line_printer();
                cout << endl;
            }
            else if (i == 1)
            {
                line_printer();
                cout << endl;
                cout << "                                           Science Books" << endl;
                line_printer();
                cout << endl;
            }
            else if (i == 2)
            {
                line_printer();
                cout << endl;
                cout << "                                             Math Books" << endl;
                line_printer();
                cout << endl;
            }
            else if (i == 3)
            {
                line_printer();
                cout << endl;
                cout << "                                         Engineering Books" << endl;
                line_printer();
                cout << endl;
            }
            displaybooks(library_shelves[i]);
        }
    }
    else if (add_remove_menu_choice == 2)
    {
        add_books_menu(head, all_requests, library_shelves);
        return;
    }
    else if (add_remove_menu_choice == 3)
    {
        remove_books_menu(head, all_requests, library_shelves);
        return;
    }
        else if (add_remove_menu_choice == 4)
    {
        library_admin_menu(head, all_requests,library_shelves);
    }
        else if (add_remove_menu_choice == 5)
    {
        main_menu(head, all_requests, library_shelves);
    }
        else if (add_remove_menu_choice == 6)
    {
        exit_program();
    }

    if (add_remove_menu_choice !=6)
    {
        line_printer();
        cout << endl << endl << "Please choose an action." << endl; 
        cout << "[1] Return to Display Books Menu" << endl << "[2] Return to Admin Menu" << endl << "[3] Return to Main Menu" << endl << "[4] Exit Program" << endl;

        while (true)
        {
            add_remove_again = input_checker();

            if (add_remove_again > 4 || add_remove_again < 1)
            {
                // for when the user enters an integer that is not on the options
                cout << "Please input an integer from the options given." << endl; 
            }
            else
            {
                break;
            }
        }

        if (add_remove_again == 1)
        {
            add_remove_books_menu(head, all_requests, library_shelves);
        }
        else if (add_remove_again == 2)
        {
            library_admin_menu(head, all_requests, library_shelves);
        }
        else if (add_remove_again == 3)
        {
            main_menu(head, all_requests, library_shelves);
        }
        else if (add_remove_again == 4)
        {
            exit_program();
        }
    }
}
// -----------------
void all_requests_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves)
{
    int all_requests_choice;
    int request_number = 1;

    system("cls");
    header();
    cout << "                                            All Requests" << endl;
    line_printer();
    cout << endl << endl;
    for (const auto& request : all_requests)
    {
        line_printer();
        cout << endl << "                                             Request " << request_number << endl;
        line_printer();

        cout << endl << endl << "SR Code: " << request.srcode << endl;
        cout << "Student Name: " << request.name << endl;
        cout << "Book Code: " << request.book_code << endl;

        if (request.type == request_type::borrowing)
        {
            cout << "Request Type: Borrowing" << endl;
        }
        else 
        {
            cout << "Request Type: Returning" << endl;
        }
        // cout << "Request Type: " << (request.type == request_type::borrowing ? "Borrowing" : "Returning") << endl;
        cout << endl;

        request_number += 1;
    }
    line_printer();

    cout << endl << endl << "Please choose an action." << endl; 
    cout << "[1] Return to Requests Menu" << endl <<"[2] Return to Admin Menu" << endl << "[3] Return to Main Menu" << endl << "[4] Exit Program" << endl;

    while (true)
    {
        all_requests_choice = input_checker();

        if (all_requests_choice > 4 || all_requests_choice < 1)
        {
            // for when the user enters an integer that is not on the options
            cout << "Please input an integer from the options given." << endl; 
        }
        else
        {
            break;
        }
    }

    if (all_requests_choice == 1)
    {
        admin_process_requests_menu(head, all_requests, library_shelves);
    }
    else if (all_requests_choice == 2)
    {
        library_admin_menu(head, all_requests, library_shelves);
    }
    else if (all_requests_choice == 3)
    {
        main_menu(head, all_requests, library_shelves);
    }
    else if (all_requests_choice == 4)
    {
        exit_program;
    }
}

void change_book_availability(vector<shelf> &library_shelves, const string &book_code)
{
    bool book_found = false;

    for (int shelf_index = 0; shelf_index < library_shelves.size(); shelf_index++)
    {
        shelf& current_shelf = library_shelves[shelf_index];

        for (int row = 0; row < current_shelf.number_of_rows; row++)
        {
            for (int col = 0; col < current_shelf.size_of_row; col++)
            {
                if (current_shelf.rows[row][col].book_code == book_code)
                {
                    // Change the availability status of the book if it is found
                    current_shelf.rows[row][col].availability = !current_shelf.rows[row][col].availability;
                    book_found = true;
                    break; // No need to continue searching if book is found
                }
            }

            if (book_found) {
                break; // No need to continue searching in other rows of the same shelf if book is found
            }
        }

        if (book_found) {
            break; // No need to continue searching in other shelves if book is found
        }
    }

    if (!book_found)
    {
        cout << "Book with book code '" << book_code << "' not found in any shelf." << endl;
    }
}

void process_requests_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves)
{
    // checks if there are still requests that need to be made before going on with the rest of the function
    if (all_requests.empty())
    {
        int no_requests_choice;
        cout << "No pending requests to process." << endl;
        cout << endl << endl << "Please choose an action." << endl; 
        cout << "[1] Return to Requests Menu" << endl << "[2] Return to Admin Menu" << endl << "[3] Return to Main Menu" << endl << "[4] Exit Program" << endl;

        while (true)
        {
            no_requests_choice = input_checker();
            if (no_requests_choice > 4 || no_requests_choice < 1)
            {
                // for when the user enters an integer that is not on the options
                cout << "Please input an integer from the options given." << endl; 
            }
            else
            {
                break;
            }
        }       

        if (no_requests_choice == 1)
        {
            admin_process_requests_menu(head, all_requests, library_shelves);
        }
        if (no_requests_choice == 2)
        {
            library_admin_menu(head, all_requests, library_shelves);
        }
        else if (no_requests_choice == 3)
        {
            main_menu(head, all_requests, library_shelves);
        }
        else if (no_requests_choice == 4)
        {
            exit_program();
        }
    }

    int process_request_menu_choice, process_next_request;
    system("cls");
    header();
    cout << "                                     Process Requests Menu" << endl;
    line_printer();

    const request_sheet& front_request = all_requests.front();
    cout << endl << endl << "SR Code: " << front_request.srcode;
    cout << endl << "Student Name: " << front_request.name;
    cout << endl << "Book Code: " << front_request.book_code;
    cout << endl << "Request Type: " << (front_request.type == borrowing ? "Borrowing" : "Returning");

    cout << endl << endl << "Please choose an action." << endl; 
    cout << "[1] Approve" << endl << "[2] Deny" << endl;

    while (true)
    {
        process_request_menu_choice = input_checker();
        if (process_request_menu_choice > 2 || process_request_menu_choice < 1)
        {
            // for when the user enters an integer that is not on the options
            cout << "Please input an integer from the options given." << endl; 
        }
        else
        {
            break;
        }
    }        

    if (process_request_menu_choice == 1)
    {
        change_book_availability(library_shelves, front_request.book_code);
        if (front_request.type == borrowing) 
        {
            cout << endl << "Request by " << front_request.name << " to borrow " << front_request.book_code << " has been approved." << endl;
            if (!all_requests.empty())
            {
                all_requests.erase(all_requests.begin());
            }            
        }       
        else if (front_request.type == returning) 
        {
            cout << endl << "Request by " << front_request.name << " to return " << front_request.book_code << " has been approved." << endl;
            if (!all_requests.empty())
            {
                all_requests.erase(all_requests.begin());
            }            
        }       
    }
    else if (process_request_menu_choice == 2)
    {
        // just delete the request without changing anything
        if (front_request.type == borrowing) 
        {
            cout << endl << "Request by " << front_request.name << " to borrow " << front_request.book_code << " has been denied";
            if (!all_requests.empty())
            {
                all_requests.erase(all_requests.begin());
            }            
        }       
        else if (front_request.type == returning) 
        {
            cout << endl << "Request by " << front_request.name << " to return " << front_request.book_code << " has been denied";
            if (!all_requests.empty())
            {
                all_requests.erase(all_requests.begin());
            }            
        }       
    }

    line_printer();

    cout << endl << "Please choose an action." << endl; 
    cout << "[1] Process Next Request" << endl << "[2] Return to Requests Menu" << endl << "[3] Return to Admin Menu" << endl << "[4] Return to Main Menu" << endl << "[5] Exit program" << endl;

    while (true)
    {
        process_next_request = input_checker();
        if (process_next_request > 4 || process_next_request < 1)
        {
            // for when the user enters an integer that is not on the options
            cout << "Please input an integer from the options given." << endl; 
        }
        else
        {
            break;
        }
    }      

    if (process_next_request == 1)
    {
        process_requests_menu(head, all_requests, library_shelves);
    }
    else if (process_next_request == 2)
    {
        admin_process_requests_menu(head, all_requests, library_shelves);
    }
    else if (process_next_request == 3)
    {
        library_admin_menu(head, all_requests, library_shelves);
    }
    else if (process_next_request == 4)
    {
        main_menu(head, all_requests, library_shelves);
    }
    else if (process_next_request == 5)
    {
        exit_program();
    }
}

void admin_process_requests_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves)
{
    int request_menu_choice;

    system("cls");
    header();
    cout << "                                           Requests Menu" << endl;
    line_printer();

    cout << endl << endl << "Please choose an action." << endl; 
    cout << "[1] Show all requests" << endl << "[2] Process first request" << endl << "[3] Return to Admin Menu" << endl << "[4] Return to Main Menu" << endl << "[5] Exit Program" << endl;

    while (true)
    {
        request_menu_choice = input_checker();

        if (request_menu_choice > 5 || request_menu_choice < 1)
        {
            // for when the user enters an integer that is not on the options
            cout << "Please input an integer from the options given." << endl; 
        }
        else
        {
            break;
        }
    }

    if (request_menu_choice == 1)
    {
        all_requests_menu(head, all_requests, library_shelves);
    }
    else if (request_menu_choice == 2)
    {
        process_requests_menu(head, all_requests, library_shelves);
    }
    else if (request_menu_choice == 3)
    {
        library_admin_menu(head, all_requests, library_shelves);
    }
    else if (request_menu_choice == 4)
    {
        main_menu(head, all_requests, library_shelves);
    }
    else if (request_menu_choice == 5)
    {
        exit_program();
    }
}

void display_accounts_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves)
{
    int display_acc_choice;
    system("cls");
    header();
    cout << "                                           Display All Accounts Menu" << endl;
    line_printer();

    display_all_student_accounts(head);

    cout << endl << endl << "Please choose an action." << endl; 
    cout << "[1] Return to Admin Menu" << endl << "[2] Return to Main Menu" << endl << "[3] Exit Program" << endl;
    while (true)
    {
        display_acc_choice = input_checker();

        if (display_acc_choice > 3 || display_acc_choice < 1)
        {
            cout << "Please input an integer from the options given." << endl; 
        }
        else
        {
            break;
        }
    }

    if (display_acc_choice == 1)
    {
        library_admin_menu(head, all_requests, library_shelves);
    }
    else if (display_acc_choice == 2)
    {
        main_menu(head, all_requests, library_shelves);
    }
    else if (display_acc_choice == 3)
    {
        exit_program();
    }
}

void search_student_acc_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves)
{
    string search_srcode_placeholder;
    int search_srcode, search_menu_choice;
    const student_account* student_found = nullptr;

    system("cls");
    header();
    cout << "                                            Search Menu" << endl;
    line_printer();

    cout << endl << endl << "Input the sr code of the account you are searching for.";

    while (true)
    {   
        try 
        {
            cout << endl << "SR Code: ";
            cin >> search_srcode_placeholder;
            search_srcode = stoi(search_srcode_placeholder);
            break;
        }
        catch (const exception&)
        {
            cout << "Please input an integer." << endl;
        }
    }

    student_found = find_student_acc(head, search_srcode);

    if (student_found != nullptr)
    {
        system("cls");
        header();
        cout << "                                            Search Menu" << endl;
        line_printer();

        cout << endl << endl << "Student found!";

        cout << endl << "SR Code: " << student_found->srcode << endl;
        cout << "Name: " << student_found->name << endl;
        cout << "Username: " << student_found->username << endl;
        cout << "Password: " << student_found->password << endl;
    }
    else 
    {
        cout << "Student not found in the system! "<< endl;
    }

    cout << endl << "Please choose an action. " << endl;
    cout << "[1] Search Again" << endl << "[2] Return to Admin Menu" << endl << "[3] Return to Main Menu" << endl << "[4] Exit Program" << endl;

    while (true)
    {   
        search_menu_choice = input_checker();
        if (search_menu_choice > 4 || search_menu_choice < 1)
        {
            cout << "Please input an integer from the options given." << endl;
        }
        else
        {
            break;
        }
    }    

    if (search_menu_choice == 1)
    {
        search_student_acc_menu(head, all_requests, library_shelves);
    }
    else if (search_menu_choice == 2)
    {
        library_admin_menu(head, all_requests, library_shelves);
    }
    else if (search_menu_choice == 3)
    {
        main_menu(head, all_requests, library_shelves);
    }
    else if (search_menu_choice == 4)
    {
        exit_program();
    }
}

void library_admin_login_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves)
{
    string username, password, login_unsuccessful_choice_placeholder;
    int acc_validity_choice, login_unsuccessful_choice;
    bool login_succesful = false;

    system("cls");
    header();
    cout << "                                          Admin Login Menu" << endl;
    line_printer();
    
    cin.ignore();
    cout << endl << endl << "Username: ";
    getline(cin, username);
    cout << endl << "Password: ";
    getline(cin, password);

    cout << endl << "What algorithm would you like to confirm your account's validity? Or would you like to return to the Main Menu? " << endl;
    cout << "[1] Binary Search" << endl << "[2] Linear Search" << endl << "[3] Return to Main Menu" << endl;
    
    while (true)
    {
        acc_validity_choice = input_checker();

        if (acc_validity_choice > 3 || acc_validity_choice < 1)
        {
            // for when the user enters an integer that is not on the options
            cout << "Please input an integer from the options given." << endl; 
        }
        else
        {
            break;
        }
    }

    if (acc_validity_choice == 1)
    {
        login_succesful = binarysearch_admin_login(username, password);
    }
    else if(acc_validity_choice == 2)
    {
        login_succesful = linearsearch_admin_login(username, password);
    }
    else if (acc_validity_choice == 3)
    {
        main();
    }

    if (login_succesful == false)
    {
        login_failed();
        while (true)
        {   
            login_unsuccessful_choice = input_checker();
            if (login_unsuccessful_choice > 2)
            {
                cout << "Please input an integer from the options given." << endl;
            }
            else
            {
                break;
            }
        }    

        if (login_unsuccessful_choice == 1)
        {
            library_admin_login_menu(head, all_requests, library_shelves);
        }
        else if (login_unsuccessful_choice == 2)
        {
            exit_program();
        }
    }
    else 
    {
        library_admin_menu(head, all_requests, library_shelves);
    }
}
// ----------------------------------- processes
void display_all_student_accounts(const student_account* head)
{
    cout << endl;
    if (head == nullptr) 
    {
        cout << "No student information on the system yet." << endl;
        return;
    }

    const student_account* current = head;
    while (current != nullptr) 
    {        
        cout << endl << "SR-Code: " << current->srcode << endl;
        cout << "Student Name: " << current->name << endl;
        cout << "Username: " << current->username << endl;
        cout << "Password: " << current->password << endl;
        current = current->next;
    }
}

const student_account* find_student_acc(const student_account* head, int srcode)
{
    const student_account* current = head;
    while (current != nullptr) {
        if (current->srcode == srcode) 
        {
            // return the student if the srcode matches
            return current;  
        }
        current = current->next;
    }
    // return nullptr if the student is not found
    return nullptr;  
}

// ----------------------------------- for student menus
student_account* register_student_account(student_account* head, int srcode, string name, string username, string password)
{
    student_account* new_student = new student_account();
    
    new_student->srcode = srcode;
    new_student->name = name;
    new_student->username = username;
    new_student->password = password;
    new_student->next = nullptr;
    
    if (head == nullptr) 
    {
        // if the linked list is empty, make the new student the head
        head = new_student;  
    } 
    else 
    {
        student_account* current = head;
        while (current->next != nullptr) { 
            // find the last node in the linked list
            current = current->next; 
        }
        // add the new student to the end of the linked list
        current->next = new_student;  
    }

    return head;
}

void student_login_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves)
{
    int student_menu_choice;

    system("cls");
    header();
    cout << "                                           Student Login Menu" << endl;
    line_printer();

    cout << endl << endl << "Please choose an action." << endl; 
    cout << "[1] Login" << endl << "[2] Register Account" << endl << "[3] Return to Main Menu" << endl <<"[4] Exit the program" << endl;

    while (true)
    {
        student_menu_choice = input_checker();

        if (student_menu_choice > 4 || student_menu_choice < 1)
        {
            // for when the user enters an integer that is not on the options
            cout << "Please input an integer from the options given." << endl; 
        }
        else
        {
            break;
        }
    }

    if (student_menu_choice == 1)
    {
        student_login_page(head, all_requests, library_shelves);
    }
    else if (student_menu_choice == 2)
    {
        student_registration_menu(head, all_requests, library_shelves);
    }
    else if (student_menu_choice == 3)
    {
        main_menu(head, all_requests, library_shelves);
    }
    else if (student_menu_choice == 4)
    {
        exit_program();
    }            
}

void student_login_page(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves)
{
    string username, password, login_unsuccessful_choice_placeholder;
    int acc_validity_choice, login_unsuccessful_choice;
    student_account* current_user;

    system("cls");
    header();
    cout << "                                           Student Login Menu" << endl;
    line_printer();

    cin.ignore();
    cout << endl << endl << "Username: ";
    getline(cin, username);
    cout << endl << "Password: ";
    getline(cin, password);

    current_user = student_login_verification(head, username, password);

    if (current_user == nullptr)
    {
        login_failed();
        while (true)
        {   
            login_unsuccessful_choice = input_checker();
            if (login_unsuccessful_choice > 3 || login_unsuccessful_choice < 1)
            {
                cout << "Please input an integer from the options given." << endl;
            }
            else 
            {
                break;
            }
        }            
        if (login_unsuccessful_choice == 1)
        {
            student_login_menu(head, all_requests, library_shelves);
        }
        else if (login_unsuccessful_choice == 2)
        {
            exit_program();
        }
    }
    else
    {
        student_menu(head, current_user, all_requests, library_shelves);
    }
}

void student_menu(student_account*& head,  student_account* current_user, vector<request_sheet> &all_requests,vector<shelf> &library_shelves)
{
    int student_menu_choice;

    system("cls");
    header();
    cout << "                                            Student Menu" << endl;
    line_printer();

    cout << endl << "welcome " << current_user->name;
    cout << endl << endl << "Plase choose an option." << endl;
    cout << "[1] Borrow Books" << endl << "[2] Return Books" << endl << "[3] View all available books" << endl << "[4] Return to Main Menu" << endl << "[5] Exit Program" << endl;

    while (true)
    {
        student_menu_choice = input_checker();
        if (student_menu_choice > 5 || student_menu_choice < 1) 
        {
            cout << "Please input an integer from the options given." << endl;
        }
        else
        {
            break;
        }
    }    

    if (student_menu_choice == 1)
    {
        borrow_book_menu(head, current_user, all_requests, library_shelves);
    }
    else if (student_menu_choice == 2)
    {
        return_book_menu(head, current_user, all_requests, library_shelves);
    }
    else if (student_menu_choice == 3)
    {
        student_available_books_menu(head, current_user, all_requests, library_shelves);
    }
    else if (student_menu_choice == 4)
    {
        main_menu(head, all_requests, library_shelves);
    }
    else if (student_menu_choice == 5)
    {
        exit_program();
    }
}

void borrow_book_menu(student_account*& head, student_account* current_user, vector<request_sheet> &all_requests,vector<shelf> &library_shelves)
{
    string book_code_borrow;
    bool book_code_in_system = false;
    int borrow_book_choice;

    system("cls");
    header();
    cout << "                                          Borrow Book Menu" << endl;
    line_printer();

    cout << endl << endl;
    for (size_t i = 0; i < library_shelves.size(); ++i)
    {
        if (i == 0)
        {
            line_printer();
            cout << endl;
            cout << "                                           Programming Books" << endl;
            line_printer();
            cout << endl;
        }
        else if (i == 1)
        {
            line_printer();
            cout << endl;
            cout << "                                           Science Books" << endl;
            line_printer();
            cout << endl;
        }
        else if (i == 2)
        {
            line_printer();
            cout << endl;
            cout << "                                             Math Books" << endl;
            line_printer();
            cout << endl;
        }
        else if (i == 3)
        {
            line_printer();
            cout << endl;
            cout << "                                         Engineering Books" << endl;
            line_printer();
            cout << endl;
        }
        display_available_books(library_shelves[i]);
    }

    cin.ignore();
    cout << endl;
    cout << "Please enter the book code of the book you want to borrow." << endl<< endl << "Book Code: ";
    getline(cin, book_code_borrow);

    for (size_t i = 0; i < library_shelves.size(); ++i)
    {
        if (borrowing_check_book_code(book_code_borrow, library_shelves[i]))
        {
            book_code_in_system = true;
            break; // Exit the loop as soon as a matching book is found
        }
    }

    if (book_code_in_system)
    {
        request_sheet new_request(current_user, book_code_borrow, request_type::borrowing);
        all_requests.push_back(new_request);

        system("cls");
        header();
        cout << "                                          Borrow Book Menu" << endl;
        line_printer();

        cout << endl << endl << "Your request to borrow a book has been successfully made. Please wait for an admin to continue." << endl;
    }
    else 
    {
        system("cls");
        header();
        cout << "                                          Borrow Book Menu" << endl;
        line_printer();

        cout << endl << endl << "The book code you entered is invalid." << endl;
    }

    cout << "Please choose an action." << endl;    
    cout << "[1] Try Again" << endl << "[2] Return to Student Menu" << endl << "[3] Return to Main Menu" << endl << "[4] Exit Program" << endl;    
    while (true)
    {
        borrow_book_choice = input_checker();
        if (borrow_book_choice > 4 || borrow_book_choice < 1) 
        {
            cout << "Please input an integer from the options given." << endl;
        }
        else
        {
            break;
        }
    }
    if (borrow_book_choice == 1)
    {
        borrow_book_menu(head, current_user, all_requests, library_shelves);
    }    
    else if (borrow_book_choice == 2)
    {
        student_menu(head, current_user, all_requests, library_shelves);
    }
    else if (borrow_book_choice == 3)
    {
        main_menu(head, all_requests, library_shelves);
    }
    else if (borrow_book_choice == 4)
    {
        exit_program();
    }
}

void return_book_menu(student_account*& head, student_account* current_user, vector<request_sheet> &all_requests,vector<shelf> &library_shelves)
{
    string book_code_return;
    bool book_code_in_system = false;
    int return_book_choice;

    system("cls");
    header();
    cout << "                                          Return Book Menu" << endl;
    line_printer();

    cout << endl << endl;
    for (size_t i = 0; i < library_shelves.size(); ++i)
    {
        if (i == 0)
        {
            line_printer();
            cout << endl;
            cout << "                                           Programming Books" << endl;
            line_printer();
            cout << endl;
        }
        else if (i == 1)
        {
            line_printer();
            cout << endl;
            cout << "                                           Science Books" << endl;
            line_printer();
            cout << endl;
        }
        else if (i == 2)
        {
            line_printer();
            cout << endl;
            cout << "                                             Math Books" << endl;
            line_printer();
            cout << endl;
        }
        else if (i == 3)
        {
            line_printer();
            cout << endl;
            cout << "                                         Engineering Books" << endl;
            line_printer();
            cout << endl;
        }
        display_unavailable_books(library_shelves[i]);
    }

    cin.ignore();
    cout << endl;
    cout << "Please enter the book code of the book you want to return." << endl<< endl << "Book Code: ";
    getline(cin, book_code_return);

    for (size_t i = 0; i < library_shelves.size(); ++i)
    {
        if (returning_check_book_code(book_code_return, library_shelves[i]))
        {
            book_code_in_system = true;
            break; // exit the loop as soon as a matching book is found
        }
    }

    if (book_code_in_system)
    {
        request_sheet new_request(current_user, book_code_return, request_type::returning);
        all_requests.push_back(new_request);

        system("cls");
        header();
        cout << "                                          Return Book Menu" << endl;
        line_printer();

        cout << endl << endl << "Your request to return a book has been successfully made. Please wait for an admin to continue." << endl;
    }
    else 
    {
        system("cls");
        header();
        cout << "                                          Return Book Menu" << endl;
        line_printer();

        cout << endl << endl << "The book code you entered is invalid." << endl;
    }

    cout << "Please choose an action." << endl;    
    cout << "[1] Try Again" << endl << "[2] Return to Student Menu" << endl << "[3] Return to Main Menu" << endl << "[4] Exit Program" << endl;    
    while (true)
    {
        return_book_choice = input_checker();
        if (return_book_choice > 4 || return_book_choice < 1) 
        {
            cout << "Please input an integer from the options given." << endl;
        }
        else
        {
            break;
        }
    }
    if (return_book_choice == 1)
    {
        return_book_menu(head, current_user, all_requests, library_shelves);
    }    
    else if (return_book_choice == 2)
    {
        student_menu(head, current_user, all_requests, library_shelves);
    }
    else if (return_book_choice == 3)
    {
        main_menu(head, all_requests, library_shelves);
    }
    else if (return_book_choice == 4)
    {
        exit_program();
    }
}

bool borrowing_check_book_code(string book_code, const shelf &shelf_to_display)
{
    for (const auto &row : shelf_to_display.rows)
    {
        for (const auto &b : row)
        {
            if (b.book_code == book_code && b.availability == true) // only returns true if books' availability = true and if it's code matches
            {
                return true;
            }
        }
    }
    return false;
}

bool returning_check_book_code(string book_code, const shelf &shelf_to_display)
{
    for (const auto &row : shelf_to_display.rows)
    {
        for (const auto &b : row)
        {
            if (b.book_code == book_code && b.availability == false) // only returns true if books' availability = false and if it's code matches
            {
                return true;
            }
        }
    }
    return false;
}

void display_available_books(const shelf &shelf_to_display)
{
    int available_books = 0;
    for (const auto &row : shelf_to_display.rows)
    {
        for (const auto &b : row)
        {
            if (b.availability) // Only display books with availability = true
            {
                cout << "Book Code: " << b.book_code << endl;
                cout << "Book Title: " << b.book_title << endl;
                cout << "Author: " << b.author << endl;
                cout << endl;
                available_books++;
            }
        }
    }

    if (available_books == 0)
    {
        cout << "No available books on this shelf that can be borrowed." << endl;
    }
}

void display_unavailable_books(const shelf &shelf_to_display)
{
    int unavailable_books = 0;
    for (const auto &row : shelf_to_display.rows)
    {
        for (const auto &b : row)
        {
            if (!b.availability) // Only display books with availability = false
            {
                cout << "Book Code: " << b.book_code << endl;
                cout << "Book Title: " << b.book_title << endl;
                cout << "Author: " << b.author << endl;
                cout << endl;
                unavailable_books++;
            }
        }
    }

    if (unavailable_books == 0)
    {
        cout << "No unavailable books on this shelf that can be returned." << endl;
    }
}

void student_available_books_menu(student_account*& head,  student_account* current_user, vector<request_sheet> &all_requests, vector<shelf> &library_shelves)
{
    int available_books_menu_choice, available_books_again;

    system("cls");
    header();
    cout << "                                        Available Books Menu" << endl;
    line_printer();

    cout << endl << endl << "Plase choose an option." << endl;
    cout << "[1] Display All Books" << endl << "[2] Display Programming Books" << endl << "[3] Display Science Books" << endl << "[4] Display Math Books" << endl << "[5] Display Engineering Books" << endl << "[6] Return to Student Menu" << endl << "[7] Return to Main Menu" << endl << "[8] Exit Program" << endl;

    while (true)
    {
        available_books_menu_choice = input_checker();
        if (available_books_menu_choice > 8 || available_books_menu_choice < 1) 
        {
            cout << "Please input an integer from the options given." << endl;
        }
        else
        {
            break;
        }
    }    

    if (available_books_menu_choice == 1)
    {
        system("cls");
        header();
        cout << "                                         Display Books Menu" << endl;
        line_printer();
        cout << endl << endl;
        for (size_t i = 0; i < library_shelves.size(); ++i)
        {
            if (i == 0)
            {
                line_printer();
                cout << endl;
                cout << "                                           Programming Books" << endl;
                line_printer();
                cout << endl;
            }
            else if (i == 1)
            {
                line_printer();
                cout << endl;
                cout << "                                           Science Books" << endl;
                line_printer();
                cout << endl;
            }
            else if (i == 2)
            {
                line_printer();
                cout << endl;
                cout << "                                             Math Books" << endl;
                line_printer();
                cout << endl;
            }
            else if (i == 3)
            {
                line_printer();
                cout << endl;
                cout << "                                         Engineering Books" << endl;
                line_printer();
                cout << endl;
            }
            display_available_books(library_shelves[i]);
        }
    }
    else if (available_books_menu_choice == 2)
    {
        system("cls");
        header();
        cout << "                                           Programming Books" << endl;
        line_printer();
        cout << endl;
        display_available_books(library_shelves[0]);
    }
    else if (available_books_menu_choice == 3)
    {
        system("cls");
        header();
        cout << "                                           Science Books" << endl;
        line_printer();
        cout << endl;
        display_available_books(library_shelves[1]);
    }
    else if (available_books_menu_choice == 4)
    {
        system("cls");
        header();
        cout << "                                             Math Books" << endl;
        line_printer();
        cout << endl;
        display_available_books(library_shelves[2]);
    }
    else if (available_books_menu_choice == 5)
    {
        system("cls");
        header();
        cout << "                                         Engineering Books" << endl;
        line_printer();
        cout << endl;
        display_available_books(library_shelves[3]);
    }
    else if (available_books_menu_choice == 6)
    {
        student_menu(head, current_user, all_requests, library_shelves);
    }
    else if (available_books_menu_choice == 7)
    {
        main_menu(head, all_requests, library_shelves);
    }
    else if (available_books_menu_choice == 8)
    {
        exit_program();
    }

    // if they don't choose to return to other menus or exit the program and they chose to display books
    if (available_books_again !=8)
    {
        line_printer();
        cout << endl << endl << "Please choose an action." << endl; 
        cout << "[1] Return to Available Books Menu" << endl << "[2] Return to Student Menu" << endl << "[3] Return to Main Menu" << endl << "[4] Exit Program" << endl;

        while (true)
        {
            available_books_again = input_checker();

            if (available_books_again > 4 || available_books_again < 1)
            {
                // for when the user enters an integer that is not on the options
                cout << "Please input an integer from the options given." << endl; 
            }
            else
            {
                break;
            }
        }

        if (available_books_again == 1)
        {
            admin_displaybooks_menu(head, all_requests, library_shelves);
        }
        else if (available_books_again == 2)
        {
            student_menu(head, current_user,all_requests, library_shelves);
        }
        else if (available_books_again == 3)
        {
            main_menu(head, all_requests, library_shelves);
        }
        else if (available_books_again == 4)
        {
            exit_program();
        }
    }
}

void student_registration_menu(student_account*& head, vector<request_sheet> &all_requests, vector<shelf> &library_shelves)
{
    string name, username, password, srcode_placeholder;
    int srcode, password_length, registration_successful_choice; 

    system("cls");
    header();
    cout << "                                   Student Registration Menu" << endl;
    line_printer();

    cout << endl;
    cin.ignore();
    while (true)
    {
        // used this instead of the input_checker to let the user know that they have to type an integer
        try
        {    
            cout << endl << "Enter your SR Code: ";
            getline(cin, srcode_placeholder);
            srcode = stoi(srcode_placeholder);
            break;
        }
        catch (const exception&)
        {
            cout << "Please input an integer!" << endl;
        }
    }

    cout << endl << "Enter your name: ";
    getline(cin, name);

    cout << endl << "Enter your desired username: ";
    getline(cin, username);

    while (true)
    {
        cout << endl <<"Enter desired password: ";
        cin >> password;
        password_length = password.length();
        if (password_length >= 8)
        {
            break;
        }
        else
        {
            cout << "Password must be 8 characters or longer!." << endl;
        }
    }

    head = register_student_account(head, srcode, name, username, password);

    system("cls");
    header();
    cout << "                                   Student Registration Menu" << endl;
    line_printer();

    cout << endl  << endl << "Registration successful!" << endl;
    cout << endl << "Please choose an option.";
    cout << endl << "[1] Return to Student Login Menu" << endl << "[2] Return to Main Menu";

    while (true)
    {
        registration_successful_choice = input_checker();
        if (registration_successful_choice > 2 || registration_successful_choice < 1)
        {
            cout << "Please input an integer from the options given." << endl;
        }
        else 
        {
            break;
        }
    }

    if (registration_successful_choice == 1)
    {
        student_login_menu(head, all_requests, library_shelves);
    }
    else 
    {
        main_menu(head, all_requests, library_shelves);
    }
}
