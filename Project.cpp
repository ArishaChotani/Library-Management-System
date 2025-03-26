#include<fstream>
#include<string>
#include<iostream>
#include<vector>
#include<cstdio>
#include<sstream>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;
void display() {
	std::cout << "\n\t*****************************************************************************************************\n\n";
	std::cout << "\t\t\t\t\t\t\tTHE BOOK NOOK\n";
	std::cout << "\n\t*****************************************************************************************************\n\n";

}
class Inventory {
public:
	void getBooksBorrowed();
	void getVideosBorrowed();
	void getAllBooks();
	void getAllVideos();
};


class Date {
	int remaining_days;
	int remaining_hours;
	int exceeded_days;
	system_clock::time_point due_date;
public:
	void setAsNull() {
		due_date = system_clock::time_point();
	}
	void timeleft() {
		system_clock::duration remaining_time = due_date - std::chrono::system_clock::now();
		remaining_days = duration_cast<hours>(remaining_time).count() / 24;
		remaining_hours = duration_cast<hours>(remaining_time).count() % 24;
		cout << remaining_days << " days and " << remaining_hours << " hours left. " << endl;

	}
	void setDueDate() {
		auto due_date = system_clock::now() + hours(24 * 14);
	}
	int timeElapsed() {
		system_clock::duration elapsed_time = system_clock::now() - due_date;
		exceeded_days = duration_cast<hours>(elapsed_time).count() / 24;
		return exceeded_days;
	}

};
class Penalty {
	float fine;
public:
	float CalculateFineTeacher(int days) {
		fine = 25 * days;
		return fine;
	}
	float CalculateFineStudent(int days) {
		fine = 35 * days;
		return fine;
	}
	bool ProcessPayment() {
		float payment;
		cout << "Enter Payment : : ";
		cin >> payment;
		if (payment == fine||payment > fine) {
			return true;
		}
		else
		{
			return false;
		}

	}
};
class LibraryItem {
protected:
	bool IsBorrowed;
public:
	LibraryItem() {
		IsBorrowed = false;
	}
	virtual bool Borrow() = 0;
	virtual bool Return() = 0;
	virtual void remove() = 0;
	virtual void writeFile() = 0;
	virtual void modifyFile() = 0;
	virtual void displaydetails() = 0;
};

class BookItem :public LibraryItem {
	std::string Publisher;
	std::string ISBN;
	std::vector<std::string> Authors;
	std::string Title;
	string genre;
public:
	Date dt;
	BookItem();
	BookItem(std::string ISBN, std::vector<std::string> Authors, std::string title);
	void setPublisher(std::string publisher);
	std::string getPublisher();
	std::vector<std::string> getAuthor();
	void setISBN(std::string ISBN);
	void setName(string name);
	void setGenre(string genre);
	bool Borrow();
	bool Return();
	void writeFile();
	void remove();
	void modifyFile();
	void setAuthors(std::vector<std::string> Authors);
	std::string getISBN();
	std::string getTitle();
	bool CheckStatus();
	void displaydetails();
	bool operator == (BookItem& b1);
};

//CLASS VIDEO ITEM
class VideoItem :public LibraryItem {
	std::string videoName;
	std::string genre;
	std::string director;
	std::string format;
	std::string ProducingHouse;
public:
	Date dt;
	VideoItem();
	VideoItem(bool b, bool r, bool rem, std::string n, std::string g, std::string d);
	void setFormat(std::string a);
	void setvideoName(std::string n);
	void setgenre(std::string g);
	void setdirector(std::string d);
	void setCompany(string a);
	std::string getformat();
	std::string getvideoName();
	std::string getgenre();
	std::string getdirector();
	bool Borrow();
	bool Return();
	void remove();
	void writeFile();
	void modifyFile();
	void displaydetails();
	bool CheckStatus();
	bool operator == (VideoItem& v1);
};

// CLASS ADMIN

class Admin {
	const std::string ID = "Admin123";
	const std::string password = "BookNook123";
public:
	bool checkAdminLogin(std::string ID, std::string password) {
		if (ID == this->ID && password == this->password) {
			return true;
		}
		return false;
	}
	void additems(VideoItem& v1);
	void additems(BookItem& b1);
	void deleteItem();
};
void Inventory::getAllBooks() {
	
	ifstream f1("BookItems.bin",ios::in | ios::binary);
	BookItem b1;
	f1.seekg(0, ios::beg);
	while (f1.read(reinterpret_cast<char*>(&b1), sizeof(b1))) {
		b1.displaydetails();
	}
	f1.close();
}

void Inventory::getAllVideos() {
	
	ifstream f1("VideoItems.bin", ios::in | ios::binary);
	VideoItem v1;
	f1.seekg(0, ios::beg);
	while (f1.read(reinterpret_cast<char*>(&v1), sizeof(v1))) {
		v1.displaydetails();
	}
	f1.close();
}
void Inventory::getBooksBorrowed() {
	ifstream f1("BookItems.bin", ios::in | ios::binary);
	f1.seekg(0, ios::beg);
	BookItem b1;
	while (f1.read((char*)&b1, sizeof(BookItem))) {
		if (b1.CheckStatus() == true)
		{
			b1.displaydetails();
		}
	}
	f1.close();
}
void Inventory::getVideosBorrowed() {
	ifstream f1("VideoItems.bin", ios::in | ios::binary);
	f1.seekg(0, ios::beg);
	VideoItem v1;
	while (f1.read((char*)&v1, sizeof(VideoItem))) {
		if (v1.CheckStatus() == true)
		{
			v1.displaydetails();
		}
	}
	f1.close();
}

//FUNCTIONS FOR CLASS BOOKITEM
bool BookItem::CheckStatus() {
	return IsBorrowed;
}
void BookItem::writeFile() {
	ofstream f1("BookItems.bin",ios::app|ios::binary);
	f1.write(reinterpret_cast<char*>(this), sizeof(BookItem));
	f1.close();
}
void BookItem::displaydetails() {
	cout << "TITLE : : " <<this->Title<< "\t";
	cout << "ISBN : : " <<this->ISBN<< "\t";
	cout << "AUTHORS : : ";
	for (int i = 0; i < Authors.size(); i++) {
		cout << Authors[i] << ",";
	}
	cout << "\n";
}
void BookItem::remove() {
	fstream f1;
	f1.open("BookItems.bin", ios::in | ios::binary);
	f1.seekg(0, ios::beg);
	fstream temp;
	temp.open("temp.bin", ios::out | ios::binary);
	BookItem b1;
	while (f1.read((char*)&b1, sizeof(BookItem))) {
		if (b1.ISBN == this->ISBN) {
			continue;
		}
		else {
			temp.write((char*)&b1, sizeof(BookItem));
		}
	}
	f1.close();
	temp.close();
	f1.open("BookItems.bin", ios::out | ios::binary);
	temp.open("temp.bin", ios::in | ios::binary);
	temp.seekg(0, ios::beg);
	while (temp.read((char*)&b1, sizeof(BookItem))) {
		f1.write((char*)&b1, sizeof(BookItem));
	}
	f1.close();
	temp.close();
}
void BookItem::modifyFile() {
	fstream f1;
	f1.open("BookItems.bin", ios::in | ios::binary);
	f1.seekg(0, ios::beg);
	fstream temp;
	temp.open("temp.bin", ios::out | ios::binary);
	BookItem tempBK;
	while (f1.read((char*)&tempBK, sizeof(BookItem))) {
		if (tempBK.ISBN == this->ISBN) {
			temp.write((char*)this, sizeof(BookItem));
		}
		else {
			temp.write((char*)&tempBK, sizeof(BookItem));
		}
	}
	f1.close();
	temp.close();
	f1.open("BookItems.bin", ios::out | ios::binary);
	temp.open("temp.bin", ios::in | ios::binary);
	temp.seekg(0, ios::beg);
	while (temp.read((char*)&tempBK, sizeof(BookItem))) {
		f1.write((char*)&tempBK, sizeof(BookItem));
	}
	f1.close();
	temp.close();
}

// CLASS USER

class User {
protected:
	std::string password;
	vector<VideoItem> VidBorrowed;
	vector<BookItem> BkBorrowed;
public:
	User();
	User(std::string);
	virtual std::string getFirstName() = 0;
	virtual std::string getLastName() = 0;
	virtual std::string getID() = 0;
	virtual void IssueItems() = 0;
	virtual void ReturnItems() = 0;
	virtual void openfile() = 0;
	virtual bool readFile(std::string id, std::string password) = 0;
};


//CLASS STUDENT 

class Student :public User {
	std::string f_name;
	std::string l_name;
	std::string roll_no;
public:
	Student();
	Student(std::string f_name, std::string l_name, std::string roll_no, std::string password) :User(password) {};
	bool readFile(std::string id, std::string password);
	std::string getFirstName();
	std::string getLastName();
	void setFirstName(std::string f_name);
	void setLastName(std::string l_name);
	void defaultIssue();
	void defaultReturn();
	std::string getID();
	void openfile();
	void IssueItems();
	void ReturnItems();
};

//CLASS TEACHER::


class Teacher :public User {
	std::string f_name;
	std::string l_name;
	std::string Emp_ID;
public:
	Teacher();
	Teacher(std::string f_name, std::string l_name, std::string Emp_ID, std::string password);
	void openfile();
	bool readFile(std::string id, std::string password);
	void IssueItems();
	void ReturnItems();
	void modifyFile();
	void defaulfForIssue();
	void defaultforReturn();
	std::string getID() {
		return Emp_ID;
	}
	std::string getFirstName() {
		return f_name;
	}
	std::string getLastName() {
		return l_name;
	}
};

class Menu {
public:
	void mainMenu();
	void Introduction();
	void AdminMenu(Admin* a1);
	void UserMenu(Teacher* t1);
	void UserMenu(Student* s1);
private:
	void UserPrompt();
	void AdminPrompt(Admin* a1);
};

Menu menu;


//CLASS VIDEOITEM FUNCTIONS

void VideoItem::displaydetails() {
	cout << "VIDEO NAME:: " << this->videoName << "\t";
	cout << "DIRECTOR:: " << this->director << "\t";
	cout << "FORMAT:: " << this->format << "\t";
	cout << "GENRE:: " << this->genre << "\t";
	cout << "PRODUCING HOUSE:: " << this->ProducingHouse << "\n";
}

bool VideoItem::CheckStatus() {
	return IsBorrowed;
}

void VideoItem::modifyFile() {
	fstream f1;
	fstream temp;
	f1.open("VideoItems.bin", ios::in | ios::binary);
	temp.open("temp.bin", ios::out | ios::binary);
	f1.seekg(0, ios::beg);
	VideoItem tempVD;
	while (f1.read((char*)&tempVD, sizeof(VideoItem))) {
		if (tempVD.videoName == this->videoName) {
			temp.write((char*)this, sizeof(VideoItem));
		}
		else
		{
			temp.write((char*)&tempVD, sizeof(VideoItem));
		}
	}
	f1.close();
	temp.close();
	f1.open("VideoItems.bin", ios::out | ios::binary);
	temp.open("temp.bin", ios::in | ios::binary);
	temp.seekg(0, ios::beg);
	while (temp.read((char*)&tempVD, sizeof(VideoItem))) {
		f1.write((char*)&tempVD, sizeof(VideoItem));
	}
	f1.close();
	temp.close();
}
void VideoItem::writeFile() {
	ofstream f1("VideoItems.bin", ios::app | ios::binary);
	f1.write(reinterpret_cast<char*>(this), sizeof(VideoItem));
	f1.close();
}
bool VideoItem ::operator==(VideoItem& v1) {
	if (v1.videoName == this->videoName) {
		return true;
	}
	else {
		return false;
	}
}
void VideoItem::remove() {
	fstream f1;
	fstream temp;
	f1.open("VideoItems.bin", ios::in | ios::binary);
	temp.open("VideoItems.bin", ios::out | ios::binary);
	f1.seekg(0, ios::beg);
	VideoItem tempVD;
	while (f1.read((char*)&tempVD, sizeof(VideoItem))) {
		if (tempVD.videoName == videoName) {
			continue;
		}
		else {
			temp.write((char*)&tempVD, sizeof(VideoItem));
		}
	}
	temp.close();
	f1.close();
	temp.open("temp.bin", ios::in | ios::binary);
	f1.open("VideoItems.bin", ios::out | ios::binary);
	VideoItem temp1;
	while (temp.read((char*)&temp1, sizeof(VideoItem))) {
		f1.write((char*)&temp1, sizeof(VideoItem));
	}
	f1.close();
	temp.close();
}

User::User() {
	password = " ";
}

User::User(std::string password) {
	this->password = password;
}

Teacher::Teacher()
{
	f_name = " ";
	l_name = " ";
	Emp_ID = " ";
}
Teacher::Teacher(std::string f_name, std::string l_name, std::string Emp_ID, std::string password) :User(password) {
	this->f_name = f_name;
	this->l_name = l_name;
	this->Emp_ID = Emp_ID;
}
bool Teacher::readFile(std::string id, std::string Password) {
	ifstream f2("UserInfo.bin", ios::in | ios::binary);
	f2.seekg(0, ios::beg);
	while (f2.read((char*)this, sizeof(Teacher)))
	{
		if ((Emp_ID == id) && (password == Password)) {
			f2.close();
			return true;
		}
	}
	f2.close();
	return false;

}
void Teacher::defaulfForIssue() {
	int next;
	cout << "1. Main Menu\t\t\t2. User Menu\t\t\t3. Issue Items\t\t\t 4. Exit the Program\n";
	cin >> next;
	switch (next) {
	case 1:
	{
		menu.mainMenu();
		break;
	}
	case 2:
	{
		menu.UserMenu(this);
		break;
	}
	case 3:
	{
		IssueItems();
		break;
	}
	case 4:
	{
		exit(1);
	}

	}

}
void Teacher::openfile() {
	ofstream f1("UserInfo.bin", ios::app | ios::binary);
	f1.write((char*)this, sizeof(Teacher));
	f1.close();
}
void Teacher::IssueItems() {
	int n;
	system("cls");
	display();
	cout << "1. Issue Book Item\t\t\t\t2. Issue Video Item\n";
	cin >> n;
	switch (n) {
	case 1:
	{
		if (BkBorrowed.size() >= 3)
		{
			cout << "You cannot borrow more books...\n";
			defaulfForIssue();

		}
		
		else{
		    BookItem b1;
			if (b1.Borrow() == true) {
				cout << "Successfully Borrowed\n";
				BkBorrowed.push_back(b1);
			}
			else {
				cout << "Borrowing Unsuccessful\n";
				defaulfForIssue();
			}
			
		
		}
		break;
	}
    case 2:
	{
		if (VidBorrowed.size() >= 3) {
			cout << "You cannot borrow more books...";
			defaulfForIssue();
		}
		else {
			VideoItem v1;
			if (v1.Borrow() == true)
			{
				cout << "Successfully Borrowed\n";
				VidBorrowed.push_back(v1);
			}
			else {
				cout << "Borrowing Unsuccessful\n";
				defaulfForIssue();
			}
		}
		break;

	}

	}
	modifyFile();
}
void Teacher::modifyFile() {
	fstream f1;
	fstream temp;
	f1.open("UserInfo.bin", ios::in | ios::binary);
	temp.open("temp.bin", ios::out | ios::binary);
	f1.seekg(0, ios::beg);
	Teacher t1;
	while (f1.read((char*)&t1, sizeof(Teacher))) {
		if (this->Emp_ID == t1.Emp_ID)
		{
			temp.write((char*)this, sizeof(Teacher));
		}
		else {
			temp.write((char*)&t1, sizeof(Teacher));
		}
	}
	f1.close();
	temp.close();
	f1.open("UserInfo.bin", ios::out | ios::binary);
	temp.open("temp.bin", ios::in | ios::binary);
	temp.seekg(0, ios::beg);
	while (temp.read((char*)&t1, sizeof(Teacher))) {
		f1.write((char*)&t1, sizeof(Teacher));
	}
	temp.close();
	f1.close();

}
void Teacher::defaultforReturn() {
	int next;
	system("cls");

	cout << "\n\n1. Call function to Return Items again\t2. Return to User Menu\t 3. Return to Main Menu\t4. Exit the Program\n";
	cin >> next;
	switch (next) {
	case 1:
	{
		ReturnItems();
		break;
	}
	case 2:
	{
		menu.UserMenu(this);
		break;
	}
	case 3:
	{
		menu.mainMenu();
		break;
	}
	case 4:
	{
		exit(1);
		break;
	}
	default:
	{
		defaultforReturn();
	}
	}
}
void Teacher::ReturnItems() {
	int n;
	cout << "1. Return Book\t\t\t\t\t2. Return Video\n";
	cin >> n;
	switch (n) {
	case 1:
	{
		//SEARCH 'BkBorrowed' VECTOR FOR THE BOOK 
		int search;
		cout << "1. To Enter Title\t\t\t\t2. To Enter ISBN\n";
		cin >> search;
		switch (search) {
		case 1:
		{
			string title;
			cout << "Enter title: ";
			cin >> title;
			for (int i = 0; i < BkBorrowed.size(); i++) {
				if (BkBorrowed[i].getTitle() == title) {
					BkBorrowed.erase(BkBorrowed.begin() + i);
					BkBorrowed[i].Return();
					cout << "Successfully Returned\n";
					break;
				}
				if (i == (BkBorrowed.size() - 1)) {
					cout << "Book Not Found\n";
					break;
				}
			}
			defaultforReturn();
			break;
		}
		case 2:
		{
			string ISBN;
			cout << "Enter ISBN: ";
			cin >> ISBN;
			for (int i = 0; i < BkBorrowed.size(); i++) {
				if (BkBorrowed[i].getISBN() == ISBN) {
					BkBorrowed.erase(BkBorrowed.begin() + i);
					cout << "Successfully Returned" << endl;
					break;

				}
				if (i == (BkBorrowed.size() - 1)) {
					cout << "Book Not Found\n";
				}
			}
			defaultforReturn();
			break;
		}
		}
		break;
	}
	case 2:
	{
		//SEARCHING VECTOR 'VidBorrowed' FOR VIDEO;
		string title;
		cout << "Enter title: ";
		cin >> title;
		for (int i = 0; i < VidBorrowed.size(); i++) {
			if (VidBorrowed[i].getvideoName() == title) {
				VidBorrowed.erase(VidBorrowed.begin() + i);
				VidBorrowed[i].Return();
				cout << "Successfully Returned" << endl;
				break;
			}
			if (i == (VidBorrowed.size() - 1)) {
				cout << "Video Not Found\n";
			}
		}
		defaultforReturn();
		break;

	}
	}

}
class Register {
	std::string id;
	std::string f_name;
	std::string l_name;
	std::string password;
public:
	void prompt();
private:
	std::string checkPassword(std::string& a);
};


//FUNCTION TO CHECK ID AND PASSWORD IN THE FILE::
bool Student::readFile(std::string id, std::string password) {
	fstream f2("UserInfo.bin", ios::in | ios::binary);
	f2.seekg(0, ios::beg);
	while (f2.read((char*)this, sizeof(Student)))
	{
		if ((this->roll_no == id) && (this->password == password)) {
			return true;
		}
		else
		{
			continue;
		}
	}
	f2.close();
	return false;
}

//FUNCTION TO WRITE OBJECT OF STUDENT INTO A FILE
void Student::openfile() {
	ofstream f1("UserInfo.bin", ios::app | ios::binary);
	f1.write((char*)this, sizeof(Student));
	f1.close();
}

//GETTERS AND SETTERS FOR CLASS STUDENT'S ATTRIBUTES
std::string Student::getLastName() {
	return l_name;
}
std::string Student::getID() {
	return this->roll_no;
}
std::string Student::getFirstName() {
	return f_name;
}
void Student::setFirstName(std::string f_name) {
	this->f_name = f_name;
}
void Student::setLastName(std::string l_name) {
	this->l_name = l_name;
}


//FUNCTIONS CALLED WHEN BOOKS ARE TO BE ISSUED

//FUNCTION FOR WHEN ISSUING IN UNSUCCESSFUL

void Student::defaultIssue() {
	int num;
	std::cout << "1. Return to Main Menu\t\t\t2. Return to User Menu\t\t\t3.Search Again\t\t\t4.Exit the Program\n";
	std::cin >> num;
	switch (num) {
	case 1:
	{
		menu.mainMenu();
		break;
	}
	case 2:
	{
		menu.UserMenu(this);
		break;
	}
	case 3:
	{
		ReturnItems();
		break;
	}
	case 4:
	{
		exit(EXIT_FAILURE);
		break;
	}
	default:
	{
		cout << "INVALID INPUT\n";
		cout << "RECALLING THE FUNCTION TO ISSUE.....\n";
		IssueItems();
		break;
	}
	}


}

//DEFAULT MESSAGE TO DISPLAY FOR RETURNING
void Student::defaultReturn() {
	int next;
	cout << "1. Call Function to Return Items Again\t2. Return to User Menu\t 3. Return to Main Menu\t 4. Exit The Program\n";
	cin >> next;
	switch (next) {
	case 1:
	{
		ReturnItems();
		break;
	}
	case 2:
	{
		menu.UserMenu(this);
		break;
	}
	case 3:
	{
		menu.mainMenu();
		break;
	}
	case 4:
	{
		exit(1);
		break;
	}
	default:
	{
		cout << "INVALID INPUT!\t\tTRY AGAIN\n";
		defaultReturn();
	}
	}
}
//FUNCTION TO RETURNITEMS
void Student::ReturnItems() {

	int n;
	cout << "1. Return Book\t\t\t\t\t2. Return Video\n";
	cin >> n;
	switch (n) {
	case 1:
	{
		//SEARCH 'BkBorrowed' VECTOR FOR THE BOOK 
		int search;
		cout << "1. To Enter Title\t\t\t\t2. To Enter ISBN\n";
		cin >> search;
		switch (search) {
		case 1:
		{
			string title;
			cout << "Enter title: ";
			cin >> title;
			for (int i = 0; i < BkBorrowed.size(); i++) {
				if (BkBorrowed[i].getTitle() == title) {
					BkBorrowed.erase(BkBorrowed.begin() + i);
					cout << "Successfully Returned\n";
					break;
				}
				if (i == (BkBorrowed.size() - 1)) {
					cout << "Book Not Found\n";
					break;
				}
			}
			this->defaultReturn();
			break;
		}
		case 2:
		{
			string ISBN;
			cout << "Enter ISBN: ";
			cin >> ISBN;
			for (int i = 0; i < BkBorrowed.size(); i++) {
				if (BkBorrowed[i].getISBN() == ISBN) {
					BkBorrowed.erase(BkBorrowed.begin() + i);
					cout << "Successfully Returned" << endl;
					break;

				}
				if (i == (BkBorrowed.size() - 1)) {
					cout << "Book Not Found\n";
				}
			}
			this->defaultReturn();
			break;
		}
		}
		break;
	}
	case 2:
	{
		//SEARCHING VECTOR 'VidBorrowed' FOR VIDEO;
		string title;
		cout << "Enter title: ";
		cin >> title;
		for (int i = 0; i < VidBorrowed.size(); i++) {
			if (VidBorrowed[i].getvideoName() == title) {
				VidBorrowed.erase(VidBorrowed.begin() + i);
				VidBorrowed[i].Return();
				cout << "Successfully Returned" << endl;
				break;
			}
			if (i == (VidBorrowed.size() - 1)) {
				cout << "Video Not Found\n";
			}
		}
		this->defaultReturn();
		break;

	}
	}

}

//FUNCTION CALLED TO ISSUEITEMS
void Student::IssueItems() {
	int n;
	cout << "1. BookItem\t\t\t\t2.VideoItem\n";
	cin >> n;
	switch (n) {
	case 1:
	{
		BookItem b1;
		if (b1.Borrow() == true) {
			BkBorrowed.push_back(b1);
			cout << "Book Issued Successfully\n";
		}
		else
		{
			cout << "Issuing Unsuccessful\n";
		}
		break;
	}
	case 2:
	{
		VideoItem v1;
		if (v1.Borrow() == true) {
			cout << "Video Issued Successfully!\n";
			VidBorrowed.push_back(v1);
		}
		else
		{
			cout << "Issuing Not Successful\n";
			defaultIssue();
		}
		break;
	}
	}
	defaultIssue();
}

Student::Student() {
	roll_no = " ";
	password = " ";
	f_name = " ";
	l_name = " ";
}
VideoItem::VideoItem() {
	videoName = "";
	genre = " ";
	director = " ";
	format = " ";
}

VideoItem::VideoItem(bool b, bool r, bool rem, std::string n, std::string g, std::string d) :
	LibraryItem(), videoName(n), genre(g), director(d) {
}

void VideoItem::setFormat(std::string a) {
	format = a;
}
void VideoItem::setvideoName(std::string n) {
	videoName = n;
}
void VideoItem::setgenre(std::string g) {
	genre = g;
}
void VideoItem::setdirector(std::string d) {
	director = d;
}
void VideoItem::setCompany(string a) {
	this->ProducingHouse = a;
}


std::string VideoItem::getformat() {
	return format;
}
std::string VideoItem::getvideoName() {
	return videoName;
}
std::string VideoItem::getgenre() {
	return genre;
}
std::string VideoItem::getdirector() {
	return director;
}

bool VideoItem::Borrow() {
	ifstream f1;
	VideoItem v1; string b;
	cout << "Enter Video Name : ";
	cin >> b;
	v1.videoName = b;
	f1.open("VideoItems.bin",ios::in|ios::binary);
	while (f1.read((char*)&v1, sizeof(VideoItem))) {
		if (this->operator==(v1)) {
			if (v1.IsBorrowed==false)
			{
				IsBorrowed = true;
				dt.setDueDate();
				f1.close();
				modifyFile();
				return true;
			}
		}
		
	}
	f1.close();
	return false;

}
bool VideoItem::Return() {
	if (IsBorrowed == true)
	{
		IsBorrowed = false;
		return true;
	}
	else
	{
		return false;
	}
}

BookItem::BookItem() {
	ISBN = " ";
	Title = " ";
	genre = " ";
	Publisher = " ";
	for (int i = 0; i < Authors.size(); i++) {
		Authors[i] = " ";
	}
}

void Admin::additems(BookItem& b1) {
	std::string a, b, c, d, e;
	int n;
	vector<std::string> authors;
	cout << "ENTER TITLE : : ";
	std::cin >> a;
	b1.setName(a);
	cout << "ENTER GENRE : : ";
	cin >> e;
	b1.setGenre(e);
	cout << "ENTER AUTHOR(S) : : ";
	cout << "ENTER NUMBER OF AUTHORS : : ";
	cin >> n;
	for (int i = 0; i < n; i++) {

		cout << "Enter Author " << i + 1 << " : ";
		cin >> b;
		authors.push_back(b);
	}
	b1.setAuthors(authors);
	cout << "ENTER ISBN : : ";
	std::cin >> c;
	b1.setISBN(c);
	cout << "ENTER PUBLISHER : : ";
	cin >> d;
	b1.setPublisher(d);
	b1.getPublisher();
	b1.writeFile();
}

void Admin::additems(VideoItem& v1) {
	std::string s, b, c, d,e;
	cout << "ENTER TITLE : : ";
	std::cin >> s;
	v1.setvideoName(s);
	cout << "ENTER VIDEO FORMAT : : ";
	std::cin >> b;
	v1.setFormat(b);
	cout << "ENTER DIRECTOR : : ";
	std::cin >> c;
	v1.setdirector(c);
	cout << "ENTER NAME OF PRODUCING COMPANY : : ";
	std::cin >> d;
	v1.setCompany(d);
	cout << "ENTER GENRE : : ";
	cin >> e;
	v1.setgenre(e);
	v1.writeFile();
}


BookItem::BookItem(std::string isbn, vector<std::string> authors, std::string title) {
	ISBN = isbn;
	for (int i = 0; i < title.size(); i++)
	{
		Authors[i] = authors[i];
	}
	Title = title;
}

bool BookItem::operator==(BookItem& b1) {
	if (b1.Title == Title || b1.ISBN == ISBN) {
		return true;
	}
}
void BookItem::setGenre(string Genre) {
	genre = Genre;
}
void BookItem::setPublisher(std::string publisher) {
	Publisher = publisher;
}
std::string BookItem::getPublisher() {
	return Publisher;
}
vector<std::string> BookItem::getAuthor() {
	return Authors;
}
void BookItem::setName(std::string name) {
	Title = name;
}
void BookItem::setISBN(std::string isbn) {
	ISBN = isbn;
}

bool BookItem::Borrow() {
	ifstream f1;
	BookItem b1; string b;
	cout << "Enter Video Name : ";
	cin >> b;
	b1.Title = b;
	f1.open("VideoItems.bin", ios::in | ios::binary);
	while (f1.read((char*)&b1, sizeof(BookItem))) {
		if (this->operator==(b1)) {
			if (b1.IsBorrowed == false)
			{
				IsBorrowed = true;
				dt.setDueDate();
				f1.close();
				modifyFile();
				return true;
			}
		}

	}
	f1.close();
	return false;


}

bool BookItem::Return() {

	if (IsBorrowed == true)
	{
		IsBorrowed = false;
		this->modifyFile();
		return true;

	}
	else
	{
		return false;
	}
}
void BookItem::setAuthors(vector<std::string> Authors) {
	this->Authors = Authors;
}
std::string BookItem::getISBN() {
	return ISBN;
}
std::string BookItem::getTitle() {
	return Title;
}
std::string Register::checkPassword(std::string& a) {
	int n = a.length();
	bool haslower = false, hasupper = false, hasnumber = false, hasspecial = false;
	for (int i = 0; i < n; i++)
	{
		if (islower(a[i]))
		{
			haslower = true;
		}
		if (isupper(a[i])) {
			hasupper = true;
		}
		if (isdigit(a[i])) {
			hasnumber = true;
		}
		if (!isdigit(a[i]) && !isalpha(a[i])) {
			hasspecial = true;
		}
	}
	if (hasupper && haslower && hasspecial && hasnumber && n >= 8) {
		return "STRONG PASSWORD";
	}
	if ((haslower || hasupper) && hasspecial && n >= 8) {
		return "MODERATE PASSWORD";
	}
	return "WEAK PASSWORD";
}
void Register::prompt()
{
	system("cls");
	display();
	cout << "ARE YOU A:\n1.STUDENT\n2.TEACHER\n" << endl;
	int a;
	std::cin >> a;
	switch (a)
	{
	case 1:
	{

		cout << "ENTER YOUR FIRST NAME : : ";
		std::cin >> f_name;
		cout << "ENTER YOUR LAST NAME : : ";
		cin >> l_name;
		cout << "ENTER YOUR ROLL NO. : : ";
		cin >> id;
		cout << "ENTER YOUR PASSWORD : : ";
		cin >> password;
		while (1) {
			if (checkPassword(password) == "STRONG PASSWORD") { cout << "YOUR PASSWORD IS STRONG\n"; break; }
			else {
				cout << checkPassword(password);
				cout << "\tPLEASE TRY AGAIN!\n";
				cout << "ENTER YOUR PASSWORD : : ";
				cin >> password;
			}
		}
		Student s1(f_name, l_name, id, password);
		s1.openfile();
		menu.UserMenu(&s1);

		break; }
	case 2:
	{

		cout << "ENTER YOUR FIRST NAME : : ";
		cin >> f_name;
		cout << "ENTER YOUR LAST NAME : : ";
		cin >> l_name;
		cout << "ENTER YOUR EMPLOYEE ID. : : ";
		cin >> id;
		cout << "ENTER YOUR PASSWORD : : ";
		cin >> password;
		Teacher t1(f_name, l_name, id, password);
		t1.openfile();  //emplyee ID starts with "TF" by design
		menu.UserMenu(&t1);
		break;
	}
	default:
	{
		cout << "INVALID INPUT" << endl << "TRY AGAIN";
		prompt();
		break;
	}

	}
}
class Login {
	std::string id;
	std::string password;
public:
	int n;
	void AnotherPrompt() {
		int num;
		display();
		cout << "ENTER 1 TO TRY AGAIN\tENTER 0 TO EXIT\n";
		cin >> num;
		switch (num) {
		case 1:
		{
			prompt();
			break;
		}
		case 0:
		{
			exit(1);
			break;
		}
		default: {
			cout << "INVALID INPUT!\n";
			cout << "TRY AGAIN!\n";
			AnotherPrompt();
		}
		}
	}
	void prompt() {
		cout << "ARE YOU A:\t1.TEACHER\t2.STUDENT\n";
		cin >> n;
		switch (n) {
		case 1: {
			cout << "ENTER YOUR ID : : ";
			cin >> id;
			cout << "ENTER YOUR PASSWORD : : ";
			cin >> password;
			Teacher t1;
			if (t1.readFile(id, password) == false) {
				cout << "INCORRECT USERNAME OR PASSWORD" << endl;
				AnotherPrompt();
			}
			else if (t1.readFile(id, password) == true) {
				cout << "LOGIN SUCCESSFUL" << endl;
				menu.UserMenu(&t1);

			}
			break;
		}
		case 2: {
			std::cout << "ENTER YOUR ID : : ";
			std::cin >> id;
			std::cout << "ENTER PASSWORD : : ";
			std::cin >> password;
			Student s1;
			if (s1.readFile(id, password) == 0) {
				cout << "INCORRECT USERNAME OR PASSWORD" << endl;
				AnotherPrompt();
			}
			else if (s1.readFile(id, password) == 1) {
				cout << "LOGIN SUCCESSFUL" << endl;
				menu.UserMenu(&s1);
			}

			break;
		}
		default: {
			AnotherPrompt();
			break;
		}
		}
	}
};
Register r1;
Login l1;
void Menu::UserMenu(Student* s1) {
	system("cls");
	display();
	int n;
	std::cout << "1. BORROW\t\t\t\t\t\t2.RETURN" << endl;
	std::cin >> n;
	switch (n) {
	case 1:
	{
		s1->IssueItems();
		break;
	}
	case 2:
	{
		s1->ReturnItems();
		break;
	}
	default:
	{
		std::cout << ": : INVALID INPUT : : \n";
		std::cout << ": : TRY AGAIN! : : \n";

	}
	}

}
void Menu::UserMenu(Teacher *t1) {
	system("cls");
	display();
	int n;
	std::cout << "1. BORROW\t\t\t\t\t\t2.RETURN" << endl;
	std::cin >> n;
	switch (n) {
	case 1:
	{
		t1->IssueItems();
		break;
	}
	case 2:
	{
		t1->ReturnItems();
		break;
	}
	default:
	{
		std::cout << ": : INVALID INPUT : : \n";
		std::cout << ": : TRY AGAIN! : : \n";

	}
	}
}
void Menu::AdminMenu(Admin* a1) {
	int n;
	system("cls");
	display();
	std::cout << "\t\t\t\t\t\t\t\t\t\t ENTER\n";
	std::cout << "1.ADD\t\t\t2.DELETE\t\t\t3.VIEW\n";
	std::cin >> n;
	switch (n) {
	case 1:
	{
		int n1;
		cout << "1. Add Book\t\t\t\t2.Add Video\n";
		cin >> n1;
		switch (n1)
		{
		case 1:
		{
			BookItem b1;
			a1->additems(b1);
			break;
		}
		case 2:
		{
			VideoItem v1;
			a1->additems(v1);
		}
		}
		break;
	}
	case 2:
	{
		a1->deleteItem();
		break;
	}
	case 3:
	{
		Inventory in1;
		int choice;
		cout << "1. View All Books\t\t\t2.View All Videos\t\t\t3.View Books Borrowed\t\t\t4.View Videos Borrowed\n";
		cin >> choice;
		switch (choice) {
		case 1:
		{
			in1.getAllBooks();
			break;
		}
		case 2:
		{
			in1.getAllVideos();
			break;
		}
		case 3:
		{
			in1.getBooksBorrowed();
			break;
		}
		case 4:
		{
			in1.getVideosBorrowed();
			break;
		}
		default:
		{
			cout << "Invalid Input\n";
			cout << "Returning to Admin Menu...";
			menu.AdminMenu(a1);
			break;
		}
		}

	}
	}
}
void Menu::Introduction() {
	display();
	std::cout << "\t\t\t\t\tWELCOME TO THE LIBRARY MANAGEMENT SYSTEM\t\t"<<endl;
	cout << "This is a Library Management system"<<endl;
	cout << "As an admin you can Add, Delete and View Items." << endl;
	cout << "To login as administrator use ID = 'Admin123' and Password = 'BookNook123'" << endl;
	cout << "As a User you can Borrow or Return a book" << endl;
	cout << "A teacher is allowed to Issue 3 videos and 3 books at a time"<<endl;
	cout << "A Student is allowed to issue 2 books and 2 videos at a time" << endl;
	cout << "The loaning period of each library item is 14 days fron the date of issuing" << endl;
	cout << "In the advent of returning late the user would be fined accordingly" << endl;
}
void Menu::mainMenu() {
	int n;
	system("cls");
	display();
	std::cout << "\n\t\t\t\t\t\t\t  MAIN MENU\n";
	std::cout << "\n\t*****************************************************************************************************\n";
	std::cout << "\nARE YOU A: \t\t\t 1.USER \t\t\t\t\t\t2.ADMINISTRATOR\n";
	std::cin >> n;
	switch (n)
	{
	case 1:
	{
		UserPrompt();
		break; }
	case 2:
	{
		Admin a1;
		AdminPrompt(&a1);
		break;
	}
	default:
	{
		std::cout << "INVALID INPUT! TRY AGAIN" << endl;
	}
	}
}
void Menu::AdminPrompt(Admin* a1)
{
	std::string a, b;
	system("cls");
	display();
	std::cout << "ENTER USERNAME : : ";
	std::cin >> a;
	std::cout << "ENTER PASSWORD : : ";
	std::cin >> b;
	if (a1->checkAdminLogin(a, b) == true)
	{
		system("cls");
		display();
		std::cout << "SUCCESSFULLY LOGGED ON\n";
		AdminMenu(a1);
	}
	else
	{
		std::cout << "LOGIN FAILED! PLEASE TRY AGAIN\n";
		AdminPrompt(a1);

	}
}
void Menu::UserPrompt() {
	int n1;
	system("cls");
	display();
	std::cout << "1.USER LOGIN\n2.USER REGISTER\n";
	std::cin >> n1;
	switch (n1)
	{
	case 1:
	{
		Login l1;
		l1.prompt();
		break;
	}
	case 2:
	{
		Register r1;
		r1.prompt();
		break;
	}
	default:
	{
		std::cout << "INVALID INPUT! TRY AGAIN" << endl;
		UserPrompt();
	}
	}
}


int main()
{
	menu.Introduction();
	menu.mainMenu();
}

void Admin::deleteItem() {
	system("cls");
	display();
	std::string id;
	int cho_ice;
	std::cout << "1. Book\t\t\t\t\t\t2.Video";
	std::cin >> cho_ice;
	switch (cho_ice) {
	case 1:
	{
		BookItem b1;
		b1.remove();
	}
	case 2:
	{
		VideoItem v1;
		v1.remove();
	}
	}
}
