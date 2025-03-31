// Garrett, Brunsch
// Lab #6 Queue File Processing S25
// CS_136_14728SP25P
// Due 3/30/25

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>

using namespace std;

// Hard coded test files to comply with assign description
const string IDENTICAL_CASE = "in_identical.txt";
const string DIFF_DIGITS_CASE = "in_diff_digits.txt";
const string DIFF_LETTERS_CASE = "in_diff_letters.txt";
const string DIFF_SPECIAL_CASE = "in_diff_special.txt";
const string DIFF_ALL_CASE = "in_diff_all.txt";
const string EMPTY_FILE_CASE = "in_empty.txt";

const string BASE_CASE = "in_base_case.txt"; // Default for File 1
const string COMPARISON_CASE = IDENTICAL_CASE; // Default for File 2. Second default is only relevant for manual testing

enum class MenuOption
{
    COMPARE_FILES = 1,
    DISPLAY_STATS,
    RUN_AUTOMATED_TESTS,
    QUIT
};

enum class CharType
{
    DIGIT = 0,
    LETTER,
    SPECIAL,
    COUNT
};

//Struct used exclusively to assist with automated test cases. 
// This is my first time playing with more automated testing so I'd appreciate feedback on if this is over-engineering 
struct TestCase 
{
    string baseFile = "n/a";
    string testFile = "n/a";
    string testName = "n/a";
};

class Node;
class Queue;
class FileManager;

class Node
{
public:
    Node(char data);

    char GetData() const { return data;}
    Node* GetNext() const { return next;}
    void SetNext(Node* next);

private:
    char data = '\0';
    Node* next = nullptr;
};

class Queue
{
public:
    Queue();
    Queue(const Queue& other);
    Queue& operator=(const Queue& other);
    ~Queue();

    void Enqueue(char data);
    char Dequeue();

    bool IsEmpty() const { return front == nullptr; }
    int GetSize() const { return size; }

private:
    void ClearNodes();
    bool CopyNodesFrom(const Queue& other);

    Node* front = nullptr;
    Node* rear = nullptr;
    int size = 0;
};

class FileManager
{
public:
    FileManager();
    bool CompareFiles(const string& fileName1, const string& fileName2);
    void DisplayStats(const string& fileName1 = BASE_CASE, const string& fileName2 = COMPARISON_CASE);
    bool IsProcessed() const;

private:
    bool LoadFileToQueue(const string& fileName, Queue& queue);
    bool CompareQueueContents(Queue& queue1, Queue& queue2);

    void CountCharTypes(const string& fileName, int stats[static_cast<int>(CharType::COUNT)]);

    bool filesProcessed = false;
    int file1Stats[static_cast<int>(CharType::COUNT) + 1];
    int file2Stats[static_cast<int>(CharType::COUNT) + 1];

    static const int TOTAL_INDEX = static_cast<int>(CharType::COUNT);
};

void DisplayMenu();
bool ProcessCompareFiles(FileManager& fileManager, const string& fileName1 = BASE_CASE, const string& fileName2 = COMPARISON_CASE);
void RunAutomatedTests(FileManager& fileManager);
void ClearInputError();

Node::Node(char data) : data(data), next(nullptr)
{
}

void Node::SetNext(Node* next)
{
    this->next = next;
}

Queue::Queue() : front(nullptr), rear(nullptr), size(0)
{
}

void Queue::ClearNodes()
{
    Node* current = front;
    while (current != nullptr)
    {
        Node* nextNode = current->GetNext();
        delete current;
        current = nextNode;
    }

    front = nullptr;
    rear = nullptr;
    size = 0;
}

bool Queue::CopyNodesFrom(const Queue& other)
{
    bool success = true;
    Node* current = other.front;

    while (current != nullptr && success)
    {
        try
        {
            Enqueue(current->GetData());
            current = current->GetNext();
        }
        catch (const bad_alloc& e)
        {
            success = false;
        }
    }
    return success;
}

Queue::Queue(const Queue& other) : front(nullptr), rear(nullptr), size(0)
{
    bool success = CopyNodesFrom(other);

    if (!success)
    {
        ClearNodes();
        throw bad_alloc();
    }
}

Queue& Queue::operator=(const Queue& other)
{
    bool success = true;

    if (this != &other)
    {
        Queue temp;
        success = temp.CopyNodesFrom(other);

        if (success)
        {
            ClearNodes();

            front = temp.front;
            rear = temp.rear;
            size = temp.size;

            temp.front = nullptr;
            temp.rear = nullptr;
            temp.size = 0;
        }
        else
        {
            throw bad_alloc();
        }
    }
    return *this;
}

Queue::~Queue()
{
    ClearNodes();
}

void Queue::Enqueue(char data)
{
    Node* newNode = nullptr;
    bool success = false;

    try
    {
        newNode = new Node(data);
        success = true;
    }
    catch (const bad_alloc& e)
    {
        success = false;
    }

    if (success)
    {
        if (IsEmpty())
        {
            front = newNode;
            rear = newNode;
        }
        else
        {
            rear->SetNext(newNode);
            rear = newNode;
        }

        size++;
    }
    else
    {
        throw bad_alloc();
    }
}

char Queue::Dequeue()
{
    char result = '\0';

    if (IsEmpty())
    {
        throw runtime_error("Cannot dequeue from empty queue");
    }
    else
    {
        Node* temp = front;
        result = temp->GetData();
        front = front->GetNext();

        if (front == nullptr)
        {
            rear = nullptr;
        }

        delete temp;
        size--;
    }
    return result;
}

FileManager::FileManager(): filesProcessed(false),file1Stats{0}, file2Stats{0}
{
}



bool FileManager::LoadFileToQueue(const string& fileName, Queue& queue)
{
    bool success = false;
    ifstream file(fileName);

    if (file.is_open())
    {
        char ch = '\0';
        bool continueReading = true;

        while (file.get(ch) && continueReading)
        {
            try
            {
                queue.Enqueue(ch);
            }
            catch (const exception& e)
            {
                cerr << "Memory allocation failed:" << e.what() << "\n";
                continueReading = false;
            }
        }

        file.close();
        success = continueReading;
    }
    else
    {
        cerr << "Error Could not open file " << fileName << "\n";
    }
    return success;
}

bool FileManager::CompareQueueContents(Queue& queue1, Queue& queue2)
{
    bool areIdentical = false;

    if (queue1.GetSize() == queue2.GetSize())
    {
        int queueSize = queue1.GetSize();
        bool contentMatches = true;
        bool exceptionOccurred = false;
        int i = 0;

        while (i < queueSize && contentMatches && !exceptionOccurred)
        {
            char char1 = '\0';
            char char2 = '\0';

            try
            {
                char1 = queue1.Dequeue();
                char2 = queue2.Dequeue();
                contentMatches = char1 == char2;
            }
            catch (const exception& e)
            {
                cerr << "Error when comparing characters " << e.what() << "\n";
                exceptionOccurred = true;
            }

            i++;
        }

        areIdentical = contentMatches && !exceptionOccurred;
    }
    return areIdentical;
}

bool FileManager::CompareFiles(const string& fileName1, const string& fileName2)
{
    Queue queue1;
    Queue queue2;
    bool areIdentical = false;
    bool file1Loaded = false;
    bool file2Loaded = false;

    file1Loaded = LoadFileToQueue(fileName1, queue1);
    file2Loaded = LoadFileToQueue(fileName2, queue2);

    if (file1Loaded && file2Loaded)
    {
        CountCharTypes(fileName1, file1Stats);
        CountCharTypes(fileName2, file2Stats);
        filesProcessed = true;

        areIdentical = CompareQueueContents(queue1, queue2);
    }
    return areIdentical;
}

void FileManager::CountCharTypes(const string& fileName, int stats[static_cast<int>(CharType::COUNT)])
{
    for (int i = 0; i <= static_cast<int>(CharType::COUNT); i++)
    {
        stats[i] = 0;
    }

    ifstream file(fileName);

    if (file.is_open())
    {
        char ch = '\0';
        while (file.get(ch))
        {
            if (isdigit(ch))
            {
                stats[static_cast<int>(CharType::DIGIT)]++;
            }
            else if (isalpha(ch))
            {
                stats[static_cast<int>(CharType::LETTER)]++;
            }
            else
            {
                stats[static_cast<int>(CharType::SPECIAL)]++;
            }

            stats[static_cast<int>(CharType::COUNT)]++;
        }

        file.close();
    }
    else
    {
        cerr << "Error: Could not open file " << fileName << " for statistics\n";
    }
}

void FileManager::DisplayStats(const string& fileName1, const string& fileName2)
{
    if (!filesProcessed)
    {
        cout << "Files must be compared before displaying the values\n";
    }
    else
    {
        // Headers
        cout << left << setw(20) << "File Name"
            << right << setw(12) << "Digits"
            << right << setw(12) << "Letters"
            << right << setw(12) << "Special"
            << right << setw(12) << "Total" << "\n"
            << setfill('-') << setw(68) << "-" << setfill(' ') << "\n"

            << left << setw(20) << fileName1
            << right << setw(12) << file1Stats[static_cast<int>(CharType::DIGIT)]
            << right << setw(12) << file1Stats[static_cast<int>(CharType::LETTER)]
            << right << setw(12) << file1Stats[static_cast<int>(CharType::SPECIAL)]
            << right << setw(12) << file1Stats[TOTAL_INDEX] << "\n"

            << left << setw(20) << fileName2
            << right << setw(12) << file2Stats[static_cast<int>(CharType::DIGIT)]
            << right << setw(12) << file2Stats[static_cast<int>(CharType::LETTER)]
            << right << setw(12) << file2Stats[static_cast<int>(CharType::SPECIAL)]
            << right << setw(12) << file2Stats[TOTAL_INDEX] << "\n";
    }
}

bool FileManager::IsProcessed() const
{
    return filesProcessed;
}

int main()
{
    FileManager fileManager;
    int choiceInt = 0;

    do
    {
        DisplayMenu();
        cin >> choiceInt;

        if (cin.fail())
        {
            ClearInputError();
        }
        else
        {
            MenuOption option = static_cast<MenuOption>(choiceInt);

            switch (option)
            {
            case MenuOption::COMPARE_FILES:
                ProcessCompareFiles(fileManager);
                break;
            case MenuOption::DISPLAY_STATS:
                fileManager.DisplayStats();
                break;
            case MenuOption::RUN_AUTOMATED_TESTS:
                RunAutomatedTests(fileManager);
                break;
            case MenuOption::QUIT:
                cout << "\nNow exiting program...\n\n";
                break;
            default:
                cout << "Invalid choice. Please select a valid menu option";
                break;
            }
        }
    } while (choiceInt != static_cast<int>(MenuOption::QUIT));

    return 0;
}

void DisplayMenu()
{
    cout << "\n\n == Main Menu ==\n"
    "1. Compare Files\n"
    "2. Display File Statistics\n"
    "3. Run Automated Tests\n"
    "4. Quit\n"
    "Choice: ";
}

bool ProcessCompareFiles(FileManager& fileManager, const string& fileName1, const string& fileName2)
{
    cout << "\nComparing files " << fileName1 << " and " << fileName2 << "\n";
    bool identical = fileManager.CompareFiles(fileName1, fileName2);

    if (identical)
    {
        cout << "Files are identical";
    }
    else
    {
        cout << "Files are different";
    }

    return identical;
}

void RunAutomatedTests(FileManager& fileManager)
{
    const TestCase testCases[] = 
    {
        {BASE_CASE, IDENTICAL_CASE, "Identical Files"},
        {BASE_CASE, DIFF_DIGITS_CASE, "Different Digits"},
        {BASE_CASE, DIFF_LETTERS_CASE, "Different Letters"},
        {BASE_CASE, DIFF_SPECIAL_CASE, "Different Special Chars"},
        {BASE_CASE, DIFF_ALL_CASE, "Completely Different"},
        {BASE_CASE, EMPTY_FILE_CASE, "Empty File"}
    };

    const int testCount = sizeof(testCases) / sizeof(testCases[0]);

    for (int i = 0; i < testCount; i++) 
    {
        const TestCase& test = testCases[i];

        bool result = ProcessCompareFiles(fileManager, test.baseFile, test.testFile);
        fileManager.DisplayStats(test.baseFile, test.testFile);
        cout << "\n";
    }
}

void ClearInputError()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid choice. Please enter a valid number: ";
}

/*

 == Main Menu ==
1. Compare Files
2. Display File Statistics
3. Run Automated Tests
4. Quit
Choice: g
Invalid choice. Please enter a valid number:

 == Main Menu ==
1. Compare Files
2. Display File Statistics
3. Run Automated Tests
4. Quit
Choice: 2
Files must be compared before displaying the values


 == Main Menu ==
1. Compare Files
2. Display File Statistics
3. Run Automated Tests
4. Quit
Choice: 1

Comparing files in_base_case.txt and in_identical.txt
Files are identical

 == Main Menu ==
1. Compare Files
2. Display File Statistics
3. Run Automated Tests
4. Quit
Choice: 2
File Name                 Digits     Letters     Special       Total
--------------------------------------------------------------------
in_base_case.txt               6          56          31          93
in_identical.txt               6          56          31          93


 == Main Menu ==
1. Compare Files
2. Display File Statistics
3. Run Automated Tests
4. Quit
Choice: 3

Comparing files in_base_case.txt and in_identical.txt
Files are identicalFile Name                 Digits     Letters     Special       Total
--------------------------------------------------------------------
in_base_case.txt               6          56          31          93
in_identical.txt               6          56          31          93


Comparing files in_base_case.txt and in_diff_digits.txt
Files are differentFile Name                 Digits     Letters     Special       Total
--------------------------------------------------------------------
in_base_case.txt               6          56          31          93
in_diff_digits.txt            10          56          32          98


Comparing files in_base_case.txt and in_diff_letters.txt
Files are differentFile Name                 Digits     Letters     Special       Total
--------------------------------------------------------------------
in_base_case.txt               6          56          31          93
in_diff_letters.txt            6          86          36         128


Comparing files in_base_case.txt and in_diff_special.txt
Files are differentFile Name                 Digits     Letters     Special       Total
--------------------------------------------------------------------
in_base_case.txt               6          56          31          93
in_diff_special.txt            6          56          37          99


Comparing files in_base_case.txt and in_diff_all.txt
Files are differentFile Name                 Digits     Letters     Special       Total
--------------------------------------------------------------------
in_base_case.txt               6          56          31          93
in_diff_all.txt               10          92          44         146


Comparing files in_base_case.txt and in_empty.txt
Files are differentFile Name                 Digits     Letters     Special       Total
--------------------------------------------------------------------
in_base_case.txt               6          56          31          93
in_empty.txt                   0           0           0           0



 == Main Menu ==
1. Compare Files
2. Display File Statistics
3. Run Automated Tests
4. Quit
Choice: 5
Invalid choice. Please select a valid menu option

 == Main Menu ==
1. Compare Files
2. Display File Statistics
3. Run Automated Tests
4. Quit
Choice: 4

Now exiting program...

Missing File example:


 == Main Menu ==
1. Compare Files
2. Display File Statistics
3. Run Automated Tests
4. Quit
Choice: 1

Comparing files missing_file_example.txt and missing_identical_example
Error Could not open file missing_file_example.txt
Error Could not open file missing_identical_example
Files are different


*/