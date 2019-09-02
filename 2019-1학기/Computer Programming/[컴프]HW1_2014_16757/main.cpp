#include <iostream>
#include <iterator>
#include <vector>
#include <regex>
#include <string>
#include <ctime>
/*
regex
"^" < 시작점
"$" < 끝점
ex) "^\d-\d\d-\d$"
정확히 이 스트링으로 끝나야 함.

ex) "(02|010)-(\d{4})-(\d{4})"
02-xxxx-xxxx
010-xxxx-xxxx 받는 pattern string.

\d+ <- 1~inf 개까지 받음
\d* <- 0~inf 까지 받음
\d

firstName과 LastName 구분 : 공백으로 구분됨
"([a-zA-Z]+) ([a-zA-Z]+)"
firstName과 LastName 받아오는 regex

*/

using namespace std;

void print_Prompt();
void print_Errmsg();
void print_Topmenu_Choices();
void print_Addmenu_Choices();
void get_Name_from_User(string &firstname, string &lastname);
void get_Phone_Number_from_User(string &phone_number);
void get_Team_from_User(string &team);
void get_Birthday_from_User(string &birthday);
void get_Age_from_User(int &age);
bool parse_Name(string &input, string &firstname, string &lastname);
bool parse_Phone_Number(string &input, string &phone_number);
bool parse_Team(string &input, string &team);
bool parse_Birthday(string &input, string &birthday);
bool parse_Age(string &input, int &age);
bool parse_index(int &index);
bool check_Month_Day(int month, int day);


class Person
{
public:
    Person(string &firstName, string &lastName, string &PhoneNumber);
    void setFirstName(string &firstName); //set firstName string
    string getFirstName(); //return firstName
    void setLastName(string &lastName); //set lastName string
    string getLastName(); //return lastName
    void setPhoneNumber(string &phoneNumber); //set phonenumber 02-XXXX-XXXX or 010-XXXX-XXXX
    string getPhoneNumber(); //return phonenumber
    virtual void print(); // print this object's infomation

private:
    string firstname;
    string lastname;
    string phone_number;
};


class Work : public Person
{ 

public:
    Work(string &firstName, string &lastName, string &PhoneNumber, string &team);
    void setTeam(string team); //set team
    string getTeam(); //return team
    virtual void print(); //print the work object, using the parent function to get the attribute

private:
    string team; //team that the person is in
};


class Friend : public Person
{

public:
    Friend(string &firstName, string &lastName, string &PhoneNumber, int age);
    void setAge(int age); //set age
    int getAge(); //return age
    virtual void print(); //print the friend object, using the parent function to get the attribute

private:
    int age; //age of friend
};

class Family : public Person
{
public:
    Family(string &firstName, string &lastName, string &PhoneNumber, string birthday);
    void setBirthday(string birthday); //set birthday (YYMMDD) string
    string getBirthday(); //return birthday
    int dDay(); //calculate the date difference between the birthday and current time
    virtual void print(); //print the family object, using the parent function to get the attribute
private:
    string birthday; //birthday of family
    int get_num_day(int month, int day); // get day from month and day
};


Person::Person(string &firstname, string &lastname, string &phone_number) : firstname(firstname), lastname(lastname), phone_number(phone_number)
{
    
}

void Person::setFirstName(string &firstName) //set firstName string
{
    this->firstname = firstName;
} 

string Person::getFirstName() //return firstName
{
    return this->firstname;
}

void Person::setLastName(string &lastName) //set lastName string
{
    this->lastname = lastName;
}

string Person::getLastName() //return lastName
{
    return this->lastname;
}

void Person::setPhoneNumber(string &phoneNumber) //set phonenumber
{
    this->phone_number = phoneNumber;
}
//02-XXXX-XXXX or 010-XXXX-XXXX

string Person::getPhoneNumber() //return phonenumber
{
    return this->phone_number;
}

void Person::print() // print this object's infomation
{
    cout << this->getFirstName() << " " << this->getLastName() << "_" << this->getPhoneNumber() << endl;
}


Work::Work(string &firstName, string &lastName, string &PhoneNumber, string &team) : Person(firstName, lastName, PhoneNumber), team(team)
{

}
void Work::setTeam(string team) //set team
{
    this->team = team;
}
string Work::getTeam() //return team
{
    return this->team;
}

void Work::print() //print the work object, using the parent function to get the attribute
{
    cout << this->getFirstName() << " " << this->getLastName() << "_" << this->getPhoneNumber();
    cout << "_" << this->getTeam() << endl;
}

Friend::Friend(string &firstName, string &lastName, string &PhoneNumber, int age) : Person(firstName, lastName, PhoneNumber), age(age)
{
}
void Friend::setAge(int age) //set age
{
    this->age = age;
}
int Friend::getAge() //return age
{
    return this->age;
}
void Friend::print() //print the work object, using the parent function to get the attribute
{
    cout << this->getFirstName() << " " << this->getLastName() << "_" << this->getPhoneNumber();
    cout << "_" << this->getAge() << endl;
}


Family::Family(string &firstName, string &lastName, string &PhoneNumber, string birthday) : Person(firstName, lastName, PhoneNumber), birthday(birthday)
{
}
void Family::setBirthday(string birthday) //set birthday (YYMMDD) string
{
    this->birthday = birthday;
}
string Family::getBirthday() //return birthday
{
    return this->birthday;
}
int Family::dDay() //calculate the date difference between the birthday and current time
{
    time_t now = time(0);
    string birth = this->getBirthday();
    tm *ltm = localtime(&now);

    int now_month = ltm->tm_mon + 1;
    int now_day = ltm->tm_mday;
    int now_day_num = this->get_num_day(now_month,now_day);

    int birth_month = stoi(birth.substr(2,2));
    int birth_day = stoi(birth.substr(4,2));
    int birth_day_num = this->get_num_day(birth_month,birth_day);

    int remainday = birth_day_num - now_day_num; // get diffrence of day.
    if(remainday < 0) remainday += 365; // if birthday is before than current day, count 365days more.

    return remainday;
}
void Family::print() //print the work object, using the parent function to get the attribute
{
    cout << this->getFirstName() << " " << this->getLastName() << "_" << this->getPhoneNumber();
    cout << "_" << this->getBirthday() << "_" << this->dDay() << endl;
}

int Family::get_num_day(int month, int day)
{
    int i;
    int res=0;
    for(i=1;i<month;i++)    // count number day of months.
    {
        switch(month)
        {
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
                res += 31;
            case 4:
            case 6:
            case 9:
            case 11:
                res += 30;
            case 2:
                res += 28;
        } 
    }
    res += day;
    return res;
}



void print_Prompt()
{
    cout << "CP-2014-16757> "; 
}

void print_Errmsg()
{
    //cout << "please insert correct input" << endl;
}

void print_Topmenu_Choices()
{
    cout << "Phone Book" << endl;
    cout << "1. Add person" << endl;
    cout << "2. Remove person" << endl;
    cout << "3. Print phone book" << endl;
}

void print_Addmenu_Choices()
{
    cout << "Select Type" << endl;
    cout << "1. Person" << endl;
    cout << "2. Work" << endl;
    cout << "3. Family" << endl;
    cout << "4. Friend" << endl;
}

void get_Name_from_User(string &firstname, string &lastname)
{
    string userinput;
    while(1)
    {
        cout << "Name: ";
        getline(cin, userinput);
        if(parse_Name(userinput, firstname, lastname) == true) break;
        else 
        {
            print_Errmsg();
            continue;
        }
    }

}

void get_Phone_Number_from_User(string &phone_number)
{
    string userinput;
    while(1)
    {
        cout << "Phone_number: ";
        getline(cin, userinput);
        if(parse_Phone_Number(userinput, phone_number) == true) break;
        else
        {
            print_Errmsg();
            continue;
        }
    }
}

void get_Team_from_User(string &team)
{
    string userinput;
    while(1)
    {
        cout << "Team: ";
        getline(cin, userinput);
        if(parse_Team(userinput, team) == true) break;
        else
        {
            print_Errmsg();
            continue;
        }
    }
}

void get_Birthday_from_User(string &birthday)
{
    string userinput;
    while(1)
    {
        cout << "Birthday: ";
        getline(cin, userinput);
        if(parse_Birthday(userinput, birthday) == true) break;
        else
        {
            print_Errmsg();
            continue;
        }
    }
}

void get_Age_from_User(int &age)
{
    string userinput;
    while(1)
    {
        cout << "Age: ";
        getline(cin, userinput);
        if(parse_Age(userinput, age) == true) break;
        else
        {
            print_Errmsg();
            continue;
        }
    }
}

bool parse_Name(string &input, string &firstname, string &lastname)
{
    regex rname("([a-zA-Z]+) ([a-zA-Z]+)");
    smatch m;
    if(regex_match(input, m, rname))
    {
        firstname = m[1];
        lastname = m[2];
        return true;
    }
    else return false;
}

bool parse_Phone_Number(string &input, string &phone_number)
{
    regex rphone("(02|010)-(\\d{4})-(\\d{4})");
    smatch m;
    if(regex_match(input, m, rphone))
    {
        phone_number = m[0];
        return true;
    }
    else return false;
}

bool parse_Team(string &input, string &team)
{
    team = input;
    return true;
    /*
    regex rteam("[a-zA-Z0-9]+");
    smatch m;
    if(regex_match(input, m, rteam))
    {
        team = m[0];
        return true;
    }
    else return false;
    */
}

bool parse_Birthday(string &input, string &birthday)
{
    regex rbirth("(\\d{2})(\\d{2})(\\d{2})");
    smatch m;
    if(regex_match(input, m, rbirth))
    {
        string year = m[1];
        string month = m[2];
        string day = m[3];
        int imonth = stoi(month);
        int iday = stoi(day);
        if(check_Month_Day(imonth, iday))
        {
            birthday = m[0];
            return true;
        }
        else return false;
            
    }
    else return false;
}

bool parse_Age(string &input, int &age)
{
    regex rage("(\\d+)");
    smatch m;
    if(regex_match(input, m, rage))
    {
        int getnum = stoi(m[1]); // string to int
        if(getnum <= 0) return false;
        else
        {
            age = getnum;
            return true;
        }
    }
    else return false;
}

bool parse_Index(string &input, int &index)
{
    regex rindex("(\\d+)");
    smatch m;
    if(regex_match(input, m, rindex))
    {
        int getnum = stoi(m[1]); // string to int
        if(getnum <= 0) return false; // if index is lower then 1
        index = getnum;
        return true;
    }
    else return false;
}

bool check_Month_Day(int month, int day)
{
    switch (month) // check month and day is vaild.
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            if((day < 32) && (day > 0)) break;
            else return false;
        case 4:
        case 6:
        case 9:
        case 11:
            if((day < 31) && (day > 0)) break;
            else return false;
        case 2:
            if((day < 29) && (day > 0)) break;
            else return false;
        default:
            return false;
    }
    return true;
}


int main()
{
    vector<Person *> phoneBook;
    string input;

    /* while loop start to process user input */
    while(1)
    {
        print_Prompt();
        getline(cin, input); /* get user's input */

        if(input.compare("") == 0) // if user input is empty line, print topmenu choices.
        {
            print_Topmenu_Choices();
        }
        else if(input.compare("exit") == 0) // if user input is exit, terminate program.
        {
            break;
        }
        else
        {
            if(input.compare("1") == 0) // if user input is 1, enter add process
            {
                while(1)
                {
                    bool added = false;

                    print_Addmenu_Choices();
                    print_Prompt();
                    getline(cin, input); /* get user's input */

                    if(input.compare("1") == 0) // add Person
                    {                    
                        string firstname, lastname;
                        string phone_number;
                        get_Name_from_User(firstname, lastname);
                        get_Phone_Number_from_User(phone_number);
                        
                        Person *person = new Person(firstname, lastname, phone_number);
                        phoneBook.push_back(person);

                        added = true;
                    }
                    else if(input.compare("2") == 0) // add Work
                    {
                        string firstname, lastname;
                        string phone_number;
                        string team;
                        get_Name_from_User(firstname, lastname);
                        get_Phone_Number_from_User(phone_number);
                        get_Team_from_User(team);

                        Work *work = new Work(firstname, lastname, phone_number, team);
                        phoneBook.push_back(work);

                        added = true;
                    }
                    else if(input.compare("3") == 0) // add Family
                    {
                        string firstname, lastname;
                        string phone_number;
                        string birthday;
                        get_Name_from_User(firstname, lastname);
                        get_Phone_Number_from_User(phone_number);
                        get_Birthday_from_User(birthday);

                        Family *family = new Family(firstname, lastname, phone_number, birthday);
                        phoneBook.push_back(family);

                        added = true;
                    }
                    else if(input.compare("4") == 0) // add Friend
                    {
                        string firstname, lastname;
                        string phone_number;
                        int age;
                        get_Name_from_User(firstname, lastname);
                        get_Phone_Number_from_User(phone_number);
                        get_Age_from_User(age);

                        Friend *friend_obj = new Friend(firstname, lastname, phone_number, age);
                        phoneBook.push_back(friend_obj);

                        added = true;
                    }
                    else
                    {
                        print_Errmsg();
                        continue;
                    }

                    if(added)
                    {
                        cout << "Successfully added new person." << endl;
                        break;
                    }
                }
            }
            else if(input.compare("2") == 0)
            {
                string userinput;
                int index;
                while(1)    // get index from user
                {
                    cout << "Enter index of person: ";
                    getline(cin, userinput);
                    if(parse_Index(userinput, index) == true) break;
                    else 
                    {
                        print_Errmsg();
                        continue;
                    }
                }

                if((index < 1) || (index > phoneBook.size())) // if index is out of bounds..
                {
                    cout << "Person does not exist!" << endl;
                }
                else
                {
                    Person *p = phoneBook[index-1];
                    delete p;
                    phoneBook.erase(phoneBook.begin() + (index - 1));
                    cout << "A person is successfully deleted from the Phone Book!" << endl;
                }
            }
            else if(input.compare("3") == 0)
            {
                cout << "Phone Book Print" << endl;
                vector<Person *>::iterator iter;
                int i = 1;

                for(iter = phoneBook.begin(); iter != phoneBook.end(); iter++)
                {
                    Person *p = *iter;
                    cout << i << ". "; 
                    p->print();
                    i++;
                }
            }
            else
            {
                print_Errmsg();
            }
        }


    }

    return 0;
}