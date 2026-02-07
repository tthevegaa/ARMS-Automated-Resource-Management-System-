
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <vector>
#include <limits>
#include <memory>
#include <functional>
#include <iomanip>
#include <sstream>
   

//avoids the break point when switching from "cin>>variable" to "getline(cin, variable)".
void CinToGetline() {
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
}



std::string Caps(std::string text) {
    
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) {return std::toupper(c);});
    
    return text;
}



//switches the entered password into a hash number in order to improve security and privacy.
std::string HashPassword(const std::string& index) {
    
    unsigned long hash = 5381;
    for (char c : index) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
    }

    std::ostringstream oss;
    oss<<std::hex<<hash; 
    
    return oss.str();
}



class User {
    
    private:
    std::string username;
    std::string password;
    std::string fname;
    std::string lname;
    std::string DOB;
    std::string Hdate;
    
   
    
    public:
    virtual ~User() = default;
    
    User(const std::string& u, const std::string& pa, const std::string& f, const std::string& l,
    const std::string& d, const std::string& h) {
        
        username = u;
        password = pa;
        fname = f;
        lname = l;
        DOB = d;
        Hdate = h;
        
    }
    
    
    std::string GetUsername() const {
        return username;
    }
    
    
    std::string GetPassword() const {
        return password;
    }
    
    
    std::string GetFname() const {
        return fname;
    }
    
    
    std::string GetLname() const {
        return lname;
    }
    
    
    std::string GetDOB() const {
        return DOB;
    }
    
    
    std::string GetHdate() const {
        return Hdate;
    }
    
};



class Employee : public User {
    
    private:
    int employeeN;
    std::string schedule;
    std::string position;
    float salary;
    
    
    
    public:
    Employee (const std::string& u,
    const std::string& pa,
    const std::string& f,
    const std::string& l,
    const std::string& d,
    const std::string& h,
    int e,
    const std::string& s,
    const std::string& po,
    float sal) : User(u, pa, f, l, d, h), employeeN(e), schedule(s), position(po), salary(sal) {
        
    }
    
    
    
    int GetEmpNum() const {
        return employeeN;
    }
    
    
    std::string GetSched() const {
        return schedule;
    }
    
    
    std::string GetPosit() const {
        return position;
    }
    
    
    float GetSal() const {
        return salary;
    }
    
    
    void SetSchedule(const std::string& schedule) {
        this->schedule = schedule;
    }
    
    
    void SetSalary(float salary) {
        this->salary = salary;
    }
    
    
    void SetPosition(const std::string& position) {
        this->position = position;
    }
    
    
};




class Supervisor : public User {
    
    private:
    
    std::string level;
    
    
    public:
    
    Supervisor (const std::string& u,
    const std::string& pa,
    const std::string& f,
    const std::string& l,
    const std::string& d,
    const std::string& h,
    const std::string& lev) : User(u, pa, f, l, d, h),
    level(lev) {
        
    }
    
    
    std::string GetLevel() const {
        return level;
    }
    
    
    
    
    void SetLevel(std::string newlevel) {
        level = newlevel;
    }
    
    
};





class UserRepository {
    
    public:
    
    void SaveUsersToFile(const std::vector<std::unique_ptr<User>>& users) {
        
        std::ofstream file("users.bin", std::ios::binary | std::ios::trunc);
        
        if(!file.is_open()) { 
            return;
        }
        
        
        for(const std::unique_ptr<User>& x : users) {
            
            
            Employee* y = dynamic_cast<Employee*>(x.get());
            
            
            int type = (y != nullptr) ? 1 : 2;
            file.write(reinterpret_cast<char*>(&type), sizeof(type));
            
            
            size_t z;
            
            z = x->GetUsername().size();
            file.write(reinterpret_cast<char*>(&z), sizeof(z));
            file.write(x->GetUsername().c_str(), z);
            
            
            z = x->GetPassword().size();
            file.write(reinterpret_cast<char*>(&z), sizeof(z));
            file.write(x->GetPassword().c_str(), z);
            
            
            z = x->GetFname().size();
            file.write(reinterpret_cast<char*>(&z), sizeof(z));
            file.write(x->GetFname().c_str(), z);
            
            
            z = x->GetLname().size();
            file.write(reinterpret_cast<char*>(&z), sizeof(z));
            file.write(x->GetLname().c_str(), z);
            
            
            z = x->GetDOB().size();
            file.write(reinterpret_cast<char*>(&z), sizeof(z));
            file.write(x->GetDOB().c_str(), z);
            
            
            z = x->GetHdate().size();
            file.write(reinterpret_cast<char*>(&z), sizeof(z));
            file.write(x->GetHdate().c_str(), z);
            
            
            if(y != nullptr) {
                
                int numindex = y->GetEmpNum();
                file.write(reinterpret_cast<char*>(&numindex), sizeof(numindex));
                
                
                z = y->GetSched().size();
                file.write(reinterpret_cast<char*>(&z), sizeof(z));
                file.write(y->GetSched().c_str(), z);
                
                
                z = y->GetPosit().size();
                file.write(reinterpret_cast<char*>(&z), sizeof(z));
                file.write(y->GetPosit().c_str(), z);
                
                
                float salaryindex = y->GetSal();
                file.write(reinterpret_cast<char*>(&salaryindex), sizeof(salaryindex));
                
            } else {
                
                Supervisor* t = dynamic_cast<Supervisor*>(x.get());
                
                if(t != nullptr) {
                    z = t->GetLevel().size();
                    file.write(reinterpret_cast<char*>(&z), sizeof(z));
                    file.write(t->GetLevel().c_str(), z);
                }
                
            }
        } 
        
        
        
    }
    
    
    
    
    void LoadEmployeeFields(std::ifstream& archive, int& employeeNumber, std::string& schedule, std::string& position, float& salary) {
        
        size_t x;
        
        archive.read(reinterpret_cast<char*>(&employeeNumber), sizeof(employeeNumber));
        
        
        archive.read(reinterpret_cast<char*>(&x), sizeof(x));
        schedule.resize(x);
        archive.read(&schedule[0], x); 
        
     
        archive.read(reinterpret_cast<char*>(&x), sizeof(x));
        position.resize(x);
        archive.read(&position[0], x);
        
        
        archive.read(reinterpret_cast<char*>(&salary), sizeof(salary));
        
    }
    
    
    
    
    void LoadUsersFromFile(std::vector<std::unique_ptr<User>>& users) {
        
        users.clear();
        
        
        std::ifstream archive("users.bin", std::ios::binary);
        
        if(!archive.is_open()) {
            return;
        }
        
        
        int type;
        
        while(archive.read(reinterpret_cast<char*>(&type), sizeof(type))) {
            
            
            size_t x;
            
            std::string username;
            archive.read(reinterpret_cast<char*>(&x), sizeof(x));
            username.resize(x);
            archive.read(&username[0], x);
            
            
            std::string password;
            archive.read(reinterpret_cast<char*>(&x), sizeof(x));
            password.resize(x);
            archive.read(&password[0], x);
            
            
            std::string fname;
            archive.read(reinterpret_cast<char*>(&x), sizeof(x));
            fname.resize(x);
            archive.read(&fname[0], x);
            
            
            std::string lname;
            archive.read(reinterpret_cast<char*>(&x), sizeof(x));
            lname.resize(x);
            archive.read(&lname[0], x);
            
            
            std::string DOB;
            archive.read(reinterpret_cast<char*>(&x), sizeof(x));
            DOB.resize(x);
            archive.read(&DOB[0], x);
            
            
            std::string Hdate;
            archive.read(reinterpret_cast<char*>(&x), sizeof(x));
            Hdate.resize(x);
            archive.read(&Hdate[0], x);
            
            
            
            if(type == 1) {
                
                int employeeNumber;
                
                std::string schedule;
                
                std::string position;
                
                float salary;
                
                LoadEmployeeFields(archive, employeeNumber, schedule, position, salary);
                
                
                auto y = std::make_unique<Employee>(username, password, fname, lname, DOB, Hdate,
                employeeNumber, schedule, position, salary);
                
                
                users.push_back(std::move(y));
                
                
            } else if(type == 2) {
                
                std::string level;
                archive.read(reinterpret_cast<char*>(&x), sizeof(x));
                level.resize(x);
                archive.read(&level[0], x);
                
                
                auto y = std::make_unique<Supervisor>(username, password, fname, lname, DOB, Hdate,
                level);
                
                users.push_back(std::move(y));
                
            }
            
        }
        
    }
    
    
    
    
};





class UserManager {
    
    public:
    
    struct EmployeeData {
        
        std::string username;
        std::string password;
        std::string fname;
        std::string lname;
        std::string DOB;
        std::string Hdate;
        
        int employeeNumber;
        std::string schedule;
        std::string position;
        float salary; 
        
    };
    
    
    struct SupervisorData {
        
        std::string username;
        std::string password;
        std::string fname;
        std::string lname;
        std::string DOB;
        std::string Hdate;
        
        std::string level;
        
    };
    
    
    
    
    private:
    
    UserRepository repository;
    std::vector<std::unique_ptr<User>> users;
    
    
    
    public:
    
    void Load() {
        
        repository.LoadUsersFromFile(users);
        if(users.empty()) {
            
            std::unique_ptr<User> admin = std::make_unique<Supervisor>("first_admin",
                HashPassword("12345"), "cesar", "vega", "07-14-2004", "01-20-2026", "accountant");
                
            users.push_back(std::move(admin));
            Save();
        }

    }
    
    
    void Save() {
        
        repository.SaveUsersToFile(users);
        
    }
    
    
    bool IsSupervisor(const User* x) {
        
        return dynamic_cast<const Supervisor*>(x) != nullptr;
        
    }
    
    
    bool AddEmployee(const EmployeeData& data, User* current_user) {
        
        if(!IsSupervisor(current_user)) {
            return false;
        }
        
        if(UsernameExists(data.username)) {
            return false;
        }
        
        if(EmployeeNumExists(data.employeeNumber)) {
            return false;
        }
        
        auto x = std::make_unique<Employee>(data.username, HashPassword(data.password),
        data.fname, data.lname,
        data.DOB, data.Hdate,
        data.employeeNumber, data.schedule,
        data.position, data.salary);
        
        users.push_back(std::move(x));
        Save();
        
        return true;
        
    }
    
    
    void AddSupervisor(const SupervisorData& data, User* current_user) {
        
        if(!IsSupervisor(current_user)) {
            std::cout<<"\nDENIED ACCESS\n";
            return;
        }
        
        if(UsernameExists(data.username)) {
            return;
        }
        
        auto x = std::make_unique<Supervisor>(data.username, HashPassword(data.password), data.fname, 
        data.lname, data.DOB, 
        data.Hdate, data.level);
        
        users.push_back(std::move(x));
        Save();
        
        std::cout<<"\nSupervisor succesfully added with username: "<<data.username<<std::endl;
        
    }
    
    
    User* LogIn(const std::string& username, const std::string& password) {
        std::string hashed = HashPassword(password);
        
        
        for(const std::unique_ptr<User>& i : users) {
            if(i->GetUsername() == username) {
                if(i->GetPassword() == hashed) {
                    return i.get();
                }
            }
            
        }
        
        return nullptr;
    }
    
    
    
    
    Employee* FindByEmployeeNumber(int num) {
        
        for(const std::unique_ptr<User>& x : users) {
            
            Employee* e = dynamic_cast<Employee*>(x.get());
            
            if(e != nullptr) {  
                if(e->GetEmpNum() == num) {
                    return e;
                }
            }
        }
        
        return nullptr;
    }
    
    
    
    User* Finding(const std::string& anyuser) {
        
        for(const std::unique_ptr<User>& i : users) {
            if(i->GetUsername() == anyuser) {
                return i.get();
            }
        }
        
        return nullptr;
       
    }
    
    
    
   
    
    void DisplayEmployeeInfo(const Employee* x) {
        std::cout<<"FIRST NAME: "<<x->GetFname()<<"  |  LAST NAME: "<<x->GetLname()<<std::endl;
        std::cout<<"USERNAME: "<<x->GetUsername()<<"  |  POSITION: "<<x->GetPosit()<<std::endl;
        std::cout<<"MONTHLY SALARY: "<<x->GetSal()<<"  |  EMPLOYEE NUMBER: "<<x->GetEmpNum()<<std::endl;
        std::cout<<"DOB: "<<x->GetDOB()<<"  |  HIRING DATE: "<<x->GetHdate()<<std::endl;  
    }
    
    
    
    void DisplaySupervisorInfo(const Supervisor* x) {
        std::cout<<"FIRST NAME: "<<x->GetFname()<<"  |  LAST NAME: "<<x->GetLname()<<std::endl;
        std::cout<<"USERNAME: "<<x->GetUsername()<<"  |  LEVEL: "<<x->GetLevel()<<std::endl;
        std::cout<<"DOB: "<<x->GetDOB()<<"  |  HIRING DATE: "<<x->GetHdate()<<std::endl;
    }
    
    
    
    void ShowUserInfo(User* x) {
        
        Employee* z = dynamic_cast<Employee*>(x);
        
        if(z != nullptr) {
            DisplayEmployeeInfo(z);
            return;
        }
        
        Supervisor* y = dynamic_cast<Supervisor*>(x);
        
        if(y != nullptr) {
            DisplaySupervisorInfo(y);
            return;
        } 
    }
    
    
    
    //checks either action is allowed or not
    Employee* ValidateEditEmployee(int numb, User* current_user) {
        
        if(!IsSupervisor(current_user)) {
            return nullptr;
        }
        
        
        Employee* z = FindByEmployeeNumber(numb);
        if(z == nullptr) {
            return nullptr;
        }
        
        DisplayEmployeeInfo(z);
        
        return z;
        
    }
    
    
    
   
    //Funtion responsible for removing either a supervisor or a employee.
    bool RemovingUser(const std::string& anyuser) {
        
        for(int i = 0; i < users.size(); i++) {
            if(users[i]->GetUsername() == anyuser) {
                users.erase(users.begin() + i);
                return true;
            }
        }
        
        return false;
        
    }
    
   
    
    
    Employee* ValidateDeleteEmployee(int numb, User* current_user) {
        
        if(!IsSupervisor(current_user)) {
            return nullptr;
        }
        
        Employee* z = FindByEmployeeNumber(numb);
        if(z == nullptr) {
            return nullptr;
        }
        
        DisplayEmployeeInfo(z);
        
        return z;
    }
    
    
    
    
    bool DeleteEmployee(Employee* z) {
        
        if(!RemovingUser(z->GetUsername())) {
            return false;
        }
        
        return true;
        
    }
    

    
    
    
    bool UsernameExists(const std::string& anyuser) const {
        
        for(const std::unique_ptr<User>& x : users) {
            if(x->GetUsername() == anyuser) {
                return true;
            } 
        }
        
        return false;
        
    }
    
    
    
    bool EmployeeNumExists(int anynum) const {
        
        for(const std::unique_ptr<User>& i : users) {
            
            Employee* x = dynamic_cast<Employee*>(i.get());
            
            if(x != nullptr) {
                if(x->GetEmpNum() == anynum) {
                    return true;
                }  
            } 
        }
        
        return false;
        
    }
    
    
    
    
    
    Supervisor* ValidateEditSupervisor(const std::string& anyuser, User* current_user) {
        
        if(!IsSupervisor(current_user)) {
            return nullptr;
        }
        
        
        Supervisor* x = dynamic_cast<Supervisor*>(Finding(anyuser));
        if(x == nullptr) {
            return nullptr;
        }
        
        ShowUserInfo(x);
        
        return x;
        
    }
    
    
    
    
    bool EditSupervisorLevel(Supervisor* x, std::string& newlevel) {
        
        x->SetLevel(newlevel);
        
        return true;
        
    }
    
    
    
    Supervisor* ValidateDeleteSupervisor(const std::string& anyuser, User* current_user) {
        
        if(!IsSupervisor(current_user)) {
            return nullptr;
        }
        
        Supervisor* x = dynamic_cast<Supervisor*>(Finding(anyuser));
        if(x == nullptr) {
            return nullptr;
        }
        
        ShowUserInfo(x);
        return x;
        
    }
    
    
    
    
    bool DeleteSupervisor(Supervisor* x)  {  
        
        if(!RemovingUser(x->GetUsername())) {
            return false;
        }
        
        return true;
    }
    
    
};




void ShowEmployeeMenu(UserManager& x, Employee* y) {
    
    int opt;
    
    while(true) {
        
        std::cout<<"\nENTER ONE OF THE OPTIONS BELOW: \n";
        std::cout<<"DISPLAY MY PROFILE -> 1\n";
        std::cout<<"VIEW MY SCHEDULE -> 2\n";
        std::cout<<"VIEW MY SALARY -> 3\n";
        std::cout<<"LOG OUT -> 0\n";
        std::cin>>opt;
        
        
        if(std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout<<"\nINVALID OPTION. TRY AGAIN: \n";
            continue;
        }
        
        if(opt < 0 || opt > 3) {
            std::cout<<"\nINVALID OPTION. TRY AGAIN: \n";
            continue;
        }
        
        
        switch(opt) {
            
            
            case 1:
            x.DisplayEmployeeInfo(y);
            break;
            
            
            case 2:
            std::cout<<"SCHEDULE: \n"<<y->GetSched()<<"\n";
            break;
            
            
            case 3:
            
            std::cout<<"SALARY: \n"<<y->GetSal()<<"\n";
            
            break;
            
            
            case 0:
            
            std::cout<<"\nLEAVING...\n";
            return;
            
            
            default:
            
            std::cout<<"\nERROR. INVALID OPTION.\n";
            break;
            
        } 
        
    }
    
}



//Gets all the needed parameters to add a new employee. 
UserManager::EmployeeData CollectData() {
    
    UserManager::EmployeeData x;
    
    
    std::cout<<"USERNAME: ";
    std::cin>>x.username;
    x.username = Caps(x.username);
    
    
    std::cout<<"PASSWORD: ";
    std::cin>>x.password;
    
    std::cout<<"FIRST NAME: ";
    std::cin>>x.fname;
    x.fname = Caps(x.fname);
    
    
    std::cout<<"LAST NAME: ";
    std::cin>>x.lname;
    x.lname = Caps(x.lname);
    
    
    std::cout<<"DOB: ";
    std::cin>>x.DOB;
    
    std::cout<<"HARING DATE: ";
    std::cin>>x.Hdate;
    
    std::cout<<"EMPLOYEE NUMBER: ";
    std::cin>>x.employeeNumber;
    
    CinToGetline();
    std::cout<<"SCHEDULE: ";
    std::getline(std::cin, x.schedule);
    
    std::cout<<"POSITION: ";
    std::getline(std::cin, x.position);
    
    std::cout<<"SALARY: ";
    std::cin>>x.salary;
    
    CinToGetline();
    
    return x;
    
}



int GetEmpNumber() {
    
    int n;
    std::cout<<"\nEMPLOYEE NUMBER: ";
    std::cin>>n;
    
    return n;
}


std::string GettUsernameInput() {
    
    std::string x;
    std::cout<<"\nUSERNAME: ";
    std::cin>>x;
    
    return Caps(x);
}



bool IsValidText(const std::string& thistext) {
    
    if(thistext.empty()) {
        return false;
    }
    
    bool allSpaces = true;
    bool allDigits = true;
    
    for(char i : thistext) {
        
        if(!std::isspace(i)) {
            allSpaces = false;
        }
        
        if(!std::isdigit(i)) {
            allDigits = false;
        }
        
    }
    
    return !allSpaces && !allDigits;
    
}



bool ConfirmingModification() {
    
    int opt;
    
    while(true) {
        
        std::cout<<"\nARE YOU CONFIRMING TO CARRY OUT THIS ACTION?\n";
        std::cout<<"CONFIRM -> 1\n";
        std::cout<<"CANCEL -> 0\n";
        std::cin>>opt;
        
        if(opt != 1 && opt != 0) {
            std::cout<<"\nINVALID OPTION. TRY AGAIN: \n";
            continue;
        }
        
        if(opt == 0) {
            std::cout<<"\nNO CHANGES MADE\n"<<std::endl;
            return false;
        }
        
        break;
        
    }
    
    return true;
    
}




void CaseOne(UserManager& x, User* current_user) {
    
    UserManager::EmployeeData data = CollectData();
    
    if(!ConfirmingModification()) {
        return;
    }
    
    if(!x.AddEmployee(data, current_user)) {
        std::cout<<"\nEMPLOYEE COULD NOT BE ADDED. TRY AGAIN LATER\n"<<std::endl;
        return;
    }
    
    x.Save();
    std::cout<<"\nEMPLOYEE WAS SUCCESFULLY ADDED\n"<<std::endl;
    return;
    
}




void CaseTwo(UserManager& x, User* current_user) {
    
    
    Employee* z = x.ValidateEditEmployee(GetEmpNumber(), current_user);
    if(z == nullptr) {
        std::cout<<"\nSOMETHING WENT WRONG. TRY AGAIN LATER.\n"<<std::endl;
        return;
    }
    
    int opt;
    
    while(true) {
        
        std::cout<<"\nCHANGE SALARY -> 1\n";
        std::cout<<"CHANGE SCHEDULE -> 2\n";
        std::cout<<"CHANGE POSITION -> 3\n";
        std::cout<<"EXIT -> 0\n";
        std::cin>>opt;
        
        if(std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout<<"\nINVALID OPTION. TRY AGAIN: \n";
            continue;
        }
        
        
        if(opt < 0 || opt > 3) {
            std::cout<<"\nINVALID OPTION. TRY AGAIN: \n";
            continue;
        }
        
        break;
        
    }
    
    switch(opt) {
        
        case 1: {
            
            while(true) {
                
                float newsalary;
                std::cout<<"\nNEW SALARY: ";
                std::cin>>newsalary;
                
                if(newsalary <= 0) {
                    std::cout<<"\nINVALID INPUT. TRY AGAIN: \n";
                    continue;
                }
                
                if(!ConfirmingModification()) {
                    return;
                }
                
                z->SetSalary(newsalary);
                
                std::cout<<"\nSALARY SUCCESFULLY MODIFIED.\n";
                break;
            }
            
            break;
            
        }
        
        case 2: {
            
            while(true) {
                
                CinToGetline();
                std::string newschedule;
                std::cout<<"\nNEW SCHEDULE: ";
                std::getline(std::cin, newschedule);
                
                if(!IsValidText(newschedule)) {
                    std::cout<<"\nINVALID INPUT. TRY AGAIN: \n";
                    continue;
                }
                
                if(!ConfirmingModification()) {
                    return;
                }
                
                z->SetSchedule(newschedule);
                
                std::cout<<"\nSCHEDULE SUCCESFULLY MODIFIED\n"<<std::endl;
                break;
                
            }
            
            break;
            
        }
        
        
        case 3: {
            
            while(true) {
                
                CinToGetline();
                std::string newposition;
                std::cout<<"\nNEW POSITION: ";
                std::getline(std::cin, newposition);
                
                
                if(!IsValidText(newposition)) {
                    std::cout<<"\nINVALID INPUT. TRY AGAIN: \n";
                    continue;
                }
                
                if(!ConfirmingModification()) {
                    return;
                }
                
                z->SetPosition(newposition);
                
                std::cout<<"\nPOSITION SUCCESFULLY MODIFIED.\n"<<std::endl;
                break;
            }
            
            break;
            
        }
        
        
        case 0:
        std::cout<<"\nLEAVING...\n";
        return;
        
        
        default:
        std::cout<<"\nINVALID OPTION. \n";
        return;
        
    }
    
    
    x.Save();
    std::cout<<"\nCHANGES WERE APPLIED.\n"<<std::endl;
    return;
    
}




void CaseThree(UserManager& x, User* current_user) {
    
    Employee* z = x.ValidateDeleteEmployee(GetEmpNumber(), current_user);
    if(z == nullptr) {
        std::cout<<"\nSOMETHING WENT WRONG. TRY AGAIN LATER.\n"<<std::endl;
        return;
    }
    
    if(!ConfirmingModification()) {
        return;
    }
    
    if(!x.DeleteEmployee(z)) {
        std::cout<<"\nEMPLOYEE COULD NOT BE DELETED. TRY AGAIN LATER.\n"<<std::endl;
        return;
    }
    
    x.Save();
    std::cout<<"\nEMPLOYEE SUCCESFULLY DELETED.\n"<<std::endl;
    return;
    
    
}




void CaseFour(UserManager& x, User* current_user) {
    
    
    Supervisor* z = x.ValidateEditSupervisor(GettUsernameInput(), current_user);
    if(z == nullptr) {
        std::cout<<"\nSOMETHING WENT WRONG. TRY AGAIN LATER.\n"<<std::endl;
        return;
    }
    
    if(!ConfirmingModification()) {
        return;
    }
    
    
    std::string newlevel;
    while(true) {
        
        CinToGetline();
        std::cout<<"\nENTER THE NEW LEVEL: ";
        std::getline(std::cin, newlevel);
        
        if(!IsValidText(newlevel)) {
            std::cout<<"\nINVALID LEVEL. TRY AGAIN.\n";
            continue;
        }
        
        break;
    }
    
    
    if(!x.EditSupervisorLevel(z, newlevel)) {
        std::cout<<"\nSOMETHING WENT WRONG. TRY AGAIN LATER.\n"<<std::endl;
        return;
    }
    
    x.Save();
    std::cout<<"\nLEVEL SUCCESFULLY EDITED.\n"<<std::endl;
    return;
    
    
}



void CaseFive(UserManager& x, User* current_user) {
    
    Supervisor* z = x.ValidateDeleteSupervisor(GettUsernameInput(), current_user);
    if(z == nullptr) {
        std::cout<<"\nSOMETHING WENT WRONG. TRY AGAIN LATER.\n"<<std::endl;
        return;
    }
    
    if(!ConfirmingModification()) {
        return;
    }
    
    if(!x.DeleteSupervisor(z)) {
        std::cout<<"\nSOMETHING WENT WRONG. TRY AGAIN LATER.\n"<<std::endl;
        return;
    }
    
    x.Save();
    std::cout<<"\nSUPERVISOR SUCCESFULLY DELETED\n"<<std::endl;
    return;
    
}




void ShowSupervisorMenu(UserManager& x, User* current_user) {
    int opt;
    
    std::cout<<"\n          WELCOME "<<current_user->GetFname()<<"    \n";
    
    while(true) {
        
        std::cout<<"\nENTER ONE OF THE OPTIONS BELOW: \n";
        std::cout<<"-----------------------------------\n";
        std::cout<<"ADD EMPLOYEE -> 1\n";
        std::cout<<"EDIT EMPLOYEE -> 2\n";
        std::cout<<"DELETE EMPLOYEE -> 3\n";
        std::cout<<"EDIT SUPERVISOR -> 4\n";
        std::cout<<"DELETE SUPERVISOR -> 5\n";
        std::cout<<"LOG OUT -> 0\n";
        std::cout<<"-----------------------------------\n";
        std::cin>>opt;
        
        
        if(std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout<<"\nINVALID OPTION. TRY AGAIN: \n";
            continue;
        }
        
        
        if(opt < 0 || opt > 5) {
            std::cout<<"\nINVALID OPTION. TRY AGAIN: \n";
            continue;
        }
        
        
        
        
        switch(opt) {
            
            
            case 1: {
                CaseOne(x, current_user);
                break;
            }
            
            
            case 2: {
                CaseTwo(x, current_user);
                break;
            }
            
            
            case 3: {
                CaseThree(x, current_user);
                break;
            }
            
            
            case 4: {
                CaseFour(x, current_user);
                break;
            }
            
            
            case 5: {
                CaseFive(x, current_user);
                break;
            }
            
            
            case 0:
            std::cout<<"\nLEAVING\n";
            return;
            
            
            default:
            std::cout<<"\nINVALID OPTION. TRY AGAIN: \n"<<std::endl;
            continue;
            
        }
        
    }  
}




//------------------------------------------------------------------------------
int main() {
    
    UserManager object;
    object.Load();
    
    
    std::cout<<"\n-----WELCOME-----\n"<<std::endl;
    
    
    while(true) {
        
        std::string username;
        std::string password;
        
        
        
        std::cout<<"\nUSERNAME: ";
        std::getline(std::cin, username);
        username = Caps(username);
        
        if(username.empty() || username.find_first_not_of("\t\r\n") == std::string::npos) {
            std::cout<<"\nINVALID USERNAME\n";
            continue;
        }
        
        if(username.find(' ') != std::string::npos) {
            std::cout<<"\nUSERNAME CANNOT CONTAIN SPACES.\n";
            continue;
        }
        
        
        
        std::cout<<"\nPASSWORD: ";
        std::getline(std::cin, password);
        
        if(password.empty() || password.find_first_not_of("\t\r\n") == std::string::npos) {
            std::cout<<"\nINVALID PASSWORD.\n";
            continue;
        }
        
        
        if(password.find(' ') != std::string ::npos) {
            std::cout<<"\nPASSWORD CANNOT CONTAIN SPACES\n";
            continue;
        }
        
        
        User* x = object.LogIn(username, password);
        
        
        if(!x) {
            std::cout<<"\nERROR\n";
            continue;
        }
        
        
        if(Employee* y = dynamic_cast<Employee*>(x)) {
            std::cout<<"\n| LOGGED AS EMPLOYEE |\n"<<std::endl;
            ShowEmployeeMenu(object, y);
        } 
        
        else if(Supervisor* y = dynamic_cast<Supervisor*>(x)) {
            std::cout<<"\n| LOGGED AS SUPERVISOR |\n"<<std::endl;
            ShowSupervisorMenu(object, x);
            
        } else {
            std::cout<<"\nINVALID USER TYPE. DATA MIGHT BE CORRUPTED.\n"<<std::endl;
            break;
            
        }
        
    }
    
    
    return 0;
}






