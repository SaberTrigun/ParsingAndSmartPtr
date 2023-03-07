#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


class CounterCash
{
protected:
    std::string name;
    unsigned int workTime;
    unsigned int salary;
private:
    unsigned int coeffDirector;
    unsigned int coeffProgrammer;
    unsigned int coeffAssistant;

    std::string fileText;
    std::string clearText;

    unsigned int counterDelimStaff;
    unsigned int currentCursorPosition;
    const unsigned int TOTALNUMBERCOLUMN;
    std::string** tableFileText;

public:

    CounterCash() : workTime(0),
                    salary(0),
                    coeffDirector(0),
                    coeffProgrammer(0),
                    coeffAssistant(0),
                    counterDelimStaff(0),
                    currentCursorPosition(0),
                    TOTALNUMBERCOLUMN(3),
                    tableFileText(nullptr)
    {}


    CounterCash(unsigned int CoeffDir, unsigned int CoeffProg, unsigned int CoeffAssist) :
        name(),
        workTime(0),
        salary(0),
        coeffDirector(CoeffDir),
        coeffProgrammer(CoeffProg),
        coeffAssistant(CoeffAssist),
        fileText(),
        clearText(),
        counterDelimStaff(0),
        currentCursorPosition(0),
        TOTALNUMBERCOLUMN(3),
        tableFileText(nullptr)
    {}


    // Считывание текста из файла без упраляющих символов типа '\n', '\r', EOF и так далее.
    std::string readFileToStr(const char* filePath)
    {
        std::ifstream fileStream(filePath, std::ios::in);
        if(!fileStream)
        {
            std::cerr << "ERROR Open file" << std::endl;
            exit(1);
        }
        while(fileStream)
        {
            getline(fileStream, fileText);
            clearText = clearText + fileText;
        }

        return clearText;
    }


    // Считаем общее колличество записей/сотрудников в строке
    int getTotalNumberEmployees(const std::string& clearText)
    {
        for(unsigned int i = 0; i < clearText.length() ; ++i)
        {
            if(clearText[i] == ';')
            {
                ++counterDelimStaff;
            }
        }
        counterDelimStaff = counterDelimStaff / 3;

        return counterDelimStaff;
    }


    // Парсим строку в таблицу |должность|фамилия|время|
    std::string** parsingString(const std::string& clearText)
    {
        const unsigned int TOTALNUMBERLINE = counterDelimStaff;
        tableFileText = new std::string* [TOTALNUMBERLINE];
        for(int i = 0; i < TOTALNUMBERLINE; ++i)
        {
            tableFileText[i] = new std::string [TOTALNUMBERCOLUMN];
        }

        //Парсим чистую строку без спец символов clearText в таблицу tableFileText
        for(unsigned int i = 0; i < TOTALNUMBERLINE; ++i)
        {
            for(unsigned int j = 0; j < TOTALNUMBERCOLUMN; ++j) // 3
            {
                while(clearText[currentCursorPosition] != ';')
                {
                    tableFileText[i][j] = tableFileText[i][j] + clearText[currentCursorPosition];
                    ++currentCursorPosition;
                }
                ++currentCursorPosition;
            }
        }

        return tableFileText;
    }


    virtual void payRoll(CounterCash& counterCash)
    {}


    virtual void showSalary()
    {}


    int crutchDir ()
    {return coeffDirector;}


    int crutchProg ()
    {return coeffProgrammer;}


    int crutchAssist ()
    {return coeffAssistant;}


    virtual ~CounterCash()
    {
        for(int i = 0; i < counterDelimStaff; ++i)
        {
            delete [] tableFileText[i];
        }
        delete [] tableFileText;
    }
};


class Director : public CounterCash
{
public:
    Director(std::string& Name, int WorkTime)
    {
        name = Name;
        workTime = WorkTime;
    }


    void payRoll(CounterCash& counterCash)
    {
        unsigned int crutchCoeff = counterCash.crutchDir();
        salary = workTime * crutchCoeff;
    }


    void showSalary()
    {
        std::string str = std::to_string(salary);
        std::cout << name << "  " << str + "$" << std::endl;
    }


    ~Director()
    {}
};


class Programmer : public CounterCash
{
public:
    Programmer(std::string& Name, int WorkTime)
    {
        name = Name;
        workTime = WorkTime;
    }


    void payRoll(CounterCash& counterCash)
    {
        unsigned int crutchCoeff = counterCash.crutchProg();
        salary = workTime * crutchCoeff;
    }


    void showSalary()
    {
        std::string str = std::to_string(salary);
        std::cout << name << "  " << str + "$" << std::endl;
    }


    ~Programmer()
    {}
};


class Assistant : public CounterCash
{
public:
    Assistant(std::string& Name, int WorkTime)
    {
        name = Name;
        workTime = WorkTime;
    }


    void payRoll(CounterCash& counterCash)
    {
        unsigned int crutchCoeff = counterCash.crutchAssist();
        salary = workTime * crutchCoeff;
    }


    void showSalary()
    {
        std::string str = std::to_string(salary);
        std::cout << name << "  " << str + "$" << std::endl;
    }


    ~Assistant()
    {}
};


void salaryCalculation(int CounterDelimStaff, std::string& EntryPosition, CounterCash& counterCash, std::string** TableFileText, std::vector<CounterCash*>& PtrCounterCash)
{
    unsigned int convertStringToIntPay = 0;
    unsigned int index = 0;

    for (int i = 0; i < CounterDelimStaff; ++i)
    {
        if(TableFileText[i][0] == EntryPosition)
        {
            std::istringstream(TableFileText[i][2]) >> convertStringToIntPay;
            PtrCounterCash.insert(PtrCounterCash.end(), new Director(TableFileText[i][1], convertStringToIntPay));
            PtrCounterCash[index]->payRoll(counterCash);
            PtrCounterCash[index]->showSalary();
            ++index;
        }
    }

    if(index != 0)
    {
        for(int i = 0; i < index; ++i)
        {
            delete PtrCounterCash[i];
        }
        index = 0;
        PtrCounterCash.clear();
    }
}


int main()
{
    CounterCash counterCash(500, 350, 200);
    std::string textFile = counterCash.readFileToStr("employes.dat");
    int counterDelimStaff = counterCash.getTotalNumberEmployees(textFile);
    std::string** tableFileText = counterCash.parsingString(textFile);

    std::string entryPosition;
    char choice;
    std::vector<CounterCash*> ptrCounterCash;
    do
    {

        std::cout << "Enter position: ";
        std::cin >> entryPosition;

        if(entryPosition == "director")
        {
            salaryCalculation(counterDelimStaff, entryPosition, counterCash, tableFileText, ptrCounterCash);
        }
        else if(entryPosition == "programmer")
        {
            salaryCalculation(counterDelimStaff, entryPosition, counterCash, tableFileText, ptrCounterCash);
        }
        else if(entryPosition == "assistant")
        {
            salaryCalculation(counterDelimStaff, entryPosition, counterCash, tableFileText, ptrCounterCash);
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
