#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <regex>
#include <string>
#include <iterator>
#include <exception>
#include <random>
#include <ctime>

using namespace std;

int get_random_number(int min, int max) // get number min~max. include min value and max value.
{
    mt19937 engine((unsigned int)time(NULL));              
    uniform_int_distribution<int> distribution(min, max);      

    return distribution(engine);
}

void print_prompt()
{
    cout << "CP-2014-16757> ";
}

void print_nextmove()
{
    cout << "Next Move? (1,2)" << endl;
}

enum Vehicle_type // enum for vehicle_type
{
TYPE_CAR,
TYPE_AIRPLANE,
TYPE_SUBMARINE
};

enum Stage_type // enum for stage_type
{
STAGE_ROAD,
STAGE_SKY,
STAGE_OCEAN,
STAGE_X,
STAGE_Y
};


class CriticalFailException : public runtime_error  // for debug. this means data structure is corrupted or not woking intened.
{
public:
    CriticalFailException(string message = "Unknown error");
};

CriticalFailException::CriticalFailException(string message) : runtime_error(message)
{}


class EnvironmentFailException : public runtime_error           // exeception that means environment failed.
{
public:
    EnvironmentFailException(string reason = "Unknown");
};

EnvironmentFailException::EnvironmentFailException(string reason) : runtime_error("Make Environment failed by " + reason + "reason.")
{}


class VehicleException : public runtime_error
{
public:
    VehicleException(string reason = "");
};

VehicleException::VehicleException(string reason) : runtime_error(reason)
{}


class VehicleChangedException : public VehicleException // vehicle mode changed, throw this exception
{
public:
    VehicleChangedException(Vehicle_type before, Vehicle_type after);
    Vehicle_type get_vehicle_before_type();
    Vehicle_type get_vehicle_after_type();
    
private:
    Vehicle_type before_type;
    Vehicle_type after_type;
};

VehicleChangedException::VehicleChangedException(Vehicle_type before, Vehicle_type after) : VehicleException("vehicle mode changed")
{
    before_type = before;
    after_type = after;
}

Vehicle_type VehicleChangedException::get_vehicle_before_type()
{
    return before_type;
}

Vehicle_type VehicleChangedException::get_vehicle_after_type()
{
    return after_type;
}


class VehicleFinishedException : public VehicleException // finished normally (arrived), throw this exception
{
public:
    VehicleFinishedException();
};

VehicleFinishedException::VehicleFinishedException() : VehicleException("run finished normally.")
{}


class VehicleAbortException : public VehicleException // finished abnormally, throw this exception
{
public:
    VehicleAbortException(Vehicle_type type, string reason);
    Vehicle_type get_vehicle_type();
private:
    Vehicle_type v_type;
};

VehicleAbortException::VehicleAbortException(Vehicle_type type, string reason) : VehicleException("by " + reason + " vehicle stoped.")
{
    v_type = type;
}

Vehicle_type VehicleAbortException::get_vehicle_type()
{
    return v_type;
}


class VehicleEnergyAbortException : public VehicleAbortException // if energy below zero, throw this exception
{
public:
    VehicleEnergyAbortException(Vehicle_type type);
};

VehicleEnergyAbortException::VehicleEnergyAbortException(Vehicle_type type) : VehicleAbortException(type, "Energy depleted")
{}


class VehicleOxygenAbortException : public VehicleAbortException // if oxygen below zero, throw this exception
{
public:
    VehicleOxygenAbortException(Vehicle_type type);
};

VehicleOxygenAbortException::VehicleOxygenAbortException(Vehicle_type type) : VehicleAbortException(type, "Oxygen depleted")
{}


class VehicleDestroyAbortException : public VehicleAbortException // if vehicle destoryed, throw this exception
{
public:
    VehicleDestroyAbortException(Vehicle_type type);
};

VehicleDestroyAbortException::VehicleDestroyAbortException(Vehicle_type type) : VehicleAbortException(type, "Vehicle destroyed") 
{}


class Stage // class for stage. road, sky, ocean, X, Y
{
public:
    Stage(Stage_type input_stage_type);
    virtual ~Stage();
    Stage_type get_stage_type();
    virtual void print();

private:
    Stage_type stage_type;
};

Stage::Stage(Stage_type input_stage_type)
{
    stage_type = input_stage_type;
}

Stage::~Stage() {}

Stage_type Stage::get_stage_type()
{
    return stage_type;
}

void Stage::print() {}


class Road : public Stage
{
public:
    Road(int input_length, int input_temperature, int input_humidity);
    int get_temperature();
    int get_humidity();
    int get_length();
    int get_unit();
    virtual void print() override;

private:
    int temperature;
    int humidity;
    int length;
    int length_unit;
};

Road::Road(int input_length, int input_temperature, int input_humidity) : Stage::Stage(STAGE_ROAD)
{
    temperature = input_temperature;
    humidity = input_humidity;
    length = input_length;
    length_unit = 50;
}

int Road::get_temperature()
{
    return temperature;
}

int Road::get_humidity()
{
    return humidity;
}

int Road::get_length()
{
    return length;
}

int Road::get_unit()
{
    return length / length_unit;
}

void Road::print()
{
    cout << "Road" << endl;
    cout << "Temperature : " << get_temperature() << endl;
    cout << "Humidity : " << get_humidity() << endl;
    cout << "Length : " << get_length() << endl;
    cout << "Unit : " << get_unit() << endl;
}



class Sky : public Stage
{
public:
    Sky(int input_length, int input_temperature, int input_humidity, int input_altitude, int input_air_density);
    int get_temperature();
    int get_humidity();
    int get_altitude();
    int get_air_density();
    int get_length();
    int get_unit();
    virtual void print() override;

private:
    int temperature;
    int humidity;
    int altitude;
    int air_density;
    int length;
    int length_unit;
};

Sky::Sky(int input_length, int input_temperature, int input_humidity, int input_altitude, int input_air_density) : Stage::Stage(STAGE_SKY)
{
    temperature = input_temperature;
    humidity = input_humidity;
    altitude = input_altitude;
    air_density = input_air_density;
    length = input_length;
    length_unit = 1000;
}

int Sky::get_temperature()
{
    return temperature;
}

int Sky::get_humidity()
{
    return humidity;
}

int Sky::get_altitude()
{
    return altitude;
}

int Sky::get_air_density()
{
    return air_density;
}

int Sky::get_length()
{
    return length;
}

int Sky::get_unit()
{
    return length / length_unit;
}

void Sky::print()
{
    cout << "Sky" << endl;
    cout << "Temperature : " << get_temperature() << endl;
    cout << "Humidity : " << get_humidity() << endl;
    cout << "Altitude : " << get_altitude() << endl;
    cout << "Air_Density : " << get_air_density() << endl;
    cout << "Length : " << get_length() << endl;
    cout << "Unit : " << get_unit() << endl;
}

class Ocean : public Stage
{
public:
    Ocean(int input_length, int input_temperature, int input_depth, int input_waterflow);
    int get_temperature();
    int get_depth();
    int get_waterflow();
    int get_length();
    int get_unit();
    virtual void print() override;

private:
    int temperature;
    int depth;
    int waterflow;
    int length;
    int length_unit;
};

Ocean::Ocean(int input_length, int input_temperature, int input_depth, int input_waterflow) : Stage::Stage(STAGE_OCEAN)
{
    temperature = input_temperature;
    depth = input_depth;
    waterflow = input_waterflow;
    length = input_length;
    length_unit = 10;
}

int Ocean::get_temperature()
{
    return temperature;
}

int Ocean::get_depth()
{
    return depth;
}

int Ocean::get_waterflow()
{
    return waterflow;
}

int Ocean::get_length()
{
    return length;
}

int Ocean::get_unit()
{
    return length / length_unit;
}

void Ocean::print()
{
    cout << "Ocean" << endl;
    cout << "Temperature : " << get_temperature() << endl;
    cout << "Depth : " << get_depth() << endl;
    cout << "Waterflow : " << get_waterflow() << endl;
    cout << "Length : " << get_length() << endl;
    cout << "Unit : " << get_unit() << endl;
}


class X_Stage : public Stage
{
public:
    X_Stage() : Stage(STAGE_X)
    {}
};

class Y_Stage : public Stage
{
public:
    Y_Stage() : Stage(STAGE_Y)
    {}
};


class Environment   // environment has whole stages, and current location to print.
{
public:
    Environment(const string &input_str, bool extra_mode); // if extra_mode is true, for extra, other value, for normal.
    Environment(const Environment &other);       //for deep copy
    ~Environment();
    void onestep_go();   // one step progress at environment.
    Stage *get_next_stage(); // get next stage of current location. if reach end, return NULL.
    bool is_next_stage_change(); // if next stage should be changed after progress, return true else return false.
    void print_map();

private:
    vector<Stage *> stages;  // save maps.
    int current_unit_location;   // current unit location. 0 means initial state. if current_unit_location == total_unit_length, then run ended.
    int next_stage_location;  // next stage location. it means stage index used for progess. (next step). if next_stage_location == total_stage_length, then run ended.
    int total_unit_length;
    int total_stage_length;

    bool is_stage_XY(Stage *stage); // if stage is X or Y, return true else false
    int get_stage_unit(Stage *stage);
};

Environment::Environment(const string &input_str, bool extra_mode) // if extra_mode is true, for extra, other value, for normal.
{
    int i;
    int total_unit;
    char str[2500];
    char * pch;
    string innerstr;
    const char* delimiter = "[], \t\r\n";
    str[0] = '\0';
    Stage *item;

    strcpy(str, input_str.c_str());

    /* parse file */

    /* ifstream openFile(filePath);
    if(openFile.is_open())  //if file is opened..
    {
        while(openFile.peek() != EOF)  // while reach file's EOF
        {
            openFile.getline(str,2400);     //get line and parse.
            */
    pch = strtok (str,delimiter);
    while (pch != NULL)
    {
        if(*pch == 'R') // if stage is road
        {
            innerstr=string(pch);
            regex rRoad("R(\\d+)T(\\d+)H(\\d+)");
            smatch m;
            if(regex_match(innerstr, m, rRoad))
            {
                int length = stoi(m[1]);
                int temperature = stoi(m[2]);
                int humidity = stoi(m[3]);
                item = new Road(length, temperature, humidity);
                stages.push_back(item);
            }
            else
            {
                throw EnvironmentFailException("stage format not matched");
            }
            
        }
        else if(*pch == 'S') // if stage is sky
        {
            innerstr=string(pch);
            regex rRoad("S(\\d+)T(\\d+)H(\\d+)A(\\d+)D(\\d+)");
            smatch m;
            if(regex_match(innerstr, m, rRoad))
            {
                int length = stoi(m[1]);
                int temperature = stoi(m[2]);
                int humidity = stoi(m[3]);
                int altitude = stoi(m[4]);
                int air_density = stoi(m[5]);
                item = new Sky(length, temperature, humidity, altitude, air_density);
                stages.push_back(item);
            }
            else
            {
                throw EnvironmentFailException("stage format not matched");
            }
        }
        else if(*pch == 'O') // if stage is ocean
        {
            innerstr=string(pch);
            regex rRoad("O(\\d+)T(\\d+)D(\\d+)W(\\d+)");
            smatch m;
            if(regex_match(innerstr, m, rRoad))
            {
                int length = stoi(m[1]);
                int temperature = stoi(m[2]);
                int depth = stoi(m[3]);
                int waterflow = stoi(m[4]);
                item = new Ocean(length, temperature, depth, waterflow);
                stages.push_back(item);
            }
            else
            {
                throw EnvironmentFailException("stage format not matched");
            }
        }
        else if(*pch == 'X') // if stage is X
        { 
            if(extra_mode == true) // for extra
            {
                item = new X_Stage();
                stages.push_back(item);
            }
        }
        else if(*pch == 'Y') // if stage is Y
        {
            if(extra_mode == true) // for extra
            {
                item = new Y_Stage();
                stages.push_back(item);
            }
        }
        else
        {
            throw EnvironmentFailException("invalid stage in input file");
        }
        
        pch = strtok (NULL,delimiter);
    }
    /*
        }
        openFile.close();
    } 
    else    //else throw exception.
    {
        throw EnvironmentFailException("file not opened");
    }
    */

    //parse complete.

    /* get map size */

    total_unit = 0;
    for(i = 0; i < stages.size(); i++)
    {
        total_unit += get_stage_unit(stages[i]);
    }

    total_unit_length = total_unit;
    total_stage_length = stages.size();
    current_unit_location = 0;
    next_stage_location = 0;
}

Environment::Environment(const Environment &other)       //for deep copy
{
    Stage *item;
    int i;
    for(i = 0; i < other.stages.size(); i++)
    {
        Road *road;
        Sky *sky;
        Ocean *ocean;
        X_Stage *x_stage;
        Y_Stage *y_stage;
        
        if((road = dynamic_cast<Road *>(stages[i])) != NULL)
        {
            item = new Road(*road);
            stages.push_back(item);
        }
        else if((sky = dynamic_cast<Sky *>(stages[i])) != NULL)
        {
            item = new Sky(*sky);
            stages.push_back(item);
        }
        else if((ocean = dynamic_cast<Ocean *>(stages[i])) != NULL)
        {
            item = new Ocean(*ocean);
            stages.push_back(item);
        }
        else if((x_stage = dynamic_cast<X_Stage *>(stages[i])) != NULL)
        {
            item = new X_Stage(*x_stage);
            stages.push_back(item);
        }
        else if((y_stage = dynamic_cast<Y_Stage *>(stages[i])) != NULL)
        {
            item = new Y_Stage(*y_stage);
            stages.push_back(item);
        }
        else
        {
            throw EnvironmentFailException("pointer is NULL! Critical Error!");
        }
        
    }
    // vector deep copy complete.

    current_unit_location = other.current_unit_location;
    next_stage_location = other.next_stage_location;
    total_unit_length = other.total_unit_length;
    total_stage_length = other.total_stage_length;
}

Environment::~Environment()
{
    int i;
    for(i = 0; i < stages.size(); i++)  // deallocate dynamically allocated memories.
    {
        delete stages[i];
    }
}

void Environment::onestep_go()   // one step progress at environment.
{
    int i;
    int total_unit = 0; 

    /* if not reached end position, progress one step */

    if(next_stage_location < total_stage_length)
    {
        // get sum of units to next_stage_location.
        for(i = 0; i <= next_stage_location; i++)    
        {
            total_unit += get_stage_unit(stages[i]);
        }

        if(current_unit_location < total_unit - 1)  // if sum of unit is less than next_stage_location, progress is not change stage. 
        {
            current_unit_location++;
        }
        else if(current_unit_location == total_unit - 1) // change to next stage.
        {
            current_unit_location++;
            next_stage_location++;
        }
        else    // this means next_stage is X or Y. just increase stage.
        {
            next_stage_location++;
        }
    }
}

Stage *Environment::get_next_stage() // get next stage of current location. if reach end, return NULL.
{
    if(next_stage_location < total_stage_length)
    {
        return stages[next_stage_location];
    }
    else return NULL;
}

bool Environment::is_next_stage_change() // if next stage should be changed after progress, return true else return false.
{
    int i;
    int total_unit = 0; 

    if(next_stage_location < total_stage_length)
    {
        for(i = 0; i <= next_stage_location; i++)    // get sum of units to next_stage_location.
        {
            total_unit += get_stage_unit(stages[i]);
        }

        if(current_unit_location < total_unit - 1)  // if sum of unit is less than next_stage_location - 1, progress is not change stage. 
        {
            return false;
        }
        else if(current_unit_location == total_unit - 1) // change to next stage.
        {
            return true;
        }
        else    // this means next_stage is X or Y. change stage.
        {
            return true;
        }
    }
}

void Environment::print_map()
{
    int i;
    string progress = "";
    for(i = 0; i < stages.size(); i++)      // make graphic
    {
        Road *road;
        Sky *sky;
        Ocean *ocean;
        
        if((road = dynamic_cast<Road *>(stages[i])) != NULL)
        {
            progress.append(road->get_unit(), '=');
        }
        else if((sky = dynamic_cast<Sky *>(stages[i])) != NULL)
        {
            progress.append(sky->get_unit(), '^');
        }
        else if((ocean = dynamic_cast<Ocean *>(stages[i])) != NULL)
        {
            progress.append(ocean->get_unit(), '~');
        }
    }

    progress.insert(current_unit_location, "@"); // insert vehicle
    progress.insert(0,"|");
    progress.append(1,'|');
    cout << progress << endl;
}

bool Environment::is_stage_XY(Stage *stage) // if stage is X or Y, return true else false
{
    Stage_type type = stage->get_stage_type();
    if(type == STAGE_X || type == STAGE_Y) return true;
    else return false;
}

int Environment::get_stage_unit(Stage *stage)
{
    Road *road;
    Sky *sky;
    Ocean *ocean;
    
    if((road = dynamic_cast<Road *>(stage)) != NULL)
    {
        return road->get_unit();
    }
    else if((sky = dynamic_cast<Sky *>(stage)) != NULL)
    {
        return sky->get_unit();
    }
    else if((ocean = dynamic_cast<Ocean *>(stage)) != NULL)
    {
        return ocean->get_unit();
    }
    else return 0;
}

class Blackbox
{
public:
    Blackbox();
    Blackbox(const Blackbox &other);
    virtual ~Blackbox();
    void Record(Vehicle_type mode, int energy, int oxygen, int speed);
    void print();

private:
    vector<Vehicle_type> mode_vector;
    vector<int> energy_vector;
    vector<int> oxygen_vector;
    vector<int> speed_vector;

    string vehicle_type_to_string(Vehicle_type mode);
};

Blackbox::Blackbox()
{}

Blackbox::Blackbox(const Blackbox &other)
{
    mode_vector = vector<Vehicle_type>(other.mode_vector);
    energy_vector = vector<int>(other.energy_vector);
    oxygen_vector = vector<int>(other.oxygen_vector);
    speed_vector = vector<int>(other.speed_vector);
}

Blackbox::~Blackbox()
{}

void Blackbox::Record(Vehicle_type mode, int energy, int oxygen, int speed)
{
    mode_vector.push_back(mode);
    energy_vector.push_back(energy);
    oxygen_vector.push_back(oxygen);
    speed_vector.push_back(speed);
}

void Blackbox::print()
{
    int i;

    cout << "Blackbox:" << endl;
    
    /* print mode */
    cout << "Mode: ";
    for(i = 0; i < mode_vector.size() ; i++)
    {
        if(i != (mode_vector.size() - 1))
        {
            cout << vehicle_type_to_string(mode_vector[i]) << " > ";
        }
        else
        {
            cout << vehicle_type_to_string(mode_vector[i]);
        }
    }
    cout << endl;

    /* print energy */
    cout << "Energy Level: ";
    for(i = 0; i < energy_vector.size() ; i++)
    {
        if(i != (energy_vector.size() - 1))
        {
            cout << energy_vector[i] << " > ";
        }
        else
        {
            cout << energy_vector[i];            
        }
    }
    cout << endl;

    /* print oxygen */
    cout << "Oxygen Level: ";
    for(i = 0; i < oxygen_vector.size() ; i++)
    {
        if(i != (oxygen_vector.size() - 1))
        {
            cout << oxygen_vector[i] << " > ";
        }
        else
        {
            cout << oxygen_vector[i];            
        }
    }
    cout << endl;

    /* print speed */
    cout << "Speed: ";
    for(i = 0; i < speed_vector.size() ; i++)
    {
        if(i != (speed_vector.size() - 1))
        {
            cout << speed_vector[i] << " > ";
        }
        else
        {
            cout << speed_vector[i];            
        }
    }
    cout << endl;
}

string Blackbox::vehicle_type_to_string(Vehicle_type mode)
{
    if(mode == TYPE_CAR) return "Car";
    else if(mode == TYPE_AIRPLANE) return "Airplane";
    else if(mode == TYPE_SUBMARINE) return "Submarine";
    else return "";
}


class Vehicle
{
public:
    Vehicle(int input_speed, int input_energy, int input_distance, int input_oxygen_level, Vehicle_type input_mode, Blackbox *input_blackbox, Environment *input_env, bool input_solar_panel_defect);
    Vehicle(const Vehicle &other);
    virtual ~Vehicle();
    virtual void unit_move() throw(VehicleException);
    virtual void chunk_move() throw(VehicleException);
    virtual void print_status();
    void print_final_status();
    void throw_exception_vehicle_status_failed() throw(VehicleEnergyAbortException, VehicleOxygenAbortException); // throw exception when vehicle status failed.
    int get_speed();
    int get_energy();
    int get_distance();
    int get_oxygen_level();
    Vehicle_type get_Vehicle_type();
    Blackbox *get_blackbox();
    Environment *get_environment();
    void set_speed(int input_speed);
    void set_energy(int input_energy);
    void set_vehicle_type(Vehicle_type input_mode);
    void set_distance(int input_distance);
    void set_oxygen_level(int input_oxygen_level);
    bool is_solar_panel_defect();
    void defect_solar_panel();
    
private:
    int speed;
    int energy;
    int distance;
    int oxygen_level;
    bool solar_panel_defect_extra; // for extra credit. if it is true, then solar panel doesn't work.
    Vehicle_type mode;
    Blackbox *blackbox;
    Environment *env;
};

Vehicle::Vehicle(int input_speed, int input_energy, int input_distance, int input_oxygen_level, Vehicle_type input_mode, Blackbox *input_blackbox, Environment *input_env, bool input_solar_panel_defect) : blackbox(input_blackbox), env(input_env)
{
    set_speed(input_speed);
    set_energy(input_energy);
    set_distance(input_distance);
    set_oxygen_level(input_oxygen_level);
    mode = input_mode;
    solar_panel_defect_extra = input_solar_panel_defect;
}

Vehicle::Vehicle(const Vehicle &other)
{
    speed = other.speed;
    energy = other.energy;
    distance = other.distance;
    oxygen_level = other.oxygen_level;
    solar_panel_defect_extra = other.solar_panel_defect_extra;
    mode = other.mode;
    blackbox = other.blackbox;
    env = other.env;
}

Vehicle::~Vehicle()
{}

void Vehicle::unit_move() throw(VehicleException)
{}

void Vehicle::chunk_move() throw(VehicleException)
{}

void Vehicle::print_status()
{}

void Vehicle::print_final_status()
{
    cout << "Final Status:" << endl;
    cout << "Distance: " << get_distance() << " km" << endl;
    cout << "Energy: " << get_energy() << endl;
    cout << "Oxygen Level: " << get_oxygen_level() << endl;
}

void Vehicle::throw_exception_vehicle_status_failed() throw(VehicleEnergyAbortException, VehicleOxygenAbortException) // throw exception when vehicle status failed.
{
    if(Vehicle::get_energy() <= 0)
    {
        throw VehicleEnergyAbortException(Vehicle::get_Vehicle_type());
    }
    else if(Vehicle::get_oxygen_level() <= 0)
    {
        throw VehicleOxygenAbortException(Vehicle::get_Vehicle_type());
    }
}

int Vehicle::get_speed()
{
    return speed;
}

int Vehicle::get_energy()
{
    return energy;
}

int Vehicle::get_distance()
{
    return distance;
}

int Vehicle::get_oxygen_level()
{
    return oxygen_level;
}

Vehicle_type Vehicle::get_Vehicle_type()
{
    return mode;
}

Blackbox *Vehicle::get_blackbox()
{
    return blackbox;
}

Environment *Vehicle::get_environment()
{
    return env;
}

void Vehicle::set_speed(int input_speed)  
{
    if(input_speed < 0)
    {
        input_speed = 0;
    }
    speed = input_speed;
}

void Vehicle::set_energy(int input_energy)   
{
    if(input_energy > 1000)
    {
        input_energy = 1000;
    }
    else if(input_energy < 0)
    {
        input_energy = 0;
    }

    energy = input_energy;
}

void Vehicle::set_vehicle_type(Vehicle_type input_mode)
{
    mode = input_mode;
}

void Vehicle::set_distance(int input_distance)
{
    if(input_distance < 0)
    {
        input_distance = 0;
    }
    distance = input_distance;
}

void Vehicle::set_oxygen_level(int input_oxygen_level)
{
    if(input_oxygen_level > 100)    // limit maximum.
    {
        input_oxygen_level = 100;
    }
    else if(input_oxygen_level < 0)
    {
        input_oxygen_level = 0;
    }
    oxygen_level = input_oxygen_level;
}

bool Vehicle::is_solar_panel_defect()
{
    return solar_panel_defect_extra;
}

void Vehicle::defect_solar_panel()
{
    solar_panel_defect_extra = true;
}

class Car : public Vehicle
{
public:
    Car(Blackbox *input_blackbox, Environment *input_env); // initial constructor.
    Car(const Vehicle &other);
    int get_temperature();
    int get_humidity();
    void set_temperature(int input_temperature);
    void set_humidity(int input_humidity);
    virtual void unit_move() throw(VehicleException) override;  // if vehicle status have problem, this function throws exception
    virtual void chunk_move() throw(VehicleException) override; // if vehicle status have problem, this function throws exception
    void print_distance(int distance);
    virtual void print_status() override;
    void solar_panel_recharge();

private:
    int temperature;
    int humidity;
    int energy_loss;

    void initialize_car_by_next_stage();
    void set_car_speed();
    void set_energy_loss();
    void exception_handle_by_distance(int distance); // function to handle exception uniformly.
    void move_one_unit() throw(VehicleException); // this function actually move one_unit, and if vehicle status have problem, throw exception.
    void env_progress_onestep();  // this function progress one unit at environment. if vehicle status is not normal, throw exception.
    void check_vehicle_status();
};

// initial constructor.
Car::Car(Blackbox *input_blackbox, Environment *input_env) : Vehicle(80, 1000, 0, 100, TYPE_CAR, input_blackbox, input_env, false)
{
    initialize_car_by_next_stage();
}

Car::Car(const Vehicle &other) : Vehicle(other)
{
    initialize_car_by_next_stage();
    //initialized by Vehicle

    set_oxygen_level(100);
    solar_panel_recharge();
    // transform effect
}

int Car::get_temperature()
{
    return temperature;
}

int Car::get_humidity()
{
    return humidity;
}

void Car::set_temperature(int input_temperature)
{
    temperature = input_temperature;
}

void Car::set_humidity(int input_humidity)
{
    humidity = input_humidity;
}

void Car::unit_move() throw(VehicleException)
{
    int distance_unit = 50;
    try
    {
        move_one_unit();
        print_distance(distance_unit);
    }
    catch(VehicleChangedException &e) // vehicle changed is normally proceeded.
    {
        exception_handle_by_distance(distance_unit);  
        throw e;
    }
    catch(VehicleFinishedException &e)
    {
        exception_handle_by_distance(distance_unit);
        throw e;
    }
    catch(VehicleEnergyAbortException &e)
    {
        exception_handle_by_distance(distance_unit);
        throw e;
    }
    catch(VehicleOxygenAbortException &e)
    {
        exception_handle_by_distance(distance_unit);
        throw e;
    }
    catch(VehicleDestroyAbortException &e)  // destory vehicle
    {
        exception_handle_by_distance(distance_unit);
        throw e;
    }
}


void Car::chunk_move() throw(VehicleException)
{
    int distance_unit = 50;
    int i = 0;
    try
    {
        while(1)
        {
            move_one_unit();
            i++;
        }
    }
    catch(VehicleChangedException &e) // vehicle changed is normally proceeded.
    {
        exception_handle_by_distance(i*distance_unit + distance_unit);
        throw e;
    }
    catch(VehicleFinishedException &e)
    {
        exception_handle_by_distance(i*distance_unit + distance_unit);
        throw e;
    }
    catch(VehicleEnergyAbortException &e)
    {
        exception_handle_by_distance(i*distance_unit + distance_unit);
        throw e;
    }
    catch(VehicleOxygenAbortException &e)
    {
        exception_handle_by_distance(i*distance_unit + distance_unit);
        throw e;
    }
    catch(VehicleDestroyAbortException &e)
    {
        exception_handle_by_distance(i*distance_unit + distance_unit);
        throw e;
    }
}

void Car::print_distance(int distance)
{   
    cout << "Successfully moved to next " << distance << " km" << endl;
}

void Car::print_status()
{
    cout << "Currrent Status: " << "Car" << endl;
    cout << "Distance: " << Vehicle::get_distance() << " km" << endl;
    cout << "Speed: " << Vehicle::get_speed() << " km/hr" << endl;
    cout << "Energy: " << Vehicle::get_energy() << endl;
    cout << "Temperature: " << get_temperature() << " C" << endl;
    cout << "Humidity: " << get_humidity() << endl;
}

void Car::solar_panel_recharge()
{
    if(Vehicle::is_solar_panel_defect() == false)
    {
        if(humidity < 50)
        {
            Vehicle::set_energy(Vehicle::get_energy() + 200);
        }
    }
}

void Car::initialize_car_by_next_stage()
{
    Vehicle::set_vehicle_type(TYPE_CAR);
    Road *next_stage = dynamic_cast<Road *>(Vehicle::get_environment()->get_next_stage());
    if(next_stage != NULL)
    {
        set_humidity(next_stage->get_humidity());
        set_temperature(next_stage->get_temperature());
        set_energy_loss();
        set_car_speed();
    }
    else
    {
        throw CriticalFailException("Car's next stage is not Road!!!!");       //if next stage is not road..
    }
}

void Car::set_car_speed()
{
    int speed = 80; //default speed

    Vehicle::set_speed(speed);
}

void Car::set_energy_loss()
{
    int total_loss = 0;

    // energy loss set by temperature
    if(temperature >= 40) // temperature >= 40
    {
        total_loss += 10;
    }
    else if(temperature > 0)   // 0 < temperature < 40
    {
        total_loss += 5;
    }
    else if(temperature == 0)
    {
        total_loss += 8;
    }

    // energy loss set by humidity
    if(humidity < 50)
    {
        total_loss += 5;   
    }
    else        // humidity >= 50
    {
        total_loss += 8;
    }
    
    energy_loss = total_loss;
}

void Car::exception_handle_by_distance(int distance) // function to handle exception uniformly.
{
    print_distance(distance);
    Vehicle::get_blackbox()->Record(TYPE_CAR, Vehicle::get_energy(), Vehicle::get_oxygen_level(), Vehicle::get_speed());
}

void Car::move_one_unit() throw(VehicleException)
{
    Environment *env = Vehicle::get_environment();
    Stage *next_stage = env->get_next_stage();
    if (next_stage != NULL)     //if not finished..
    {
        
        Road *road;
        Sky *sky;
        Ocean *ocean;
        X_Stage *x_stage;
        Y_Stage *y_stage;
        
        if((road = dynamic_cast<Road *>(next_stage)) != NULL)  // move one step
        {
            Vehicle::set_energy(Vehicle::get_energy() - energy_loss);
            Vehicle::set_distance(Vehicle::get_distance() + 50);
            env_progress_onestep();
        }
        // one step progress.
        
        next_stage = env->get_next_stage();   // process if next stage is x or y

        if((x_stage = dynamic_cast<X_Stage *>(next_stage)) != NULL)
        {
            int random_num = get_random_number(0,9); // get value from 0 to 9.
            if(random_num < 2) // 20% chance destroyed.
            {
                throw VehicleDestroyAbortException(Vehicle::get_Vehicle_type());
            }
            else    //energy loss
            {
                Vehicle::set_energy(Vehicle::get_energy() - 100);
                env_progress_onestep();
            }
        }
        else if((y_stage = dynamic_cast<Y_Stage *>(next_stage)) != NULL)
        {
            int random_num = get_random_number(0,99); // get value from 0 to 99.
            if(random_num < 35) // 35% chance
            {
                throw VehicleDestroyAbortException(Vehicle::get_Vehicle_type());
            }
            else
            {
                random_num = get_random_number(0,1);
                if(random_num < 1) // 50% chance
                {
                    Vehicle::defect_solar_panel();
                }
                env_progress_onestep();
            }
        }
        // X or Y case processed.

        next_stage = env->get_next_stage();     // need to transform if next stage is not road
        if((sky = dynamic_cast<Sky *>(next_stage)) != NULL)
        {
            throw VehicleChangedException(TYPE_CAR, TYPE_AIRPLANE);
        }
        else if((ocean = dynamic_cast<Ocean *>(next_stage)) != NULL)
        {
            throw VehicleChangedException(TYPE_CAR, TYPE_SUBMARINE);
        }
    }
    else
    {
        throw VehicleFinishedException();
    }
}

void Car::env_progress_onestep()  // this function progress one unit at environment. if vehicle status is not normal, throw exception.
{
    Environment *env = Vehicle::get_environment();
    env->onestep_go();
    check_vehicle_status(); // if current state couldn't progress.. raise error. 
}

void Car::check_vehicle_status()
{
    Environment *env = Vehicle::get_environment();
    if(env->get_next_stage() == NULL) throw VehicleFinishedException();
    else Vehicle::throw_exception_vehicle_status_failed();
}


class AirPlane : public Vehicle
{
public:
    AirPlane(const Vehicle &other);
    virtual void unit_move() throw(VehicleException) override;  // if vehicle status have problem, this function throws exception
    virtual void chunk_move() throw(VehicleException) override; // if vehicle status have problem, this function throws exception
    void print_distance(int distance);
    virtual void print_status() override;
    int get_temperature();
    int get_humidity();
    int get_altitude();
    int get_air_density();
    void set_temperature(int input_temperature);
    void set_humidity(int input_humidity);
    void set_altitude(int input_altitude);
    void set_air_density(int input_air_density);

private:
    int temperature;
    int humidity;    
    int altitude;
    int air_density;
    int energy_loss;
    int oxygen_loss;

    void initialize_airplane_by_next_stage();
    void set_airplane_speed();
    void set_oxygen_loss();
    void set_energy_loss();
    void exception_handle_by_distance(int distance); // function to handle exception uniformly.
    void move_one_unit() throw(VehicleException); // this function actually move one_unit, and if vehicle status have problem, throw exception.
    void env_progress_onestep();  // this function progress one unit at environment. if vehicle status is not normal, throw exception.
    void check_vehicle_status();
};

AirPlane::AirPlane(const Vehicle &other) : Vehicle(other)
{
    initialize_airplane_by_next_stage();
    // set airplane
}

void AirPlane::unit_move() throw(VehicleException)
{
    int distance_unit = 1000;
    try
    {
        move_one_unit();
        print_distance(distance_unit);
    }
    catch(VehicleChangedException &e) // vehicle changed is normally proceeded.
    {
        exception_handle_by_distance(distance_unit);
        throw e;
    }
    catch(VehicleFinishedException &e)
    {
        exception_handle_by_distance(distance_unit);
        throw e;
    }
    catch(VehicleEnergyAbortException &e)
    {
        exception_handle_by_distance(distance_unit);
        throw e;
    }
    catch(VehicleOxygenAbortException &e)
    {
        exception_handle_by_distance(distance_unit);
        throw e;
    }
    catch(VehicleDestroyAbortException &e)
    {
        exception_handle_by_distance(distance_unit);
        throw e;
    }
}

void AirPlane::chunk_move() throw(VehicleException)
{
    int distance_unit = 1000;
    int i = 0;
    try
    {
        while(1)
        {
            move_one_unit();
            i++;
        }
    }
    catch(VehicleChangedException &e) // vehicle changed is normally proceeded.
    {
        exception_handle_by_distance(i * distance_unit + distance_unit);
        throw e;
    }
    catch(VehicleFinishedException &e)
    {
        exception_handle_by_distance(i * distance_unit + distance_unit);
        throw e;
    }
    catch(VehicleEnergyAbortException &e)
    {
        exception_handle_by_distance(i * distance_unit + distance_unit);
        throw e;
    }
    catch(VehicleOxygenAbortException &e)
    {
        exception_handle_by_distance(i * distance_unit + distance_unit);
        throw e;
    }
    catch(VehicleDestroyAbortException &e)
    {
        exception_handle_by_distance(i * distance_unit + distance_unit);
        throw e;
    }
}

void AirPlane::print_distance(int distance)
{   
    cout << "Successfully moved to next " << distance << " km" << endl;
}

void AirPlane::print_status()
{
    cout << "Currrent Status: " << "Airplane" << endl;
    cout << "Distance: " << Vehicle::get_distance() << " km" << endl;
    cout << "Speed: " << Vehicle::get_speed() << " km/hr" << endl;
    cout << "Energy: " << Vehicle::get_energy() << endl;
    cout << "Oxygen Level: " << Vehicle::get_oxygen_level() << endl;
    cout << "Temperature: " << get_temperature() << " C" << endl;
    cout << "Humidity: " << get_humidity() << endl;
    cout << "Altitude: " << get_altitude() << " m" << endl;
    cout << "Air Density: " << get_air_density() << endl;
}

int AirPlane::get_temperature()
{
    return temperature;
}

int AirPlane::get_humidity()
{
    return humidity;
}

int AirPlane::get_altitude()
{
    return altitude;
}

int AirPlane::get_air_density()
{
    return air_density;
}

void AirPlane::set_temperature(int input_temperature)
{
    temperature = input_temperature;
}

void AirPlane::set_humidity(int input_humidity)
{
    humidity = input_humidity;
}

void AirPlane::set_altitude(int input_altitude)
{
    altitude = input_altitude;
}

void AirPlane::set_air_density(int input_air_density)
{
    air_density = input_air_density;
}

void AirPlane::initialize_airplane_by_next_stage()
{
    Vehicle::set_vehicle_type(TYPE_AIRPLANE);

    Sky *next_stage = dynamic_cast<Sky *>(Vehicle::get_environment()->get_next_stage());
    if(next_stage != NULL)
    {
        set_humidity(next_stage->get_humidity());
        set_temperature(next_stage->get_temperature());
        set_altitude(next_stage->get_altitude());
        set_air_density(next_stage->get_air_density());

        set_airplane_speed();
        set_energy_loss();
        set_oxygen_loss();
    }
    else
    {
        throw CriticalFailException("Airplane's next stage is not Sky!!!!");       //if next stage is not sky..
    }
}

void AirPlane::set_airplane_speed()
{
    int speed = 900; // default speed

    if(air_density >= 70)
    {
        speed -= 500;
    }
    else if(air_density >= 50)
    {
        speed -= 300;
    }
    else if(air_density >= 30)
    {
        speed -= 200;
    }

    Vehicle::set_speed(speed);
}

void AirPlane::set_oxygen_loss()
{
    int total_loss = 0;
    total_loss = (altitude / 1000) * 10;
    
    oxygen_loss = total_loss;
}

void AirPlane::set_energy_loss()
{
    int total_loss = 0;

    // energy loss set by temperature
    if(temperature >= 40) // temperature >= 40
    {
        total_loss += 10;
    }
    else if(temperature > 0)   // 0 < temperature < 40
    {
        total_loss += 5;
    }
    else if(temperature == 0)
    {
        total_loss += 8;
    }

    // energy loss set by humidity
    if(humidity < 50)
    {
        total_loss += 5;   
    }
    else        // humidity >= 50
    {
        total_loss += 8;
    }
    
    energy_loss = total_loss;
}

void AirPlane::exception_handle_by_distance(int distance) // function to handle exception uniformly.
{
    print_distance(distance);
    Vehicle::get_blackbox()->Record(TYPE_AIRPLANE, Vehicle::get_energy(), Vehicle::get_oxygen_level(), Vehicle::get_speed());
}

void AirPlane::move_one_unit() throw(VehicleException)
{
    Environment *env = Vehicle::get_environment();
    Stage *next_stage = env->get_next_stage();
    if (next_stage != NULL)     //if not finished..
    {
        Road *road;
        Sky *sky;
        Ocean *ocean;
        X_Stage *x_stage;
        Y_Stage *y_stage;
        
        if((sky = dynamic_cast<Sky *>(next_stage)) != NULL)
        {
            Vehicle::set_energy(Vehicle::get_energy() - energy_loss);
            Vehicle::set_distance(Vehicle::get_distance() + 1000);
            Vehicle::set_oxygen_level(Vehicle::get_oxygen_level() - oxygen_loss);
            env_progress_onestep();
        }
        // one step progress.
        
        next_stage = env->get_next_stage();   // process if next stage is x or y

        if((x_stage = dynamic_cast<X_Stage *>(next_stage)) != NULL)
        {
            int random_num = get_random_number(0,9); // get value from 0 to 9.
            if(random_num < 2) // 20% chance destroyed.
            {
                throw VehicleDestroyAbortException(Vehicle::get_Vehicle_type());
            }
            else    //energy loss
            {
                Vehicle::set_energy(Vehicle::get_energy() - 100);
                env_progress_onestep();
            }
        }
        else if((y_stage = dynamic_cast<Y_Stage *>(next_stage)) != NULL)
        {
            int random_num = get_random_number(0,99); // get value from 0 to 99.
            if(random_num < 35) // 35% chance
            {
                throw VehicleDestroyAbortException(Vehicle::get_Vehicle_type());
            }
            else
            {
                random_num = get_random_number(0,1);
                if(random_num < 1) // 50% chance
                {
                    Vehicle::set_oxygen_level(Vehicle::get_oxygen_level() - 30);
                }
                env_progress_onestep();
            }
        }
        // X or Y case processed.

        next_stage = env->get_next_stage();     // need to transform if next stage is not road
        if((road = dynamic_cast<Road *>(next_stage)) != NULL)
        {
            throw VehicleChangedException(TYPE_AIRPLANE, TYPE_CAR);
        }
        else if((ocean = dynamic_cast<Ocean *>(next_stage)) != NULL)
        {
            throw VehicleChangedException(TYPE_AIRPLANE, TYPE_SUBMARINE);
        }

    }
    else
    {
        throw VehicleFinishedException();
    }
}

void AirPlane::env_progress_onestep()  // this function progress one unit at environment. if vehicle status is not normal, throw exception.
{
    Environment *env = Vehicle::get_environment();
    env->onestep_go();
    check_vehicle_status(); // if current state couldn't progress.. raise error. 
}

void AirPlane::check_vehicle_status()
{
    Environment *env = Vehicle::get_environment();
    if(env->get_next_stage() == NULL) throw VehicleFinishedException();
    else Vehicle::throw_exception_vehicle_status_failed();
}


class SubMarine : public Vehicle
{
public:
    SubMarine(const Vehicle &other);
    virtual void unit_move() throw(VehicleException) override;  // if vehicle status have problem, this function throws exception
    virtual void chunk_move() throw(VehicleException) override; // if vehicle status have problem, this function throws exception
    void print_distance(int distance);
    virtual void print_status() override;
    int get_temperature();
    int get_depth();
    int get_waterflow();
    void light();
    void set_temperature(int input_temperature);
    void set_depth(int input_depth);
    void set_waterflow(int input_waterflow);

private:
    int temperature;
    int depth;
    int waterflow;
    int energy_loss;
    int oxygen_loss;
    bool light_on;

    void initialize_submarine_by_next_stage();
    void set_submarine_speed();
    void set_oxygen_loss();
    void set_energy_loss();
    void exception_handle_by_distance(int distance); // function to handle exception uniformly.
    void move_one_unit() throw(VehicleException); // this function actually move one_unit, and if vehicle status have problem, throw exception.
    void env_progress_onestep();  // this function progress one unit at environment. if vehicle status is not normal, throw exception.
    void check_vehicle_status();

};

SubMarine::SubMarine(const Vehicle &other) : Vehicle(other)
{
    initialize_submarine_by_next_stage();
    // set submarine
}

void SubMarine::unit_move() throw(VehicleException)
{
    int distance_unit = 10;
    try
    {
        move_one_unit();
        print_distance(distance_unit);
    }
    catch(VehicleChangedException &e) // vehicle changed is normally proceeded.
    {
        exception_handle_by_distance(distance_unit);
        throw e;
    }
    catch(VehicleFinishedException &e)
    {
        exception_handle_by_distance(distance_unit);
        throw e;
    }
    catch(VehicleEnergyAbortException &e)
    {
        exception_handle_by_distance(distance_unit);
        throw e;
    }
    catch(VehicleOxygenAbortException &e)
    {
        exception_handle_by_distance(distance_unit);
        throw e;
    }
    catch(VehicleDestroyAbortException &e)
    {
        exception_handle_by_distance(distance_unit);
        throw e;
    }
}


void SubMarine::chunk_move() throw(VehicleException)
{
    int distance_unit = 10;
    int i = 0;
    try
    {
        while(1)
        {
            move_one_unit();
            i++;
        }
    }
    catch(VehicleChangedException &e) // vehicle changed is normally proceeded.
    {
        exception_handle_by_distance(i * distance_unit + distance_unit);
        throw e;
    }
    catch(VehicleFinishedException &e)
    {
        exception_handle_by_distance(i * distance_unit + distance_unit);
        throw e;
    }
    catch(VehicleEnergyAbortException &e)
    {
        exception_handle_by_distance(i * distance_unit + distance_unit);
        throw e;
    }
    catch(VehicleOxygenAbortException &e)
    {
        exception_handle_by_distance(i * distance_unit + distance_unit);
        throw e;
    }
    catch(VehicleDestroyAbortException &e)
    {
        exception_handle_by_distance(i * distance_unit + distance_unit);
        throw e;
    }
}

void SubMarine::print_distance(int distance)
{   
    cout << "Successfully moved to next " << distance << " km" << endl;
}

void SubMarine::print_status()
{
    cout << "Currrent Status: " << "Submarine" << endl;
    cout << "Distance: " << Vehicle::get_distance() << " km" << endl;
    cout << "Speed: " << Vehicle::get_speed() << " km/hr" << endl;
    cout << "Energy: " << Vehicle::get_energy() << endl;
    cout << "Oxygen Level: " << Vehicle::get_oxygen_level() << endl;
    cout << "Temperature: " << get_temperature() << " C" << endl;
    cout << "Depth: " << get_depth() << " m" << endl;
    cout << "Water Flow: " << get_waterflow() << endl;
}

int SubMarine::get_temperature()
{
    return temperature;
}

int SubMarine::get_depth()
{
    return depth;
}

int SubMarine::get_waterflow()
{
    return waterflow;
}

void SubMarine::light()
{
    light_on = true;
}

void SubMarine::set_temperature(int input_temperature)
{
    temperature = input_temperature;
}

void SubMarine::set_depth(int input_depth)
{
    depth = input_depth;
}

void SubMarine::set_waterflow(int input_waterflow)
{
    waterflow = input_waterflow;
}



void SubMarine::initialize_submarine_by_next_stage()
{
    Vehicle::set_vehicle_type(TYPE_SUBMARINE);

    Ocean *next_stage = dynamic_cast<Ocean *>(Vehicle::get_environment()->get_next_stage());
    if(next_stage != NULL)
    {
        set_temperature(next_stage->get_temperature());
        set_depth(next_stage->get_depth());
        set_waterflow(next_stage->get_waterflow());
        light();        // light on

        set_submarine_speed();
        set_energy_loss();
        set_oxygen_loss();
    }
    else
    {
        throw CriticalFailException("Submarine's stage is not ocean!!!");       //if next stage is not ocean..
    }
}

void SubMarine::set_submarine_speed()
{
    int speed = 20; // default speed

    if(waterflow >= 70)
    {
        speed -= 10;
    }
    else if(waterflow >= 50)
    {
        speed -= 5;
    }
    else if(waterflow >= 30)
    {
        speed -= 3;
    }

    Vehicle::set_speed(speed);
}

void SubMarine::set_oxygen_loss()
{
    int total_loss = 0;
    total_loss = (depth / 50) * 5;
    
    oxygen_loss = total_loss;
}

void SubMarine::set_energy_loss()
{
    int total_loss = 0;

    // energy loss set by temperature
    if(temperature >= 40) // temperature >= 40
    {
        total_loss += 10;
    }
    else if(temperature > 0)   // 0 < temperature < 40
    {
        total_loss += 5;
    }
    else if(temperature == 0)
    {
        total_loss += 8;
    }

    if(light_on == true)
    {
        total_loss += 30;
    }
    
    energy_loss = total_loss;
}

void SubMarine::exception_handle_by_distance(int distance) // function to handle exception uniformly.
{
    print_distance(distance);
    Vehicle::get_blackbox()->Record(TYPE_SUBMARINE, Vehicle::get_energy(), Vehicle::get_oxygen_level(), Vehicle::get_speed());
}

void SubMarine::move_one_unit() throw(VehicleException)
{
    Environment *env = Vehicle::get_environment();
    Stage *next_stage = env->get_next_stage();
    if (next_stage != NULL)     //if not finished..
    {
        Road *road;
        Sky *sky;
        Ocean *ocean;
        X_Stage *x_stage;
        Y_Stage *y_stage;
        
        if((ocean = dynamic_cast<Ocean *>(next_stage)) != NULL)
        {
            Vehicle::set_energy(Vehicle::get_energy() - energy_loss);
            Vehicle::set_distance(Vehicle::get_distance() + 10);
            Vehicle::set_oxygen_level(Vehicle::get_oxygen_level() - oxygen_loss);
            env_progress_onestep();
        }
        // one step progress.
        
        next_stage = env->get_next_stage();   // process if next stage is x or y

        if((x_stage = dynamic_cast<X_Stage *>(next_stage)) != NULL)
        {
            int random_num = get_random_number(0,9); // get value from 0 to 9.
            if(random_num < 2) // 20% chance destroyed.
            {
                throw VehicleDestroyAbortException(Vehicle::get_Vehicle_type());
            }
            else    //energy loss
            {
                Vehicle::set_energy(Vehicle::get_energy() - 100);
                env_progress_onestep();
            }
        }
        else if((y_stage = dynamic_cast<Y_Stage *>(next_stage)) != NULL)
        {
            int random_num = get_random_number(0,99); // get value from 0 to 99.
            if(random_num < 35) // 35% chance
            {
                throw VehicleDestroyAbortException(Vehicle::get_Vehicle_type());
            }
            else
            {
                random_num = get_random_number(0,1);
                if(random_num < 1) // 50% chance
                {
                    Vehicle::set_oxygen_level(Vehicle::get_oxygen_level() - 30);
                }
                env_progress_onestep();
            }
        }
        // X or Y case processed.

        next_stage = env->get_next_stage();     // need to transform if next stage is not ocean
        if((road = dynamic_cast<Road *>(next_stage)) != NULL)
        {
            throw VehicleChangedException(TYPE_SUBMARINE, TYPE_CAR);
        }
        else if((sky = dynamic_cast<Sky *>(next_stage)) != NULL)
        {
            throw VehicleChangedException(TYPE_SUBMARINE, TYPE_AIRPLANE);
        }

    }
    else
    {
        throw VehicleFinishedException();
    }
}

void SubMarine::env_progress_onestep()  // this function progress one unit at environment. if vehicle status is not normal, throw exception.
{
    Environment *env = Vehicle::get_environment();
    env->onestep_go();
    check_vehicle_status(); // if current state couldn't progress.. raise error. 
}

void SubMarine::check_vehicle_status()
{
    Environment *env = Vehicle::get_environment();
    if(env->get_next_stage() == NULL) throw VehicleFinishedException();
    else Vehicle::throw_exception_vehicle_status_failed();
}


int main()
{
    int input;
    bool extra_mode = false;

    string TC1 = "[R500T20H20],[X],[S3000T10H5A2000D30],[Y],[O80T0D100W100]";
    string TC2 = "[R4950T20H20],[S1000T10H5A2000D30]";
    string TC3 = "[R4750T20H20],[S6000T10H5A2000D30]";
    string TC4 = "[R500T20H20],[X],[S4000T10H5A2000D30],[Y],[O80T0D100W100]";
    string TC5 = "[R500T20H20],[Y],[S1000T10H5A2000D30],[R500T20H20],[Y],[S1000T10H5A2000D30],[R5000T20H20]";
    string TC6 = "[R100T10H10],[S3000T10H5A2000D30],[O40T0D100W100]";
    string TC7 = "[R100T10H10],[S3000T10H5A2000D30],[O40T0D100W100]";
    string TC8 = "[R100T10H10],[S3000T10H5A2000D30],[O40T0D100W100]";
    string TC9 = "[R100T10H10],[S3000T10H5A2000D30],[O40T0D100W100]";
    string TC10 = "[R100T10H10],[S3000T10H5A2000D30],[O40T0D100W100]";
    string test_case;

    cout << "PJ1.KBC.2014-16757" << endl;
    cout << "Mode Select(1 for EXTRA, 2 for NORMAL) : ";
    cin >> input;
    if(input == 1)
    {
        extra_mode = true;
    }

    while(1)
    {
        cout << "Choose the number of the test case (1~10) : ";
        cin >> input;
        if(input == 0) break; // if get 0, exit program
        
        if(input > 0 && input < 11)
        {
            cout << "Test case #" << input << "." << endl << endl;

            Blackbox *blackbox = new Blackbox();
            //Environment *env = new Environment("./TC" + to_string(input) + ".txt", extra_mode);
            if(input == 1) test_case = TC1;
            else if(input == 2) test_case = TC2;
            else if(input == 3) test_case = TC3;
            else if(input == 4) test_case = TC4;
            else if(input == 5) test_case = TC5;
            else if(input == 6) test_case = TC6;
            else if(input == 7) test_case = TC7;
            else if(input == 8) test_case = TC8;
            else if(input == 9) test_case = TC9;
            else if(input == 10) test_case = TC10;
            else throw EnvironmentFailException();

            Environment *env = new Environment(test_case, extra_mode);



            Vehicle *current_vehicle = new Car(blackbox, env);      //initialize new car.
            
            current_vehicle->print_status();
            
            while(1)        // run start
            {
                try
                {
                    env->print_map();   // for extra
                    print_nextmove();
                    print_prompt();
                    cin >> input;

                    if(input == 1)
                    {
                        current_vehicle->unit_move();
                        current_vehicle->print_status();
                    }
                    else if(input == 2)
                    {
                        current_vehicle->chunk_move();
                        current_vehicle->print_status();
                    }
                }
                catch(VehicleChangedException &e)       // transform vehicle.
                {
                    current_vehicle->print_status();     // need to print current state.

                    Vehicle_type before_type, after_type;
                    before_type = e.get_vehicle_before_type();
                    after_type = e.get_vehicle_after_type();

                    if(after_type == TYPE_CAR)
                    {
                        Vehicle *next_vehicle = new Car(*current_vehicle);
                        delete current_vehicle;
                        current_vehicle = next_vehicle;
                    }
                    else if(after_type == TYPE_AIRPLANE)
                    {
                        Vehicle *next_vehicle = new AirPlane(*current_vehicle);
                        delete current_vehicle;
                        current_vehicle = next_vehicle;
                    }
                    else if(after_type == TYPE_SUBMARINE)
                    {
                        Vehicle *next_vehicle = new SubMarine(*current_vehicle);
                        delete current_vehicle;
                        current_vehicle = next_vehicle;
                    }
                    else    // error occured!
                    {
                        throw CriticalFailException("Couldn't Vehicle Change");
                    }

                }
                catch(VehicleFinishedException &e)
                {
                    current_vehicle->print_final_status();
                    env->print_map();
                    cout << endl;
                    cout << "!FINISHED : Arrived" << endl;
                    blackbox->print();
                    cout << "--------------------" << endl;
                    break;
                }
                catch(VehicleEnergyAbortException &e)
                {
                    current_vehicle->print_final_status();
                    env->print_map();
                    cout << endl;
                    cout << "!FINISHED : Energy failure" << endl;
                    blackbox->print();
                    cout << "--------------------" << endl;
                    break;
                }
                catch(VehicleOxygenAbortException &e)
                {
                    current_vehicle->print_final_status();
                    env->print_map();
                    cout << endl;
                    cout << "!FINISHED : Oxygen failure" << endl;
                    blackbox->print();
                    cout << "--------------------" << endl;
                    break;
                }
                catch(VehicleDestroyAbortException &e)
                {
                    current_vehicle->print_final_status();
                    env->print_map();
                    cout << endl;
                    cout << "!FINISHED : Vehicle stop" << endl;
                    blackbox->print();
                    cout << "--------------------" << endl;
                    break;
                }
            }
            
            // delete dynamically allocated members.
            delete current_vehicle;
            delete blackbox;
            delete env;
        }
    }

    return 0;
}