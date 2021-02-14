#include <iostream>
#include<fstream>
#include "filesys.h"
#include "file.h"
#include<conio.h>
#include<string.h>
#include "filesys.h"
#include "sqlite3.h"
#include <stack>

using namespace std;

string mhash(string val)
{
    string fin = "";
    for (int i = 0; i < val.length() - 1;i += 2)
    {
        swap(val[i], val[i + 1]);
    }
    for (int i = 0; i < val.length();i++)
    {
        int ch = int(val[i]);
        int f = ch * (i + 1) % 256;
        fin += char(f);
    }
    return(fin);
}

void swap1(char* r, char* s)
{
    char* p = r;
    r = s;
    s = p;
}

struct node {
    int key;
    struct node* left, * right;
};

struct node* newNode(int item) {
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}

struct node* insert(struct node* node, int key) {
    if (node == NULL) return newNode(key);
    if (key < node->key)
        node->left = insert(node->left, key);
    else
        node->right = insert(node->right, key);

    return node;
}
string preorderIterative(node* root)
{
    // return if tree is empty
    if (root == nullptr)
        return "";

    // create an empty stack and push root node
    stack<node*> stack;
    stack.push(root);
    string f = "";
    // loop till stack is empty
    while (!stack.empty())
    {
        // pop a node from the stack and print it
        node* curr = stack.top();
        stack.pop();
        int x = curr->key;
        f += char(x);
        //cout << curr->key << " ";

        // push right child of popped node to the stack
        if (curr->right)
            stack.push(curr->right);

        // push left child of popped node to the stack
        if (curr->left)
            stack.push(curr->left);

        // important note - right child is pushed first so that left child
        // is processed first (FIFO order)
    }
    return f;
}

string mhash1(string val)
{
    struct node* root = NULL;
    for (int i = 0; i < val.length() - 1;i += 2)
    {
        swap(val[i], val[i + 1]);
    }
    int arr[100];
    for (int i = 0; i < val.length();i++)
    {
        int ch = int(val[i]);
        int x = ch * (i + 1) % 256;
        //fin += char(f);
        root = insert(root, x);
    }
    string fin = preorderIterative(root);
    return(fin);
}

void swap(char* r, char* s)
{
    char* p = r;
    r = s;
    s = p;
}

static int createDB(const char* s)
{
    sqlite3* DB;
    int exit = 0;

    exit = sqlite3_open(s, &DB);
    sqlite3_close(DB);

    return 0;
}

static int createTable(const char* s)
{
    sqlite3* DB;

    string sql = "CREATE TABLE IF NOT EXISTS Credential_table(ID INTEGER PRIMARY KEY AUTOINCREMENT, username_v VARCHAR(20), password_hash_v VARCHAR(100));";

    try
    {
        int exit = 0;

        exit = sqlite3_open(s, &DB);

        char* messageError;

        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

        if (exit != SQLITE_OK)
        {
            cerr << "Error creating table" << endl;
            sqlite3_free(messageError);
        }
        else
        {
            cout << "Table created successfully" << endl;
        }

        sqlite3_close(DB);

    }
    catch (const exception& e)
    {
        cerr << e.what();
    }

    return 0;
}

static int insertData(const char* s, string username, string password)
{
    sqlite3* DB;

    char* messageError;

    int exit = sqlite3_open(s, &DB);

    string sql = "INSERT INTO Credential_table (username_v, password_hash_v) VALUES ('" + username + "', '" + password + "');";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK)
    {
        cerr << "Error registering user" << endl;
        sqlite3_free(messageError);
    }
    else
    {
        cout << "User registered successfully" << endl;
    }
    return 0;
}

static int callback(void* NotUsed, int argc, char** argv, char** arzColName)
{
    int flag;
    flag = 0;

    for (int i = 0; i < argc; i++)
    {
        flag = 1;
        //cout << arzColName[i] << " " << argv[i] << "\n";
    }

    return flag;
}

static int selectData(const char* s)
{
    sqlite3* DB;

    int exit = sqlite3_open(s, &DB);

    string sql = "SELECT * FROM Credential_table;";

    exit = sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);

    return 0;
}

static int selectData(const char* s, string username, string password)
{
    sqlite3* DB;

    int exit = sqlite3_open(s, &DB);

    string sql = "SELECT * FROM Credential_table WHERE username_v = '" + username + "' AND password_hash_v = '" + password + "';";

    exit = sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);

    if (exit != 0)
    {
        cout << "Successful Login!" << endl;
        return 0;
    }
    else
    {
        cout << "Login failed! ";
        return 1;
    }

}


int main(int argc, char* argv[])
{
    char name[80], content[200], name1[80], keyword[60], enc_tec[100], * c = NULL;
    
    const char* dir = "D:\\Cred.db";

    cout << "|----------------------------------------------------------------------------------------------------------------------\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|";
    cout << "\t\t\t\t\t\t\t SECUVFS\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n";
    cout << "|----------------------------------------------------------------------------------------------------------------------\n";
    int choice;
    //filesys f1(argv[1]);


    createDB(dir);

    string username;
    string password;

    char response = 'n';
    char ch;
    int return_value = 0;
    int index = 1;

    index = 1;

    while (true)
    {
        cout << "\nExisting user? : ";
        cin >> response;

        system("cls");

        cout << "Enter username : ";
        cin >> username;

        cout << "Enter password : ";
        //cin >> password;

        password = "";
        
        ch = 'o';

        while(ch != '\r') {

            ch = _getch();
            cout << "*";

            if(ch != '\r'/* && ch != '\b'*/)
            password += ch;
        }

        cout << endl;
        
        string password_hash;
        password_hash = mhash(password);

        //NOT AN EXISTING USER, CREATE NEW ENTRY
        if (response != 'y' && response != 'Y')
        {
            createTable(dir);
            insertData(dir, username, password_hash);
            return_value = selectData(dir);
            break;
        }

        //EXISTING USER, CHECK ENTRY
        else
        {
            return_value = selectData(dir, username, password_hash);

            if (return_value == 1)
            {
                cout << "Try again!" << endl;
            }
            else
            {
                break;
            }
        }
    }

    index = 1;

    if (username == "aditya")
    {
        index = 1;
    }
    else if (username == "aahan")
    {
        index = 2;
    }
    else if (username == "mihirgupta")
    {
        index = 3;
    }
    else if (username == "shreyans")
    {
        index = 4;
    }
    else if (username == "pranavp")
    {
        index = 5;
    }
    else
    {
        index = 6;
    }
        
    filesys f1(argv[index]);

    while (1) {
        cout << "\nEnter your choice:\n\n";
        cout << "1. List files in the file system\n";
        cout << "2. Show file content\n";
        cout << "3. Search a file\n";
        //cout << "4. Search for a keyword in a file\n";
        cout << "4. Create new file\n";
        cout << "5. Delete a file\n";
        cout << "6. Encryption \n";
        cin >> choice;

        switch (choice) {
        case 1:
            f1.list_files();
            break;
        case 2:
            cout << "Enter file name\n";
            cin >> name;
            c = f1.show_file_content(name);

            //DECRYPT OR CHECK IF IT IS THERE IN THE DATABASE
            /*
            if (c != NULL)
                cout << c;
            */
            break;

        case 3:
            cout << "Enter file name\n";
            cin >> name;
            c = f1.search_file(name);
            if (c != NULL)
                cout << c;
            break;

            /*
        case 4:
            cout << "Enter the file name\n";
            cin >> name;
            cout << "Enter the keyword to be searched\n";
            cin >> keyword;
            f1.search_keyword(name, keyword);
            break;
            */

        case 4:
            cout << "Enter the name of new file\n";
            cin >> name;
            cout << "Enter the content of the file\n";
            cin.clear();            
            cin.ignore();
            fflush(stdin);
            cin.getline(content, sizeof(content));

            //ENCRYPT

            f1.create_file(name, content);
            break;

        case 5:
            cout << "Enter the name of the file to be deleted\n";
            cin >> name;
            f1.delete_file(name);
            break;
            //my addition

            
        case 6:
            cout << "Choose your encryption technique";
            f1.encrypt();
            break;
            // end
            
        default:
            cout << "Enter a valid option!\n";
        }
        cout << "\nEnter 0 to exit; 1 to continue\n";
        cin >> choice;
        if (!choice)
            break;
            
    }
    return 0;
}

//------------------------------------------------------------------------------------------
