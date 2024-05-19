#include <iostream>
#include <unordered_map>
#include <queue>
#include <string>
#include <fstream>

using namespace std;

/**
 * @brief Tạo Node của cây Huffman
 * @param ch: Ký tự tương ứng với nút.
 * @param character: ký tự tương ứng với nút
 * @param data: tần suất xuất hiện của dữ liệu
 * @param left: con trỏ trỏ tới nút trái
 * @param right: con trỏ trỏ tới nút phải
 * @brief isLeaf(): kiểm tra xem con trỏ có phải là lá
 * @return true nếu là nút lá, false nếu không.
*/
class Node {
public:
    char character;
    int data;
    Node* left;
    Node* right;

    Node(char ch, int d) : character(ch), data(d), left(nullptr), right(nullptr) {}
    Node(int d, Node* l, Node* r) : data(d), left(l), right(r) {}
    bool isLeaf() const {
        return left == nullptr && right == nullptr;
    }
};

/**
 * @brief Xuất bảng mã ra một file txt.
 * @param codeMap Bảng mã Huffman.
 * @param filename Tên file đầu ra.
 */

void exportCodeMapToFile(const unordered_map<char, string>& codeMap, const string& filename) {
    ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        cerr << "Unable to open file " << filename << " for writing." << endl;
        return;
    }
    for (const auto& pair : codeMap) {
        outputFile << pair.first << ":" << pair.second << endl;
    }
    outputFile.close();
}

class HuffmanCoding {
private:
    /**
     * @param root con trỏ tới nút gốc
     * @param freqMap: Map lưu trữ tần suất xuất hiện của các kí tự trong chuỗi
     * @param codeMap: Map lưu trữ mã huffman tương ứng với mỗi kí tự
     * @param str: chuỗi đầu vào đc mã hóa
    */
    Node* root = nullptr;
    unordered_map<char, int> freqMap;
    unordered_map<char, string> codeMap;
    string str;

    Node* buildTree(const unordered_map<char, int>& codeMap) { // Xây dựng câu Huffman
        //nút có tần suất xuất hiện nhỏ hơn sẽ có ưu tiên cao hơn.
        auto compare = [](const Node* a, const Node* b) { return a->data > b->data; };
        priority_queue<Node*, vector<Node*>, decltype(compare)> q(compare);

        for (const auto& pair : codeMap) {
            q.push(new Node(pair.first, pair.second));
        }
        Node* root = nullptr;
        while (q.size() > 1) {
            Node* x = q.top(); q.pop();
            Node* y = q.top(); q.pop();
            //trọng số priority chính là trọng số của nút = tổng trọng số 2 nút vừa được chọn
            root = new Node(x->data + y->data, x, y);
            q.push(root);
        }
        return root;
    }
/**
* @brief Tạo mã Huffman cho các ký tự trong cây.
* @param node Nút hiện tại trong cây.
* @param code Mã Huffman hiện tại.
*/
    void buildCode(Node* node, const string& code) { // Tạo mã Huffman
        if (node->isLeaf()) {
            codeMap[node->character] = code;
            return;
        }
        buildCode(node->left, code + "0");
        buildCode(node->right, code + "1");
    }
/**
* @brief Đếm tần suất xuất hiện của các ký tự trong chuỗi.
* @param str Chuỗi đầu vào.
*/
    void countFreq(const string& str) {
        for (char c : str) {
            freqMap[c]++;//bảng thống kê
        }
    }

public:
/**
* @brief Quá trình mã hóa Huffman.
* @param str Chuỗi cần mã hóa.
* @return HuffmanCoding Đối tượng HuffmanCoding sau khi thực hiện quá trình mã hóa.
*/
    HuffmanCoding process(const string& str) {
        this->str = str;
        countFreq(str); //thống kê tần số
        // Nếu chuỗi cần mã hóa chỉ có 1 kí tự lặp đi lặp lại thì mã hóa kí tự đó = 0
        if ( freqMap.size()==1){
            cout << "Encoded string: " ;
            for(auto i : str) cout << 0;
            //
            ofstream outputFile("codeMap.txt");
            outputFile << str[0] << ":" << 0 << endl;

            outputFile.close();
            //
            exit(0) ;
        }
        else {
        this->root = buildTree(freqMap);// xây dựng cây huffman dựa trên bảng thống kê
        this->codeMap.clear();
        buildCode(root, ""); // Tạo mã Huffman

        return *this;
    }
    }
/**
* @brief Mã hóa chuỗi đầu vào thành chuỗi mã Huffman.
* @return Chuỗi đã mã hóa.
*/
    string encode() {
        if (this->codeMap.empty()) {
            return "";
        }
        string encodedString;
        for (char c : str) {
            encodedString += codeMap[c];
        }
        return encodedString;
    }

/**
* @brief Lấy bảng mã Huffman.
* @return Bảng mã Huffman.
*/
    unordered_map<char, string> getCodeMap() const {
        return codeMap;
    }
};

int main() {
    // Chuỗi cần mã hóa
    std::string inputString ;
    cout << "Nhap chuoi can ma hoa: ";
    getline(cin,    inputString);

    // Khởi tạo đối tượng HuffmanCoding và thực hiện quá trình mã hóa
    HuffmanCoding huffman;
    huffman.process(inputString);

    // Lấy chuỗi đã mã hóa
    std::string encodedString = huffman.encode();

    // In ra chuỗi đã mã hóa
    std::cout << "Chuoi ma hoa: " << encodedString << std::endl;
    exportCodeMapToFile(huffman.getCodeMap(), "codeMap.txt");
    return 0;
}

