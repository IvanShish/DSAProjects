#include <string>
#include <vector>

using namespace std;


class Tree {

private:

	struct node {

		char elem;          // �������� �������
		bool isElem;        // �������� �� ��������� (���� ���, �� ��� ����)
		int weight;         // ��� ����

		int left;           // ������ ������ �����������
		int right;          // ������ ������� �����������

		node() {
			left = right = weight = -1;
			elem = '\0';
		}
	};


	vector <node> t;            // ������

	vector <int> p;             // ������� ������ ��� ��������
	vector  <int> e;            //

	string way;                 // ��� ��������

	int parentEsc;              // ���������� Esc �������
	int len;                    // ����� ������� t
	int head;                   // ������ ������ ������

	int nowElem = 0;            // ������ ���������� � ������
	int Esc = 1;                // ���� = 1 ���� ����� ������� Esc





//------------PUBLIC------------------

public:

	int flag;

	Tree() {
		node a;                 // ������� ������, ��������� �� Esc �������
		a.isElem = true;
		a.weight = 0;
		head = 0;
		len = 0;
		parentEsc = 0;
		t.push_back(a);
	}

	//--------------------------------------- ��������� ����� ������ � ������

	void addElem(char c) {

		node a;                     // �������� ����� � ��������
		a.isElem = true;
		a.elem = c;
		a.weight = 1;
		t.push_back(a);
		len++;

		node b;                    // �������� ����
		b.isElem = false;
		b.weight = 1;
		b.left = 0;                 // ����� �sc ������
		b.right = len;              // ������ �������� ����
		t.push_back(b);
		len++;

		if (len == 2) {                   // �������� Esc �� �������� ����
			head = 2;
			parentEsc = 2;
		}
		else {
			t[parentEsc].left = len;
			parentEsc = len;
		}
	}

	//--------------------------------------- ���� � ������ ������� �

	bool findElem(char c, int ind) {

		bool answer = false;

		if (head == 0) {              // ������ ������
			return false;
		}

		int l = t[ind].left;
		int r = t[ind].right;

		if (t[l].isElem && t[l].elem == c) {          // ���� �������, �� ���������� true

			if (flag) {                       //  ��������� ����
				way = c;
			}
			else {
				way = '0';
			}

			return true;
		}

		if (t[r].isElem && t[r].elem == c) {          // ���� �������, �� ���������� true

			if (flag) {                           //  ��������� ����
				way = c;
			}
			else {
				way = '1';
			}

			return true;
		}

		if (!t[l].isElem) {                   // ���� ����, �� ���� � ����
			answer = findElem(c, l);

			if (answer) {                 //  ��������� ����
				way = '0' + way;
			}

		}

		if (!answer && !t[r].isElem) {        // ���� ����, �� ���� � ����
			answer = findElem(c, r);

			if (answer) {             //  ��������� ����
				way = '1' + way;
			}
		}

		return answer;
	}
	//--------------------------------------- ��������� 1 � ���� �������� �

	void addOne(char c) {

		int i;

		for (i = 0; i <= len; i++) {

			if (t[i].elem == c) {
				(t[i].weight)++;
				break;
			}
		}
	}
	//---------------------------------------  ������������� ���� ����� ������

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
	//--------------------------------------- ������� ��������

	string retCode() {
		return way;
	}

	int retNowElem() {
		return nowElem;
	}

	int retEsc() {
		return Esc;

	}
	//--------------------------------------- �������� �� ������������ ������� � ������

	void check1() {

		int i = 2;
		int j = -1;

		int lenE = 0;
		int lenP = 0;
		int next;

		p.push_back(2);
		e.push_back(2);

		while (i != parentEsc) {             // ������ ������� ������� � �����
			j++;
			i = p[j];


			int l = t[i].left;
			int r = t[i].right;

			if (t[r].isElem) {             // ���� ����, �� �������� � ������ �
				e.push_back(r);
				lenE++;
			}
			else {                       // ���� ���� ��������� � ������ �
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

		do {                                         // ���� �������� ����
			i = next;
			if (j > 0) next = e[j - 1];

			if (j != 0 && t[i].weight > t[next].weight) {
				break;
			}

			j--;

		} while (j > 0);

		if (j <= 0) {            // ���� �������� ������ ���, �� �������� ������
			p.clear();
			e.clear();
			return;
		}

		j = 0;

		do {                     // ���� ������, �� ������ ��� �� ������ �����
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

	//--------------------------------------- ��������� ��������������� ������

	void check2() {

		int i = 2;
		int j = 0;

		int r, l;
		int next;

		p.push_back(2);



		do {                         // ���������� ������� ������ � ������ � ���������� �������

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

		do {                                 // ��������� ���������������
			i = p[j];

			if (t[i].weight > t[p[j - 1]].weight) {
				break;
			}

			j--;

		} while (j != 1);

		if (j == 1) {                     // ���� ��� �����, �� ��������� ������
			p.clear();
			e.clear();
			return;
		}

		j = 0;

		do {                             // ����� ������ ������������ ������� � ������ �����
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

	//---------------------------------------  ������������ ������� ������ �� ��������������� �����
	char processSym(char c) {

		int l = t[nowElem].left;
		int r = t[nowElem].right;


		if (nowElem == 0) {               // ��������� � Esc �������, ������� ������� ������
			flag = 1;
			addElem(c);                 // ��������� ��� � ������
			nowElem = 2;
			Esc = 0;
			return c;
		}

		if (nowElem == 2) {
			way.clear();
		}



		if (c == '0' && t[l].isElem && l != 0) {      // ���� ����� ������
			way.push_back(c);
			t[l].weight++;                          // ����������� ��� ���
			nowElem = 2;
			return t[l].elem;                       // ��������� ���
		}

		else if (c == '1' && t[r].isElem && r != 0) { // ���� ������ ������ (����������)
			way.push_back(c);
			t[r].weight++;
			nowElem = 2;
			return t[r].elem;
		}

		if (c == '0' && !t[l].isElem) {          // ���� ����� ����, ���� � ����
			way.push_back(c);
			nowElem = l;
		}

		else if (c == '1' && !t[r].isElem) {  // ���� ������ ���� ���� � ����
			way.push_back(c);
			nowElem = r;
		}

		else if (c == '0' && t[l].isElem) {  // ���� ����� Esc, �� ���� � ����
			way.push_back(c);               // ��������� ������� ����� ������
			nowElem = l;
			Esc = 1;
		}

		return '\0';
	}

	//--------------------------------------- �������� ������ (���)

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