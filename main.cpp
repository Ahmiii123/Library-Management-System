#include"header.h"

class Library
{
private:
    struct Node
    {
        int id;
        string name, author, publisher;
        Node *next_add;
    };

public:
    Node *head = NULL;
    MYSQL *conn;

    Library()
    {
        conn = mysql_init(0);
        conn = mysql_real_connect(conn, "localhost", "root", "", "mydb", 0, NULL, 0);
        if (conn)
        {
            cout << "Connected to MySQL database!" << endl;
        }
        else
        {
            cout << "Connection failed!" << endl;
            exit(1);
        }
    }

    void menu();
    void insert();
    void search();
    void update();
    void del();
    void Bub_sort();
    void show(bool showDatabase);
    void show();
};

void Library::menu()
{
    while (true)
    {
        system("cls");
        int choice;
        cout << "\n\n\t\t\t===========================================";
        cout << "\n\n\t\t\t=====LIBRARY MANAGEMENT SYSTEM=============";
        cout << "\n\n\t\t\t===========================================";
        cout << "\n\n 1. INSERT NEW RECORD";
        cout << "\n\n 2. SEARCH RECORD";
        cout << "\n\n 3. UPDATE RECORD";
        cout << "\n\n 4. DELETE RECORD";
        cout << "\n\n 5. SHOW ALL RECORD";
        cout << "\n\n 6. EXIT";
        cout << "\n\nEnter your Choice : ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            insert();
            break;
        case 2:
            search();
            break;
        case 3:
            update();
            break;
        case 4:
            del();
            break;
        case 5:
            Bub_sort();
            show(true);
            break;
        case 6:
            exit(0);
            break;
        default:
            cout << "\nInvalid Choice.....Please Try Again...";
        }

        getch();
    }
}

void Library::insert()
{
    system("cls");
    cout << "\n\n\t\t\t===========================================";
    cout << "\n\n\t\t\t=====LIBRARY MANAGEMENT SYSTEM=============";
    cout << "\n\n\t\t\t===========================================";

    Node *new_node = new Node;

    srand(static_cast<unsigned int>(time(0)));
    bool uniqueID = false;
    do
    {
        new_node->id = rand();
        Node *checkID = head;
        uniqueID = true;
        while (checkID != NULL)
        {
            if (checkID->id == new_node->id)
            {
                uniqueID = false;
                break;
            }
            checkID = checkID->next_add;
        }
    } while (!uniqueID);

    cin.ignore();
    cout << "\n\n Book Name : ";
    getline(cin, new_node->name);
    cout << "\n\n Author Name : ";
    getline(cin, new_node->author);
    cout << "\n\n Publisher Name : ";
    getline(cin, new_node->publisher);
    new_node->next_add = head;
    head = new_node;

    stringstream queryStream;
    queryStream << "INSERT INTO test (Book_Id, Book_Name, Author, Publisher) VALUES ("
                << new_node->id << ", '" << new_node->name << "', '"
                << new_node->author << "', '" << new_node->publisher << "')";
    string query = queryStream.str();
    const char *sqlQuery = query.c_str();

    if (!(mysql_query(conn, sqlQuery)))
    {
        cout << "Successfully Inserted" << endl;
    }
    else
    {
        cout << "Insertion failed: " << mysql_error(conn) << endl;
    }
}

void Library::search()
{
    system("cls");
    int t_id;
    cout << "\n\n\t\t\t===========================================";
    cout << "\n\n\t\t\t=====LIBRARY MANAGEMENT SYSTEM=============";
    cout << "\n\n\t\t\t===========================================";

    cout << "\n\n Enter Book Id to Search: ";
    cin >> t_id;

    stringstream queryStream;
    queryStream << "SELECT * FROM test WHERE Book_Id = " << t_id;
    string query = queryStream.str();
    const char *sqlQuery = query.c_str();

    if (mysql_query(conn, sqlQuery) == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);

        if (res)
        {
            int numFields = mysql_num_fields(res);
            int numRows = mysql_num_rows(res);

            if (numRows > 0)
            {
                MYSQL_ROW row;
                while ((row = mysql_fetch_row(res)))
                {
                    for (int i = 0; i < numFields; ++i)
                    {
                        cout << "\n\n " << mysql_fetch_field_direct(res, i)->name << " (from DB): " << row[i];
                    }

                    cout << "\n\n ===================================";
                }
            }
            else
            {
                cout << "Book ID not found in the database." << endl;
            }

            mysql_free_result(res);
        }
        else
        {
            cout << "Error fetching results from the database." << endl;
        }
    }
    else
    {
        cout << "Search failed: " << mysql_error(conn) << endl;
    }
}


void Library::update()
{
    int t_id;
    system("cls");
    cout << "\n\n\t\t\t===========================================";
    cout << "\n\n\t\t\t=====LIBRARY MANAGEMENT SYSTEM=============";
    cout << "\n\n\t\t\t===========================================";

    cout << "\n\n Enter Book Id to Update: ";
    cin >> t_id;

    stringstream queryStream;
    queryStream << "SELECT * FROM test WHERE Book_Id = " << t_id;
    string query = queryStream.str();
    const char *sqlQuery = query.c_str();

    if (mysql_query(conn, sqlQuery) == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);

        if (res && mysql_num_rows(res) > 0)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            cout << "\n\n Updating Book with ID " << t_id << "\n";
            cin.ignore();
            cout << "\n\n Enter New Book Name : ";
            string newName;
            getline(cin, newName);
            cout << "\n\n Enter New Author Name : ";
            string newAuthor;
            getline(cin, newAuthor);
            cout << "\n\n Enter New Publisher Name : ";
            string newPublisher;
            getline(cin, newPublisher);

            stringstream updateStream;
            updateStream << "UPDATE test SET Book_Name = '" << newName << "', Author = '"
                          << newAuthor << "', Publisher = '" << newPublisher << "' WHERE Book_Id = " << t_id;
            string updateQuery = updateStream.str();
            const char *updateSqlQuery = updateQuery.c_str();

            if (mysql_query(conn, updateSqlQuery) == 0)
            {
                cout << "\n\n\t\t\tBOOK UPDATED SUCCESSFULLY....";
            }
            else
            {
                cout << "Update failed: " << mysql_error(conn) << endl;
            }
        }
        else
        {
            cout << "Book ID not found in the database." << endl;
        }

        mysql_free_result(res);
    }
    else
    {
        cout << "Search failed: " << mysql_error(conn) << endl;
    }
}

void Library::del()
{
    int t_id;
    system("cls");
    cout << "\n\n\t\t\t===========================================";
    cout << "\n\n\t\t\t=====LIBRARY MANAGEMENT SYSTEM=============";
    cout << "\n\n\t\t\t===========================================";

    cout << "\n\n Enter Book Id to Delete: ";
    cin >> t_id;

    stringstream queryStream;
    queryStream << "DELETE FROM test WHERE Book_Id = " << t_id;
    string query = queryStream.str();
    const char *sqlQuery = query.c_str();

    if (mysql_query(conn, sqlQuery) == 0)
    {
        // Check the number of affected rows
        if (mysql_affected_rows(conn) > 0)
        {
            cout << "\n\n Delete Book Successfully.....";
        }
        else
        {
            cout << "\n\n Book ID not found. Deletion failed." << endl;
        }
    }
    else
    {
        cout << "Delete failed: " << mysql_error(conn) << endl;
    }
}


void Library::Bub_sort()
{
    // Sort the linked list if not empty
    if (head != NULL)
    {
        int count = 0, t_id;
        string t_name, t_author, t_publisher;
        Node *ptr = head;
        while (ptr != NULL)
        {
            count++;
            ptr = ptr->next_add;
        }
        for (int i = 1; i <= count; i++)
        {
            Node *ptr = head;
            for (int j = 1; j < count; j++)
            {
                if (ptr->id > ptr->next_add->id)
                {
                    t_id = ptr->id;
                    t_name = ptr->name;
                    t_author = ptr->author;
                    t_publisher = ptr->publisher;

                    ptr->id = ptr->next_add->id;
                    ptr->name = ptr->next_add->name;
                    ptr->author = ptr->next_add->author;
                    ptr->publisher = ptr->next_add->publisher;

                    ptr->next_add->id = t_id;
                    ptr->next_add->name = t_name;
                    ptr->next_add->author = t_author;
                    ptr->next_add->publisher = t_publisher;
                }
                ptr = ptr->next_add;
            }
        }
    }
}

void Library::show(bool showDatabase)
{
    system("cls");
    if (!conn)
    {
        cout << "\n\n Database connection is not open.\n";
        return;
    }


    if (head == NULL)
    {
        showDatabase = true;
    }

    if (showDatabase)
    {
        cout << "\n\n Showing all records from the database in sorted order:\n";
        const char *sqlQuery = "SELECT * FROM test ORDER BY Book_Id";

        if (mysql_query(conn, sqlQuery) == 0)
        {
            MYSQL_RES *res = mysql_store_result(conn);

            if (res)
            {
                int numFields = mysql_num_fields(res);

                MYSQL_ROW row;
                while ((row = mysql_fetch_row(res)))
                {
                    for (int i = 0; i < numFields; ++i)
                    {
                        cout << "\n\n " << mysql_fetch_field_direct(res, i)->name << " (from DB): " << row[i];
                    }

                    cout << "\n\n ===================================";
                }

                mysql_free_result(res);
            }
            else
            {
                cout << "No records found in the database." << endl;
            }
        }
        else
        {
            cout << "Show all records failed: " << mysql_error(conn) << endl;
        }
    }
    else
    {
        cout << "\n\n Showing all records from the linked list:\n";
        Node *ptr = head;
        while (ptr != NULL)
        {
            cout << "\n\n Book Id (from Linked List): " << ptr->id;
            cout << "\n\n Book Name (from Linked List): " << ptr->name;
            cout << "\n\n Author Name (from Linked List): " << ptr->author;
            cout << "\n\n Publisher Name (from Linked List): " << ptr->publisher;
            cout << "\n\n\n ===================================";
            ptr = ptr->next_add;
        }
    }
}

int main()
{
    Library obj;
    obj.menu();
    return 0;
}
