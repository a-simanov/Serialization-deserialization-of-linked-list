#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

using namespace std;

struct ListNode {
    ListNode* prev = nullptr;
    ListNode* next = nullptr;
    ListNode* rand = nullptr;
    std::string data;
};

pair<string, int> ParsingData (const string& data) { //вспомогательная функция для парсинга строки файла
    if (data.size() == 0) return{"", -1};
    int pos_semicolon = data.find_last_of(";");
    return {data.substr(0, pos_semicolon), stoi(data.substr(pos_semicolon + 1))};
}

ListNode* DeserializationLinkedList (ifstream& in) {
    ListNode* head = nullptr;
    ListNode* tail = nullptr;

    vector<ListNode*> nodes; //вспомогательный вектор для инициализации поля rand
    vector<int> indicies; //вспомогательный вектор для инициализации поля rand

    string line;
    while (getline(in, line)) {
        pair<string, int> parse_data = ParsingData(line);
        ListNode* tmp = new ListNode();
        tmp->data = parse_data.first;
        indicies.push_back(parse_data.second);
        nodes.push_back(tmp);
        if (!head) {
            head = tmp;
            tail = tmp;
        } else {
            tail->next = tmp;
            tmp->prev = tail;
            tail = tmp;
        }
    }

    for (int i = 0; i < nodes.size(); i++) {
        int idx = indicies[i];
        if (idx >= 0 && idx < static_cast<int>(nodes.size())) {
            nodes[i]->rand = nodes[idx];
        } else {
            nodes[i]->rand = nullptr;
        }
    }
    return head;
}

void SerializationLinkedList (ofstream& out, ListNode* head) {
    if (!head) return;
    ListNode* current = head;
    unordered_map<ListNode*, int> nodes; //вспомогательный контейнер для сопоставления узлов и их индексов
    nodes[nullptr] = -1;
    int idx = 0;
    while (current) {
        nodes[current] = idx;
        current = current->next;
        idx++;
    }
    current = head;
    while (current) {
        string line = current->data + ";" + to_string(nodes[current->rand]) + "\n";
        out.write(line.c_str(), line.size());
        current = current->next;
    }
}

int main()
{
    ifstream in("inlet.in",  ios::binary);
    if (!in.is_open()) {
        cerr << "Ошибка открытия файла для чтения!\n";
        return -1;
    }

    ListNode* list_node = DeserializationLinkedList(in);

    ofstream out ("outlet.out", ios::binary);
    if (!out.is_open()) {
        cerr << "Ошибка открытия файла для записи!\n";
        return -1;
    }
    SerializationLinkedList(out, list_node);
    return 0;
}
