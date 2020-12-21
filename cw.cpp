#include <iostream>
#include <memory>
#include <cwchar>
#include <cwctype>
#include <cctype>
#include <sstream>
#include <vector>
#include <deque>
#include <utility>
#include <cstring>
#include <clocale>

#define PRINT

#ifdef PRINT

constexpr const wchar_t* kRED = L"\033[1;31m";
constexpr const wchar_t* kBLACK = L"\033[1;30m";
constexpr const wchar_t* kBLUE = L"\033[1;34m";
constexpr const wchar_t* kORANGE = L"\033[1;33m";
constexpr const wchar_t* kNORMAL = L"\033[0m";
constexpr const wchar_t* kNULL = L"nullptr";
#define SPACER (L'\n' + std::wstring(42, 0x2500) + L'\n')
constexpr wchar_t kVERTICAL = 0x2502;
constexpr wchar_t kHORIZONTAL = 0x2500;
constexpr wchar_t kLEFT_UPPER_CORNER = 0x250c;
constexpr wchar_t kLEFT_DOWN_CORNER = 0x2514;
constexpr wchar_t kRIGHT_UPPER_CORNER = 0x2510;
constexpr wchar_t kRIGHT_DOWN_CORNER = 0x2518;
constexpr wchar_t kCONNECT_UP = 0x2534;
constexpr wchar_t kCONNECT_LEFT = 0x2524;
constexpr wchar_t kCONNECT_RIGHT = 0x251c;

#endif

template<typename T>
class RedBlackTreeNode{
public:
    bool is_red = true;
    bool is_left = false;
    // Поля left_pos и right_pos хранят позиции узла в дереве для печати
    int left_pos = 0, right_pos = 0;
    T data;
    std::shared_ptr<RedBlackTreeNode<T>> left{nullptr}, right{nullptr};
    std::weak_ptr<RedBlackTreeNode<T>> parent;
    RedBlackTreeNode() = default;

    RedBlackTreeNode(T& data){
        this->data = data;
    }
    
    RedBlackTreeNode(T&& data){
        this->data = data;
    }

    ~RedBlackTreeNode() = default;
    
    RedBlackTreeNode(const RedBlackTreeNode<T>& node){
        data = node.data;
        left = node.left;
        right = node.right;
        parent = node.parent;
        is_red = node.is_red;
        is_left = node.is_left;
        left_pos = left_pos;
        right_pos = right_pos;
    }

    RedBlackTreeNode& operator=(const RedBlackTreeNode<T>& node){
        if(&node == this) return *this;
        data = node.data;
        left = node.left;
        right = node.right;
        parent = node.parent;
        is_red = node.is_red;
        is_left = node.is_left;
        left_pos = left_pos;
        right_pos = right_pos;
        return *this;
    }
    
    RedBlackTreeNode(RedBlackTreeNode<T>&& node){
        data = std::move(node.data);
        left = std::move(node.left);
        right = std::move(node.right);
        parent = std::move(node.parent);
        is_red = node.is_red;
        is_left = node.is_left;
        left_pos = left_pos;
        right_pos = right_pos;
    }

    RedBlackTreeNode& operator=(RedBlackTreeNode<T>&& node){
        if(&node == this) return *this;
        data = std::move(node.data);
        left = std::move(node.left);
        right = std::move(node.right);
        parent = std::move(node.parent);
        is_red = node.is_red;
        is_left = node.is_left;
        left_pos = left_pos;
        right_pos = right_pos;
        return *this;
    }
};

template<typename T>
std::wostream& operator<<(std::wostream& os, const RedBlackTreeNode<T>& node){
    os << "{data: " << node.data << "; is_red: " << node.is_red
       << "; is_left: " << node.is_left << "; left: ";
    if(node.left) os << node.left->data;
    else os << "nullptr";
    os << "; right: ";
    if(node.right) os << node.right->data;
    else os << "nullptr";
    os << "; parent: ";
    if(node.parent.lock()) os << node.parent.lock()->data;
    else os << "nullptr";
    os << ";}";
    return os;
}

template<typename T>
class RedBlackTree{
using NodePtr = std::shared_ptr<RedBlackTreeNode<T>>;
protected:
    NodePtr head{nullptr};
    std::wostream& out = std::wcout;

    bool IsBlackNode(NodePtr node){
        return (node == nullptr || !node->is_red);
    }

    bool IsRedNode(NodePtr node){
        return !IsBlackNode(node);
    }

    bool Recolor(NodePtr node){
        if(!node->parent.lock()){
            #ifdef PRINT
            out << L"Элемент " << (node->is_red ? kRED : kBLACK)
            << node->data << kNORMAL << L" является корнем дерева. Перекрашиваем его в "
            << kBLACK << L"чёрный" << kNORMAL << L" цвет.\n";
            node->is_red = false;
            PrintTree(out, node);
            out << SPACER;
            #else
            node->is_red = false;
            #endif
            return true;
        }
        if(!node->parent.lock()->parent.lock()){
            auto parent = node->parent.lock();
            #ifdef PRINT
            out << L"Отец " << kBLACK << parent->data << kNORMAL << L" является корнем дерева и имеет "
            << kBLACK << L"чёрный" << kNORMAL << L" цвет. Дополнительные действия не требуются.\n";
            parent->is_red = false;
            PrintTree(out, node);
            out << SPACER;
            #else
            parent->is_red = false;
            #endif
            return true;
        }
        auto parent = node->parent.lock();
        auto grandparent = parent->parent.lock();
        auto uncle = (parent->is_left ? grandparent->right : grandparent->left);
        if(IsRedNode(parent) && IsRedNode(uncle)){
            #ifdef PRINT
            out << L"Отец красный(" << kRED << parent->data << kNORMAL << L") и дядя красный(" << kRED
            << uncle->data << kNORMAL << L"). Перекрашиваем их в " << kBLACK << L"чёрный" << kNORMAL << L" цвет." 
            << L"Деда перекрашиваем в " << kRED << L"красный" << kNORMAL << L" цвет.\n";
            parent->is_red = uncle->is_red = false;
            grandparent->is_red = true;
            PrintTree(out, node);
            out << SPACER;
            #else
            parent->is_red = uncle->is_red = false;
            grandparent->is_red = true;
            #endif
            Balance(grandparent, false);
        }else if(IsRedNode(parent)){
            #ifdef PRINT
            out << L"Отец красный(" << kRED << node->data << kNORMAL << L").Перекрашиваем его в "
            << kBLACK << L"чёрный" << kNORMAL << L" цвет.\n";
            parent->is_red = false;
            PrintTree(out, node);
            out << SPACER;
            #else
            parent->is_red = false;
            #endif
        }else{
            #ifdef PRINT
            out << L"Отец чёрный(" << kBLACK << parent->data << kNORMAL << L").Дополнительные действия не требуются.\n";
            PrintTree(out, node);
            out << SPACER;
            #endif
            return false;
        }
        return true;
    }

    void SmallRotate(NodePtr node){
        if(!node) return;
        if(!node->parent.lock()) return;
        if(!node->parent.lock()->parent.lock()) return;
        auto parent = node->parent.lock();
        auto grandparent = parent->parent.lock(); 
        #ifdef PRINT
        auto uncle = (parent->is_left ? grandparent->right : grandparent->left);
        out << L"Дядя чёрный(" << kBLACK;
        if(uncle) out << uncle->data;
        else out << kNULL;
        out << kNORMAL<< L"), отец красный("
        << kRED << parent->data << kNORMAL << L"). Отец и новый элемент находятся в разных сторонах. Необходимо выполнить малый поворот.\n";
        PrintTree(out, node);
        out << SPACER;
        #endif
        node->parent = grandparent;
        if(parent->is_left){
            grandparent->left = node;
            node->is_left = true;
            parent->right = node->left;
            if(node->left){
                node->left->is_left = false;
                node->left->parent = parent;
            }
            parent->parent = node;
            node->left = parent;
        }else{
            grandparent->right = node;
            node->is_left = false;
            parent->left = node->right;
            if(node->right){
                node->right->is_left = true;
                node->right->parent = parent;
            }
            parent->parent = node;
            node->right = parent;
        }
        #ifdef PRINT
        out << L"Малый поворот для элемента " << kORANGE
        << node->data << kNORMAL << L" выполнен. Теперь необходимо выполнить большой поворот\n";
        PrintTree(out, node, grandparent);
        out << SPACER;
        #endif
    }
        
    void BigRotate(NodePtr node){
        if(!node) return;
        if(!node->parent.lock()) return;
        if(!node->parent.lock()->parent.lock()) return;
        auto parent = node->parent.lock();
        auto grandparent = parent->parent.lock();
        auto uncle = (parent->is_left ? grandparent->right : grandparent->left);
        #ifdef PRINT
        out << L"Дядя чёрный(" << kBLACK;
        if(uncle) out << uncle->data;
        else out << kNULL;
        out << kNORMAL << L"), отец красный("
        << kRED << parent->data << kNORMAL << L"). Отец и новый элемент находятся в одной стороне. Необходимо выполнить большой поворот.\n";
        PrintTree(out, node);
        out << SPACER;
        #endif
        std::swap(parent->data, grandparent->data);
        if(parent->is_left){
            parent->left = parent->right;
            if(parent->left) parent->left->is_left = true;
            parent->right = uncle;
            if(uncle) uncle->parent = parent;
            grandparent->right = parent;
            parent->is_left = false;
            grandparent->left = node;
            node->parent = grandparent;
        }else{
            parent->right = parent->left;
            if(parent->right) parent->right->is_left = false;
            parent->left = uncle;
            if(uncle) uncle->parent = parent;
            grandparent->left = parent;
            parent->is_left = true;
            grandparent->right = node;
            node->parent = grandparent;
        }
        #ifdef PRINT
        out << L"Большой поворот для элемента " << kORANGE
        << node->data << kNORMAL << L" выполнен.\n";
        PrintTree(out, node, grandparent);
        out << SPACER;
        #endif
    }

    bool Rotate(NodePtr& node){
        if(!node->parent.lock()) return false;
        if(!node->parent.lock()->parent.lock()) return false;
        auto parent = node->parent.lock();
        auto uncle = (parent->is_left ? parent->parent.lock()->right : 
                        parent->parent.lock()->left);
        if(IsRedNode(parent) && IsBlackNode(uncle)){
            if(node->is_left != parent->is_left){
                SmallRotate(node);
                BigRotate(parent);
                node = parent;
            }else{
                BigRotate(node);
            }
        }else return false;
        return true;
    }

    #ifdef PRINT
    void Balance(NodePtr node, bool is_start = true){
        out << L"Необходимо выполнить перебалансировку дерева по элементу " 
        << kRED << node->data << kNORMAL << L"." << SPACER;
        bool was_rotate = Rotate(node);
        bool was_recolor = Recolor(node);
        if(is_start){
            if(!(was_rotate || was_recolor)) out << L"Дерево уже сбалансированно, никаких дополнительных действий выполнять не нужно.\n";
            else out << L"Перебалансировка закончена. Итоговое дерево: \n";
            PrintTree(out);
            out << SPACER;
        }
    }
    #else
    void Balance(NodePtr node){
        Rotate(node);
        Recolor(node);
    }
    #endif

    #ifdef PRINT
    void PrintTree(std::wostream& os = std::wcout, NodePtr new_node = nullptr, NodePtr start_node = nullptr){
        if(!start_node && new_node){
            if(new_node->parent.lock()){
                if(new_node->parent.lock()->parent.lock()) start_node = new_node->parent.lock()->parent.lock();
                else start_node = new_node->parent.lock();
            }else start_node = new_node;
        }
        if(!start_node){
            if(!head){
                const int len = wcslen(kNULL);
                os << kBLACK << kLEFT_UPPER_CORNER
                << std::wstring(len, kHORIZONTAL) << kRIGHT_UPPER_CORNER << L"\n"
                << kVERTICAL << kNULL << kVERTICAL << L"\n"
                << kLEFT_DOWN_CORNER << std::wstring(len, kHORIZONTAL)
                << kRIGHT_DOWN_CORNER << kNORMAL << SPACER;
                return;
            }
            start_node = head;
        }
        auto get_len = [](T& data)->int{
            std::wstringstream sstream;
            sstream << data;
            std::wstring str = sstream.str();
            for(int i = 0; i < str.size(); i++){
                if(iswcntrl(str[i])) throw std::logic_error("T print has symbol control characters!");
            }
            return str.size();
        };
        int pos = 0;
        const int len_null = wcslen(kNULL);
        auto set_positions = [&pos, &len_null](NodePtr node, auto&& get_len, auto&& set_positions)->void{
            if(!node){
                pos += len_null + 2;
                return;
            }
            set_positions(node->left, get_len, set_positions);
            node->left_pos = pos;
            pos += get_len(node->data) + 2;
            node->right_pos = pos;
            set_positions(node->right, get_len, set_positions);
        };
        set_positions(start_node, get_len, set_positions);
        auto get_center = [&len_null](NodePtr node)->int{
            return (node->right_pos == -1 ? (2 * node->left_pos + len_null + 2)/2 :
                                        (node->left_pos + node->right_pos)/2 );
        };
        std::deque<NodePtr> nodes;
        nodes.emplace_back(start_node);
        auto print_tree = [&os, &nodes, &len_null](int max_iters, NodePtr new_node, auto&& get_center)->void{
            int iter = 0;
            while(!nodes.empty()){
                int offset = nodes.size();
                for(auto node : nodes){
                    if(node->right_pos != -1){
                        if(!node->left){
                            NodePtr null_node = std::make_shared<RedBlackTreeNode<T>>();
                            null_node->left_pos = node->left_pos - len_null - 2;
                            null_node->right_pos = -1;
                            nodes.emplace_back(null_node);
                        }else nodes.emplace_back(node->left);
                        if(!node->right){
                            NodePtr null_node = std::make_shared<RedBlackTreeNode<T>>();
                            null_node->left_pos = node->right_pos;
                            null_node->right_pos = -1;
                            nodes.emplace_back(null_node);
                        }else nodes.emplace_back(node->right);
                    }
                }
                int last_index = 0;
                int last_pos = 0;
                for(int i = 0; i < offset; i++){
                    NodePtr node = nodes[i];
                    os << std::wstring(node->left_pos - last_pos, L' ');
                    if(node->right_pos == -1){
                        os << kBLACK << kLEFT_UPPER_CORNER << std::wstring(get_center(node) - node->left_pos - 1, kHORIZONTAL)
                        << kCONNECT_UP << std::wstring(len_null - get_center(node) + node->left_pos, kHORIZONTAL) << kRIGHT_UPPER_CORNER << kNORMAL;
                        last_pos = node->left_pos + len_null + 2;
                    }else{
                        os << (node == new_node ? kORANGE : (node->is_red ? kRED : kBLACK));
                        os << kLEFT_UPPER_CORNER << std::wstring(get_center(node) - node->left_pos - 1, kHORIZONTAL)
                        << (node->parent.lock() ? kCONNECT_UP : kHORIZONTAL)
                        << std::wstring(node->right_pos - get_center(node) - 2, kHORIZONTAL) << kRIGHT_UPPER_CORNER;
                        os << kNORMAL;
                        last_pos = node->right_pos;
                    }
                }
                os << L"\n";
                last_pos = 0;
                for(int i = 0; i < offset; i++){
                    NodePtr node = nodes[i];
                    if(node->right_pos == -1){
                        os << std::wstring(node->left_pos - last_pos, L' ')
                        << kBLACK << kVERTICAL << kNULL << kVERTICAL << kNORMAL;
                        last_pos = node->left_pos + len_null + 2;
                    }else{
                        int left_center = get_center(nodes[offset + 2 * last_index]);
                        int right_center = get_center(nodes[offset + 2 * last_index + 1]);
                        os << std::wstring(left_center - last_pos, L' ')
                        //<< (node->is_red ? kRED : kBLACK)
                        << (node == new_node ? kORANGE : (node->is_red ? kRED : kBLACK))
                        << kLEFT_UPPER_CORNER
                        << std::wstring(node->left_pos - left_center - 1, kHORIZONTAL)
                        << kCONNECT_LEFT << node->data << kCONNECT_RIGHT
                        << std::wstring(right_center - node->right_pos, kHORIZONTAL)
                        << kRIGHT_UPPER_CORNER
                        << kNORMAL;
                        last_pos = right_center + 1;
                        last_index++;
                    }
                }
                os << L"\n";
                last_pos = 0;
                last_index = 0;
                for(int i = 0; i < offset; i++){
                    NodePtr node = nodes[i];
                    if(node->right_pos == -1){
                        os << std::wstring(node->left_pos - last_pos, L' ')
                        << kBLACK << kLEFT_DOWN_CORNER << std::wstring(len_null, kHORIZONTAL) << kRIGHT_DOWN_CORNER << kNORMAL;
                        last_pos = node->left_pos + len_null + 2;
                    }else{
                        int left_center = get_center(nodes[offset + 2 * last_index]);
                        int right_center = get_center(nodes[offset + 2 * last_index + 1]);
                        os << std::wstring(left_center - last_pos, L' ')
                        << kBLUE << kVERTICAL << kNORMAL
                        << std::wstring(node->left_pos - left_center - 1, L' ')
                        //<< (node->is_red ? kRED : kBLACK) << kLEFT_DOWN_CORNER
                        << (node == new_node ? kORANGE : (node->is_red ? kRED : kBLACK))
                        << kLEFT_DOWN_CORNER
                        << std::wstring(node->right_pos - node->left_pos - 2, kHORIZONTAL)
                        << kRIGHT_DOWN_CORNER << kNORMAL
                        << std::wstring(right_center - node->right_pos, L' ')
                        << kBLUE << kVERTICAL << kNORMAL
                        << kNORMAL;
                        last_pos = right_center + 1;
                        last_index++;
                    }
                }
                os << L"\n";
                for(int i = 0; i < offset; i++) nodes.pop_front();
                for(int i = 0; i < 3; i++){
                    last_pos = 0;
                    for(auto node : nodes){
                        int center = get_center(node);
                        os << std::wstring(center - last_pos, L' ');
                        os << kBLUE << kVERTICAL << kNORMAL;
                        last_pos = center + 1;
                    }
                    os << L"\n";
                }
                if(++iter == max_iters) break;
            }
        };
        print_tree((start_node == head ? -1 : 4), new_node, get_center);

    }
    #endif

public:
    RedBlackTree() = default;
    ~RedBlackTree() = default;

    RedBlackTree(std::wostream& os){
        out = os;
    }

    RedBlackTree(const RedBlackTree<T>& tree){
        auto Copy = [](NodePtr parent, NodePtr& dest, const NodePtr& src,  auto&& Copy)->void{
            if(src == nullptr) return;
            dest = std::make_shared<RedBlackTreeNode<T>>(src->data);
            dest->parent = parent;
            Copy(dest, dest->left, src->left, Copy);
            Copy(dest, dest->right, src->right, Copy);
        };
        Copy(nullptr, head, tree.head, Copy);
        out = tree.out;
    }
    
    RedBlackTree& operator=(const RedBlackTree<T>& tree){
        if(&tree == this) return *this;
        auto Copy = [](NodePtr parent, NodePtr& dest, const NodePtr& src,  auto&& Copy)->void{
            if(src == nullptr) return;
            dest = std::make_shared<RedBlackTreeNode<T>>(src->data);
            dest->parent = parent;
            Copy(dest, dest->left, src->left, Copy);
            Copy(dest, dest->right, src->right, Copy);
        };
        Copy(nullptr, head, tree.head, Copy);
        out = tree.out;
        return *this;
    }

    RedBlackTree(RedBlackTree<T>&& tree){
        head = std::move(tree.head);
        out = tree.out;
    }
    
    RedBlackTree& operator=(RedBlackTree<T>&& tree){
        if(&tree == this) return *this;
        head = std::move(tree.head);
        out = tree.out;
        return *this;
    }

    std::wostream& GetOutputStream(){
        return out;
    }

    void SetOutputStream(std::wostream& os){
        out = os;
    }
    
    void Insert(T new_data){
        auto new_node = std::make_shared<RedBlackTreeNode<T>>(new_data);
        if(head == nullptr){
            head = new_node;
            #ifdef PRINT
            out << L"Производится вставка элемента "
            << kORANGE << new_node->data << kNORMAL
            << L", как в обычное бинарное дерево поиска.\n";
            PrintTree(out, new_node, head);
            out << SPACER;
            #endif
            Balance(new_node);
            return;
        }
        NodePtr cur = head;
        while(true){
            if(new_data == cur->data){
                cur->data = new_data;
                #ifdef PRINT
                out << L"Производится вставка элемента "
                << kORANGE << new_node->data << kNORMAL
                << L", как в обычное бинарное дерево поиска. Данный элемент уже существует в дереве, поэотму он заменяется новым. Дополнительных действий для перебалансировки дерева не требуется\n";
                PrintTree(out, cur, head);
                out << SPACER;
                #endif
                return;
            }
            if(new_data < cur->data){
                if(cur->left == nullptr){
                    cur->left = new_node;
                    new_node->parent = cur;
                    new_node->is_left = true;
                    break;
                }else{
                    cur = cur->left;
                }
            }
            if(new_data > cur->data){
                if(cur->right == nullptr){
                    cur->right = new_node;
                    new_node->parent = cur;
                    new_node->is_left = false;
                    break;
                }else{
                    cur = cur->right;
                }
            }
        }
        #ifdef PRINT
        out << L"Производится вставка элемента "
        << kORANGE << new_node->data << kNORMAL
        << L", как в обычное бинарное дерево поиска.\n";
        PrintTree(out, new_node, head);
        out << SPACER;
        #endif
        Balance(new_node);
    }
    
    void PrintData(std::wostream& os = std::wcout){
        auto print = [&os](NodePtr node, auto&& print)->void{
            if(node == nullptr) return;
            print(node->left, print);
            os << node->data << " ";
            print(node->right, print);
        };
        print(head, print);
        os << "\n";
    }

    #ifdef PRINT
    void Print(std::wostream& os = std::wcout){
        PrintTree(os);
    }
    #endif

    bool Find(T find_data){
        int count = 0;
        auto find = [&find_data, &count](NodePtr node, auto&& find)->NodePtr{
            if(node == nullptr) return nullptr;
            if(find_data == node->data) return node;
            if(find_data < node->data) return find(node->left, find);
            else return find(node->right, find);
        };
        NodePtr node = find(head, find);
        PrintTree(out, node, head);
        return (node != nullptr);
    }

};

template<typename T>
std::wostream& operator<<(std::wostream& os, RedBlackTree<T>& rbt){
    rbt.Print(os);
    return os;
}


int main(){
    setlocale(LC_ALL,"ru_RU.utf8");
    RedBlackTree<int> rbt;
    std::wstring cmd;
    try{
        while(true){
            std::wcout << kRED << L"Красно-" << kBLACK << L"чёрное" << kNORMAL << L" дерево. Команды:\n"
            << L"(t)ree или (д)ерево - вывести текущее дерево\n"
            << L"(q)uit или (в)ыйти - выйти из программы\n"
            << L"(f)ind или (н)айти - найти элемент в дереве\n"
            << L"Число - вставить элемент в дерево. Вставить можно только одно число. Все остальные будут проигнорированы\n"
            << L"Что вы хотите сделать: ";
            std::getline(std::wcin, cmd);
            try{
                if((cmd == L"q") || (cmd == L"quit") || (cmd == L"в") || (cmd == L"выйти") ||
                    (cmd == L"Q") || (cmd == L"Quit") || (cmd == L"В") || (cmd == L"Выйти")){
                    std::wcout << kBLUE << L"Программа завершает свою работу..." << kNORMAL << L"\n";
                    break;
                }else if((cmd == L"t") || (cmd == L"tree") || (cmd == L"д") || (cmd == L"дерево") ||
                    (cmd == L"T") || (cmd == L"Tree") || (cmd == L"Д") || (cmd == L"Дерево")){
                    std::wcout << L"Текущее дерево:\n" << rbt;
                }else if((cmd == L"f") || (cmd == L"find") || (cmd == L"н") || (cmd == L"найти") ||
                    (cmd == L"F") || (cmd == L"Find") || (cmd == L"Н") || (cmd == L"Найти")){
                    while(true){
                        try{
                            std::wcout << kORANGE << L"Поиск элемента." << kNORMAL << L"\n Введите число для поиска, (q)uit или(в)ыйти для выхода из поиска\n"
                            << L"Что вы хотите сделать: ";
                            std::getline(std::wcin, cmd);
                            if((cmd == L"q") || (cmd == L"quit") || (cmd == L"в") || (cmd == L"выйти") ||
                                (cmd == L"Q") || (cmd == L"Quit") || (cmd == L"В") || (cmd == L"Выйти")){
                                std::wcout << L"Выход из поиска" << SPACER;
                                break;
                            }else{
                                int elem = std::stoi(cmd);
                                std::wcout << L"Поиск элемента " << kORANGE << elem << kNORMAL << L"...\n";
                                bool is_find = rbt.Find(elem);
                                std::wcout << L"Элемент " << kORANGE << elem << kNORMAL
                                << (!is_find ? kRED : kBLUE) << (!is_find ? L" отсутствует" : L" присутствует")
                                << kNORMAL << L" в дереве" << SPACER;
                            }
                        }catch(std::invalid_argument& e){
                            std::wcout << kRED << L"Некорректная команда!" << kNORMAL << SPACER;
                        }
                    }

                }else{
                    int elem = std::stoi(cmd);
                    std::wcout << L"Вставка элемента " << kRED << elem << kNORMAL << SPACER;
                    rbt.Insert(elem);
                }
            }catch(std::invalid_argument& e){
                std::wcout << kRED << L"Некорректная команда!" << kNORMAL << SPACER;
            }
        }
    }catch(std::exception& e){
        std::wcout << L"Во время работы программы возникла ошибка: " << e.what() << L"\n";
    }

    return 0;
}
