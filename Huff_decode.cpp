#include <iostream>
#include <unordered_map>
#include <string>
#include<fstream>
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

    Node(char ch) : character(ch), left(nullptr), right(nullptr) {}
    bool isLeaf() const {
        return left == nullptr && right == nullptr;
    }
};

/**
 * @brief Đọc một tập tin và tạo một unordered_map nơi mỗi ký tự ánh xạ tới một mã chuỗi.
 * Tập tin có các dòng theo định dạng: `character:code`
 * @param character kí tự cần mã hóa
 * @param code đoạn mã mã hóa cho kí tự đó
 * @param filename Tên của tập tin cần đọc.
 * @return Một unordered_map<char, string> trong đó mỗi cặp key-value tương ứng với một ký tự và chuỗi mã hóa của nó.
 *         Nếu tập tin không thể mở, trả về một map rỗng.
 */
unordered_map<char, string> readCodeMapFromFile(const string& filename) {
    unordered_map<char, string> codeMap;
    ifstream inputFile(filename);
    string line;
    while (getline(inputFile, line)) {
        // Tìm vị trí của dấu hai chấm
        size_t colonPos = line.find(':');
        // Gán character là ký tự trước dấu hai chấm
        char character = line[0];
        //Gán code là các kí tự sau dấu hai chấm (chuỗi mã hóa của từ)
        string code = line.substr(colonPos + 1);
        //Lưu trữ các thành phần trên vào map
        codeMap[character] = code;
    }
    inputFile.close();
    return codeMap;
}


/**
 * @brief Xây dựng cây Huffman từ bảng mã.
 * @param codeMap Bảng mã lưu trữ các ký tự và mã tương ứng.
 * @return Con trỏ tới nút gốc của cây Huffman.
 */
Node* buildTree(const unordered_map<char, string>& codeMap) { 
    Node* root = new Node('\0');
    // Duyệt qua từng cặp ký tự - mã
    for (const auto& pair : codeMap) {
        Node* currentNode = root;
        const string& code = pair.second;
        for (char bit : code) {
            // Nếu bit là '0', đi sang trái
            if (bit == '0') {
                if (currentNode->left == nullptr) {
                    currentNode->left = new Node('\0');
                }
                currentNode = currentNode->left;
            }
            // Nếu bit là '1', đi sang phải
            else {
                if (currentNode->right == nullptr) {
                    currentNode->right = new Node('\0');
                }
                currentNode = currentNode->right;
            }
        }
        // Gán ký tự cho nút lá
        currentNode->character = pair.first;
    }

    return root;
    }


/**
 * @brief Giải mã văn bản đã được mã hóa bằng cây Huffman.
 * @param root Con trỏ tới nút gốc của cây Huffman.
 * @param encoded Chuỗi đã được mã hóa.
 * @return Chuỗi đã giải mã.
*/
string decodeHuffman(Node* root, const string& encoded) {
    string decodedString;
    Node* currentNode = root;

    // Duyệt qua từng bit trong chuỗi mã hóa
    for (char bit : encoded) {
        // Nếu bit là '0', đi sang trái
        if (bit == '0') {
            currentNode = currentNode->left;
        }
        // Nếu bit là '1', đi sang phải
        else {
            currentNode = currentNode->right;
        }

        // Nếu gặp nút lá, thêm ký tự tương ứng vào chuỗi giải mã và quay về nút gốc
        if (currentNode->left == nullptr && currentNode->right == nullptr) {
            decodedString.push_back(currentNode->character);
            currentNode = root;
        }
    }
    return decodedString;
}

int main() {

    //Nhập bảng mã Huffman từ file
    string filename = "codeMap.txt";
    unordered_map<char, string> codeMap = readCodeMapFromFile(filename);


    // Xây dựng cây Huffman từ bảng mã
    Node* root = buildTree(codeMap);

    // Nhập chuỗi cần giải mã
    string encodedString ;
    cout << "Nhap chuoi can giai ma: ";
    getline(cin,encodedString);

    //Giải mã và in ra
    string decodedString = decodeHuffman(root, encodedString);
    cout << "Chuoi da duoc giai ma: " << decodedString << endl;

    return 0;
}
