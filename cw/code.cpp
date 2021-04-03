#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include "dynamicHuffman.h"
#include "SymCode.h"

using namespace std;

struct ltcode {
	unsigned long long frequency = 0;       //частота встречаемости символа
	unsigned char letter = NULL;               //буква
	string code = "";                   //код для буквы
};

struct node {
	string list;      //сочетание символов
	int i;            //частота встречаемости символа/сочетания символов

	int left;        //индекс левого поддерева
	int right;       //индекс правого поддерева

	string code = "";       //код для сочетания символов

	node() {                 //конструктор
		left = 0;
		right = 0;
		list = "";
		i = 0;
	}
};

struct nodeDecode {
	unsigned char letter = NULL;
	bool isNode = 1;  //если это узел, то хранит 1
	string code = "";
	nodeDecode* left = NULL;
	nodeDecode* right = NULL;
};

void zeroset(ltcode* c);            //обнуляет все значения элемента c
bool cmpForStr(ltcode x, ltcode y);                //следующие 4 функции - это компараторы для сортировки
bool cmpForStrDecoding(ltcode x, ltcode y);
bool cmpForTree(node x, node y);
bool cmpForTreeDecoding(node x, node y);
void encodeThis(vector <node>& tree, ltcode* count, ifstream& in, string* res);        //функция, которая начинает статическое кодирование
void huffmanEncoding(int num, string code, vector <node>& tree, ltcode* letter_codes);   //функция, которая продолжает статическое кодирование
void huffmanDecoding(nodeDecode* tree, string s, string* res);                           //функция, которая продолжает статическое декодирование
void decodeThis(nodeDecode* tree, ltcode* count, ifstream& in, string* res);          //функция, которая начинает статическое декодирование
void dynamicEncoding(ifstream& fin, ofstream& fout, string* res);            //функция, в которой производится динамическое кодирование
void dynamicDecoding(ifstream& fin, ofstream& fout, string* res);           //функция, в которой производится динамическое декодирование
char reading(ifstream& in);                                          //посимвольное считывание
void printTree(int num, vector <node>& tree, int level = 0);        //печать дерева при статическом кодировании
void printDecodeTree(nodeDecode* p, int indent);        //печать дерева при статическом декодировании

int main() {
	setlocale(LC_ALL, "Russian");
	string S;                                       //строка, которую вводит пользователь
	ltcode* count = new ltcode[256];                //выделяем память под массив типа ltcode, который хранит в себе частоту встречаемости введенных символов
	ifstream in;
	ofstream out;
	char how = NULL;
	string filePath;
	vector <node> ttree;
	nodeDecode* decodeTree = NULL;
	string res = "";
	Tree dynamicEncodingTree;
	//блок переменных для хранения времени выполнения функций
	long long staticDecodingStart = 0;
	long long staticDecodingEnd = 0;
	long long staticEncodingStart = 0;
	long long staticEncodingEnd = 0;
	long long dynamicEncodingStart = 0;
	long long dynamicEncodingEnd = 0;
	long long dynamicDecodingStart = 0;
	long long dynamicDecodingEnd = 0;

	cout << "Что вы хотите сравнить?\n1 - Динамическое кодирование со статическим\n2 - Динамическое декодирование со статическим\n";
	how = getchar();
	if (how == '1') {
		getchar();
		cout << "Введите путь до txt файла\n";
		cin >> filePath;
		in.open(filePath);
		if (!in.is_open()) {
			cout << "Неверно введен путь до файла\n";
			return 0;
		}
		cout << "Содержимое файла:\n";
		string file = "";                          //переменная, которая хранит в себе содержимое файла
		while (!in.eof()) {
			getline(in, file);
			cout << file << endl;
		}
		in.close();
		in.open(filePath);

		//---------------------------------------------------------------выполнение статического кодирования

		staticEncodingStart = clock();
		encodeThis(ttree, count, in, &res);
		staticEncodingEnd = clock();

		out.open("outputStaticE.txt");
		int i = 0;
		while (count[i++].frequency) {
			out << count[i - 1].letter << " " << count[i - 1].code << endl;
		}
		out << endl << endl << res;
		out.close();
		in.close();
		in.open(filePath);
		cout << endl << "Результат для статического кодирования:\n" << res << endl;
		int staticBytes = res.size();

		//---------------------------------------------------------------выполнение динамического кодирования

		res = "";
		//memset(res, NULL, strlen(res));
		out.open("outputDynamicE.txt");
		dynamicEncodingStart = clock();
		dynamicEncoding(in, out, &res);
		dynamicEncodingEnd = clock();
		out << endl << endl << res;
		out.close();
		cout << endl << "Результат для динамического кодирования:\n" << res << endl;
		cout << "Количество бит у закодированной статически строки: " << staticBytes << endl;
		cout << "Количество бит у закодированной динамически строки: " << res.size() << endl;

		cout << "Время статического кодирования: " << staticEncodingEnd - staticEncodingStart << endl;
		cout << "Время динамического кодирования: " << dynamicEncodingEnd - dynamicEncodingStart << endl;
	}

	else if (how == '2') {
		getchar();
		how = NULL;
		cout << "Введите название файла для статического декодирования (если файл называется outputStaticE.txt - нажмите Enter)\n";
		how = getchar();
		if (how == '\n') filePath = "outputStaticE.txt";
		else cin >> filePath;
		in.open(filePath);
		if (!in.is_open()) {
			cout << "Неверно введен путь до файла\n";
			return 0;
		}
		cout << "Содержимое файла:\n";
		string file = "";                          //переменная, которая хранит в себе содержимое файла
		while (!in.eof()) {
			getline(in, file);
			cout << file << endl;
		}
		in.close();
		in.open(filePath);

		//---------------------------------------------------------------выполнение статического декодирования

		staticDecodingStart = clock();
		decodeThis(decodeTree, count, in, &res);
		staticDecodingEnd = clock();

		out.open("outputStaticD.txt");
		out << res;
		out.close();
		cout << endl << "Результат для статического декодирования:\n" << res << endl;
		in.close();

		//---------------------------------------------------------------выполнение динамического декодирования

		how = NULL;
		cout << "Введите название файла для динамического декодирования (если файл называется outputDynamicE.txt - нажмите Enter)\n";
		how = getchar();
		if (how == '\n') filePath = "outputDynamicE.txt";
		else cin >> filePath;
		in.open(filePath);
		if (!in.is_open()) {
			cout << "Неверно введен путь до файла\n";
			return 0;
		}
		cout << "Содержимое файла:\n";
		file = "";
		while (!in.eof()) {
			getline(in, file);
			cout << file << endl;
		}
		in.close();
		in.open(filePath);

		res = "";
		//memset(res, NULL, strlen(res));
		out.open("outputDynamicD.txt");
		dynamicDecodingStart = clock();
		dynamicDecoding(in, out, &res);
		dynamicDecodingEnd = clock();
		out << res;
		cout << endl << "Результат для динамического декодирования:\n" << res << endl;
		out.close();
		in.close();

		cout << "\nВремя статического декодирования: " << staticDecodingEnd - staticDecodingStart << endl;
		cout << "\nВремя динамического декодирования: " << dynamicDecodingEnd - dynamicDecodingStart << endl;
	}
	else {
		cout << "Неверно выбран вариант сравнения\n";
		system("pause");
		return 0;
	}

	delete[] count;
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

bool cmpForStrDecoding(ltcode x, ltcode y) {
	if ((x.code).size() != (y.code).size()) return (x.code).size() < (y.code).size();
	else return x.letter < y.letter;
}

bool cmpForTree(node x, node y) {            //компаратор для сортировки дерева
	if (x.i != y.i) return x.i < y.i;
	else return x.list.size() > y.list.size();
}

bool cmpForTreeDecoding(node x, node y) {
	if ((x.code).size() != (y.code).size()) return (x.code).size() > (y.code).size();
	else return x.list.size() > y.list.size();
}

void encodeThis(vector <node> & tree, ltcode * count, ifstream & in, string * res) {
	cout << "\nКодирование...\n";
	char c = NULL;
	ltcode* stringLinks = new ltcode[256];
	string S;            //хранит в себе изначальную строку из букв

	zeroset(count);              //обнуление всех переменных массива структур count

	while (c = reading(in)) {                       //производится посимвольное считывание строки с подсчетом частоты встречаемости
		//if (c == '\n' || in.eof()) break;
		if (in.eof()) break;
		S += c;
		++count[c].frequency;
	}

	if (S.size() == 0) {
		cout << "Вы выбрали пустой файл\n";
		return;
	}

	sort(count, count + 256, cmpForStr);              //сортировка элементов массива count в порядке убывания частоты встречаемости символа
	node tmp;
	tmp.list = "0";
	int j = 0;
	for (j = 0; count[j].frequency; j++) {
		tmp.i = count[j].frequency;
		tmp.list[0] = count[j].letter;
		tree.push_back(tmp);                 //добавление в конец вектора
	}
	int maxSize = j;
	long long size = 0;

	if (maxSize == 1) {
		count[0].code = "0";
		for (int i = 0; i < S.size(); i++) *res += count[0].code;
		return;
	}

	sort(tree.begin(), tree.end(), cmpForTree);    //сортировка дерева в порядке возрастания частоты встречаемости

	for (j = 0; size < maxSize; ) {
		tmp.list = tree[j].list + tree[j + 1].list;        //складываем символы или сочетания символов
		tmp.i = tree[j].i + tree[j + 1].i;                //складываем частоты встречаемости
		tmp.left = j;
		tmp.right = j + 1;

		size = tmp.list.size();

		tree.push_back(tmp);
		j += 2;
		sort(tree.begin() + j, tree.end(), cmpForTree);      //сортируем в порядке возрастания частоты все элементы, кроме тех, которые уже прошли проверку
	}
	/*cout << "Дерево:\n";
	printTree(tree.size() - 1, tree);
	cout << endl;*/

	cout << tree.size() - 1 << endl;

	/*if (maxSize == 1) {
		count[0].code = "0";
	}*/
	huffmanEncoding(tree.size() - 1, "", tree, count);

	//cout << "\nКоды для букв:\n";
	for (j = 0; (count[j].frequency); ++j) {
		//cout << '\'' << count[j].letter << "' - " << count[j].code << "  ";
		stringLinks[count[j].letter].code = count[j].code;
	}

	//cout << "\n\nЗакодированная строка:\n";
	size = 0;
	for (int i = 0; i < S.size(); i++) {
		*res += stringLinks[S[i]].code;
		//strcat(*res, stringLinks[S[i]].code.c_str());
		//cout << stringLinks[S[i]].code;
		//size += stringLinks[S[i]].code.size();
	}
	//cout << "\nКоличество бит: " << size << endl;
	cout << "Кодирование завершено\n";
	delete[] stringLinks;
}

void huffmanEncoding(int num, string code, vector <node> & tree, ltcode * letterCodes) {
	//cout << "Частота: " << tree[num].i << " , сочетание символов: " << tree[num].list << " , код: " << code << endl;
	tree[num].code = code;
	if (tree[num].list.size() > 1) {
		//cout << "Рекурсивный вызов функции для левого поддерева:\n";
		huffmanEncoding(tree[num].left, code + "0", tree, letterCodes);
		//cout << "\nРекурсивный вызов функции для правого поддерева:\n";
		huffmanEncoding(tree[num].right, code + "1", tree, letterCodes);
		//cout << endl;
	}
	else {
		//cout << "Это единичный символ";
		for (int j = 0; ; j++) {                               //поиск в элементе letterCodes структуры ltcode нужного символа и запись туда кода
			if (letterCodes[j].letter == tree[num].list[0]) {
				letterCodes[j].code = code;
				break;
			}
		}
	}
}

void decodeThis(nodeDecode * tree, ltcode * count, ifstream & in, string * res) {
	cout << "\nДекодирование...\n";
	char sym = NULL;    //считывание символа
	ltcode* stringLinks = new ltcode[256];
	std::string S;            //хранит в себе строку, которую нужно декодировать
	string result;
	int numOfKeys = 0;
	int k = 0;       //переменная для записи ключей из ltcode stringLinks в vector <node> tree

	while (!in.eof()) {                       //производится считывание ключей
		char tmp = NULL;    //для считывания ненужных символов
		string codee;        //считывание кода символа
		sym = reading(in);
		if (sym == '\n') {
			sym = reading(in);
			if (sym != ' ') break;
			else sym = '\n';
		}
		else {
			tmp = reading(in);
			if (tmp != ' ') {
				cout << "Неверно введены ключи!\n";
				return;
			}
		}
		in >> codee;
		if (codee[0] != '0' && codee[0] != '1') {
			cout << "Неверно введены ключи!\n";
			return;
		}
		tmp = reading(in);

		stringLinks[sym].code = codee;
		stringLinks[sym].letter = sym;
		stringLinks[sym].frequency = 1;
		k++;
		//cout << stringLinks[sym].code << " " << stringLinks[sym].letter << " " << (int)stringLinks[sym].letter << endl;
	}
	in >> S;

	sort(stringLinks, stringLinks + 256, cmpForStrDecoding);              //сортировка элементов массива count в порядке возрастания длины кода
	//cout << "Количество бит: " << S.size() << endl;
	//for (int i = 0; i < 256; i++) cout << "sym:" << stringLinks[i].letter << " code " << stringLinks[i].code << endl;
	numOfKeys = k;
	k = 256 - k;
	//if (stringLinks[k].code == "") k++;

	tree = new nodeDecode;
	nodeDecode* head = tree;
	//int forTree = 0;  //для подсчета количества узлов дерева. Дерево начинается с 0
	for (int i = k; i < 256; i++) {
		//cout << stringLinks[i].letter << endl;
		int j = 0;

		for (j = 0; j < stringLinks[i].code.size() - 1; j++) {
			tree->isNode = 1;
			if (stringLinks[i].code[j] == '0') {
				if (tree->left == NULL)
					tree->left = new nodeDecode;
				tree = tree->left;
			}
			else {
				if (tree->right == NULL)
					tree->right = new nodeDecode;
				tree = tree->right;
			}
		}
		if (tree == NULL)
			tree = new nodeDecode;
		tree->isNode = 1;
		if (stringLinks[i].code[j] == '0') {
			tree->left = new nodeDecode;
			tree->left->code += stringLinks[i].code;
			tree->left->isNode = 0;
			tree->left->letter = stringLinks[i].letter;
		}
		else {
			tree->right = new nodeDecode;
			tree->right->code += stringLinks[i].code;
			tree->right->isNode = 0;
			tree->right->letter = stringLinks[i].letter;
		}
		tree = head;
	}

	huffmanDecoding(tree, S, res);

	//cout << "Дерево:\n";
	//printDecodeTree(tree, 0);
	cout << "Декодирование завершено\n";

}

void huffmanDecoding(nodeDecode * tree, string s, string * res) {
	nodeDecode* head = tree;

	for (int i = 0; i < s.size() + 1; i++) {
		if (s[i] == '0') {
			if (tree->left) {
				tree = tree->left;
			}
			else {
				*res += tree->letter;
				tree = head;
				tree = tree->left;
			}
		}
		else {
			if (tree->right) {
				tree = tree->right;
			}
			else {
				*res += tree->letter;
				tree = head;
				tree = tree->right;
			}
		}
	}
}

void dynamicEncoding(ifstream & fin, ofstream & fout, string * res) {
	cout << "\nКодирование...\n";
	Tree t;
	Sym a;
	char c;
	string str;
	string answer;

	while (!fin.eof()) {
		fin.get(c);

		if (fin.eof()) {
			break;
		}

		if (!fin.eof()) {

			if (t.findElem(c, 2)) {                                              // поиск символа в дереве
				t.addOne(c);                                                    // елси найден, то увеличиваем вес
				*res += t.retCode();
			}
			else {
				t.flag = 1;
				t.addElem(c);                                               // елси не найден, то добавляем в дерево
				t.findElem(c, 2);
				t.flag = 0;

				str = t.retCode();                                          // получаем код

				str.pop_back();
				str = str + a.code(c);                                      // обрабатываем полученный код

				*res += str;
				str.clear();
			}
			t.reSum(2);                                                 // проверяем и сортируем дерево
			t.check1();
			t.reSum(2);
			t.check2();
			t.reSum(2);
		}
	}
	fin.close();
	if ((*res).size() == 0) {
		cout << "Вы выбрали пустой файл\n";
	}
	a.print_Codes(fout);
	cout << "Кодирование завершено\n";
}

void dynamicDecoding(ifstream & fin, ofstream & fout, string * res) {
	cout << "\nДекодирование...\n";
	char c;
	int flag = 1;
	Tree t;
	Sym a;
	string str;
	string answer;

	if (a.readFile(fin)) {                                                  // читаем файл со статическими кодами
		cout << " Файл с кодами неверный" << endl;
		fin.close();
		return;
	}

	while (!fin.eof()) {
		fin.get(c);
		//cout << *res << endl;
		if (fin.eof()) {
			cout << "Декодирование завершено" << endl;
			return;
		}

		//cout << "Был получен символ: " << c << endl;

		if ((c != '1' && c != '0' && !fin.eof())) {           // елси считаный символ не 0 или не 1
			cout << "Неверный файл" << endl;
			return;
		}


		if (!fin.eof()) {


			if (t.retEsc()) {                                                                         // елси находися в Esc, то считываем
				//cout << "  Считываем статический код символа и ищем его в фале с кодами"  << endl;  // статический код символа
				str.push_back(c);

				for (int i = 0; i < 7; i++) {                        // считваение

					if ((c != '1' && c != '0' && !fin.eof())) {
						cout << "Неверный файл" << endl;
						return;
					}

					fin.get(c);
					str.push_back(c);

					t.flag = 1;
				}

				c = a.whatSym(str);                     // определяем символ по коду

			   /* if(c != '\n')
					cout << "  Статический код " << str << "  принадлежит символу " << c << endl;
				else if(c != ' ')
					cout << "  Статический код " << str << "  принадлежит символу space" << endl;
				else
					cout << "  Статический код " << str << "  принадлежит символу \\n" <<  endl;

				cout << "      Записываем полученный символ в дерево" << endl;*/

				str.clear();
			}



			c = t.processSym(c);                    // обрабаываем символ
			t.findElem(c, 2);
			t.flag = 0;

			if (c != '\0' && !fout.eof()) {

				if (flag) {
					str = t.retCode();
					/*if(c == '\n')
						cout << "  Наден символ: \\n";
					else if(c == ' ')
						cout << "  Наден символ: space" ;
					else
						cout << "  Наден символ: " << c;

					cout << " по коду " << str << endl;
					cout << "    Увеличиваем вес символа" << endl;*/
					str.clear();
				}

				flag = 1;

				//cout << "      Проверяем и сортируем деерво" << endl;
				t.reSum(2);                                                     // упорядочиваем дерево
				t.check1();
				t.reSum(2);
				t.check2();
				t.reSum(2);

				//fout << c;
				*res += c;
			}
			/*else if( !fin.eof()){
				cout << "  Данный символ является частью динамического кода" << endl
					<< "  Продолжаем считывание" << endl;
			}*/
		}
	}
	cout << "Декодирование завершено\n";
}

char reading(ifstream & in) {
	char c;
	if (in.is_open()) c = in.get();
	else c = getchar();
	return c;
}

void printTree(int num, vector <node> & tree, int level) {
	if (tree[num].list.size() > 1) {
		if (tree[tree[num].right].list.size() >= 1) printTree(tree[num].right, tree, level + 4);
		if (level) {
			for (int i = 0; i < level; i++) cout << " ";
			cout << " ";
		}
		if (tree[tree[num].right].list.size() >= 1) {
			cout << " /\n";
			for (int i = 0; i < level; i++) cout << " ";
			cout << " ";
		}
		cout << tree[num].list << "\n ";
		if (tree[tree[num].left].list.size() >= 1) {
			for (int i = 0; i < level; i++) cout << " ";
			cout << " " << " \\\n";
			printTree(tree[num].left, tree, level + 4);
		}
	}
	else if (tree[num].list.size() == 1) {
		if (level) {
			for (int i = 0; i < level; i++) cout << " ";
		}
		cout << tree[num].list << "\n ";
	}
}

void printDecodeTree(nodeDecode * p, int indent) {
	if (p != NULL) {
		if (p->right) {
			printDecodeTree(p->right, indent + 4);
		}
		if (indent) {
			for (int i = 0; i < indent; i++) cout << " ";
		}
		if (p->right) {
			cout << " /\n";
			for (int i = 0; i < indent; i++) cout << " ";
		}
		if (p->isNode == 1) cout << "ND";
		cout << p->letter << "\n ";
		if (p->left) {
			for (int i = 0; i < indent; i++) cout << " ";
			cout << ' ' << " \\\n";
			printDecodeTree(p->left, indent + 4);
		}
	}

}
