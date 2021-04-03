#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

struct ltcode {
	unsigned long long frequency = 0;       //частота встречаемости символа
	unsigned char letter = NULL;               //буква
	string code = "";                   //код для буквы
};

struct tree {
	string list;      //сочетание символов
	int freq;            //частота встречаемости символа/сочетания символов

	int left;        //индекс левого поддерева
	int right;       //индекс правого поддерева

	string code = "";       //код для сочетания символов

	tree() {                 //конструктор
		left = 0;
		right = 0;
		list = "";
		freq = 0;
	}
};

void zeroset(ltcode* c);
bool cmpForStr(ltcode x, ltcode y);
bool cmpForTree(tree x, tree y);
void huffmanEncoding(int num, string code, vector <tree>& treee, ltcode* letter_codes);
char reading(ifstream &in);
void printTree(int num, vector <tree>& treee, int level = 0);

int main() {
	setlocale(LC_ALL, "Russian");
	char c = NULL;                                         //переменная для посимвольного считывания
	string S;                                       //строка, которую вводит пользователь
	ltcode* count = new ltcode[256];                //выделяем память под массив типа ltcode, который хранит в себе частоту встречаемости введенных символов
	ltcode* stringLinks = new ltcode[256];
	ifstream in;
	char how = NULL;
	string filePath;

	cout << "Как вы хотите ввести строку? 1 - из файла, 2 - из терминала\n";   //выбор ввода строки
	how = getchar();
	if (how > '2' || how < '1') {
		cout << "Неверно выбран вариант ввода\n";
		return 0;
	}
	if (how == '1') {
		cout << "Введите путь до txt файла\n";
		cin >> filePath;
		in.open(filePath);
		if (!in.is_open()) {
			cout << "Неверно введен путь до файла\n";
			return 0;
		}
		string file;
		getline(in, file);
		cout << "Содержимое файла:\n" << file << endl;
		in.close();
		in.open(filePath);
	}
	getchar();
	zeroset(count);              //обнуление всех переменных массива структур count

	while (c = reading(in)) {                       //производится посимвольное считывание строки с подсчетом частоты встречаемости
		if (c == '\n' || in.eof()) break;
		S += c;
		++count[c].frequency;
	}
	sort(count, count + 256, cmpForStr);              //сортировка элементов массива count в порядке убывания частоты встречаемости символа

	//вывод элементов и частоты
	for (int i = 0; count[i].frequency; i++) {
		cout << "Частота встречаемости символа '" << count[i].letter << "' - " << count[i].frequency << endl;
	}

	vector <tree> treee;
	tree tmp;
	tmp.list = "0";
	int j = 0;
	for (j = 0; count[j].frequency; j++) {
		tmp.freq = count[j].frequency;
		tmp.list[0] = count[j].letter;
		treee.push_back(tmp);                 //добавление в конец вектора
	}
	int maxSize = j;
	int size = 0;

	if (maxSize == 1) {
		count[0].code = "0";
		tmp.list = count[0].letter;
		tmp.freq = count[0].frequency;
	}
	else {
		sort(treee.begin(), treee.end(), cmpForTree);    //сортировка дерева в порядке возрастания частоты встречаемости

		for (j = 0; size < maxSize; ) {
			tmp.list = treee[j].list + treee[j + 1].list;        //складываем символы или сочетания символов
			tmp.freq = treee[j].freq + treee[j + 1].freq;                //складываем частоты встречаемости
			tmp.left = j;
			tmp.right = j + 1;

			size = tmp.list.size();

			treee.push_back(tmp);
			j += 2;
			sort(treee.begin() + j, treee.end(), cmpForTree);      //сортируем в порядке возрастания частоты все элементы, кроме тех, которые уже прошли проверку
		}

		//!!!функция кодирования
		cout << endl;
		huffmanEncoding(treee.size() - 1, "", treee, count);
	}

	cout << "Дерево:\n";
	printTree(treee.size() - 1, treee);
	cout << endl;

	cout << "\nКоды для букв:\n";
	for (j = 0; (count[j].frequency); ++j) {
		cout << '\'' << count[j].letter << "' - " << count[j].code << "  ";
		stringLinks[count[j].letter].code = count[j].code;
	}

	cout << "\n\nЗакодированная строка:\n";
	size = 0;
	for (unsigned int i = 0; i < S.size(); ++i) {
		cout << stringLinks[S[i]].code << " ";
		size += stringLinks[S[i]].code.size();
	}
	cout << "\nКоличество бит: " << size << endl;

	delete[] count;
	delete[] stringLinks;
	return 0;
}

void zeroset(ltcode* c) {               //функция, обнуляющая массив структур c
	for (int i = 0; i < 256; i++) {
		c[i].frequency = 0;
		c[i].letter = i;
	}
}

bool cmpForStr(ltcode x, ltcode y) {           //компаратор для сортировки строки
	if (x.frequency != y.frequency) return x.frequency > y.frequency;
	else return x.letter < y.letter;
}

bool cmpForTree(tree x, tree y) {            //компаратор для сортировки дерева
	if (x.freq != y.freq) return x.freq < y.freq;
	else return x.list.size() > y.list.size();
}

void huffmanEncoding(int num, string code, vector <tree>& treee, ltcode* letterCodes) {
	cout << "Частота: " << treee[num].freq << " , сочетание символов: " << treee[num].list << " , код: " << code << endl;
	treee[num].code = code;
	if (treee[num].list.size() > 1) {
		cout << "Рекурсивный вызов функции для левого поддерева:\n";
		huffmanEncoding(treee[num].left, code + "0", treee, letterCodes);
		cout << "\nРекурсивный вызов функции для правого поддерева:\n";
		huffmanEncoding(treee[num].right, code + "1", treee, letterCodes);
		cout << endl;
	}
	else {
		cout << "Это единичный символ";
		for (int j = 0; ; j++) {                               //поиск в элементе letterCodes структуры ltcode нужного символа и запись туда кода
			if (letterCodes[j].letter == treee[num].list[0]) {
				letterCodes[j].code = code;
				break;
			}
		}
	}
}

char reading(ifstream &in) {
	char c;
	if (in.is_open()) c = in.get();
	else c = getchar();
	return c;
}

void printTree(int num, vector <tree>& treee, int level) {
	if (treee[num].list.size() > 1) {
		if (treee[treee[num].right].list.size() >= 1) printTree(treee[num].right, treee, level + 4);
		if (level) {
			for (int i = 0; i < level; i++) cout << " ";
			cout << " ";
		}
		if (treee[treee[num].right].list.size() >= 1) {
			cout << " /\n";
			for (int i = 0; i < level; i++) cout << " ";
			cout << " ";
		}
		cout << treee[num].list << "\n ";
		if (treee[treee[num].left].list.size() >= 1) {
			for (int i = 0; i < level; i++) cout << " ";
			cout << " " << " \\\n";
			printTree(treee[num].left, treee, level + 4);
		}
	}
	else if (treee[num].list.size() == 1) {
		if (level) {
			for (int i = 0; i < level; i++) cout << " ";
		}
		cout << treee[num].list << "\n ";
	}
}

