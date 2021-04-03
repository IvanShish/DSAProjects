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
	unsigned long long frequency = 0;       //������� ������������� �������
	unsigned char letter = NULL;               //�����
	string code = "";                   //��� ��� �����
};

struct node {
	string list;      //��������� ��������
	int i;            //������� ������������� �������/��������� ��������

	int left;        //������ ������ ���������
	int right;       //������ ������� ���������

	string code = "";       //��� ��� ��������� ��������

	node() {                 //�����������
		left = 0;
		right = 0;
		list = "";
		i = 0;
	}
};

struct nodeDecode {
	unsigned char letter = NULL;
	bool isNode = 1;  //���� ��� ����, �� ������ 1
	string code = "";
	nodeDecode* left = NULL;
	nodeDecode* right = NULL;
};

void zeroset(ltcode* c);            //�������� ��� �������� �������� c
bool cmpForStr(ltcode x, ltcode y);                //��������� 4 ������� - ��� ����������� ��� ����������
bool cmpForStrDecoding(ltcode x, ltcode y);
bool cmpForTree(node x, node y);
bool cmpForTreeDecoding(node x, node y);
void encodeThis(vector <node>& tree, ltcode* count, ifstream& in, string* res);        //�������, ������� �������� ����������� �����������
void huffmanEncoding(int num, string code, vector <node>& tree, ltcode* letter_codes);   //�������, ������� ���������� ����������� �����������
void huffmanDecoding(nodeDecode* tree, string s, string* res);                           //�������, ������� ���������� ����������� �������������
void decodeThis(nodeDecode* tree, ltcode* count, ifstream& in, string* res);          //�������, ������� �������� ����������� �������������
void dynamicEncoding(ifstream& fin, ofstream& fout, string* res);            //�������, � ������� ������������ ������������ �����������
void dynamicDecoding(ifstream& fin, ofstream& fout, string* res);           //�������, � ������� ������������ ������������ �������������
char reading(ifstream& in);                                          //������������ ����������
void printTree(int num, vector <node>& tree, int level = 0);        //������ ������ ��� ����������� �����������
void printDecodeTree(nodeDecode* p, int indent);        //������ ������ ��� ����������� �������������

int main() {
	setlocale(LC_ALL, "Russian");
	string S;                                       //������, ������� ������ ������������
	ltcode* count = new ltcode[256];                //�������� ������ ��� ������ ���� ltcode, ������� ������ � ���� ������� ������������� ��������� ��������
	ifstream in;
	ofstream out;
	char how = NULL;
	string filePath;
	vector <node> ttree;
	nodeDecode* decodeTree = NULL;
	string res = "";
	Tree dynamicEncodingTree;
	//���� ���������� ��� �������� ������� ���������� �������
	long long staticDecodingStart = 0;
	long long staticDecodingEnd = 0;
	long long staticEncodingStart = 0;
	long long staticEncodingEnd = 0;
	long long dynamicEncodingStart = 0;
	long long dynamicEncodingEnd = 0;
	long long dynamicDecodingStart = 0;
	long long dynamicDecodingEnd = 0;

	cout << "��� �� ������ ��������?\n1 - ������������ ����������� �� �����������\n2 - ������������ ������������� �� �����������\n";
	how = getchar();
	if (how == '1') {
		getchar();
		cout << "������� ���� �� txt �����\n";
		cin >> filePath;
		in.open(filePath);
		if (!in.is_open()) {
			cout << "������� ������ ���� �� �����\n";
			return 0;
		}
		cout << "���������� �����:\n";
		string file = "";                          //����������, ������� ������ � ���� ���������� �����
		while (!in.eof()) {
			getline(in, file);
			cout << file << endl;
		}
		in.close();
		in.open(filePath);

		//---------------------------------------------------------------���������� ������������ �����������

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
		cout << endl << "��������� ��� ������������ �����������:\n" << res << endl;
		int staticBytes = res.size();

		//---------------------------------------------------------------���������� ������������� �����������

		res = "";
		//memset(res, NULL, strlen(res));
		out.open("outputDynamicE.txt");
		dynamicEncodingStart = clock();
		dynamicEncoding(in, out, &res);
		dynamicEncodingEnd = clock();
		out << endl << endl << res;
		out.close();
		cout << endl << "��������� ��� ������������� �����������:\n" << res << endl;
		cout << "���������� ��� � �������������� ���������� ������: " << staticBytes << endl;
		cout << "���������� ��� � �������������� ����������� ������: " << res.size() << endl;

		cout << "����� ������������ �����������: " << staticEncodingEnd - staticEncodingStart << endl;
		cout << "����� ������������� �����������: " << dynamicEncodingEnd - dynamicEncodingStart << endl;
	}

	else if (how == '2') {
		getchar();
		how = NULL;
		cout << "������� �������� ����� ��� ������������ ������������� (���� ���� ���������� outputStaticE.txt - ������� Enter)\n";
		how = getchar();
		if (how == '\n') filePath = "outputStaticE.txt";
		else cin >> filePath;
		in.open(filePath);
		if (!in.is_open()) {
			cout << "������� ������ ���� �� �����\n";
			return 0;
		}
		cout << "���������� �����:\n";
		string file = "";                          //����������, ������� ������ � ���� ���������� �����
		while (!in.eof()) {
			getline(in, file);
			cout << file << endl;
		}
		in.close();
		in.open(filePath);

		//---------------------------------------------------------------���������� ������������ �������������

		staticDecodingStart = clock();
		decodeThis(decodeTree, count, in, &res);
		staticDecodingEnd = clock();

		out.open("outputStaticD.txt");
		out << res;
		out.close();
		cout << endl << "��������� ��� ������������ �������������:\n" << res << endl;
		in.close();

		//---------------------------------------------------------------���������� ������������� �������������

		how = NULL;
		cout << "������� �������� ����� ��� ������������� ������������� (���� ���� ���������� outputDynamicE.txt - ������� Enter)\n";
		how = getchar();
		if (how == '\n') filePath = "outputDynamicE.txt";
		else cin >> filePath;
		in.open(filePath);
		if (!in.is_open()) {
			cout << "������� ������ ���� �� �����\n";
			return 0;
		}
		cout << "���������� �����:\n";
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
		cout << endl << "��������� ��� ������������� �������������:\n" << res << endl;
		out.close();
		in.close();

		cout << "\n����� ������������ �������������: " << staticDecodingEnd - staticDecodingStart << endl;
		cout << "\n����� ������������� �������������: " << dynamicDecodingEnd - dynamicDecodingStart << endl;
	}
	else {
		cout << "������� ������ ������� ���������\n";
		system("pause");
		return 0;
	}

	delete[] count;
	return 0;
}

void zeroset(ltcode* c) {               //�������, ���������� ������ �������� c
	for (int i = 0; i < 256; i++) {
		c[i].frequency = 0;
		c[i].letter = i;
	}
}

bool cmpForStr(ltcode x, ltcode y) {           //���������� ��� ���������� ������
	if (x.frequency != y.frequency) return x.frequency > y.frequency;
	else return x.letter < y.letter;
}

bool cmpForStrDecoding(ltcode x, ltcode y) {
	if ((x.code).size() != (y.code).size()) return (x.code).size() < (y.code).size();
	else return x.letter < y.letter;
}

bool cmpForTree(node x, node y) {            //���������� ��� ���������� ������
	if (x.i != y.i) return x.i < y.i;
	else return x.list.size() > y.list.size();
}

bool cmpForTreeDecoding(node x, node y) {
	if ((x.code).size() != (y.code).size()) return (x.code).size() > (y.code).size();
	else return x.list.size() > y.list.size();
}

void encodeThis(vector <node> & tree, ltcode * count, ifstream & in, string * res) {
	cout << "\n�����������...\n";
	char c = NULL;
	ltcode* stringLinks = new ltcode[256];
	string S;            //������ � ���� ����������� ������ �� ����

	zeroset(count);              //��������� ���� ���������� ������� �������� count

	while (c = reading(in)) {                       //������������ ������������ ���������� ������ � ��������� ������� �������������
		//if (c == '\n' || in.eof()) break;
		if (in.eof()) break;
		S += c;
		++count[c].frequency;
	}

	if (S.size() == 0) {
		cout << "�� ������� ������ ����\n";
		return;
	}

	sort(count, count + 256, cmpForStr);              //���������� ��������� ������� count � ������� �������� ������� ������������� �������
	node tmp;
	tmp.list = "0";
	int j = 0;
	for (j = 0; count[j].frequency; j++) {
		tmp.i = count[j].frequency;
		tmp.list[0] = count[j].letter;
		tree.push_back(tmp);                 //���������� � ����� �������
	}
	int maxSize = j;
	long long size = 0;

	if (maxSize == 1) {
		count[0].code = "0";
		for (int i = 0; i < S.size(); i++) *res += count[0].code;
		return;
	}

	sort(tree.begin(), tree.end(), cmpForTree);    //���������� ������ � ������� ����������� ������� �������������

	for (j = 0; size < maxSize; ) {
		tmp.list = tree[j].list + tree[j + 1].list;        //���������� ������� ��� ��������� ��������
		tmp.i = tree[j].i + tree[j + 1].i;                //���������� ������� �������������
		tmp.left = j;
		tmp.right = j + 1;

		size = tmp.list.size();

		tree.push_back(tmp);
		j += 2;
		sort(tree.begin() + j, tree.end(), cmpForTree);      //��������� � ������� ����������� ������� ��� ��������, ����� ���, ������� ��� ������ ��������
	}
	/*cout << "������:\n";
	printTree(tree.size() - 1, tree);
	cout << endl;*/

	cout << tree.size() - 1 << endl;

	/*if (maxSize == 1) {
		count[0].code = "0";
	}*/
	huffmanEncoding(tree.size() - 1, "", tree, count);

	//cout << "\n���� ��� ����:\n";
	for (j = 0; (count[j].frequency); ++j) {
		//cout << '\'' << count[j].letter << "' - " << count[j].code << "  ";
		stringLinks[count[j].letter].code = count[j].code;
	}

	//cout << "\n\n�������������� ������:\n";
	size = 0;
	for (int i = 0; i < S.size(); i++) {
		*res += stringLinks[S[i]].code;
		//strcat(*res, stringLinks[S[i]].code.c_str());
		//cout << stringLinks[S[i]].code;
		//size += stringLinks[S[i]].code.size();
	}
	//cout << "\n���������� ���: " << size << endl;
	cout << "����������� ���������\n";
	delete[] stringLinks;
}

void huffmanEncoding(int num, string code, vector <node> & tree, ltcode * letterCodes) {
	//cout << "�������: " << tree[num].i << " , ��������� ��������: " << tree[num].list << " , ���: " << code << endl;
	tree[num].code = code;
	if (tree[num].list.size() > 1) {
		//cout << "����������� ����� ������� ��� ������ ���������:\n";
		huffmanEncoding(tree[num].left, code + "0", tree, letterCodes);
		//cout << "\n����������� ����� ������� ��� ������� ���������:\n";
		huffmanEncoding(tree[num].right, code + "1", tree, letterCodes);
		//cout << endl;
	}
	else {
		//cout << "��� ��������� ������";
		for (int j = 0; ; j++) {                               //����� � �������� letterCodes ��������� ltcode ������� ������� � ������ ���� ����
			if (letterCodes[j].letter == tree[num].list[0]) {
				letterCodes[j].code = code;
				break;
			}
		}
	}
}

void decodeThis(nodeDecode * tree, ltcode * count, ifstream & in, string * res) {
	cout << "\n�������������...\n";
	char sym = NULL;    //���������� �������
	ltcode* stringLinks = new ltcode[256];
	std::string S;            //������ � ���� ������, ������� ����� ������������
	string result;
	int numOfKeys = 0;
	int k = 0;       //���������� ��� ������ ������ �� ltcode stringLinks � vector <node> tree

	while (!in.eof()) {                       //������������ ���������� ������
		char tmp = NULL;    //��� ���������� �������� ��������
		string codee;        //���������� ���� �������
		sym = reading(in);
		if (sym == '\n') {
			sym = reading(in);
			if (sym != ' ') break;
			else sym = '\n';
		}
		else {
			tmp = reading(in);
			if (tmp != ' ') {
				cout << "������� ������� �����!\n";
				return;
			}
		}
		in >> codee;
		if (codee[0] != '0' && codee[0] != '1') {
			cout << "������� ������� �����!\n";
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

	sort(stringLinks, stringLinks + 256, cmpForStrDecoding);              //���������� ��������� ������� count � ������� ����������� ����� ����
	//cout << "���������� ���: " << S.size() << endl;
	//for (int i = 0; i < 256; i++) cout << "sym:" << stringLinks[i].letter << " code " << stringLinks[i].code << endl;
	numOfKeys = k;
	k = 256 - k;
	//if (stringLinks[k].code == "") k++;

	tree = new nodeDecode;
	nodeDecode* head = tree;
	//int forTree = 0;  //��� �������� ���������� ����� ������. ������ ���������� � 0
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

	//cout << "������:\n";
	//printDecodeTree(tree, 0);
	cout << "������������� ���������\n";

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
	cout << "\n�����������...\n";
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

			if (t.findElem(c, 2)) {                                              // ����� ������� � ������
				t.addOne(c);                                                    // ���� ������, �� ����������� ���
				*res += t.retCode();
			}
			else {
				t.flag = 1;
				t.addElem(c);                                               // ���� �� ������, �� ��������� � ������
				t.findElem(c, 2);
				t.flag = 0;

				str = t.retCode();                                          // �������� ���

				str.pop_back();
				str = str + a.code(c);                                      // ������������ ���������� ���

				*res += str;
				str.clear();
			}
			t.reSum(2);                                                 // ��������� � ��������� ������
			t.check1();
			t.reSum(2);
			t.check2();
			t.reSum(2);
		}
	}
	fin.close();
	if ((*res).size() == 0) {
		cout << "�� ������� ������ ����\n";
	}
	a.print_Codes(fout);
	cout << "����������� ���������\n";
}

void dynamicDecoding(ifstream & fin, ofstream & fout, string * res) {
	cout << "\n�������������...\n";
	char c;
	int flag = 1;
	Tree t;
	Sym a;
	string str;
	string answer;

	if (a.readFile(fin)) {                                                  // ������ ���� �� ������������ ������
		cout << " ���� � ������ ��������" << endl;
		fin.close();
		return;
	}

	while (!fin.eof()) {
		fin.get(c);
		//cout << *res << endl;
		if (fin.eof()) {
			cout << "������������� ���������" << endl;
			return;
		}

		//cout << "��� ������� ������: " << c << endl;

		if ((c != '1' && c != '0' && !fin.eof())) {           // ���� �������� ������ �� 0 ��� �� 1
			cout << "�������� ����" << endl;
			return;
		}


		if (!fin.eof()) {


			if (t.retEsc()) {                                                                         // ���� �������� � Esc, �� ���������
				//cout << "  ��������� ����������� ��� ������� � ���� ��� � ���� � ������"  << endl;  // ����������� ��� �������
				str.push_back(c);

				for (int i = 0; i < 7; i++) {                        // ����������

					if ((c != '1' && c != '0' && !fin.eof())) {
						cout << "�������� ����" << endl;
						return;
					}

					fin.get(c);
					str.push_back(c);

					t.flag = 1;
				}

				c = a.whatSym(str);                     // ���������� ������ �� ����

			   /* if(c != '\n')
					cout << "  ����������� ��� " << str << "  ����������� ������� " << c << endl;
				else if(c != ' ')
					cout << "  ����������� ��� " << str << "  ����������� ������� space" << endl;
				else
					cout << "  ����������� ��� " << str << "  ����������� ������� \\n" <<  endl;

				cout << "      ���������� ���������� ������ � ������" << endl;*/

				str.clear();
			}



			c = t.processSym(c);                    // ����������� ������
			t.findElem(c, 2);
			t.flag = 0;

			if (c != '\0' && !fout.eof()) {

				if (flag) {
					str = t.retCode();
					/*if(c == '\n')
						cout << "  ����� ������: \\n";
					else if(c == ' ')
						cout << "  ����� ������: space" ;
					else
						cout << "  ����� ������: " << c;

					cout << " �� ���� " << str << endl;
					cout << "    ����������� ��� �������" << endl;*/
					str.clear();
				}

				flag = 1;

				//cout << "      ��������� � ��������� ������" << endl;
				t.reSum(2);                                                     // ������������� ������
				t.check1();
				t.reSum(2);
				t.check2();
				t.reSum(2);

				//fout << c;
				*res += c;
			}
			/*else if( !fin.eof()){
				cout << "  ������ ������ �������� ������ ������������� ����" << endl
					<< "  ���������� ����������" << endl;
			}*/
		}
	}
	cout << "������������� ���������\n";
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
