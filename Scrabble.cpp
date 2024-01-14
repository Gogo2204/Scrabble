#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

const constexpr char* PATH_TO_DICTIONARY = "D:\words.txt";
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

unsigned myStrLenght(const char* str)
{
	if (!str)
		return 0;

	unsigned count = 0;

	while (*str)
	{
		count++;
		str++;		
	}

	return count;
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
	return (rand() % 26);
}

char getLetter(const unsigned index)
{
	return 'a' + index;
}

unsigned getIndexByLetter(const char letter)
{
	return letter - 'a';
}

char* generateRandomLetters(int countOfLetters, int countingArr[])
{
	srand(time(0));	

	char* str = new char[countOfLetters];

	for (unsigned i = 0; i < countOfLetters; i++)
	{
		int index = random();
		countingArr[index]++; //count of every letter that is print
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
		unsigned index = getIndexByLetter(*word);
		count[index]++;
		word++;
	}

	for (unsigned i = 0; i < size; i++)
	{
		if (count[i])
		{
			if (count[i] > countingArr[i])
				return false;
		}
	}

	return true;
}

void printGameLetters(const char* str)
{
	if (!str)
		return;

	unsigned strLenght = myStrLenght(str);

	for (unsigned i = 0; i < strLenght; i++)
	{
		cout << str[i] << " ";
	}

	cout << endl;
}

void beginNewGame()
{
	char** dictionary;
	unsigned sizeOfDictionary;

	unsigned rounds = 10;

	constexpr size_t letters = 26;
	int countingArr[letters];

	readWords(PATH_TO_DICTIONARY, dictionary, sizeOfDictionary);
	fillArrWithZeros(countingArr, letters);

	char* gameLetters = generateRandomLetters(10, countingArr);

	unsigned points = 0;

	unsigned i = 1;
	while (i <= rounds)
	{
		cout << "Round " << i << ". Available letters: ";
		printGameLetters(gameLetters);

		char inputWord[MAX_INPUT];
		cin >> inputWord;

		if (isValidWord(inputWord, countingArr, letters) && isWordInDictionary(dictionary, inputWord, sizeOfDictionary))
		{
			points += myStrLenght(inputWord);
		}
		else
		{
			cout << "Invalid word." << endl;
		}

		fillArrWithZeros(countingArr, letters);
		gameLetters = generateRandomLetters(10, countingArr);

		i++;
	}

	cout << "Your total points are " << points << "." << endl;
}

void settings() {}

void addWordToDictionary() {}

int main()
{
	bool validInput = false;
	while (!validInput)
	{
		cout << "1. Begin a new game" << endl
			<< "2. Settings" << endl
			<< "   a. Change the count of letters" << endl
			<< "   b. Change the count of rounds" << endl
			<< "3. Add a new word to the dictionary" << endl
			<< "4. Exit" << endl;

		unsigned choise = 1;
		cin >> choise;

		switch (choise)
		{
		case 1:
			beginNewGame();
			validInput = true;
			break;
		case 2:
			settings();
			validInput = true;
			break;
		case 3:
			addWordToDictionary();
			validInput = true;
			break;
		case 4:
			return 0;
			break;
		default:
			cout << "Please, choose valid choise from the menu!" << endl;
		}
	}	
}
