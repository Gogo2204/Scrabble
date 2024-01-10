#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

const constexpr char* FILE_NAME = "D:\words.txt";
const constexpr int MAX_INPUT = 100;

void fillArrWithZeros(int arr[], size_t size)
{
	for (unsigned i = 0; i < size; i++)
	{
		arr[i] = 0;
	}
}

unsigned linesInFile(const char* fileName)
{
	if (!fileName)
		return 1;

	ifstream MyFile(fileName);

	if (!MyFile.is_open())
		return 1;

	unsigned lines = 0;
	
	constexpr size_t size = 100;
	char str[size];

	while (!MyFile.eof())
	{
		MyFile.getline(str, size);
		lines++;		
	}

	MyFile.close();

	return lines;
}

char** createMatrix(unsigned rows, unsigned colls)
{
	char** mtx = new char* [rows];

	for (unsigned i = 0; i < rows; i++)
	{
		mtx[i] = new char[colls];
	}

	return mtx;
}

void freeMatrix(char** matrix, unsigned rows)
{
	for (unsigned i = 0; i < rows; i++)
	{
		delete[] matrix[i];
	}

	delete[] matrix;
}

void readWords(const char* fileName, char**& words, unsigned& rows)
{
	if (!fileName)
		return;

	rows = linesInFile(fileName);
	constexpr size_t colls = 100;
	words = createMatrix(rows, colls);

	ifstream MyFile(fileName);

	if (!MyFile.is_open())
		return;	

	for (unsigned i = 0; i < rows; i++)
	{
		MyFile.getline(words[i], colls);		
	}

	MyFile.close();	
}

int myStrcmp(const char* first, const char* second)
{
	if (!first || !second)
		return 0;

	while (*first && (*first == *second))
	{
		first++;
		second++;
	}

	return (*first - *second);
}

bool isWordInDictionary(char** dictionary, const char* word, unsigned sizeOfDictionary)
{
	if (!word)
		return 0;

	unsigned leftIndex = 0;
	unsigned rightIndex = sizeOfDictionary - 1;

	while (leftIndex <= rightIndex)
	{
		int mid = leftIndex + (rightIndex - leftIndex) / 2;

		int cmp = myStrcmp(dictionary[mid], word);

		if (cmp == 0)
		{
			return 1;
		}
		else if (cmp < 0)
		{
			leftIndex = mid + 1;
		}
		else if (cmp > 0)
		{
			rightIndex = mid - 1;
		}
	}

	return 0;
}

int random()
{	
	return (rand() % 25 + 1);
}

char getLetter(unsigned index)
{
	return 'a' + index;
}

char* generateRandomLetters(int countOfLetters, int countingArr[])
{
	srand(time(0));	

	char* str = new char[countOfLetters];

	for (unsigned i = 0; i < countOfLetters; i++)
	{
		int index = random();
		countingArr[index - 1]++; //count the letter that are print
		str[i] = getLetter(index);		
	}

	str[countOfLetters] = '\0';

	return str;
}

bool isValidWord(const char* word, int countingArr[], size_t size)
{
	if (!word)
		return 0;

	int count[26];
	fillArrWithZeros(count, size);

	while (*word)
	{		
		count['a' - *word]++;
		word++;
	}

	for (unsigned i = 0; i < 10; i++)
	{
		if (count[i])
		{
			if (count[i] != countingArr[i])
				return false;
		}
	}

	return true;
}

int main()
{
	char** dictionary;
	unsigned sizeOfDictionary;

	unsigned rounds = 10;	

	constexpr size_t size = 26;
	int countingArr[size];

	readWords(FILE_NAME, dictionary, sizeOfDictionary);
	fillArrWithZeros(countingArr, size);	

	char* gameLetters = generateRandomLetters(10, countingArr);
	while (rounds)
	{
		cout << gameLetters << endl;

		char inputWord[MAX_INPUT];
		cin >> inputWord;

		if (isValidWord(inputWord, countingArr, size) && isWordInDictionary(dictionary, inputWord, sizeOfDictionary))
		{
			rounds--;
			gameLetters = generateRandomLetters(10, countingArr);
		}
	}
}
