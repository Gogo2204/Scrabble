#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

const constexpr char* PATH_TO_DICTIONARY = "D:\words.txt";
const constexpr char* PATH_TO_GAME_SETTINGS = "D:\settings.txt";
const constexpr int MAX_INPUT = 100;
const constexpr int MIN_ROUNDS = 3;
const constexpr int MAX_ROUNDS = 30;

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
		return 0;

	ifstream MyFile(fileName);

	if (!MyFile.is_open())
		return 0;

	unsigned lines = 0;

	MyFile >> lines; //on the first row we store the number of rows in the file

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

void myStrcpy(char* source, char* dest)
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

void readWords(const char* fileName, char** words, unsigned rows, unsigned colls)
{
	if (!fileName)
		return;	

	ifstream MyFile(fileName);

	if (!MyFile.is_open())
		return;	

	for (unsigned i = 0; i < rows; i++)
	{
		MyFile.getline(words[i], colls);		
	}

	MyFile.close();	
}

bool isWordInDictionary(char** dictionary, const char* word, unsigned sizeOfDictionary)
{
	if (!word || !dictionary)
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

void generateRandomLetters(char* str, int countOfLetters, int countingArr[])
{
	srand(time(0));

	for (unsigned i = 0; i < countOfLetters; i++)
	{		
		int index = random();
		countingArr[index]++; //count of every game generated letter 
		str[i] = getLetter(index);		
	}

	str[countOfLetters] = '\0';	
}

bool areSmallLetters(const char* str)
{
	if (!str)
		return 0;

	while (*str)
	{
		if (!(*str >= 'a' && *str <= 'z'))
			return false;
		str++;
	}

	return true;
}

bool isValidWord(const char* word, int countingArr[], size_t size)
{
	if (!word)
		return 0;

	constexpr size_t letters = 26; //letters in EN alphabet
	int count[letters];
	fillArrWithZeros(count, size);

	while (*word)
	{		
		unsigned index = getIndexByLetter(*word);
		count[index]++;
		word++;
	}

	for (unsigned i = 0; i < size; i++)
	{
		if (count[i] && count[i] > countingArr[i])
			return false;		
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

	cout << "Please, enter enter new count of game letters! The old one is " << settings[0] << endl;
	cin >> settings[0];

	while (!(settings[0] >= 1 && settings[0] <= MAX_INPUT))
	{
		cout << "Please, enter valid count for the game letters! Consider that the maximum count is 100" << endl;
		cin >> settings[0];
	}

	writeInSettingFile(PATH_TO_GAME_SETTINGS, settings, size);
}

void changeRoundsCount(int* settings, size_t size)
{
	readFromSettingFile(PATH_TO_GAME_SETTINGS, settings, size);

	cout << "Please, enter enter new count of rounds! The old one is " << settings[1] << endl;
	cin >> settings[1];

	while (!(settings[1] >= MIN_ROUNDS && settings[1] <= MAX_ROUNDS))
	{
		cout << "Please, enter valid count for the rounds! Consider that the minimum rounds are " << MIN_ROUNDS
			<< " and the maximum are " << MAX_ROUNDS << endl;
		cin >> settings[1];
	}

	writeInSettingFile(PATH_TO_GAME_SETTINGS, settings, size);
}

void settings()
{
	constexpr size_t countOfSettings = 2;
	int settings[countOfSettings];

	cout << "\n2. Settings" << endl
		<< "   1. Change the count of letters" << endl
		<< "   2. Change the count of rounds" << endl
		<< "Please, choose the setting you would like to change!" << endl;

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
			cout << "Please, choose valid choise from the menu!" << endl;
			break;
		}
	}
}

bool writeNewWordInDictionary(const char* fileName, char** dictionary, unsigned rows, unsigned index, const char* newWord)
{
	if (!dictionary || !newWord)
		return false;

	ofstream MyFile(fileName);

	if (!MyFile.is_open())
		return false;

	MyFile << rows << endl;	

	for (unsigned i = 1, j = 1; i < rows - 1; i++, j++)
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

	unsigned leftIndex = 0;
	unsigned rightIndex = rows - 1;

	while (leftIndex <= rightIndex)
	{
		int mid = leftIndex + (rightIndex - leftIndex) / 2;

		int cmp1 = myStrcmp(dictionary[mid], newWord);
		int cmp2 = myStrcmp(dictionary[mid + 1], newWord);

		if (cmp1 < 0 && cmp2 > 0)
		{
			return mid + 1;
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
	unsigned rowsInDictionary = linesInFile(PATH_TO_DICTIONARY);
	char** dictionary = createMatrix(rowsInDictionary, MAX_INPUT);
	readWords(PATH_TO_DICTIONARY, dictionary, rowsInDictionary, MAX_INPUT);

	cout << "Please, enter a new word to include in dictionary" << endl;

	char newWord[MAX_INPUT];

	bool isValidInput = false;
	while (!isValidInput)
	{
		cin >> newWord;

		if (!newWord || !areSmallLetters(newWord) || isWordInDictionary(dictionary, newWord, rowsInDictionary))
		{
			cout << "The entered word is either invalid or already exist in the dictionary!" << endl;
		}
		else
		{
			isValidInput = true;
		}
	}

	int index = findIndexOfNewWord(dictionary, rowsInDictionary, newWord);			

	if (writeNewWordInDictionary(PATH_TO_DICTIONARY, dictionary, rowsInDictionary + 1, index, newWord))
		cout << "You added the word " << newWord << " successfully" << endl;	

	freeMatrix(dictionary, rowsInDictionary);
}

void beginNewGame()
{
	unsigned rowsInDictionary = linesInFile(PATH_TO_DICTIONARY);	
	char** dictionary = createMatrix(rowsInDictionary, MAX_INPUT);			


	constexpr size_t countOfSettings = 2; //we have two criteria(count of game letters and rounds) by which the game is defined
	int settings[countOfSettings];
	readFromSettingFile(PATH_TO_GAME_SETTINGS, settings, countOfSettings);

	int countOfGameLetters = settings[0];
	int rounds = settings[1];

	constexpr size_t letters = 26; //letters in EN alphabet
	int countingArr[letters];

	readWords(PATH_TO_DICTIONARY, dictionary, rowsInDictionary, MAX_INPUT);
	fillArrWithZeros(countingArr, letters);
	
	char* gameLetters = new char[countOfGameLetters + 1];
	generateRandomLetters(gameLetters, countOfGameLetters, countingArr);

	char* inputWord = new char[countOfGameLetters];

	unsigned points = 0;

	unsigned i = 1;
	while (i <= rounds)
	{
		cout << "Round " << i << ". Available letters: ";
		printGameLetters(gameLetters);
		
		cin >> inputWord;

		if (myStrLenght(inputWord) <= countOfGameLetters && areSmallLetters(inputWord) 
			&& isValidWord(inputWord, countingArr, letters) && isWordInDictionary(dictionary, inputWord, rowsInDictionary))
		{
			points += myStrLenght(inputWord);
		}
		else
		{
			cout << "Invalid word." << endl;
		}

		fillArrWithZeros(countingArr, letters);
		generateRandomLetters(gameLetters, countOfGameLetters, countingArr);

		i++;
	}

	cout << "Your total points are " << points << "." << endl;

	freeMatrix(dictionary, rowsInDictionary);	
	delete[] gameLetters;
	delete[] inputWord;
}

int main()
{
	bool isValidInput = true;
	while (isValidInput)
	{
		cout << "1. Begin a new game" << endl
			<< "2. Settings" << endl
			<< "   1. Change the count of letters" << endl
			<< "   2. Change the count of rounds" << endl
			<< "3. Add a new word to the dictionary" << endl
			<< "4. Exit" << endl;

		unsigned choise = 1;
		cin >> choise;

		switch (choise)
		{
		case 1:
			beginNewGame();			
			break;
		case 2:
			settings();
			break;
		case 3:
			addWordToDictionary();
			break;
		case 4:
			return 0;
			break;
		default:
			cout << "Please, choose valid choise from the menu!" << endl;
			break;
		}
	}	
}