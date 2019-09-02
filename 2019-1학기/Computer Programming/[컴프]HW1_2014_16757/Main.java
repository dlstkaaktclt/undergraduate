import java.util.ArrayList;
import java.util.Scanner;
import java.util.regex.*;
import java.util.Calendar;




class Person
{
    private String firstname;
    private String lastname;
    private String phone_number;

    public Person(String firstname, String lastname, String phone_number)
    {
        this.firstname = firstname;
        this.lastname = lastname;
        this.phone_number = phone_number;
    }
    public void setFirstName(String firstname) //set firstName string
    {
        this.firstname = firstname;
    }
    public String getFirstname() //return firstName
    {
        return this.firstname;
    }
    public void setLastName(String lastname) //set lastName string
    {
        this.lastname = lastname;
    }
    public String getLastName() //return lastName
    {
        return this.lastname;
    }
    public void setPhoneNumber(String phoneNumber) //set phonenumber 02-XXXX-XXXX or 010-XXXX-XXXX
    {
        this.phone_number = phoneNumber;
    }
    public String getPhoneNumber() //return phonenumber
    {
        return this.phone_number;
    }
    public void print() // print this object's infomation
    {
        String printmsg = String.format("%s %s_%s", this.getFirstname(), this.getLastName(), this.getPhoneNumber());
        System.out.println(printmsg);
    }


}

class Work extends Person
{
    private String team; //team that the person is in


    public Work(String firstname, String lastname, String phone_number, String team)
    {
        super(firstname, lastname, phone_number);
        this.team = team;
    }
    public void setTeam(String team) //set team
    {
        this.team = team;
    }
    public String getTeam() //return team
    {
        return this.team;
    }

    @Override
    public void print() //print the work object, using the parent function to get the attribute
    {
        String printmsg = String.format("%s %s_%s_%s", this.getFirstname(), this.getLastName(), this.getPhoneNumber(), this.getTeam());
        System.out.println(printmsg);
    }
}

class Friend extends Person
{
    private int age; //age of friend

    public Friend(String firstname, String lastname, String phone_number, int age)
    {
        super(firstname, lastname, phone_number);
        this.age = age;
    }
    public void setAge(int age) //set age
    {
        this.age = age;
    }
    public int getAge() //return age
    {
        return this.age;
    }

    @Override
    public void print() //print the friend object, using the parent function to get the attribute
    {
        String printmsg = String.format("%s %s_%s_%d", this.getFirstname(), this.getLastName(), this.getPhoneNumber(), this.getAge());
        System.out.println(printmsg);
    }
}

class Family extends Person
{
    private String birthday; //birthday of family


    public Family(String firstname, String lastname, String phone_number, String birthday)
    {
        super(firstname, lastname, phone_number);
        this.birthday = birthday;
    }

    public void setBirthday(String birthday) //set birthday (YYMMDD) string
    {
        this.birthday = birthday;
    }

    public String getBirthday() //return birthday
    {
        return this.birthday;
    }

    public int dDay() //calculate the date difference between the birthday and current time
    {
        Calendar calendar = Calendar.getInstance();
        String birth = this.getBirthday();

        int now_month = calendar.get(Calendar.MONTH) + 1; //calender.month starts 0,1,2....
        int now_day = calendar.get(Calendar.DAY_OF_MONTH);
        int now_day_num = this.get_num_day(now_month, now_day);

        int birth_month = Integer.parseInt(birth.substring(2,4));
        int birth_day = Integer.parseInt(birth.substring(4,6));
        int birth_day_num = this.get_num_day(birth_month, birth_day);

        int remainday = birth_day_num - now_day_num; // get difference of day.
        if(remainday < 0) remainday += 365; // if birthday is before than current day, count 365days more.

        return remainday;
    }

    @Override
    public void print() //print the family object, using the parent function to get the attribute
    {
        String printmsg = String.format("%s %s_%s_%s_%d", this.getFirstname(), this.getLastName(), this.getPhoneNumber(), this.getBirthday(), this.dDay());
        System.out.println(printmsg);
    }

    private int get_num_day(int month, int day) // get day from month and day
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
}



/* class about helper functions */
class Helper {

    private static Scanner sc = new Scanner(System.in);

    public void print_Prompt()
    {
        System.out.print("CP-2014-16757> ");
    }

    public void print_Errmsg()
    {
        //System.out.println("please insert correct input");
    }

    public void print_Topmenu_Choices()
    {
        System.out.println("Phone Book");
        System.out.println("1. Add person");
        System.out.println("2. Remove person");
        System.out.println("3. Print phone book");
    }

    public void print_Addmenu_Choices()
    {
        System.out.println("Select Type");
        System.out.println("1. Person");
        System.out.println("2. Work");
        System.out.println("3. Family");
        System.out.println("4. Friend");
    }

    public void get_Name_from_User(StringBuffer firstname, StringBuffer lastname)
    {
        String userinput;
        while(true)
        {
            System.out.print("Name: ");
            userinput = sc.nextLine();
            if(this.parse_Name(userinput, firstname, lastname) == true) break;
            else
            {
                this.print_Errmsg();
                continue;
            }
        }
    }

    public void get_Phone_Number_from_User(StringBuffer phone_number)
    {
        String userinput;
        while(true)
        {
            System.out.print("Phone_number: ");
            userinput = sc.nextLine();
            if(this.parse_Phone_Number(userinput, phone_number) == true) break;
            else
            {
                this.print_Errmsg();
                continue;
            }
        }
    }

    public void get_Team_from_User(StringBuffer team)
    {
        String userinput;
        while(true)
        {
            System.out.print("Team: ");
            userinput = sc.nextLine();
            if(this.parse_Team(userinput, team) == true) break;
            else
            {
                this.print_Errmsg();
                continue;
            }
        }
    }

    public void get_Birthday_from_User(StringBuffer birthday)
    {
        String userinput;
        while(true)
        {
            System.out.print("Birthday: ");
            userinput = sc.nextLine();
            if(this.parse_Birthday(userinput, birthday) == true) break;
            else
            {
                this.print_Errmsg();
                continue;
            }
        }
    }

    public int get_Age_from_User()
    {
        String userinput;
        IntHolder iholder = new IntHolder(0);
        while(true)
        {
            System.out.print("Age: ");
            userinput = sc.nextLine();
            if(parse_Age(userinput, iholder) == true) break;
            else
            {
                this.print_Errmsg();
                continue;
            }
        }
        return iholder.getVal();
    }

    public int get_Index_from_User()
    {
        String userinput;
        IntHolder iholder = new IntHolder(0);
        while(true)
        {
            System.out.print("Enter index of person: ");
            userinput = sc.nextLine();
            if(parse_Index(userinput, iholder) == true) break;
            else
            {
                this.print_Errmsg();
                continue;
            }
        }
        return iholder.getVal();
    }


    private boolean parse_Name(String input, StringBuffer firstname, StringBuffer lastname)
    {
        Pattern name_pattern = Pattern.compile("^([a-zA-Z]+) ([a-zA-Z]+)$");
        Matcher name_matcher = name_pattern.matcher(input);
        if(name_matcher.find())
        {
            firstname.setLength(0); // initiate firstname
            lastname.setLength(0); // initiate lastname

            firstname.append(name_matcher.group(1));
            lastname.append(name_matcher.group(2));

            return true;
        }
        return false;
    }

    private boolean parse_Phone_Number(String input, StringBuffer phone_number)
    {
        Pattern phone_pattern = Pattern.compile("^(02|010)-(\\d{4})-(\\d{4})$");
        Matcher phone_matcher = phone_pattern.matcher(input);
        if(phone_matcher.find())
        {
            phone_number.setLength(0); // initiate phone_number
            phone_number.append(phone_matcher.group(0));

            return true;
        }
        return false;
    }
    private boolean parse_Team(String input, StringBuffer team)
    {
        team.setLength(0); // initiate team
        team.append(input);
        return true;
        /*
        Pattern team_pattern = Pattern.compile("^[a-zA-Z0-9]+$");
        Matcher team_matcher = team_pattern.matcher(input);
        if(team_matcher.find())
        {
            team.setLength(0); // initiate phone_number
            team.append(team_matcher.group(0));

            return true;
        }
        return false;
        */
    }
    private boolean parse_Birthday(String input, StringBuffer birthday)
    {
        Pattern birth_pattern = Pattern.compile("^(\\d{2})(\\d{2})(\\d{2})$");
        Matcher birth_matcher = birth_pattern.matcher(input);
        if(birth_matcher.find())
        {
            String year = birth_matcher.group(1);
            String month = birth_matcher.group(2);
            String day = birth_matcher.group(3);

            int imonth = Integer.parseInt(month);
            int iday = Integer.parseInt(day);

            if(this.check_Month_Day(imonth,iday))
            {
                birthday.setLength(0);
                birthday.append(birth_matcher.group(0));
                return true;
            }
            else return false;
        }
        return false;
    }

    private boolean parse_Age(String input, IntHolder age)
    {
        Pattern age_pattern = Pattern.compile("^(\\d+)$");
        Matcher age_matcher = age_pattern.matcher(input);
        if(age_matcher.find())
        {
            int getnum = Integer.parseInt(age_matcher.group(0));
            if(getnum <= 0) return false;
            else
            {
                age.setVal(getnum);
                return true;
            }
        }
        else return false;
    }

    private boolean parse_Index(String input, IntHolder index)
    {
        Pattern index_pattern = Pattern.compile("^(\\d+)$");
        Matcher index_matcher = index_pattern.matcher(input);
        if(index_matcher.find())
        {
            int getnum = Integer.parseInt(index_matcher.group(0));
            if(getnum <= 0) return false; // if index is lower then 1
            index.setVal(getnum);
            return true;
        }
        else return false;
    }

    public boolean check_Month_Day(int month, int day)
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

    private class IntHolder // class for use int reference type (int &) in c++.
    {
        private int val;

        public IntHolder(int val)
        {
            this.val = val;
        }

        public void setVal(int val)
        {
            this.val = val;
        }

        public int getVal()
        {
            return this.val;
        }
    }
}


public class Main {

    public static void main(String[] args) {

        ArrayList<Person> phoneBook = new ArrayList<Person>();
        Helper helper = new Helper();
        Scanner sc = new Scanner(System.in);
        String input;

        /* while loop start to process user input */
        while(true)
        {
            helper.print_Prompt();
            input = sc.nextLine(); /* get user's input */

            if(input.compareTo("") == 0) // if user input is empty line, print topmenu choices.
            {
                helper.print_Topmenu_Choices();
            }
            else if(input.compareTo("exit") == 0) // if user input is exit, terminate program.
            {
                break;
            }
            else
            {
                if(input.compareTo("1") == 0) // if user input is 1, enter add process
                {
                    while(true)
                    {
                        boolean added = false;

                        helper.print_Addmenu_Choices();
                        helper.print_Prompt();
                        input = sc.nextLine(); /* get user's input */

                        if(input.compareTo("1") == 0) // add Person
                        {
                            StringBuffer firstname = new StringBuffer();
                            StringBuffer lastname = new StringBuffer();
                            StringBuffer phone_number = new StringBuffer();

                            helper.get_Name_from_User(firstname, lastname);
                            helper.get_Phone_Number_from_User(phone_number);

                            Person person = new Person(firstname.toString(), lastname.toString(), phone_number.toString());
                            phoneBook.add(person);

                            added = true;
                        }
                        else if(input.compareTo("2") == 0) // add Work
                        {
                            StringBuffer firstname = new StringBuffer();
                            StringBuffer lastname = new StringBuffer();
                            StringBuffer phone_number = new StringBuffer();
                            StringBuffer team = new StringBuffer();

                            helper.get_Name_from_User(firstname, lastname);
                            helper.get_Phone_Number_from_User(phone_number);
                            helper.get_Team_from_User(team);

                            Work work = new Work(firstname.toString(), lastname.toString(), phone_number.toString(), team.toString());
                            phoneBook.add(work);

                            added = true;
                        }
                        else if(input.compareTo("3") == 0) // add Family
                        {
                            StringBuffer firstname = new StringBuffer();
                            StringBuffer lastname = new StringBuffer();
                            StringBuffer phone_number = new StringBuffer();
                            StringBuffer birthday = new StringBuffer();

                            helper.get_Name_from_User(firstname, lastname);
                            helper.get_Phone_Number_from_User(phone_number);
                            helper.get_Birthday_from_User(birthday);

                            Family family = new Family(firstname.toString(), lastname.toString(), phone_number.toString(), birthday.toString());
                            phoneBook.add(family);

                            added = true;
                        }
                        else if(input.compareTo("4") == 0) // add Friend
                        {
                            StringBuffer firstname = new StringBuffer();
                            StringBuffer lastname = new StringBuffer();
                            StringBuffer phone_number = new StringBuffer();
                            int age;
                            helper.get_Name_from_User(firstname, lastname);
                            helper.get_Phone_Number_from_User(phone_number);
                            age = helper.get_Age_from_User();

                            Friend friend_obj = new Friend(firstname.toString(), lastname.toString(), phone_number.toString(), age);
                            phoneBook.add(friend_obj);

                            added = true;
                        }
                        else
                        {
                            helper.print_Errmsg();
                            continue;
                        }

                        if(added)
                        {
                            System.out.println("Successfully added new person.");
                            break;
                        }
                    }
                }
                else if(input.compareTo("2") == 0) // if user input is 2, enter delete process
                {
                    int index;
                    index = helper.get_Index_from_User();

                    if((index < 1) || (index > phoneBook.size())) // if index is out of bounds..
                    {
                        System.out.println("Person does not exist!");
                    }
                    else
                    {
                        phoneBook.remove(index - 1);

                        System.out.println("A person is successfully deleted from the Phone Book!");
                    }
                }
                else if(input.compareTo("3") == 0) // if user input is 3, enter print process
                {
                    System.out.println("Phone Book Print");
                    int i = 1;

                    for(Person p : phoneBook)
                    {
                        System.out.print(i);
                        System.out.print(".");
                        p.print();
                        i++;
                    }

                }
                else
                {
                    helper.print_Errmsg();
                }
            }


        }

    }






}
