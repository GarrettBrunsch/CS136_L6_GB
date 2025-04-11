// Garrett, Brunsch
// Lab #6 Queue File Processing S25
// CS_136_14728SP25P
// Due 3/30/25 with resubmission due 4/13

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>
#include <stdexcept>
#include <utility>

using namespace std;

// Hard coded test files to comply with assign description
const string IDENTICAL_CASE = "in_identical.txt";
const string DIFF_DIGITS_CASE = "in_diff_digits.txt";
const string DIFF_LETTERS_CASE = "in_diff_letters.txt";
const string DIFF_SPECIAL_CASE = "in_diff_special.txt";
const string DIFF_ALL_CASE = "in_diff_all.txt";
const string EMPTY_FILE_CASE = "in_empty.txt";

const string BASE_CASE = "in_base_case.txt"; // Default for File 1
const string COMPARISON_CASE = IDENTICAL_CASE; // Default for File 2

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

struct TestCase
{
    string baseFile = "n/a";
    string testFile = "n/a";
    string testName = "n/a";
};

class Node
{
private:
    char data = '\0';
    Node* next = nullptr;

public:
    Node();
    Node(char nodeData);

    char GetData() const;
    Node* GetNext() const;

    void SetNext(Node* next);
};

class Queue
{
private:
    Node* front = nullptr;
    Node* rear = nullptr;
    int size;

    void ClearNodes();
    bool CopyNodesFrom(const Queue& other);

public:
    Queue();
    Queue(const Queue& other);
    ~Queue();

    Queue& operator=(const Queue& other);

    void Enqueue(char data);
    char Dequeue();

    bool IsEmpty() const;
    int GetSize() const;
};

class FileManager
{
private:
    bool filesProcessed = false;
    static const int TOTAL_INDEX = static_cast<int>(CharType::COUNT);
    int file1Stats[static_cast<int>(CharType::COUNT) + 1];
    int file2Stats[static_cast<int>(CharType::COUNT) + 1];

    bool LoadFileToQueue(const string& fileName, Queue& queue);
    bool CompareQueueContents(Queue& queue1, Queue& queue2);
    void CountCharTypes(const string& fileName, int stats[static_cast<int>(CharType::COUNT) + 1]);

public:
    FileManager();

    bool CompareFiles(const string& fileName1, const string& fileName2);
    void DisplayStats(const string& fileName1, const string& fileName2);

    bool IsProcessed() const;
};

void DisplayMenu();
bool ProcessCompareFiles(FileManager& fileManager, const string& fileName1, const string& fileName2);
void RunAutomatedTests(FileManager& fileManager);
void ClearInputError();

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
            try
            {
                MenuOption option = static_cast<MenuOption>(choiceInt);

                switch (option)
                {
                case MenuOption::COMPARE_FILES:
                    ProcessCompareFiles(fileManager, BASE_CASE, COMPARISON_CASE);
                    break;
                case MenuOption::DISPLAY_STATS:
                    fileManager.DisplayStats(BASE_CASE, COMPARISON_CASE);
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
            catch (const std::exception& e)
            {
                cerr << "An error occurred: " << e.what() << "\n";
            }
        }
    } while (choiceInt != static_cast<int>(MenuOption::QUIT));

    return 0;
}

Node::Node()
{
    data = '\0';
    next = nullptr;
}

Node::Node(char nodeData)
{
    data = nodeData;
    next = nullptr;
}

char Node::GetData() const
{
    return data;
}

Node* Node::GetNext() const
{
    return next;
}

void Node::SetNext(Node* nextNode)
{
    next = nextNode;
}

Queue::Queue()
{
    front = nullptr;
    rear = nullptr;
    size = 0;
}

Queue::Queue(const Queue& other)
{
    front = nullptr;
    rear = nullptr;
    size = 0;

    CopyNodesFrom(other);
}

Queue::~Queue()
{
    ClearNodes();
}

Queue& Queue::operator=(const Queue& other)
{
    if (this != &other)
    {
        Queue temp(other);
        swap(front, temp.front);
        swap(rear, temp.rear);
        swap(size, temp.size);
    }
    return *this;
}

void Queue::ClearNodes()
{
    while (front != nullptr)
    {
        Node* temp = front;
        front = front->GetNext();
        delete temp;
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
        catch (const bad_alloc&)
        {
            ClearNodes();
            success = false;
        }
    }
    return success;
}

void Queue::Enqueue(char data)
{
    Node* newNode = new Node(data);

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

char Queue::Dequeue()
{
    char result = '\0';

    if (IsEmpty())
    {
        throw runtime_error("Cannot dequeue from empty queue");
    }

    Node* temp = front;
    result = temp->GetData();
    front = front->GetNext();

    if (front == nullptr)
    {
        rear = nullptr;
    }

    delete temp;
    size--;

    return result;
}

bool Queue::IsEmpty() const
{
    return front == nullptr;
}

int Queue::GetSize() const
{
    return size;
}

FileManager::FileManager()
{
    filesProcessed = false;

    for (int i = 0; i <= static_cast<int>(CharType::COUNT); i++)
    {
        file1Stats[i] = 0;
        file2Stats[i] = 0;
    }
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
            catch (const bad_alloc& e)
            {
                cerr << "Memory allocation failed:" << e.what() << "\n";
                continueReading = false;
            }
            catch (const exception& e)
            {
                cerr << "Error during enqueue:" << e.what() << "\n";
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
    bool contentMatchStatus = false;

    if (queue1.GetSize() == queue2.GetSize())
    {
        int queueSize = queue1.GetSize();
        contentMatchStatus = true;
        int i = 0;

            while (i < queueSize && contentMatchStatus)
            {
                char char1 = '\0';
                char char2 = '\0';

                if (!queue1.IsEmpty() && !queue2.IsEmpty())
                {
                    char1 = queue1.Dequeue();
                    char2 = queue2.Dequeue();
                    contentMatchStatus = char1 == char2;
                }

                else
                {
                    contentMatchStatus = false;
                }

                i++;
            }
        }
    return contentMatchStatus;
}

bool FileManager::CompareFiles(const string& fileName1, const string& fileName2)
{
    Queue queue1;
    Queue queue2;

    bool file1Loaded = LoadFileToQueue(fileName1, queue1);
    bool file2Loaded = LoadFileToQueue(fileName2, queue2);

    if (file1Loaded && file2Loaded)
    {
        CountCharTypes(fileName1, file1Stats);
        CountCharTypes(fileName2, file2Stats);
        filesProcessed = true;
    }
    bool areIdentical = CompareQueueContents(queue1, queue2);

    return areIdentical;
}

void FileManager::CountCharTypes(const string& fileName, int stats[static_cast<int>(CharType::COUNT) +1])
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

void DisplayMenu()
{
    cout << "\n\n == Main Menu ==\n"
        << "1. Compare Files\n"
        << "2. Display File Statistics\n"
        << "3. Run Automated Tests\n"
        << "4. Quit\n"
        << "Choice: ";
}

bool ProcessCompareFiles(FileManager& fileManager, const string& fileName1, const string& fileName2)
{
    cout << "\nComparing files " << fileName1 << " and " << fileName2 << "\n";
    bool identical = fileManager.CompareFiles(fileName1, fileName2);

    if (identical)
    {
        cout << "Files are identical\n";
    }
    else
    {
        cout << "Files are different\n";
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
