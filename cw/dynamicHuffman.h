#include <string>
#include <vector>

using namespace std;


class Tree {

private:

	struct node {

		char elem;          // хранимый эелмент
		bool isElem;        // явдяеься ли элементом (если нет, то это узел)
		int weight;         // вес узла

		int left;           // индекс левого ответвления
		int right;          // индекс правого ответвления

		node() {
			left = right = weight = -1;
			elem = '\0';
		}
	};


	vector <node> t;            // дерево

	vector <int> p;             // вектора нужные для проверок
	vector  <int> e;            //

	string way;                 // код элемента

	int parentEsc;              // родительнь Esc символа
	int len;                    // длина вектора t
	int head;                   // индекс головы дерева

	int nowElem = 0;            // индекс нахождения в дереве
	int Esc = 1;                // флаг = 1 если левый элемент Esc





//------------PUBLIC------------------

public:

	int flag;

	Tree() {
		node a;                 // создаем дерево, состоящее из Esc символа
		a.isElem = true;
		a.weight = 0;
		head = 0;
		len = 0;
		parentEsc = 0;
		t.push_back(a);
	}

	//--------------------------------------- добавляет новый символ в дерево

	void addElem(char c) {

		node a;                     // создание листа с символом
		a.isElem = true;
		a.elem = c;
		a.weight = 1;
		t.push_back(a);
		len++;

		node b;                    // создание узла
		b.isElem = false;
		b.weight = 1;
		b.left = 0;                 // слева Еsc символ
		b.right = len;              // справа созданый лист
		t.push_back(b);
		len++;

		if (len == 2) {                   // заменяем Esc на созданый узел
			head = 2;
			parentEsc = 2;
		}
		else {
			t[parentEsc].left = len;
			parentEsc = len;
		}
	}

	//--------------------------------------- ищет в дереве элемент с

	bool findElem(char c, int ind) {

		bool answer = false;

		if (head == 0) {              // пустое дерево
			return false;
		}

		int l = t[ind].left;
		int r = t[ind].right;

		if (t[l].isElem && t[l].elem == c) {          // если элемент, то возвращаем true

			if (flag) {                       //  постоение кода
				way = c;
			}
			else {
				way = '0';
			}

			return true;
		}

		if (t[r].isElem && t[r].elem == c) {          // если элемент, то возвращаем true

			if (flag) {                           //  постоение кода
				way = c;
			}
			else {
				way = '1';
			}

			return true;
		}

		if (!t[l].isElem) {                   // если узел, то идем в него
			answer = findElem(c, l);

			if (answer) {                 //  постоение кода
				way = '0' + way;
			}

		}

		if (!answer && !t[r].isElem) {        // если узел, то идем в него
			answer = findElem(c, r);

			if (answer) {             //  постоение кода
				way = '1' + way;
			}
		}

		return answer;
	}
	//--------------------------------------- добавляет 1 к весу элемента с

	void addOne(char c) {

		int i;

		for (i = 0; i <= len; i++) {

			if (t[i].elem == c) {
				(t[i].weight)++;
				break;
			}
		}
	}
	//---------------------------------------  пересчитывает веса узлов дерева

	void reSum(int ind) {

		int l = t[ind].left;
		int r = t[ind].right;

		if (!t[l].isElem) {
			reSum(l);
		}

		if (!t[r].isElem) {
			reSum(r);
		}

		t[ind].weight = t[l].weight + t[r].weight;

	}
	//--------------------------------------- функции возврата

	string retCode() {
		return way;
	}

	int retNowElem() {
		return nowElem;
	}

	int retEsc() {
		return Esc;

	}
	//--------------------------------------- проверка на соответсвтия листьев в деревк

	void check1() {

		int i = 2;
		int j = -1;

		int lenE = 0;
		int lenP = 0;
		int next;

		p.push_back(2);
		e.push_back(2);

		while (i != parentEsc) {             // строим векторы листьев и узлов
			j++;
			i = p[j];


			int l = t[i].left;
			int r = t[i].right;

			if (t[r].isElem) {             // если лист, то добавлем в вектор е
				e.push_back(r);
				lenE++;
			}
			else {                       // еслу узел добавляем в вектор р
				p.push_back(r);
				lenP++;
			}

			if (t[l].isElem) {
				e.push_back(l);
				lenE++;
			}
			else {
				p.push_back(l);
				lenP++;
			}
		}

		j = lenE;
		next = e[j];

		do {                                         // ищем неверный лист
			i = next;
			if (j > 0) next = e[j - 1];

			if (j != 0 && t[i].weight > t[next].weight) {
				break;
			}

			j--;

		} while (j > 0);

		if (j <= 0) {            // если неверных листов нет, то звершаем работу
			p.clear();
			e.clear();
			return;
		}

		j = 0;

		do {                     // если найден, то ставим его на верное место
			next = e[j];

			if (t[next].weight < t[i].weight) {
				node a = t[next];
				t[next] = t[i];
				t[i] = a;
				p.clear();
				e.clear();
				return;
			}
			j++;

		} while (j <= lenE);

		p.clear();
		e.clear();
	}

	//--------------------------------------- проверяет упорядоченность дерева

	void check2() {

		int i = 2;
		int j = 0;

		int r, l;
		int next;

		p.push_back(2);



		do {                         // составляем векторо вершин и листов в правильном порядке

			if (!t[i].isElem) {

				r = t[i].right;
				l = t[i].left;

				p.push_back(r);
				p.push_back(l);
			}

			j++;
			i = p[j];

		} while (i != 0);

		j = len;

		do {                                 // проверяем упорядоченность
			i = p[j];

			if (t[i].weight > t[p[j - 1]].weight) {
				break;
			}

			j--;

		} while (j != 1);

		if (j == 1) {                     // если все верно, то завершаем работу
			p.clear();
			e.clear();
			return;
		}

		j = 0;

		do {                             // иначе ставим неправильный элемент в нужное место
			next = p[j];


			if (t[next].weight < t[i].weight) {
				node a = t[next];
				t[next] = t[i];
				t[i] = a;

				if (next == parentEsc) {
					parentEsc = i;
				}

				p.clear();
				e.clear();
				return;
			}

			j++;


		} while (j <= len);

		p.clear();
		e.clear();
	}

	//---------------------------------------  обрабатывает входной символ из зикодированного файла
	char processSym(char c) {

		int l = t[nowElem].left;
		int r = t[nowElem].right;


		if (nowElem == 0) {               // находимся в Esc символе, входной элемент символ
			flag = 1;
			addElem(c);                 // добавляем его в дерево
			nowElem = 2;
			Esc = 0;
			return c;
		}

		if (nowElem == 2) {
			way.clear();
		}



		if (c == '0' && t[l].isElem && l != 0) {      // если слева символ
			way.push_back(c);
			t[l].weight++;                          // увеличиваем его вес
			nowElem = 2;
			return t[l].elem;                       // возращаем его
		}

		else if (c == '1' && t[r].isElem && r != 0) { // если справа символ (аналогично)
			way.push_back(c);
			t[r].weight++;
			nowElem = 2;
			return t[r].elem;
		}

		if (c == '0' && !t[l].isElem) {          // если слева узел, идем в него
			way.push_back(c);
			nowElem = l;
		}

		else if (c == '1' && !t[r].isElem) {  // если справа узел идем в него
			way.push_back(c);
			nowElem = r;
		}

		else if (c == '0' && t[l].isElem) {  // если слева Esc, то идем в него
			way.push_back(c);               // следующий элемент будет символ
			nowElem = l;
			Esc = 1;
		}

		return '\0';
	}

	//--------------------------------------- печатает дерево (ПКЛ)

	void print_Tree(int p, int level, int n) {
		if (p != -1)
		{
			print_Tree(t[p].right, level + 1, 0);
			for (int i = 0; i < level; i++) cout << "     ";

			if (n == 1)
				cout << "\\";
			else if (n == 0)
				cout << "/";

			if (t[p].isElem && t[p].elem != '\n')
				cout << t[p].elem << "(" << t[p].weight << ")" << endl;

			else if (t[p].isElem && t[p].elem == '\n')
				cout << "\\n(" << t[p].weight << ")" << endl;

			else
				cout << "S" << (int)t[p].elem << "(" << t[p].weight << ")" << endl;

			print_Tree(t[p].left, level + 1, 1);
		}
	}

	//---------------------------------------
};