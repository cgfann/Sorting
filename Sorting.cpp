 /* Program 3: BubbleSort v2.3
  * Description: uses command-line arguments to populate a vector with neighborhoods from a .csv file of Airbnbs in Barcelona
    and sorts the elements in alphabetical order using bubbleSort and mergeSort algorithms; prints sorted lists to .txt output
    files and calculates and prints the runtimes for each sorting algorithm for use in subsequent run-time efficiency analysis
  * Author: Charlotte Fanning
  * Due: February 21, 2020 */
#include <iostream>
#include <vector>
#include <ctime>
#include <cassert>
#include <fstream>
#include <sstream>
using namespace std;

//mergeSort function declarations
void mergeSort(vector<string>& a, vector<string>& tmp, int left, int right);
void mergeSortedLists(vector<string>& a, vector<string>& tmp, int leftPos, int rightPos, int rightEnd);

//bubbleSort function declarations
void swap (string* a, string* b);
void bubbleSort(string* a, int n);

int main(int argc, char **argv)  //main method takes command-line arguments
{
    ifstream inFile;
    string inFileName;
    string sLength; int length;
    if (argc != 3)  //exit program if an incorrect number of command-line arguments are provided
    {
        cerr << "Format: " << argv[0] << " [fileName][int]" << endl;
        return -1;
    }
    else
    {
        inFileName = argv[1];   //input file name from first command-line argument
        sLength = argv[2]; length = atoi(sLength.c_str());  //list length from second command-line argument
    }

    inFile.open(inFileName);    //input file stream object opens the input file provided from command-line
    if (!inFile.is_open()) //exit program if the input file was not opened
    {
        cerr << "Unable to open file: " << inFileName << endl;
        return -1;
    }

    vector<string> v(length); //vector to be sorted
    vector<string> t(length); //temporary workspace

    //define a string pointer dynamically allocate an array of strings
    string* stringArray;
    stringArray = new string[length];

    ofstream outBubble;
    outBubble.open("bubble.txt"); //output file stream object opens output file to write contents of bubbleSorted list
    if (!outBubble.is_open()) //exit program if the input file was not opened
    {
        cerr << "Unable to open file: bubble.txt" << endl;
        return -1;
    }
    ofstream outMerge;
    outMerge.open("merge.txt"); //output file stream object opens output file to write contents of mergeSorted list
    if (!outMerge.is_open()) //exit program if the input file was not opened
    {
        cerr << "Unable to open file: merge.txt" << endl;
        return -1;
    }

    //populate and print input to output files
    outBubble << "Unsorted:" << endl;
    outMerge << "Unsorted:" << endl;
    string line;
    getline(inFile, line);  //first line containing attribute names is not included in the vector
    for (int i = 0; i < length; i++)
    {
        getline(inFile, line);  //line is initialized with the entire next line in the file
        istringstream streamLine(line); //initializes input string stream object with line
        string type;
        getline(streamLine, type, ','); //type is initialized with the first part of the line up to the comma
        string neighborhood;
        getline(streamLine, neighborhood, ','); //neighborhood becomes the next part of the line between commas
        v.at(i) = neighborhood; //populates the vector at element i with the neighborhood from current line
        outBubble << v.at(i) << '\t';
        outMerge << v.at(i) << '\t';
    }
    outBubble << endl;
    outMerge << endl;

    //copy the list of strings from vector to array
    for (int i = 0; i < v.size(); i++)
    {
        stringArray[i] = v.at(i);
    }

    clock_t start_mergeSort = clock();
    mergeSort(v,t, 0, v.size() - 1);    //sort vector using mergeSort
    clock_t end_mergeSort = clock();

    //check output, make sure vector is sorted after mergeSort
    for (int i = 1; i < v.size(); i++)
    {
        assert(v.at(i-1) <= v.at(i));
    }

    //unit test for swap function
    {
        string test[] = {"one", "two"};
        swap(test, test + 1);
        assert(test[0] == "two" && test[1] =="one");
    }

    clock_t start_bubbleSort = clock();
    bubbleSort(stringArray, length);    //sort array using bubbleSort
    clock_t end_bubbleSort = clock();

    //check output, make sure array is sorted after bubbleSort
    for(int i = 1; i < length; i++)
    {
        assert(stringArray[i - 1] <= stringArray[i]);
    }

    //print sorted vector after mergeSort into merge.txt
    outMerge << "\nSorted:" << endl;
    for (int i = 0; i < v.size(); i++)
    {
        outMerge << v.at(i) << '\t';
    }
    outMerge << endl;

    //print sorted array after bubbleSort into file bubble.txt
    outBubble << "\nSorted:" << endl;
    for (int i = 0; i < length; i++)
    {
        outBubble << stringArray[i] << '\t';
    }
    outBubble << endl;

    //print elapsed time for mergeSort and bubbleSort
    double elapsed_mergeSort = double(end_mergeSort - start_mergeSort) / CLOCKS_PER_SEC;
    double elapsed_bubbleSort = double(end_bubbleSort - start_bubbleSort) / CLOCKS_PER_SEC;

    outBubble << "\nbubbleSort time: " << elapsed_bubbleSort << "     mergeSort time: " << elapsed_mergeSort << endl;
    outMerge <<"\nmergeSort time: " << elapsed_mergeSort << "     bubbleSort time: " << elapsed_bubbleSort << endl;

    outBubble.close();
    outMerge.close();
    inFile.close();
    delete[] stringArray;   //deallocates memory from array of strings

    return 0;
}

void mergeSortedLists(vector<string>& a, vector<string>& tmp, int leftPos, int rightPos, int rightEnd)
{
    int leftEnd = rightPos - 1;
    int tempPos = leftPos;
    int numElements = rightEnd - leftPos + 1;

    while (leftPos <= leftEnd && rightPos <= rightEnd)
    {
        if (a[leftPos] <= a[rightPos]) {
            tmp[tempPos++] = a[leftPos++];
        }
        else
        {
            tmp[tempPos++] = a[rightPos++];
        }

    }
    
    while (leftPos <= leftEnd)
    {
        tmp[tempPos++] = a[leftPos++];
    }
    while (rightPos <= rightEnd)
    {
        tmp[tempPos++] = a[rightPos++];
    }

    for (int i = 0; i < numElements; i++, --rightEnd)
    {
        a[rightEnd] = tmp[rightEnd];
    }
}

void mergeSort(vector<string>& a, vector<string>& tmp, int left, int right)
{
    if (left < right)
    {
        int center = ( left + right ) / 2;
        mergeSort(a, tmp, left, center);
        mergeSort(a, tmp, center + 1, right);
        mergeSortedLists(a, tmp, left, center + 1, right);
    }
}

void swap(string* a, string* b)
{
    string temp = *a;   //temporary value to hold initial value of *a
    *a = *b;
    *b = temp;  //a* now has the value of *b, *b becomes the original value of *a
}

void bubbleSort(string* a, int n)
{
    bool sorted;
    do  //goes through the list at least once
    {
        sorted = true;
        //pushes the next largest element to the index before the last largest element
        for (int i = 0; i < n - 1; i++)
        {
            if (a[i] > a[i + 1])    //compares adjacent elements
            {
                swap(a + i, a + i + 1); //sends pointers to values that are being swapped
                sorted = false;
            }
        }
    } while (!sorted); //boolean remains true when the list requires no more swaps
}