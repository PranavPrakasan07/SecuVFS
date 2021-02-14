
//filesys.cpp
#include "filesys.h"
#include "file.h"
#include<iostream>
#include<fstream>
#include<string.h>
#include<stdio.h>
#include<string>
#include<math.h>
#include"sqlite3.h"
#include <map>
using namespace std;

#define MX 5
 int x, y, n, t, i, flag;
 long int e[50], d[50], temp[50], j;
 char en[50], m[50];
 char msg[100];
 map<string, string> file_stored;

int choice;
const char* dir = "D:\\Cred.db";

static int createTable(const char* s)
{
    sqlite3* DB;

    string sql = "CREATE TABLE IF NOT EXISTS DataTable(encrypted VARCHAR(200), decrypted VARCHAR(200));";

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
            //cout << "Table created successfully" << endl;
        }

        sqlite3_close(DB);

    }
    catch (const exception& e)
    {
        cerr << e.what();
    }

    return 0;
}

static int insertData(const char* s, string encrypt, string decrypt)
{
    sqlite3* DB;

    char* messageError;

    int exit = sqlite3_open(s, &DB);

    string sql = "INSERT INTO DataTable (encrypted, decrypted) VALUES ('" + encrypt + "', '" + decrypt + "');";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK)
    {
        cerr << "Error!" << endl;
        sqlite3_free(messageError);
    }
    else
    {
        //cout << "Successfully" << endl;
    }
    return 0;
}

static int callback(void* NotUsed, int argc, char** argv, char** arzColName)
{
    int flag;
    flag = 0;

    /*
    for (int i = 0; i < argc; i++)
    {
        //flag = 1;
        cout << arzColName[i] << " " << argv[i] << "\n";
    }
    */
    return flag;
}

static int callbackf(void* NotUsed, int argc, char** argv, char** arzColName)
{
    int flag;
    flag = 0;

    //cout << argc;

    file_stored.insert(pair<string, string>(argv[0], argv[1]));

    /*
    for (int i = 0; i < argc; i++)
    {
        //flag = 1;
        cout << arzColName[i] << " " << argv[i] << "\n";
    }
    */

    return flag;
}

static int selectData(const char* s)
{
    sqlite3* DB;
    file_stored.clear();

    int exit = sqlite3_open(s, &DB);

    string sql = "SELECT * FROM DataTable;";

    exit = sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);

    return 0;
}

static int selectData(const char* s, string encrypt)
{
    sqlite3* DB;

    int exit = sqlite3_open(s, &DB);


    file_stored.clear();
    string sql = "SELECT * FROM DataTable; SELECT * FROM DataTable WHERE encrypted = '" + encrypt + "';";

    exit = sqlite3_exec(DB, sql.c_str(), callbackf, NULL, NULL);
    /*
    if (exit == 0)
    {
        cout << "Successful Login!" << endl;
        return 0;
    }
    else
    {
        cout << "Login failed! ";
        return 1;
    }
    */
    return 0;

}

int filesys::prime(long int pr) {
    int i;
    j = sqrt(pr);
    for (i = 2; i <= j; i++)
    {
        if (pr % i == 0)
            return 0;
    }
    return 1;
}

void filesys::encryption_key() {
    int k;
    k = 0;
    for (i = 2; i < t; i++)
    {
        if (t % i == 0)
            continue;
        flag = prime(i);
        if (flag == 1 && i != x && i != y)
        {
            e[k] = i;
            flag = cd(e[k]);
            if (flag > 0)
            {
                d[k] = flag;
                k++;
            }
            if (k == 99)
                break;
        }
    }
}
long int filesys::cd(long int a) {
    long int k = 1;
    while (1)
    {
        k = k + t;
        if (k % a == 0)
            return(k / a);
    }
}
char* filesys::encrypt_rsa(char* msg) {
    long int pt, ct, key = e[0], k, len;
    i = 0;
    len = strlen(msg);

    while (i != len)
    {
        pt = msg[i];
        pt = pt - 96;
        k = 1;
        for (j = 0; j < key; j++)
        {
            k = k * pt;
            k = k % n;
        }
        temp[i] = k;
        ct = k + 96;
        en[i] = ct;
        i++;
    }
    en[i] = -1;

    string s = "";

    for (i = 0; en[i] != -1; i++) {
        s += en[i];
    }

    return en;
}

string filesys::decrypt_rsa(char* en) {
    long int pt, ct, key = d[0], k;
    char m[50];
    i = 0;
    while (en[i] != -1)
    {
        ct = temp[i];
        k = 1;
        for (j = 0; j < key; j++)
        {
            k = k * ct;
            k = k % n;
        }
        pt = k + 96;
        m[i] = pt;
        i++;
    }
    m[i] = -1;

    string s;
    s = "";

    for (i = 0; m[i] != -1; i++)
    {
        s += m[i];
    }

    cout << endl;

    return s;
}

void filesys::set_file_system_name() {
    cout << "Enter file system name\n";
    char temp[80];
    cin >> temp;
    strcpy_s(file_system_name, temp);
    initialize();
}
filesys::filesys(char f_name[]) {
    if (f_name == NULL)
        set_file_system_name();
    else {
        cout << "Existing File System\n";
        strcpy_s(file_system_name, f_name);
        read_from_file();
    }
}


void filesys::initialize() {
    fstream myfile(file_system_name, ios::out|ios::_Nocreate|ios::_Noreplace);
    int i;
    myfile.seekp(0, ios::beg);
    for (i = 0; i < MAX_FILES; i++) {
        strcpy_s(files[i].name, "\0");
        files[i].len = 0;
        files[i].startpos = 0;
    }
    myfile.close();
    write_to_file();
}


void filesys::list_files() {
    for (int i = 0; i < MAX_FILES; i++) {
        if (!strcmp(files[i].get_file_name(), "\0"))
            break;
        cout << files[i].get_file_name() << endl;
    }
}
char* filesys::show_file_content(char* f_name) {
    fstream myfile(file_system_name, ios::in|ios::_Nocreate|ios::_Noreplace);
    int i;

    char* file_contents = new char[MAX_FILE_LEN];
    cout << f_name << "\n";
    for (i = 0; i < MAX_FILES; i++) {
        if (!strcmp(files[i].get_file_name(), f_name))
        {
            //====================================================================================================================           
        
            //====================================================================================================================
            myfile.seekg(files[i].get_startpos(), ios::beg);
            myfile.read(file_contents, files[i].get_file_length());
            *(file_contents + files[i].get_file_length()) = '\0';
            /*
            string dec = decrypt_rsa(file_contents);
            cout << "Decryption : " << dec;
            */
            myfile.close();
            
            //cout << endl<<"Fetching from table:" << file_contents<<endl;
            selectData(dir, file_contents);

            //cout << "\nHashmap data : \n";

            map<string, string>::iterator itr;
            int same;

            //Decrypt--------------------------

            for (itr = file_stored.begin(); itr != file_stored.end(); ++itr) {
                
                //cout << itr->first << " "<< itr->second << '\n';
                same = 1;

                for (int i = 0; i < itr->first.length(); i++)
                {
                    if (file_contents[i] != itr->first[i])
                    {
                        same = 0;
                        break;
                    }
                }
                if (same == 1)
                {
                    cout << "FOUND!" << endl;
                    cout << itr->second << '\n';
                   
                    //-------------------------------------------------------------------------------------
                 
                    //--------------------------------------------------------------------------------------

                    break;
                }
                
            }
            cout << endl;

            return file_contents;
        }
    }
    cout << "File not found!";
    myfile.close();
    return NULL;
}
char* filesys::search_file(char* f_name) {
    int i;
    for (i = 0; i < MAX_FILES; i++) {
        if (!strcmp(files[i].get_file_name(), f_name)) {
            cout << "File found\nFile name:";
            return files[i].get_file_name();
        }
    }
    cout << "File not found!";
    return NULL;
}


void filesys::search_keyword(char* f_name, char* keyword) {
    fstream myfile(file_system_name, ios::in);
    for (int i = 0; i < MAX_FILES; i++) {
        if (!strcmp(f_name, files[i].get_file_name())) {
            char* file_content = new char[MAX_FILE_LEN];
            myfile.seekg(files[i].get_startpos());
            myfile.read(file_content, files[i].get_file_length());
            char* p = strstr(file_content, keyword);
            if (p == NULL) {
                cout << "\nKeyword not Found!\n";
                myfile.close();
                return;
            }
            int pos = p - file_content + 1;
            cout << "\nKeyword Found!\nPosition of keyword:\t" << pos;
            myfile.close();
            return;
        }
    }
    cout << "\nFile not found!\n";
    myfile.close();
    return;
}


void filesys::delete_file(char* f_name)
{
    int i, j;
    char* file_content = new char[MAX_FILE_LEN];
    for (i = 0; i < MAX_FILES; i++) {
        if (!strcmp(files[i].get_file_name(), f_name)) {
            int del_len;
            strcpy_s(files[i].name, "\0");
            del_len = files[i].get_file_length();
            files[i].len = 0;
            files[i].startpos = 0;
            write_to_file();
            for (j = i + 1; j < MAX_FILES; j++) {
                strcpy_s(files[j - 1].name, files[j].get_file_name());
                files[j - 1].len = files[j].get_file_length();
                files[j - 1].startpos = files[j].get_startpos() - del_len;
            }
            write_to_file();
            cout << "\nFile deleted!\n";
            return;
        }
    }
    cout << "File not found!";
}


void filesys::create_file(char* f_name, char* file_contents) {
    int i;
    int flag;
    flag = 0;

    //---------------------------------------------------

    x = 23;
    y = 5;

    for (i = 0; file_contents[i] != NULL; i++)
        m[i] = file_contents[i];
    
    n = x * y;
    t = (x - 1) * (y - 1);

    encryption_key();

    char* enc = encrypt_rsa(file_contents);
    

    string encrypted_text;

    encrypted_text = "";

    for (i = 0; enc[i] != -1; i++) {
        encrypted_text += enc[i];
    }

    //cout << "Encryption : "<<encrypted_text;

    string dec;
    dec = decrypt_rsa(enc);

    //cout << "Decryption : " << dec <<endl;
    
    createTable(dir);
    insertData(dir, encrypted_text, dec);

    //cout << "Fetching from table" << endl;

    //selectData(dir);

    //----------------------------------------------------

    for (i = 0; i < MAX_FILES; i++) {
        if (!strcmp(files[i].get_file_name(), "\0"))
            break;

        if (!strcmp(files[i].get_file_name(), f_name))
        {
            flag = 1;
        }
    }
    if (i == MAX_FILES)
        cout << "No space";
    else if (flag == 1) {
        cout << "File already exists!\n";
    }
    else {
        fstream myfile(file_system_name, ios::out | ios::app);
        strcpy_s(files[i].name, f_name);
        files[i].len = strlen(enc);
        myfile.seekp(0, ios::end);
        files[i].startpos = myfile.tellp();
        myfile.write(enc, sizeof(char) * strlen(enc));

        myfile.close();
        write_to_file();
    }
}


void filesys::read_from_file() {
    fstream myfile(file_system_name, ios::in);
    int i;
    myfile.seekg(0, ios::beg);
    for (i = 0; i < MAX_FILES; i++)
    {
        myfile.read((char*)&(files[i].name), sizeof(files[i].name));
        if (!strcmp(files[i].name, "\0"))
            break;
        myfile.read((char*)&files[i].len, sizeof(long int));
        myfile.read((char*)&files[i].startpos, sizeof(int));
    }
    while (i < MAX_FILES) {
        strcpy_s(files[i].name, "\0");
        files[i].len = 0;
        files[i].startpos = 0;
        i++;
    }
    myfile.close();
}


void filesys::write_to_file() {
    fstream myfile(file_system_name, ios::in);
    fstream newfile("temp.txt", ios::out);
    int i;
    char file_content[MAX_FILE_LEN];
    newfile.seekp(0, ios::beg);
    for (i = 0; i < MAX_FILES; i++) {
        newfile.write((char*)&files[i].name, sizeof(files[i].name));
        newfile.write((char*)&files[i].len, sizeof(long int));
        newfile.write((char*)&files[i].startpos, sizeof(int));
    }
    for (i = 0; i < MAX_FILES; i++) {
        if (files[i].get_file_length()) {
            myfile.seekg(files[i].get_startpos(), ios::beg);
            myfile.read((char*)&file_content, sizeof(char) * files[i].len);
            newfile.seekp(0, ios::end);
            newfile.write((char*)&file_content, sizeof(char) * files[i].len);
        }
    }
    newfile.close();
    myfile.close();
    remove(file_system_name);
    rename("temp.txt", file_system_name);
}
/*
void filesys:: caesar(char msg[100], int key){

} */
void filesys::playfair(char ch1, char ch2, char key[MX][MX]) {
    int i, j, w, x, y, z;
    for (i = 0; i < MX; i++) {
        for (j = 0; j < MX; j++) {
            if (ch1 == key[i][j]) {
                w = i;
                x = j;
            }
            else if (ch2 == key[i][j]) {
                y = i;
                z = j;
            }
        }
    }
    //printf("%d%d %d%d",w,x,y,z);
    if (w == y) {
        if (choice == 1) {
            x = (x + 1) % 5;
            z = (z + 1) % 5;
        }
        else {
            x = ((x - 1) % 5 + 5) % 5;
            z = ((z - 1) % 5 + 5) % 5;
        }
        printf("%c%c", key[w][x], key[y][z]);
    }
    else if (x == z) {
        if (choice == 1) {
            w = (w + 1) % 5;
            y = (y + 1) % 5;
        }
        else {
            w = ((w - 1) % 5 + 5) % 5;
            y = ((y - 1) % 5 + 5) % 5;
        }
        printf("%c%c", key[w][x], key[y][z]);
    }
    else {
        printf("%c%c", key[w][z], key[y][x]);
    }
}

void filesys::removeDuplicates(char str[]) {
    int hash[256] = { 0 };
    int currentIndex = 0;
    int lastUniqueIndex = 0;
    while (*(str + currentIndex)) {
        char temp = *(str + currentIndex);
        if (0 == hash[temp]) {
            hash[temp] = 1;
            *(str + lastUniqueIndex) = temp;
            lastUniqueIndex++;
        }
        currentIndex++;
    }
    *(str + lastUniqueIndex) = '\0';
}

void filesys::encipher() {
    unsigned int i, j;
    char input[257], key[33];
    printf("Enter Text to be Encrypted [Max. 256 characters/ only alphabets]:\n ");
    fgets(input, 256, stdin);
    printf("Enter Encryption Key [Max. 32 Characters/ only aphabets]: ");
    fgets(key, 32, stdin);
    for (i = 0, j = 0;i < strlen(input);i++, j++)
    {
        //repeat the key if you are at end of it.
        if (j >= strlen(key))
        {
            j = 0;
        }
        //actual logic -> character from input + character from key % 26 is encrypted charater
        printf("%c", 65 + (((toupper(input[i]) - 65) + (toupper(key[j]) - 65)) % 26));
    }
}

void filesys::decipher() {
    unsigned int i, j;
    char input[257], key[33];
    int value;
    printf("Enter Text to be Decrypted [Max. 256 characters/ only alphabets]:\n ");
    fgets(input, 256, stdin);
    printf("Enter Decryption Key [Max. 32 Characters/ only aphabets]: ");
    fgets(key, 32, stdin);
    for (i = 0, j = 0;i < strlen(input);i++, j++)
    {
        //repeat the key if you are at end of it.
        if (j >= strlen(key))
        {
            j = 0;
        }
        //similar to encipher only difference is you need to subtract
        value = (toupper(input[i]) - 64) - (toupper(key[j]) - 64);
        //if value is negative. We have to rotate it backwards (like backwards from z,y,x)
        //so add it to 26 (it's a negative value to adding will actually cause subtraction) to get original character.
        if (value < 0)
        {
            value = 26 + value;
        }
        printf("%c", 65 + (value % 26));
    }

}
/*
int filesys::prime(long int pr) {
    int i;
    j = sqrt(pr);
    for (i = 2; i <= j; i++)
    {
        if (pr % i == 0)
            return 0;
    }
    return 1;
}

void filesys::encryption_key() {
    int k;
    k = 0;
    for (i = 2; i < t; i++)
    {
        if (t % i == 0)
            continue;
        flag = prime(i);
        if (flag == 1 && i != x && i != y)
        {
            e[k] = i;
            flag = cd(e[k]);
            if (flag > 0)
            {
                d[k] = flag;
                k++;
            }
            if (k == 99)
                break;
        }
    }
}
long int filesys::cd(long int a) {
    long int k = 1;
    while (1)
    {
        k = k + t;
        if (k % a == 0)
            return(k / a);
    }
}
string filesys::encrypt_rsa() {
    long int pt, ct, key = e[0], k, len;
    i = 0;
    len = strlen(msg);

    while (i != len)
    {
        pt = m[i];
        pt = pt - 96;
        k = 1;
        for (j = 0; j < key; j++)
        {
            k = k * pt;
            k = k % n;
        }
        temp[i] = k;
        ct = k + 96;
        en[i] = ct;
        i++;
    }
    en[i] = -1;
    cout << "\n\nTHE ENCRYPTED MESSAGE IS\n";
    
    string s = "";
    
    for (i = 0; en[i] != -1; i++) {
        s += en[i];
        cout << en[i];
    }

    return s;
}
void filesys::decrypt_rsa() {
    long int pt, ct, key = d[0], k;
    i = 0;
    while (en[i] != -1)
    {
        ct = temp[i];
        k = 1;
        for (j = 0; j < key; j++)
        {
            k = k * ct;
            k = k % n;
        }
        pt = k + 96;
        m[i] = pt;
        i++;
    }
    m[i] = -1;
    cout << "\n\nTHE DECRYPTED MESSAGE IS\n";
    for (i = 0; m[i] != -1; i++)
        cout << m[i];

    cout << endl;
}

*/

void filesys::encrypt() {
    int choice;
    while (1) {
        //cout << "\n1. Vernam Cipher \n";    //only enc
        cout << "1. Caesar Cipher\n";
        //cout << "3. RSA \n";
        //cout << "4. Playfair Cipher\n";     //dec wrong
        cout << "2. Vigenere Cipher\n";
        cin >> choice;

        //2+5

        switch (choice) {
            /*
        case 1: {
            int t, n, i, j, k, sum = 0;
            string m;
            cout << "Enter the message" << '\n';
            cin >> m;
            string key;
            cout << "Enter the key" << '\n';
            cin >> key;
            int mod = key.size();
            j = 0;
            for (i = key.size();i < m.size();i++) {
                key += key[j % mod];
                j++;
            }
            string ans = "";
            for (i = 0;i < m.size();i++) {
                ans += (m[i] - key[i] + 26) % 26 + 'A';
            }
            cout << "Encrypted message: " << ans << '\n';
            break;
        }
        */
        case 1: {
            int i, x;
            char str[100];

            cout << "Please enter a string:\t";
            cin >> str;

            cout << "\nPlease choose following options:\n";
            cout << "1 = Encrypt the string.\n";
            cout << "2 = Decrypt the string.\n";
            cin >> x;

            //using switch case statements
            switch (x)
            {
                //first case for encrypting a string
            case 1:
                for (i = 0; (i < 100 && str[i] != '\0'); i++)
                    str[i] = str[i] + 2; //the key for encryption is 3 that is added to ASCII value

                cout << "\nEncrypted string: " << str << endl;
                break;

                //second case for decrypting a string
            case 2:
                for (i = 0; (i < 100 && str[i] != '\0'); i++)
                    str[i] = str[i] - 2; //the key for encryption is 3 that is subtracted to ASCII value

                cout << "\nDecrypted string: " << str << endl;
                break;

            }
            break;
        }
              /*
        case 3: {
            cout << "\nENTER FIRST PRIME NUMBER\n";
            cin >> x;

            //checking whether input is prime or not
            flag = prime(x);
            if (flag == 0)
            {
                cout << "\nINVALID INPUT\n";
                exit(0);
            }

            cout << "\nENTER SECOND PRIME NUMBER\n";
            cin >> y;

            flag = prime(y);
            if (flag == 0 || x == y)
            {
                cout << "\nINVALID INPUT\n";
                exit(0);
            }

            cout << "\nENTER MESSAGE OR STRING TO ENCRYPT\n";
            cin >> msg;

            for (i = 0; msg[i] != NULL; i++)
                m[i] = msg[i];
            n = x * y;
            t = (x - 1) * (y - 1);

            encryption_key();
            cout << "\nPOSSIBLE VALUES OF e AND d ARE\n";

            for (i = 0; i < j - 1; i++)
                cout << "\n" << e[i] << "\t" << d[i];

            encrypt_rsa(msg);
            //decrypt_rsa();

            break;
        }

        case 4:
        {

            int i, j, k = 0, l, m = 0, n;
            char key[MX][MX], keyminus[25], keystr[10], str[25] = {
                0
            };

            char alpa[26] = {
                'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
            };
            printf("\n1.Encryption\n2.Decryption\n\nChoice(1 or 2):");
            scanf_s("%d", &choice);
            if (choice != 1 && choice != 2) { printf("Invalid Choice"); return; }
            fflush(stdin);
            printf("\nEnter key:");
            cin.clear();
            cin.ignore();
            fflush(stdin);
            fgets(keystr, 32, stdin);
            printf("Enter the text:");
            fgets(str, 32, stdin);
            removeDuplicates(keystr);
            n = strlen(keystr);
            //convert the characters to uppertext
            for (i = 0; i < n; i++) {
                if (keystr[i] == 'j') keystr[i] = 'i';
                else if (keystr[i] == 'J') keystr[i] = 'I';
                keystr[i] = toupper(keystr[i]);
            }
            //convert all the characters of plaintext to uppertext
            for (i = 0; i < strlen(str); i++) {
                if (str[i] == 'j') str[i] = 'i';
                else if (str[i] == 'J') str[i] = 'I';
                str[i] = toupper(str[i]);
            }
            // store all characters except key
            j = 0;
            for (i = 0; i < 26; i++) {
                for (k = 0; k < n; k++) {
                    if (keystr[k] == alpa[i]) break;
                    else if (alpa[i] == 'J') break;
                }
                if (k == n) {
                    keyminus[j] = alpa[i];
                    j++;
                }
            }
            //construct key keymatrix
            k = 0;
            for (i = 0; i < MX; i++) {
                for (j = 0; j < MX; j++) {
                    if (k < n) {
                        key[i][j] = keystr[k];
                        k++;
                    }
                    else {
                        key[i][j] = keyminus[m];
                        m++;
                    }
                    printf("%c ", key[i][j]);
                }
                printf("\n");
            }
            // construct diagram and convert to cipher text
            printf("\nEntered text :%s\nOutput Text :", str);
            for (i = 0; i < strlen(str); i++) {
                if (str[i] == 'J') str[i] = 'I';
                if (str[i + 1] == '\0') playfair(str[i], 'X', key);
                else {
                    if (str[i + 1] == 'J') str[i + 1] = 'I';
                    if (str[i] == str[i + 1]) playfair(str[i], 'X', key);
                    else {
                        playfair(str[i], str[i + 1], key);
                        i++;
                    }
                }
            }
            if (choice == 2) printf(" (Remove unnecessary X)");

            break;
        }
        */
        case 2:
        {
            printf("\n1. Encrypt Text\n");
            printf("2. Decrypt Text\n");
            printf("Enter Your Choice : ");
            scanf_s("%d", &choice);
            fflush(stdin);

            if (choice == 1)
                encipher();
            else if (choice == 2)
                decipher();
            else
                printf("Please Enter Valid Option.");
            break;
        }

        }
        break;

    }
}
//----------------------------------------------------------------------------------------------------END