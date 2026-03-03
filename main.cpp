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
    if (pos_semicolon == string::npos) return {data, -1};
    int idx = -1;
    try {
        idx = stoi(data.substr(pos_semicolon + 1));
    } catch (invalid_argument& e) {
        cerr << e.what();
    }

    return {data.substr(0, pos_semicolon), idx};
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
    int count = 0;
    while (current) {
        nodes[current] = count;
        current = current->next;
        count++;
    }
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    if (!out) {
        throw std::runtime_error("Ошибка записи");
    }
    current = head;
    while (current) {
        size_t string_length = current->data.size();
        out.write(reinterpret_cast<const char*>(&string_length), sizeof(string_length));
        if (!out) {
            throw std::runtime_error("Ошибка записи");
        }
        out.write(current->data.c_str(), string_length);
        if (!out) {
            throw std::runtime_error("Ошибка записи");
        }
        int idx = (current->rand) ? nodes[current->rand] : -1;
        out.write(reinterpret_cast<const char*>(&idx), sizeof(idx));
        if (!out) {
            throw std::runtime_error("Ошибка записи");
        }
        current = current->next;
    }
}

int main()
{
    ifstream in("inlet.in");
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
    out.close();
    return 0;
}
