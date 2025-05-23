#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

typedef unordered_map<string, vector<string>> taskmap;
typedef unordered_map<string, vector<string>> datemap;

typedef string header;
const header TODAY = "---TODAY---";
const header WEEK = "---WEEK---";
const header MONTH = "---MONTH---";

typedef string datelabel;
const datelabel MONDAY = "mon";
const datelabel TUESDAY = "tue";
const datelabel WEDNESDAY = "wed";
const datelabel THURSDAY = "thu";
const datelabel FRIDAY = "fri";
const datelabel SATURDAY = "sat";
const datelabel SUNDAY = "sun";


bool LineIsHeader(const string &line) 
{
    if (line == TODAY) return true;
    else if (line == WEEK) return true;
    else if (line == MONTH) return true;
    else return false;
}

void MoveTasks(taskmap &map, const header &from, const header &to) 
{
    for (int i = 0; i < map[from].size(); i++) 
    {
        map[to].push_back(map[from][i]);
    }

    map[from].clear();
    map[from].push_back(""); //This is so there's a blank line between headers
}

void AddTaskToSection(taskmap &map, const header &section, const string &task) 
{
    map[section].push_back(task);
}

taskmap ParseTodoList(ifstream &file) 
{
    taskmap taskMap {};

    string input {};
    header header = "";
    while (getline(file, input))
    {
        if (input == "" || input[0] == '#')
            continue;
            
        if (LineIsHeader(input)) 
        {
            header = input;
            taskMap[input] = {};
            continue;
        }
        
        if (header == "")
            continue;

        taskMap[header].push_back(input);
    }

    file.close();

    return taskMap;
}

datemap ParseDateFile(ifstream &file) 
{
    vector<datelabel> allDateLabels = {
        MONDAY,
        TUESDAY,
        WEDNESDAY,
        THURSDAY,
        FRIDAY,
        SATURDAY,
        SUNDAY
    };

    datemap dateMap {};

    string input {};
    header header = "";
    while (getline(file, input))
    {
        if (input == "" || input[0] == '#')
            continue;
            
        if (input[0] == '[') 
        {
            //Days
            for(auto label : allDateLabels)
            {
                if (input.substr(1, 3) == label) 
                {
                    dateMap[label].push_back(input);
                }
            }
        }
    }

    file.close();

    return dateMap;
}

//TODO: figure out how to launch a text editor in c++

int main() 
{ 
    //parse todo file
    ifstream todoFile { "ExampleTodo\\Todo.txt" };
    if (!todoFile) 
    {
        cerr << "Couldn't open the todo file!\n";
        return 1;
    }
    taskmap taskMap = ParseTodoList(todoFile);

    //parse date file
    ifstream dateFile { "ExampleTodo\\datetasks.txt" };
    if (!dateFile) 
    {
        cerr << "Couldn't open the date file!\n";
        return 1;
    }
    datemap dateMap = ParseDateFile(dateFile);

    return 0;
}
