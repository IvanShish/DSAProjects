#include "header.h"

using namespace std;
using namespace binTree_modul;

typedef unsigned int unInt;

binTree enterBT(char c, int curr, ifstream& f, int* check, int* openingBracket, int* endingBracket);       //функция для ввода бинарного дерева
void outBT(binTree b, char elem, int *check, int *count, int length, int* min);          //функция длв вывода элементов дерева и для подсчета нужных сведений о запрашиваемом элементе
char reading(ifstream &f);        //функция, возвращающая вводимый с терминала/файла элемент

int main()
{
	binTree b;
	int how;
	int curr = 0;
	string s;
	ifstream in;
	char elem;
	char c = NULL;
	int check = 1;
	int checkForElem = 0;
	int count = 0;        //счетчик найденных элементов
	int length = 1;        //длина до каждого найденного элемента
	int min = 100000;    //для расчета минимального пути
	int openingBracket = 0;
	int endingBracket = 0;

	/*SetConsoleCP(1251);			// для вывода кирилицы
	SetConsoleOutputCP(1251);	// для вывода кирилицы*/
	setlocale(LC_ALL, "RUS");

	cout << "Как вы хотите ввести бинарное дерево?\n1 - из файла\n2 - из терминала\n";
	cin >> how;
	if (how != 1 && how != 2) {
		cout << "Нужно ввести 1 или 2!";
		return 0;
	}
	
	if (how == 1) {
		string nameOfFile;

		cout << "Доступные файлы для выбора:\n";
		WIN32_FIND_DATA FindFileData;
		HANDLE hf;
		hf = FindFirstFile(".\\*.txt", &FindFileData);
		if (hf != INVALID_HANDLE_VALUE) {
			do {
				cout << FindFileData.cFileName << endl;
			} while (FindNextFile(hf, &FindFileData) != 0);
			FindClose(hf);
		}

		cout << "Введите название файла\n";
		cin >> nameOfFile;
		in.open(nameOfFile);

		if (!in.is_open()) {
			cout << "Неверное название файла!\n";
			return 0;
		}

		getline(in, s);
		cout << "Строка в файле:\n" << s << endl;
		in.close();
		in.open(nameOfFile);
	}

	cin.sync();
	cin.get();

	b = enterBT(c, curr, in, &check, &openingBracket, &endingBracket);
	cout << endl;

	if (openingBracket != endingBracket || check == 4) {
		cout << "Это не бинарное дерево!" << endl;
		return 0;
	}

	cout << "Введите элемент\n";
	cin >> elem;
	if (!isalpha(elem)) {
		cout << "Неверно введен элемент или это не бинарное дерево\n";
		return 0;
	}

	outBT(b, elem, &checkForElem, &count, length, &min);
	if (checkForElem) {
		cout << "\nЗапрашиваемый элемент был найден\n";
		cout << "Число вхождений элемента в дерево - " << count << endl;
		cout << "Длина пути от корня до ближайшего запрашиваемого элемента - " << min << endl;
	}
	else {
		cout << "\nЗапрашиваемый элемент не был найден\n";
		return 0;
	}
	cout << endl;

	destroy(b);
	cout << endl;
	return (0);
}

binTree enterBT(char c, int curr, ifstream& f, int* check, int* openingBracket, int* endingBracket) {         //функция для ввода бинарного дерева
	binTree p, q;

	if (c == EOF || c == '\n') return NULL;
	c = reading(f);

	if (curr == 0) {
		if (c == '(') {
			(*openingBracket)++;
			c = reading(f);
			if (c == ')') {
				(*endingBracket)++;
				return NULL;
			}
		}
		else {
			*check = 4;
			return NULL;
		}
	}

	else if (c == ')') {
		*check = 0;
		(*endingBracket)++;
		return NULL;
	}

	else if (isspace(c)) {
		*check = 1;
		return NULL;
	}

	else if (c == '(') {
		(*openingBracket)++;
		*check = 2;
		return NULL;
	}

	if (isalpha(c)) {
		p = enterBT(c, curr + 1, f, check, openingBracket, endingBracket);

		if (*check == 2) {                       //Если открывающаяся скобка
			p = enterBT(c, curr + 1, f, check, openingBracket, endingBracket);
			if (*check != 5) {
				*check = 3;
			}
			else q = NULL;
		}
		else if (*check == 1) {                  //Если пробел
			p = enterBT(c, curr + 1, f, check, openingBracket, endingBracket);
		}

		if (*check == 0) {                        //Если закрывающаяся скобка
			binTree tmp = NULL;
			if (curr != 0) {
				tmp = enterBT(c, curr + 1, f, check, openingBracket, endingBracket);
			}
			if (*check == 2) q = NULL;
			else if (*check == 0) {
				q = tmp;
				*check = 5;
			}
		}
		else if (*check != 5) {
			q = enterBT(c, curr + 1, f, check, openingBracket, endingBracket);
			if (*check == 0) q = enterBT(c, curr + 1, f, check, openingBracket, endingBracket);
			if (*check == 2) q = enterBT(c, curr + 1, f, check, openingBracket, endingBracket);
			else if (*check == 5) enterBT(c, curr + 1, f, check, openingBracket, endingBracket);
		}
		else if (*check == 5) {
			*check = 3;
			q = NULL;
		}

		return ConsBT(c, p, q);
	}

	return 0;
}

void outBT(binTree b, char elem, int *check, int *count, int length, int *min) {         //функция вывода запрашиваемых сведений для элемента
	if (b != NULL) {
		if (b->info == elem) {               //если найден запрашиваемый элемент
			if (length < *min) {
				*min = length;
			}
			(*count)++;
			*check = 1;
			cout << "Найден запрашиваемый элемент " << RootBT(b) << " ";
		}
		else cout << RootBT(b) << " ";
		cout << " LEFT:";
		outBT(Left(b), elem, check, count, length+1, min);
		cout << " RIGHT:";
		outBT(Right(b), elem, check, count, length+1, min);
	}
	else cout << "Л";
}

char reading(ifstream& f) {
	char c = NULL;
	if (f.is_open()) c = f.get();
	else c = cin.get();
	cout << c << endl;
	return c;
}