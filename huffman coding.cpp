#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <set>

using namespace std;

class Node {
public:
    Node(shared_ptr <Node> child_left_, shared_ptr <Node> child_right_, size_t amount_, char c_ = '\u0000') 
        : child_left(child_left_), child_right(child_right_), amount(amount_), c(char(c_)) {}

    size_t GetAmount() const {
        return amount;
    }

    shared_ptr <Node> GetLeftChild() const {
        return child_left;
    }

    shared_ptr <Node> GetRightChild() const {
        return child_right;
    }

    char GetChar() const {
        return c;
    }

private:
    shared_ptr <Node> child_left;
    shared_ptr <Node> child_right;
    size_t amount;
    char c;
};

struct kernel {
    bool operator()(const shared_ptr<Node>& left, const shared_ptr<Node>& right) const {
        return (*left).GetAmount() < (*right).GetAmount();
    }
};


shared_ptr<Node> BuildTheTree(multiset<shared_ptr<Node>, kernel>& rate, const string& txt) {
    shared_ptr<Node> root = nullptr;
    while (!rate.empty())
    {
        shared_ptr<Node> temp = *rate.begin();
        rate.erase(rate.begin());
        shared_ptr<Node> temp2 = *rate.begin();
        rate.erase(rate.begin());
        rate.insert({ make_shared<Node>(temp, temp2, (*temp).GetAmount() + (*temp2).GetAmount()) });
        if (rate.size() == 1)
        {
            root = *rate.begin();
            rate.erase(rate.begin());
        }
    }
    return root;
}
void FillAccordMap(unordered_map <char, string>& code, shared_ptr <Node> root, const string& cur_code)
{
    char a = (*root).GetChar();

    if (a != '\u0000') {
        code[a] = cur_code;
    }
    else {
        FillAccordMap(code, (*root).GetLeftChild(), cur_code + "0");
        FillAccordMap(code, (*root).GetRightChild(), cur_code + "1");
    }
}

void ProcessOneCharCase(const string& txt) {
    cout << "1" << ' ' << txt.size() << endl;
    cout << txt[0] << ": " << 0 << endl;
    for (size_t y = 0; y < txt.size(); ++y)
    {
        cout << 0;
    }
}

void GetResult(const string& txt, const unordered_map <char, string>& char_to_its_code) {
    size_t length = 0;
    string result;
    for (size_t j = 0; j < txt.size(); ++j)
    {
        length += char_to_its_code.at(txt[j]).size();
        result += char_to_its_code.at(txt[j]);
    }
    cout << char_to_its_code.size() << ' ' << length << endl;
    for (auto char_ : char_to_its_code)
    {
        cout << static_cast<char>(char_.first) << ": " << char_.second << endl;
    }
    cout << result;
}

void MakeAmountRate(multiset<shared_ptr<Node>, kernel>& chars_amount_rate, const vector<pair<char, size_t>>& chars_rate) {
    for (size_t h = 0; h < chars_rate.size(); ++h) {
        if (chars_rate[h].first != '\u0000') {
            chars_amount_rate.insert(make_shared<Node>(nullptr, nullptr, chars_rate[h].second, chars_rate[h].first));
        }
    }
}

size_t MakeUseRate(const string& txt, vector<pair<char, size_t>>& chars_use_rate) {

    size_t temp;
    size_t dif_char_counter = 0;
    char last_new_sign;
    for (size_t i = 0; i < txt.size(); ++i)
    {
        char c = txt[i];
        temp = (c - 'a') % 26;
        if (chars_use_rate[temp].first != txt[i])
        {
            dif_char_counter++;
            last_new_sign = txt[i];
        }
        chars_use_rate[temp].first = txt[i];
        chars_use_rate[temp].second++;
    }
    return dif_char_counter;
}

int main() {
    string str;
    cin >> str;
    vector <pair<char, size_t>> chars_use_rate(26);
    size_t dif_char_counter = MakeUseRate(str, chars_use_rate);
    if (dif_char_counter == 1) {
        ProcessOneCharCase(str);
    }
    else {
        multiset<shared_ptr<Node>, kernel> chars_amount_rate;
        MakeAmountRate(chars_amount_rate, chars_use_rate);
        shared_ptr<Node> root = BuildTheTree(chars_amount_rate, str);
        string cur_code;
        unordered_map <char, string> char_to_its_code;
        FillAccordMap(char_to_its_code, root, cur_code);
        GetResult(str, char_to_its_code);
    }
    return 0;
}