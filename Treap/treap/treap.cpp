﻿#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include<stack>

using namespace std;

template <class T>
class Node {
public:
    Node() {
        left = right = NULL;
    }
    Node(const T& val, const double& pri, Node* l = NULL, Node* r = NULL) {
        value = val;
        priorty = pri;
        left = l;
        right = r;
    }
    ~Node() {};
    T value;
    double priorty;
    Node* left, * right;
    Node* root = NULL;
    void processData();
    void initNode(Node*& root, T& value, double priorty);
    void rotateLeft(Node*& root);
    void rotateRight(Node*& root);
    void insertNode(string m_name, int m_num_of_confirmed, int m_num_of_death);
    void accessRoot(string s_name, int operation);
    void searchNode(string s_name, Node* refer);
    void deleteNode(Node*& root, string name);
    void showNode(Node*& root);
    void sortNodeKey(Node*& root);

};

typedef struct context {
    string name;
    int num_of_confirmed;
    int num_of_death;
    double rate_of_death = ((double)num_of_death / num_of_confirmed) * 100;
} context;
vector<context> list;
stack<context>st1;
void pushData();


template <class T>
void Node<T>::processData() {
    for (int i = 0; i < list.size(); i++) {
        initNode(root, list[i], list[i].rate_of_death);
    }
}
template <class T>
void Node<T>::initNode(Node*& root, T& value, double priorty) { //루트위치고정
    if (root == NULL) {
        Node* newNode = new Node;
        newNode->priorty = priorty;
        newNode->value = value;
        newNode->left = NULL;
        newNode->right = NULL;
        root = newNode;
        return;
    }
    if (value.name == root->value.name) {
        cout << "중복되는 자료입니다" << endl;
        searchNode(value.name, root);
        cout << "덮어씁니까? (예:1 / 작업을 취소합니다:0)" << endl;
        bool opr; cin >> opr;
        if (opr == true) {
            root->value.name = "deleted";
            cout << "기존 데이터";
            deleteNode(root, "deleted");
            initNode(root, value, priorty);
            opr = 0;
        }
        else return;
    }
    else if (value.name < root->value.name) {
        initNode(root->left, value, priorty);
        if (root->left != NULL && root->left->value.rate_of_death > root->value.rate_of_death)
          
  rotateRight(root);
    }
    else {
        initNode(root->right, value, priorty);
        if (root->right != NULL && root->right->value.rate_of_death > root->value.rate_of_death)
            rotateLeft(root);
    }

}
template <class T>
void Node<T>::rotateRight(Node*& root) {
    Node* temp = root->left;
    Node* leafDir = root->left->right;
    temp->right = root;
    root->left = leafDir;
    root = temp;
}

template <class T>
void Node<T>::rotateLeft(Node*& root) {
    Node* temp = root->right;
    Node* leafDir = root->right->left;
    temp->left = root;
    root->right = leafDir;
    root = temp;
}
template <class T>
void Node<T>::insertNode(string m_name, int m_num_of_confirmed, int m_num_of_death) {
    list.push_back({ m_name, m_num_of_confirmed, m_num_of_death });
    initNode(root, list.back(), list.back().rate_of_death);
}
template <class T>
void Node<T>::accessRoot(string s_name, int operation) {
    if (operation == 0)
        searchNode(s_name, root);
    if (operation == 1)
        deleteNode(root, s_name);
    if (operation == 2)
        showNode(root);
    if (operation == 3)
        sortNodeKey(root);
}

template <class T>
void Node<T>::searchNode(string s_name, Node* refer) {
    if (s_name < (refer->value.name)) {
        if (refer->left == NULL)
            cout << "찾을수없습니다." << endl;
        else searchNode(s_name, refer->left);
    }
    if (s_name > (refer->value.name)) {
        if (refer->right == NULL)
            cout << "찾을 수 없습니다." << endl;
        else searchNode(s_name, refer->right);
    }
    //찾는값이 노드랑 일치하지 않음 보다 큰 알파벳 오른쪽 탐색, 작은 좌측 탐색,찾는값이면 출력,리프노드인데 찾는값이 없다? 미확인표시
    else if (s_name == refer->value.name) {
        cout << (string)refer->value.name << "에 대한 자료입니다" << endl
            << "확진자" << (int)refer->value.num_of_confirmed << "명, 사망자"
            << (int)refer->value.num_of_death << "명, 치명률"
            << (double)refer->value.rate_of_death << "%" << endl;
    }
}

template <class T>
void Node<T>::deleteNode(Node*& root, string name) {
    if (name != (root->value.name)) {
        if (name < (root->value.name)) {
            if (root->left == NULL)
                cout << "찾을수없습니다." << endl;
            else deleteNode(root->left, name);
        }
        if (name > (root->value.name)) {
            if (root->right == NULL)
                cout << "찾을수없습니다." << endl;
            else deleteNode(root->right, name);
        }
    }
    else {
        if (root->left == NULL && root->right == NULL) {
            delete root;
            root = NULL;
            cout << "1유형삭제됨." << endl;
        }
        else if (root->left && root->right) {
            if (root->left->value.num_of_death < root->right->value.num_of_death) {
                rotateLeft(root);
                deleteNode(root->left, name);
            }
            else {
                rotateRight(root);
                deleteNode(root->right, name);
            }
        }
        else {
            Node* leaf = (root->left) ? root->left : root->right;
            Node* temp = root;
            root = leaf;
            delete temp;
            cout << "2유형삭제됨." << endl;
        }
    }
}

/*
template <class T>
void Node<T>::deleteNode(Node*& root, string name) {
    if (name == "root") {
        cout << "루트노드 " << root->value.name << "을 삭제합니다" << endl;
        deleteNode(root, root->value.name);
    }
    else if (name != (root->value.name)) {
        if (name < (root->value.name)) {
            if (root->left == NULL)
                cout << "찾을 수 없습니다." << endl;
            else deleteNode(root->left, name);
        }
        if (name > (root->value.name)) {
            if (root->right == NULL)
                cout << "찾을 수 없습니다." << endl;
            else deleteNode(root->right, name);
        }
    }
    else {
        if (root->left == NULL && root->right == NULL) {
            delete root;
            root = NULL;
            cout << "삭제완료(leaf node delete successed)" << endl;
        }//역순
        else if (root->left && root->right) {
            if (root->left->value.num_of_death < root->right->value.num_of_death) {
                rotateLeft(root);
                deleteNode(root->left, name);
            }
            else {
                rotateRight(root);
                deleteNode(root->right, name);
            }
        }
        else {
            Node* leaf = (root->left) ? root->left : root->right;
            Node* temp = root;
            root = leaf;
            delete temp;
            cout << "삭제완료(leaf node merge successful)" << endl;
        }
    }
}
*/
template <class T>
void Node<T>::showNode(Node*& root) {
    if (root == NULL) return;
    showNode(root->right);
    cout << (string)root->value.name << "에 대한 자료입니다" << endl
        << "확진자" << (int)root->value.num_of_confirmed << "명, 사망자"
        << (int)root->value.num_of_death << "명, 치명률"
        << (double)root->value.rate_of_death << "%" << endl;
    showNode(root->left);
}






template <class T>
void Node<T>::sortNodeKey(Node*& root) {
    while (root) {
        st1.push(root->value);
        deleteNode(root, root->value.name);
    }
    while (!st1.empty()) {
        insertNode(st1.top().name, st1.top().num_of_confirmed, st1.top().num_of_death);
        cout << st1.top().name << "에 대한 자료입니다" << endl
            << "확진자" << st1.top().num_of_confirmed << "명, 사망자"
            << st1.top().num_of_death << "명, 치명률"
            << st1.top().rate_of_death << "%" << endl;
        st1.pop();
    }
}




int main() {
    bool Init = false;
    int mainMenuSet;
    string m_name;
    int m_num_of_confirmed;
    int m_num_of_death;
    Node<context> nodedata;
    pushData();
    nodedata.processData();

    cout << "\n\n데이터구조::Treap을 사용하는 코로나 발병리스트 ";
    while (true) {
        cout << "\n\n----------명령을 선택하십시오----------\n"
            << "1.새로운 국가데이터를 추가\n" << "2.데이터 검색\n"
            << "3.기존 국가데이터를 삭제\n" << "4.모든 데이터 출력()\n" << "5.프로그램 종료\n" << endl;
        cin >> mainMenuSet;
        if ((mainMenuSet < 1) || (mainMenuSet > 10)) {
            printf("잘못 입력하였습니다.\n");
            cin.clear();//내용초기화
            cin.ignore(100, '\n');//무시할 명령어의수와 이그노어 종료키 
            continue;
        }
        switch (mainMenuSet) {
        case 1:
            cout << "국가명입력: "; cin >> m_name;
            cout << "\n확진자수입력: "; cin >> m_num_of_confirmed;
            cout << "\n사망자수입력: ";  cin >> m_num_of_death;
            nodedata.insertNode(m_name, m_num_of_confirmed, m_num_of_death);
            cout << "입력 완료" << endl;
            break;
        case 2:
            cout << "국가명입력: "; cin >> m_name;
            nodedata.accessRoot(m_name, 0);
            break;
        case 3:
            cout << "삭제할 국가명을 입력하십시오, root를  입력해 루트노트를 삭제가능합니다.";
            cin >> m_name;
            nodedata.accessRoot(m_name, 1);
            break;
        case 4:
            nodedata.accessRoot(m_name, 2);
            break;
        case 5:
            nodedata.accessRoot(m_name, 3);
            break;
        case 6:
            exit(0);
            break;
        }
    }

return 0;
}

//may 31 1400 first commit
//may 31 2200 템플릿, 틀잡기완료
//June 1  0100 입출력구조, 구조체 입력완료
//June 1  2200 데이터삽입구현
//June 1  2300 탐색구현완료
//June 1  2400 메인메뉴구성
//June 2  1500 데이터삽입오류발견
//June 2  1800오류확인: leaf뒤에 root가 붙음, 원인은 루트가 자꾸흔들려서->노드구축을 두군데에서하니까
//June 2  1930 데이터알고리즘수정
//June 2  2100 삭제구현완료
//June 2  2300 최종리팩토링,테스트용 자동입력코드 제거
//June 3  0100 relese
//June 8  1420 root삭제 추가
//June 11 1910 출력방식변경(value기반 출력)
void pushData() {
    list.push_back({ "Spain", 140510, 13798 });
    list.push_back({ "Italy", 135586, 17127 });
    list.push_back({ "Germany", 107591, 2012 });
    list.push_back({ "China", 81802, 3333 });
    list.push_back({ "France", 78167, 10328 });
    list.push_back({ "Iran", 62589, 3872 });
    list.push_back({ "U.K", 55242, 6159 });
    list.push_back({ "Japan",207591, 15487 });
    list.push_back({ "Korea",104521, 14567 });
    list.push_back({ "U.S",545247, 42348 });
}
    /*list.push_back({ "Comoros", 3872, 146 });
    list.push_back({ "Congo", 11476, 150 });
    list.push_back({ "Cote_dIvoire", 46942, 298 });
    list.push_back({ "Democratic_Republic_of_the_Congo", 30863, 779 });
    list.push_back({ "Djibouti", 11493, 153 });
    list.push_back({ "Egypt", 253835, 14721 });
    list.push_back({ "Equatorial_Guinea", 8476, 113 });
    list.push_back({ "Eritrea", 3932, 14 });
    list.push_back({ "Eswatini", 18551, 672 });
    list.push_back({ "Ethiopia", 269194, 4076 });
    list.push_back({ "Gabon", 24107, 147 });
    list.push_back({ "Gambia", 5978, 178 });
    list.push_back({ "Ghana", 93644, 783 });
    list.push_back({ "Guinea", 23052, 159 });
    list.push_back({ "Guinea_Bissau", 3749, 68 });
    list.push_back({ "Kenya", 168432, 3059 });
    list.push_back({ "Lesotho", 10822, 326 });
    list.push_back({ "Liberia", 2142, 85 });
    list.push_back({ "Libya", 183592, 3112 });
    list.push_back({ "Madagascar", 40876, 800 });
    list.push_back({ "Malawi", 34284, 1153 });
    list.push_back({ "Mali", 14241, 514 });
    list.push_back({ "Mauritania", 19149, 458 });
    list.push_back({ "Mauritius", 1322, 17 });
    list.push_back({ "Morocco", 517023, 9122 });
    list.push_back({ "Mozambique", 70590, 831 });
    list.push_back({ "Namibia", 52946, 765 });
    list.push_back({ "Niger", 5383, 192 });
    list.push_back({ "Nigeria", 166019, 2067 });
    list.push_back({ "Rwanda", 26688, 349 });
    list.push_back({ "Sao_Tome_and_Principe", 2338, 37 });
    list.push_back({ "Senegal", 41142, 1132 });
    list.push_back({ "Seychelles", 10433, 38 });
    list.push_back({ "Sierra_Leone", 4121, 79 });
    list.push_back({ "Somalia", 14594, 758 });
    list.push_back({ "South_Africa", 1635465, 55802 });
    list.push_back({ "South_Sudan", 10670, 115 });
    list.push_back({ "Sudan", 34889, 2446 });
    list.push_back({ "Togo", 13374, 125 });
    list.push_back({ "Tunisia", 336414, 12273 });
    list.push_back({ "Uganda", 43734, 356 });
    list.push_back({ "United_Republic_of_Tanzania", 509, 21 });
    list.push_back({ "Western_Sahara", 766, 1 });
    list.push_back({ "Zambia", 93201, 1268 });
    list.push_back({ "Zimbabwe", 38682, 1586 });
    list.push_back({ "Anguilla", 109,0 });
    list.push_back({ "Antigua_and_Barbuda", 1258, 42 });
    list.push_back({ "Argentina", 3562072, 74480 });
    list.push_back({ "Aruba", 10922, 107 });
    list.push_back({ "Bahamas", 11552, 226 });
    list.push_back({ "Barbados", 4000, 47 });
    list.push_back({ "Belize", 12764, 323 });
    list.push_back({ "Bermuda", 2490, 32 });
    list.push_back({ "Bolivia", 349653, 13965 });
    list.push_back({ "Bonaire, Saint Eustatiusand Saba", 1610, 17 });
    list.push_back({ "Brazil", 16047439, 448208 });
    list.push_back({ "British_Virgin_Islands", 248, 1 });
    list.push_back({ "Canada", 1361564, 25265 });
    list.push_back({ "Cayman_Islands", 578, 2 });
    list.push_back({ "Chile", 1335261, 28548 });
    list.push_back({ "Colombia", 3249433, 85207 });
    list.push_back({ "Costa_Rica", 304529, 3849 });
    list.push_back({ "Cuba", 134208, 890 });
    list.push_back({ "Curaçao", 12271, 122 });
    list.push_back({ "Dominica", 184,0 });
    list.push_back({ "Dominican_Republic", 284567, 3610 });
    list.push_back({ "Ecuador", 424635, 20210 });
    list.push_back({ "El_Salvador", 72821, 2224 });
    list.push_back({ "Falkland_Islands_(Malvinas});", 63,0 });
    list.push_back({ "Greenland", 34,0 });
    list.push_back({ "Grenada", 161, 1 });
    list.push_back({ "Guatemala", 247644, 8009 });
    list.push_back({ "Guyana", 16130, 359 });
    list.push_back({ "Haiti", 14037, 292 });
    list.push_back({ "Honduras", 232672, 6146 });
    list.push_back({ "Jamaica", 48054, 925 });
    list.push_back({ "Mexico", 2397307, 221695 });
    list.push_back({ "Montserrat", 20, 1 });
    list.push_back({ "Nicaragua", 7193, 185 });
    list.push_back({ "Panama", 374121, 6331 });
    list.push_back({ "Paraguay", 332971, 8360 });
    list.push_back({ "Peru", 1926923, 68358 });
    list.push_back({ "Puerto_Rico", 263767, 2480 });
    list.push_back({ "Saint_Kitts_and_Nevis", 60,0 });
    list.push_back({ "Saint_Lucia", 4945, 77 });
    list.push_back({ "Saint_Vincent_and_the_Grenadines", 1989, 12 });
    list.push_back({ "Sint_Maarten", 2370, 28 });
    list.push_back({ "Suriname", 13302, 256 });
    list.push_back({ "Trinidad_and_Tobago", 20017, 375 });
    list.push_back({ "Turks_and_Caicos_islands", 2409, 17 });
    list.push_back({ "United_States_of_America", 33117765, 589893 });
    list.push_back({ "United_States_Virgin_Islands", 3383, 27 });
    list.push_back({ "Uruguay", 267888, 3922 });
    list.push_back({ "Venezuela", 222052, 2499 });
    list.push_back({ "Afghanistan", 66275, 2812 });
    list.push_back({ "Bahrain", 218047, 820 });
    list.push_back({ "Bangladesh", 789080, 12376 });
    list.push_back({ "Bhutan", 1394, 1 });
    list.push_back({ "Brunei_Darussalam", 236, 3 });
    list.push_back({ "Cambodia", 25761, 179 });
    list.push_back({ "China", 102873, 4846 });
    list.push_back({ "India", 26752447, 303720 });
    list.push_back({ "Indonesia", 1775220, 48093 });
    list.push_back({ "Iran", 2823887, 78381 });
    list.push_back({ "Iraq", 1167940, 16190 });
    list.push_back({ "Israel", 839319, 6404 });
    list.push_back({ "Japan", 714274, 12236 });
    list.push_back({ "Jordan", 731436, 9384 });
    list.push_back({ "Kazakhstan", 423193, 4933 });
    list.push_back({ "Kuwait", 299215, 1734 });
    list.push_back({ "Kyrgyzstan", 102702, 1751 });
    list.push_back({ "Laos", 1801, 2 });
    list.push_back({ "Lebanon", 538518, 7677 });
    list.push_back({ "Malaysia", 518600, 2248 });
    list.push_back({ "Maldives", 55924, 129 });
    list.push_back({ "Mongolia", 53100, 253 });
    list.push_back({ "Myanmar", 143296, 3216 });
    list.push_back({ "Nepal", 513241, 6346 });
    list.push_back({ "Oman", 210364, 2265 });
    list.push_back({ "Pakistan", 903599, 20308 });
    list.push_back({ "Palestine", 334264, 3734 });
    list.push_back({ "Philippines", 1179812, 19951 });
    list.push_back({ "Qatar", 215443, 544 });
    list.push_back({ "Saudi_Arabia", 440914, 7249 });
    list.push_back({ "Singapore", 61824, 32 });
    list.push_back({ "South_Korea", 136467, 1934 });
    list.push_back({ "Sri_Lanka", 164201, 1210 });
    list.push_back({ "Syria", 24117, 1734 });
    list.push_back({ "Taiwan", 4322, 23 });
    list.push_back({ "Tajikistan", 13714, 91 });
    list.push_back({ "Thailand", 132213, 806 });
    list.push_back({ "Timor_Leste", 5637, 13 });
    list.push_back({ "United_Arab_Emirates", 556107, 1651 });
    list.push_back({ "Uzbekistan", 98657, 680 });
    list.push_back({ "Vietnam", 5308, 43 });
    list.push_back({ "Yemen", 6653, 1305 });
    list.push_back({ "Albania", 132209, 2444 });
    list.push_back({ "Andorra", 13609, 127 });
    list.push_back({ "Armenia", 221982, 4394 });
    list.push_back({ "Austria", 637381, 10283 });
    list.push_back({ "Azerbaijan", 332454, 4860 });
    list.push_back({ "Belarus", 386025, 2771 });
    list.push_back({ "Belgium", 1049727, 24851 });
    list.push_back({ "Bosnia_and_Herzegovina", 203150, 9108 });
    list.push_back({ "Bulgaria", 416646, 17496 });
    list.push_back({ "Croatia", 354060, 7929 });
    list.push_back({ "Cyprus", 71911, 354 });
    list.push_back({ "Czechia", 1658092, 30028 });
    list.push_back({ "Denmark", 274413, 2509 });
    list.push_back({ "Estonia", 128669, 1241 });
    list.push_back({ "Faroe_Islands", 692, 1 });
    list.push_back({ "Finland", 91619, 939 });
    list.push_back({ "France", 5603666, 108625 });
    list.push_back({ "Georgia", 337961, 4639 });
    list.push_back({ "Germany", 3651640, 87423 });
    list.push_back({ "Gibraltar", 4288, 94 });
    list.push_back({ "Greece", 389804, 11772 });
    list.push_back({ "Guernsey", 822, 14 });
    list.push_back({ "Holy_See", 27, 0 });
    list.push_back({ "Hungary", 802346, 29560 });
    list.push_back({ "Iceland", 6556, 29 });
    list.push_back({ "Ireland", 257903, 4941 });
    list.push_back({ "Isle_of_Man", 1591, 29 });
    list.push_back({ "Italy", 4192183, 125225 });
    list.push_back({ "Jersey", 3238, 69 });
    list.push_back({ "Kosovo", 107244, 2239 });
    list.push_back({ "Latvia", 130945, 2326 });
    list.push_back({ "Liechtenstein", 3002, 58 });
    list.push_back({ "Lithuania", 271682, 4209 });
    list.push_back({ "Luxembourg", 69607, 811 });
    list.push_back({ "Malta", 30506, 417 });
    list.push_back({ "Moldova", 254676, 6075 });
    list.push_back({ "Monaco", 2501, 32 });
    list.push_back({ "Montenegro", 99248, 1574 });
    list.push_back({ "Netherlands", 1626816, 17532 });
    list.push_back({ "North_Macedonia", 155063, 5311 });
    list.push_back({ "Norway", 122372, 782 });
    list.push_back({ "Poland", 2866181, 72945 });
    list.push_back({ "Portugal", 845465, 17018 });
    list.push_back({ "Romania", 1075773, 29977 });
    list.push_back({ "Russia", 5001505, 118482 });
    list.push_back({ "San_Marino", 5089, 90 });
    list.push_back({ "Serbia", 709939, 6777 });
    list.push_back({ "Slovakia", 772833, 12296 });
    list.push_back({ "Slovenia", 251773, 4675 });
    list.push_back({ "Spain", 3647520, 79711 });
    list.push_back({ "Sweden", 1063183, 14396 });
    list.push_back({ "Switzerland", 687029, 10182 });
    list.push_back({ "Turkey", 5186487, 46268 });
    list.push_back({ "Ukraine", 2183855, 49436 });
    list.push_back({ "United_Kingdom", 4462538, 127721 });
    list.push_back({ "Australia", 30011, 910 });
    list.push_back({ "Fiji", 230, 4 });
    list.push_back({ "French_Polynesia", 18844, 141 });
    list.push_back({ "Guam", 8136, 139 });
    list.push_back({ "Marshall_Islands", 4,0 });
    list.push_back({ "New_Caledonia", 125,0 });
    list.push_back({ "New_Zealand", 2311, 26 });
    list.push_back({ "Northern_Mariana_Islands", 181, 2 });
    list.push_back({ "Papua_New_Guinea", 15133, 154 });
    list.push_back({ "Solomon_Islands", 20, 0 });
    list.push_back({ "Vanuatu", 3,0 });
    list.push_back({ "Wallis_and_Futuna", 445, 7 });*/

