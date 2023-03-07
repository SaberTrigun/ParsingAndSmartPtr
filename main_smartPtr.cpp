#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>

struct Employee
{
    std::string position;
    std::string name;
    std::string workTime;

    Employee() : position(), name(), workTime()
    {}

    Employee(const std::string& Position, const std::string& Name, const std::string& WorkTime) :
        position(Position),
        name(Name),
        workTime(WorkTime)
    {}

    void show()
    {
        std::cout << position << '\n';
        std::cout << name << '\n';
        std::cout << workTime << '\n';
    }
};


class CounterCash
{
protected:

    std::string nameEmp;
    unsigned int workTimeEmp;
    unsigned int salaryEmp;

private:

    unsigned int coeffDirector;
    unsigned int coeffProgrammer;
    unsigned int coeffAssistant;

    unsigned int counterDelimStaff;

    std::string fileText;
    std::string clearStr;
    std::vector<std::unique_ptr<Employee>> uniquePtrEmp;
    std::unique_ptr<std::string[]> uniquePtrClearText;

public:

    CounterCash() : nameEmp(),
                    workTimeEmp(0),
                    salaryEmp(0),
                    coeffDirector(0),
                    coeffProgrammer(0),
                    coeffAssistant(0),
                    counterDelimStaff(0),
                    fileText(),
                    clearStr(),
                    uniquePtrClearText(new std::string[3])
    {}

    CounterCash(unsigned int coeffDir, unsigned int coeffProg, unsigned int coeffAssist) :
        nameEmp(),
        workTimeEmp(0),
        salaryEmp(0),
        coeffDirector(coeffDir),
        coeffProgrammer(coeffProg),
        coeffAssistant(coeffAssist),
        counterDelimStaff(0),
        fileText(),
        clearStr(),
        uniquePtrClearText(new std::string[3])
    {}


    CounterCash(const std::string& Name, const int WorkTime) : nameEmp(Name), workTimeEmp(WorkTime)
    {}


    //Считываем текст из файлов без спец вимволов типа '\n' и считаем количество записей
    std::string readFile(const char* filePath)
    {
        std::ifstream fileStream(filePath, std::ios::in);
        if(!fileStream)
        {
            std::cout << "Error Open file" << std::endl;
            exit(1);
        }
        while(fileStream)
        {
            if(getline(fileStream, fileText, '\n'))
            {
                clearStr += fileText;
                ++counterDelimStaff;
            }
        }

        return clearStr;
    }


    //Парсим строку заполняя структуру Employee, добавляя каждый экземпляр структуры Employee в вектор со смарт поинтерами
    std::vector<std::unique_ptr<Employee>> parsingString(const std::string& clearStr)
    {
        unsigned int currentCursorPosition = 0;

        for(unsigned int i = 0; i < counterDelimStaff; ++i)
        {
            for(unsigned int j = 0; j < 3; ++j)
            {
                while(clearStr[currentCursorPosition] != ';')
                {
                    uniquePtrClearText[j] = uniquePtrClearText[j] + clearStr[currentCursorPosition];
                    ++currentCursorPosition;
                }
                ++currentCursorPosition;
            }
            uniquePtrEmp.emplace_back(new Employee(uniquePtrClearText[0], uniquePtrClearText[1], uniquePtrClearText[2]));
            uniquePtrClearText[0].clear();
            uniquePtrClearText[1].clear();
            uniquePtrClearText[2].clear();
        }

        return std::move(uniquePtrEmp);
    }


    int crutchDir () const
    {return coeffDirector;}


    int crutchProg () const
    {return coeffProgrammer;}


    int crutchAssist () const
    {return coeffAssistant;}


    virtual void payRoll(CounterCash& counterCash)
    {}


    virtual void showSalary()
    {}


    virtual ~CounterCash() = default;
};


class Director : public CounterCash
{
public:

    Director(const std::string& Name, const unsigned int WorkTime) : CounterCash(Name, WorkTime)
    {}


    void payRoll(CounterCash& counterCash)
    {
        unsigned int crutchCoeff = counterCash.crutchDir();
        salaryEmp = workTimeEmp * crutchCoeff;
    }


    void showSalary()
    {
        std::string str = std::to_string(salaryEmp);
        std::cout << nameEmp << "  " << str + "$" << std::endl;
    }


    ~Director()
    {}
};


class Programmer : public CounterCash
{
public:

    Programmer(const std::string& Name, const unsigned int WorkTime) : CounterCash(Name, WorkTime)
    {}


    void payRoll(CounterCash& counterCash)
    {
        unsigned int crutchCoeff = counterCash.crutchProg();
        salaryEmp = workTimeEmp * crutchCoeff;
    }


    void showSalary()
    {
        std::string str = std::to_string(salaryEmp);
        std::cout << nameEmp << "  " << str + "$" << std::endl;
    }


    ~Programmer()
    {}
};


class Assistant : public CounterCash
{
public:
    Assistant(const std::string& Name, const unsigned int WorkTime) : CounterCash(Name, WorkTime)
    {}


    void payRoll(CounterCash& counterCash)
    {
        unsigned int crutchCoeff = counterCash.crutchAssist();
        salaryEmp = workTimeEmp * crutchCoeff;
    }


    void showSalary()
    {
        std::string str = std::to_string(salaryEmp);
        std::cout << nameEmp << "  " << str + "$" << std::endl;
    }


    ~Assistant()
    {}
};

void salaryCalculation(const std::string& EntryPos,
                       const unsigned int sizeUniquePtrCounterCash,
                       CounterCash& counterCash,
                       std::vector<std::unique_ptr<Employee>>& UniquePtrEmployee,
                       std::vector<std::unique_ptr<CounterCash>>& UniquePtrCounterCash)
{
    unsigned int index = 0;
    unsigned int convertStringToIntPay = 0;

    for(int i = 0; i < sizeUniquePtrCounterCash; ++i)
    {
        if(UniquePtrEmployee[i]->position == EntryPos && EntryPos == "director")
        {
            std::istringstream(UniquePtrEmployee[i]->workTime) >> convertStringToIntPay;
            UniquePtrCounterCash.emplace_back(new Director(UniquePtrEmployee[i]->name, convertStringToIntPay));
            UniquePtrCounterCash[index]->payRoll(counterCash);
            UniquePtrCounterCash[index]->showSalary();
            ++index;
        }
        else if(UniquePtrEmployee[i]->position == EntryPos && EntryPos == "programmer")
        {
            std::istringstream(UniquePtrEmployee[i]->workTime) >> convertStringToIntPay;
            UniquePtrCounterCash.emplace_back(new Programmer(UniquePtrEmployee[i]->name, convertStringToIntPay));
            UniquePtrCounterCash[index]->payRoll(counterCash);
            UniquePtrCounterCash[index]->showSalary();
            ++index;
        }
        else if(UniquePtrEmployee[i]->position == EntryPos && EntryPos == "assistant")
        {
            std::istringstream(UniquePtrEmployee[i]->workTime) >> convertStringToIntPay;
            UniquePtrCounterCash.emplace_back(new Assistant(UniquePtrEmployee[i]->name, convertStringToIntPay));
            UniquePtrCounterCash[index]->payRoll(counterCash);
            UniquePtrCounterCash[index]->showSalary();
            ++index;
        }
    }
    if(index != 0)
    {
        index = 0;
        UniquePtrCounterCash.clear();
    }
}



int main()
{
    CounterCash counterCash(500, 350, 200);
    std::string fileTxt = counterCash.readFile("employes.dat");
    std::vector<std::unique_ptr<Employee>> uniquePtrEmployee = counterCash.parsingString(fileTxt);
    unsigned int size = uniquePtrEmployee.size();

    std::string entryPosition;
    char choice;
    std::vector<std::unique_ptr<CounterCash>> uniquePtrCounterCash;
    do
    {
        std::cout << "Enter position: ";
        std::cin >> entryPosition;


        if(entryPosition == "director")
        {
            salaryCalculation(entryPosition, size, counterCash, uniquePtrEmployee, uniquePtrCounterCash);
        }
        else if(entryPosition == "programmer")
        {
            salaryCalculation(entryPosition, size, counterCash, uniquePtrEmployee, uniquePtrCounterCash);
        }
        else if(entryPosition == "assistant")
        {
            salaryCalculation(entryPosition, size, counterCash, uniquePtrEmployee, uniquePtrCounterCash);
        }
        else
        {
            std::cout << "Unknown position " << std::endl;
        }


        std::cout << "Choose another position or finish (y/n)?: ";
        std::cin >> choice;
    }
    while(choice == 'y');


    return 0;
}
