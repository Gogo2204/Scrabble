/**
*
* Solution to course project # 7
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Georgi Ivanov Ivanov
* @idnumber 0MI0600286
* @compiler VC
*
* main file of the project
*
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <ctime>
using namespace std;

const constexpr char* PATH_TO_DICTIONARY = "D:words.txt";
const constexpr char* PATH_TO_GAME_SETTINGS = "D:settings.txt";
const constexpr int MAX_INPUT = 100;
const constexpr int MIN_ROUNDS = 3;
const constexpr int MAX_ROUNDS = 30;

/****************************** Basic functions ******************************/
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

void myStrcpy(const char* source, char* dest)
{
	if (!source)
		return;

	while (*source)
	{
		*dest = *source;
		dest++;
		source++;
	}

	*dest = '\0';
}

void initArrWithZeros(int* arr, size_t size)
{
	for (unsigned i = 0; i < size; i++)
	{
		arr[i] = 0;
	}
}

/****************************** Create dictionary ******************************/
void getDimOfDictionary(const char* fileName, unsigned& rows, unsigned& colls)
{
	if (!fileName)
		return;

	ifstream MyFile(fileName);

	if (!MyFile.is_open())
		return;

	MyFile >> rows; //on the first row we store the number of rows in the file
	MyFile >> colls; // on the second row we store the lenght of the longest word in the file

	MyFile.close();
}

void readWords(const char* fileName, char** dictionary, unsigned rows, unsigned colls)
{
	if (!fileName)
		return;

	ifstream MyFile(fileName);

	if (!MyFile.is_open())
		return;

	unsigned lines = 0;
	unsigned longestWord = 0;
	MyFile >> lines;
	MyFile >> longestWord; // skip the first two lines in the text file, because we already know them

	for (unsigned i = 0; i < rows; i++)
	{
		MyFile >> dictionary[i];
	}

	MyFile.close();
}

/****************************** Generate game letters ******************************/
int getRandomLetter()
{
	return (rand() % 26);
}

char getLetterByIndex(const unsigned index)
{
	return 'a' + index;
}

unsigned getIndexByLetter(const char letter)
{
	return letter - 'a';
}

void generateRandomLetters(char* str, unsigned countOfLetters, int* countGameLetters)
{
	srand(time(0));

	for (unsigned i = 0; i < countOfLetters; i++)
	{
		int index = getRandomLetter();
		countGameLetters[index]++; //store the count of every game generated letter 
		str[i] = getLetterByIndex(index);
	}

	str[countOfLetters] = '\0';
}

/****************************** Validations ******************************/
bool isLower(const char ch)
{
	return (ch >= 'a' && ch <= 'z');
}

bool areLowerLetters(const char* str)
{
	if (!str)
		return 0;

	while (*str)
	{
		if (!isLower(*str))
			return false;
		str++;
	}

	return true;
}

bool isWordInDictionary(char** dictionary, const char* word, unsigned sizeOfDictionary)
{
	if (!word || !dictionary)
		return 0;

	//using binary search to check if a word is in the dictionary
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

bool isSubsetOfGameLetters(const char* word, int* countGameLetters, size_t size)
{
	if (!word)
		return 0;

	const constexpr size_t letters = 26; //letters in EN alphabet
	int countInputWord[letters];
	initArrWithZeros(countInputWord, size);

	while (*word)
	{
		unsigned index = getIndexByLetter(*word);
		countInputWord[index]++;
		word++;
	}

	for (unsigned i = 0; i < size; i++)
	{
		if (countInputWord[i] && countInputWord[i] > countGameLetters[i])
			return false;
	}

	return true;
}

/****************************** Settings ******************************/
void readFromSettingFile(const char* fileName, int* settings, size_t size)
{
	if (!fileName)
		return;

	ifstream MyFile(fileName);

	if (!MyFile.is_open())
		return;

	for (unsigned i = 0; i < size; i++)
	{
		MyFile >> settings[i];
	}

	MyFile.close();
}

void writeInSettingFile(const char* fileName, const int* settings, size_t size)
{
	if (!fileName)
		return;

	ofstream MyFile(fileName);

	if (!MyFile.is_open())
		return;

	for (unsigned i = 0; i < size; i++)
	{
		MyFile << settings[i] << endl;
	}

	MyFile.close();
}

void changeGameLettersCount(int* settings, size_t size)
{
	readFromSettingFile(PATH_TO_GAME_SETTINGS, settings, size);

	cout << "\nPlease, enter enter new count of game letters! The old one is " << settings[0] << endl;
	cin >> settings[0];

	while (!(settings[0] >= 1 && settings[0] <= MAX_INPUT))
	{
		cout << "\nPlease, enter valid count for the game letters! Consider that the maximum count is 100" << endl;
		cin >> settings[0];
	}

	writeInSettingFile(PATH_TO_GAME_SETTINGS, settings, size);
}

void changeRoundsCount(int* settings, size_t size)
{
	readFromSettingFile(PATH_TO_GAME_SETTINGS, settings, size);

	cout << "\nPlease, enter enter new count of rounds! The old one is " << settings[1] << endl;
	cin >> settings[1];

	while (!(settings[1] >= MIN_ROUNDS && settings[1] <= MAX_ROUNDS))
	{
		cout << "\nPlease, enter valid count for the rounds! Consider that the minimum rounds are " << MIN_ROUNDS
			<< " and the maximum are " << MAX_ROUNDS << endl;
		cin >> settings[1];
	}

	writeInSettingFile(PATH_TO_GAME_SETTINGS, settings, size);
}

void loadSettingMenu()
{
	const constexpr size_t countOfSettings = 2;
	int settings[countOfSettings];

	cout << "\n2. Settings" << endl
		<< "   1. Change the count of letters" << endl
		<< "   2. Change the count of rounds" << endl
		<< "\nPlease, choose the setting you would like to change!" << endl;

	bool hasChooen = false;
	while (!hasChooen)
	{
		unsigned choice = 1;
		cin >> choice;

		switch (choice)
		{
		case 1:
			changeGameLettersCount(settings, countOfSettings);
			hasChooen = true;
			break;
		case 2:
			changeRoundsCount(settings, countOfSettings);
			hasChooen = true;
			break;
		default:
			cout << "\nPlease, choose valid option from the menu!" << endl;
			break;
		}
	}
}

/****************************** Add new word ******************************/
bool writeNewWordInDictionary(const char* fileName, char** dictionary, unsigned rows, unsigned colls,
	unsigned index, const char* newWord)
{
	if (!dictionary || !newWord)
		return false;

	ofstream MyFile(fileName);

	if (!MyFile.is_open())
		return false;

	MyFile << rows << endl;
	MyFile << colls << endl;

	for (unsigned i = 0, j = 0; i < rows - 1; i++, j++)
	{
		if (j != index)
		{
			MyFile << dictionary[i] << endl;
		}
		else
		{
			MyFile << newWord << endl;
			i--;
		}
	}

	MyFile.close();

	return true;
}

int findIndexOfNewWord(char** dictionary, unsigned rows, const char* newWord)
{
	if (!newWord || !dictionary)
		return 0;

	//using binary search to find the index on which should be the new word
	unsigned leftIndex = 0;
	unsigned rightIndex = rows - 1;

	while (leftIndex <= rightIndex)
	{
		int mid = leftIndex + (rightIndex - leftIndex) / 2;

		int cmp1 = myStrcmp(dictionary[mid], newWord);
		int cmp2 = myStrcmp(dictionary[mid + 1], newWord);

		if (cmp1 < 0 && cmp2 > 0)
		{
			return mid + 1; // the index is mid + 1, because the new word is 'smaller' than the word on position mid
		}
		else if (cmp1 < 0 && cmp2 < 0)
		{
			leftIndex = mid + 1;
		}
		else if (cmp1 > 0 && cmp2 > 0)
		{
			rightIndex = mid - 1;
		}
	}

	return 0;
}

void addWordToDictionary()
{
	unsigned rowsInDictionary = 0;
	unsigned collsInDictionary = 0;
	getDimOfDictionary(PATH_TO_DICTIONARY, rowsInDictionary, collsInDictionary);

	char** dictionary = createMatrix(rowsInDictionary, collsInDictionary);
	readWords(PATH_TO_DICTIONARY, dictionary, rowsInDictionary, collsInDictionary);

	cout << "\nPlease, enter a new word to include in dictionary" << endl;

	char newWord[MAX_INPUT];

	bool isValidInput = false;
	while (!isValidInput)
	{
		//get only the needed letters(the user can not enter more than 100 symbols)
		cin >> setw(static_cast <streamsize> (MAX_INPUT)) >> newWord;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		if (!newWord || !areLowerLetters(newWord) || isWordInDictionary(dictionary, newWord, rowsInDictionary))
		{
			cout << "\nThe entered word is either invalid or already exist in the dictionary!" << endl;
		}
		else
		{
			isValidInput = true;
		}
	}

	unsigned lenghtOfNewWord = myStrLenght(newWord) + 1; //plus one because of '\0'
	if (lenghtOfNewWord > collsInDictionary)
		collsInDictionary = lenghtOfNewWord;

	int index = findIndexOfNewWord(dictionary, rowsInDictionary, newWord);

	if (writeNewWordInDictionary(PATH_TO_DICTIONARY, dictionary, rowsInDictionary + 1, collsInDictionary, index, newWord))
		cout << "\nYou added the word " << newWord << " successfully!" << endl;

	freeMatrix(dictionary, rowsInDictionary);
}

/****************************** Start a game ******************************/
void printGameLetters(const char* str)
{
	if (!str)
		return;

	while (*str)
	{
		cout << *str << " ";
		str++;
	}

	cout << endl;
}

void gamePlay(char** dictionary, unsigned rowsInDictionary, char* gameLetters,
	unsigned rounds, unsigned countOfGameLetters, int* countGameLetters, const size_t letters)
{
	char* inputWord = new char[countOfGameLetters + 1];

	unsigned points = 0;

	unsigned i = 1;
	while (i <= rounds)
	{
		cout << "\nRound " << i << ". Available letters: ";
		printGameLetters(gameLetters);

		//get only the needed letters(the user can not enter more than 100 symbols)
		cin >> setw(static_cast <streamsize> (countOfGameLetters) + 1) >> inputWord;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		if (areLowerLetters(inputWord) && isSubsetOfGameLetters(inputWord, countGameLetters, letters)
			&& isWordInDictionary(dictionary, inputWord, rowsInDictionary))
		{
			points += myStrLenght(inputWord);
		}
		else
		{
			cout << "\nInvalid word." << endl;
		}

		initArrWithZeros(countGameLetters, letters);
		generateRandomLetters(gameLetters, countOfGameLetters, countGameLetters);

		i++;
	}

	cout << "\nYour total points are " << points << "." << endl;

	delete[] inputWord;
}

void beginNewGame()
{
	unsigned rowsInDictionary = 0;
	unsigned collsInDictionary = 0;
	getDimOfDictionary(PATH_TO_DICTIONARY, rowsInDictionary, collsInDictionary);

	char** dictionary = createMatrix(rowsInDictionary, collsInDictionary);

	const constexpr size_t countOfSettings = 2; //we have two criteria(count of game letters and rounds) by which the game is defined
	int settings[countOfSettings];
	readFromSettingFile(PATH_TO_GAME_SETTINGS, settings, countOfSettings);

	unsigned countOfGameLetters = settings[0];
	unsigned rounds = settings[1];

	cout << "\nGame letters: " << countOfGameLetters << endl;
	cout << "Rounds: " << rounds << endl;

	const constexpr size_t letters = 26; //letters in EN alphabet
	int countGameLetters[letters];

	readWords(PATH_TO_DICTIONARY, dictionary, rowsInDictionary, collsInDictionary);
	initArrWithZeros(countGameLetters, letters);

	char* gameLetters = new char[countOfGameLetters + 1]; // because of '\0' we add one to the size of gameLetters
	generateRandomLetters(gameLetters, countOfGameLetters, countGameLetters);

	gamePlay(dictionary, rowsInDictionary, gameLetters, rounds, countOfGameLetters, countGameLetters, letters);

	freeMatrix(dictionary, rowsInDictionary);
	delete[] gameLetters;
}

int main()
{
	cout << "===================== Scrabble =====================" << endl;
	while (true)
	{
		cout << "\n1. Begin a new game" << endl
			<< "2. Settings" << endl
			<< "   1. Change the count of letters" << endl
			<< "   2. Change the count of rounds" << endl
			<< "3. Add a new word to the dictionary" << endl
			<< "4. Exit" << endl;

		unsigned choise = 0;
		cin >> choise;

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize> ::max(), '\n'); // if an entered symbol is different of a number  
		}

		switch (choise)
		{
		case 1:
			beginNewGame();
			break;
		case 2:
			loadSettingMenu();
			break;
		case 3:
			addWordToDictionary();
			break;
		case 4:
			return 0;
			break;
		default:
			cout << "\nPlease, choose valid option from the menu!" << endl;
			break;
		}
	}
}