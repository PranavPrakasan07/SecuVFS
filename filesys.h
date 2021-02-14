#pragma once

//filesys.h (header file)
#define MAX_FILE_LEN 1000
#define MAX_FILES 15
#ifndef fs
#define fs
#include "file.h"
#include<fstream>
#define MX 5
using namespace std;
class filesys
{
public:
	file files[MAX_FILES];
	void initialize();
	void read_from_file();
	void write_to_file();
	void set_file_system_name();
	filesys(char f_name[]);
	char file_system_name[20];
	void list_files();
	char* show_file_content(char* f_name);
	char* search_file(char* f_name);
	void search_keyword(char* f_name, char* keyword);
	void delete_file(char* f_name);
	void create_file(char* f_name, char* file_contents);
	void encrypt();
	//void caesar(char msg[100], int key);
	void playfair(char ch1, char ch2, char key[MX][MX]);
	void removeDuplicates(char str[]);
	void encipher();
	void decipher();
	int prime(long int); //function to check for prime number
	void encryption_key();
	long int cd(long int);
	char* encrypt_rsa(char*);
	string decrypt_rsa(char*);

};
#endif // fs
