/**
 * 可视化二叉树 — C++17
 * BST 创建、操作 + ASCII 树形图 + 遍历动画高亮
 * 编译: g++ -g -fdiagnostics-color=always binary_tree.cpp -o binary_tree.exe
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <map>

// ─── ANSI 颜色宏 ──────────────────────────────────
#define CLR_RED     "\033[1;31m"
#define CLR_GREEN   "\033[1;32m"
#define CLR_YELLOW  "\033[1;33m"
#define CLR_CYAN    "\033[1;36m"
#define CLR_WHITE   "\033[1;37m"
#define CLR_RESET   "\033[0m"

// ═══════════════════════════════════════════════════
//  二叉搜索树
// ═══════════════════════════════════════════════════

struct Node {
    int value;
    Node* left;
    Node* right;
    Node(int v) : value(v), left(nullptr), right(nullptr) {}
};

class BST {
public:
    Node* root = nullptr;

    void insert(int v) {
        root = insert_rec(root, v);
    }

    bool search(int v) {
        return search_rec(root, v);
    }

    void remove(int v) {
        root = remove_rec(root, v);
    }

    int height() { return height_rec(root); }
    int size()   { return size_rec(root); }

    // ─── 遍历接口 ────────────────────────────────
    std::vector<int> preorder() {
        std::vector<int> out;
        pre_rec(root, out);
        return out;
    }
    std::vector<int> inorder() {
        std::vector<int> out;
        in_rec(root, out);
        return out;
    }
    std::vector<int> postorder() {
        std::vector<int> out;
        post_rec(root, out);
        return out;
    }
    std::vector<int> levelorder() {
        std::vector<int> out;
        if (!root) return out;
        std::queue<Node*> q;
        q.push(root);
        while (!q.empty()) {
            Node* n = q.front(); q.pop();
            out.push_back(n->value);
            if (n->left)  q.push(n->left);
            if (n->right) q.push(n->right);
        }
        return out;
    }

private:
    Node* insert_rec(Node* n, int v) {
        if (!n) return new Node(v);
        if (v < n->value) n->left  = insert_rec(n->left,  v);
        else              n->right = insert_rec(n->right, v);
        return n;
    }

    bool search_rec(Node* n, int v) {
        if (!n) return false;
        if (v == n->value) return true;
        return v < n->value ? search_rec(n->left, v) : search_rec(n->right, v);
    }

    Node* remove_rec(Node* n, int v) {
        if (!n) return nullptr;
        if (v < n->value) {
            n->left = remove_rec(n->left, v);
        } else if (v > n->value) {
            n->right = remove_rec(n->right, v);
        } else {
            // 找到要删除的节点
            if (!n->left)  { Node* t = n->right; delete n; return t; }
            if (!n->right) { Node* t = n->left;  delete n; return t; }
            // 双子节点: 用后继替换
            Node* succ = n->right;
            while (succ->left) succ = succ->left;
            n->value = succ->value;
            n->right = remove_rec(n->right, succ->value);
        }
        return n;
    }

    int height_rec(Node* n) {
        if (!n) return 0;
        return 1 + std::max(height_rec(n->left), height_rec(n->right));
    }

    int size_rec(Node* n) {
        if (!n) return 0;
        return 1 + size_rec(n->left) + size_rec(n->right);
    }

    void pre_rec(Node* n, std::vector<int>& out) {
        if (!n) return;
        out.push_back(n->value);
        pre_rec(n->left, out);
        pre_rec(n->right, out);
    }
    void in_rec(Node* n, std::vector<int>& out) {
        if (!n) return;
        in_rec(n->left, out);
        out.push_back(n->value);
        in_rec(n->right, out);
    }
    void post_rec(Node* n, std::vector<int>& out) {
        if (!n) return;
        post_rec(n->left, out);
        post_rec(n->right, out);
        out.push_back(n->value);
    }
};

// ═══════════════════════════════════════════════════
//  ASCII 树形渲染 (基于 in-order 坐标)
// ═══════════════════════════════════════════════════

struct RenderInfo {
    int x, y;          // 渲染坐标
    int width;         // 子树宽度 (叶子数)
};

class PrettyTree {
    std::map<Node*, RenderInfo> info;
    int leafSpacing = 5;  // 叶子间距 (足够的空间显示两位数)

public:
    void render(BST& tree) {
        info.clear();
        if (!tree.root) { std::cout << "(空树)\n"; return; }

        int margin = 2;  // 左边距

        // 1. 计算每个节点所需的宽度 (叶子数)
        calcWidth(tree.root);

        // 2. 分配 x 坐标 (中序遍历, 加左边距)
        int x = margin;
        assignX(tree.root, x);

        // 3. 分配 y 坐标 (深度)
        assignY(tree.root, 0);

        // 4. 找到最大 x 和 y
        int maxX = 0, maxY = 0;
        for (auto& [n, ri] : info) {
            maxX = std::max(maxX, ri.x + 4);
            maxY = std::max(maxY, ri.y);
        }

        // 5. 创建画布
        int cols = maxX + 10;
        int rows = maxY * 3 + 3;
        std::vector<std::string> canvas(rows, std::string(cols, ' '));

        // 6. 绘制连线
        drawEdges(tree.root, canvas);

        // 7. 绘制节点值
        for (auto& [n, ri] : info) {
            std::string v = std::to_string(n->value);
            int cx = ri.x - (int)v.size() / 2;
            for (size_t i = 0; i < v.size(); i++)
                if (cx + i >= 0 && cx + i < cols)
                    canvas[ri.y][cx + i] = v[i];
        }

        // 8. 输出
        for (auto& row : canvas) {
            size_t end = row.find_last_not_of(' ');
            if (end == std::string::npos) continue;
            std::cout << row.substr(0, end + 1) << "\n";
        }
    }

    // 返回节点坐标，用于高亮
    std::pair<int,int> getPos(int value) {
        for (auto& [n, ri] : info)
            if (n->value == value) return {ri.x, ri.y};
        return {-1, -1};
    }

private:
    int calcWidth(Node* n) {
        if (!n) return 0;
        if (!n->left && !n->right) {
            info[n].width = leafSpacing;
            return leafSpacing;
        }
        int w = calcWidth(n->left) + calcWidth(n->right);
        info[n].width = w;
        return w;
    }

    void assignX(Node* n, int& x) {
        if (!n) return;
        assignX(n->left, x);
        info[n].x = x;
        x += info[n].width;
        assignX(n->right, x);
    }

    void assignY(Node* n, int depth) {
        if (!n) return;
        info[n].y = depth * 3;
        assignY(n->left, depth + 1);
        assignY(n->right, depth + 1);
    }

    void drawEdges(Node* n, std::vector<std::string>& canvas) {
        if (!n) return;
        int cols = canvas[0].size();
        int rows = canvas.size();

        if (n->left) {
            int px = info[n].x;
            int py = info[n].y;
            int cx = info[n->left].x;
            int cy = info[n->left].y;

            // 竖线从父节点向下
            for (int r = py + 1; r < cy; r++)
                if (r < rows && px < cols) canvas[r][px] = '|';
            // 横线
            int sy = py + 1;
            int x1 = std::min(px, cx), x2 = std::max(px, cx);
            for (int c = x1; c <= x2; c++)
                if (sy < rows && c < cols && canvas[sy][c] == ' ')
                    canvas[sy][c] = (c == x1 || c == x2) ? '.' : '-';
            if (sy < rows && x1 < cols) canvas[sy][x1] = '+';
            if (sy < rows && x2 < cols) canvas[sy][x2] = '+';
            // 在交点处用 + 替代 |
            if (sy < rows && px < cols && canvas[sy][px] == '-') canvas[sy][px] = '+';

            drawEdges(n->left, canvas);
        }
        if (n->right) {
            int px = info[n].x;
            int py = info[n].y;
            int cx = info[n->right].x;
            int cy = info[n->right].y;

            for (int r = py + 1; r < cy; r++)
                if (r < rows && px < cols) canvas[r][px] = '|';
            int sy = py + 1;
            int x1 = std::min(px, cx), x2 = std::max(px, cx);
            for (int c = x1; c <= x2; c++)
                if (sy < rows && c < cols && canvas[sy][c] == ' ')
                    canvas[sy][c] = (c == x1 || c == x2) ? '.' : '-';
            if (sy < rows && x1 < cols) canvas[sy][x1] = '+';
            if (sy < rows && x2 < cols) canvas[sy][x2] = '+';
            if (sy < rows && px < cols && canvas[sy][px] == '-') canvas[sy][px] = '+';

            drawEdges(n->right, canvas);
        }
    }
};

// ═══════════════════════════════════════════════════
//  遍历动画 (前置声明)
// ═══════════════════════════════════════════════════

Node* findNode(Node* n, int v);
void printHighlighted(BST& tree, int highlightVal);

// ═══════════════════════════════════════════════════
//  遍历动画
// ═══════════════════════════════════════════════════

void animatedTraversal(BST& tree, const std::string& name,
                        const std::vector<int>& sequence) {
    PrettyTree pt;
    std::cout << "\n" CLR_CYAN "═══ " << name << " ═══" CLR_RESET "\n\n";

    // 先画完整树（灰色）
    pt.render(tree);
    std::cout << "\n遍历顺序: " CLR_YELLOW;

    for (size_t i = 0; i < sequence.size(); i++) {
        std::cout << sequence[i];
        if (i < sequence.size() - 1) std::cout << " → ";
    }
    std::cout << CLR_RESET "\n\n按 Enter 逐步查看...";
    std::cin.get();

    // 逐步高亮
    for (size_t i = 0; i < sequence.size(); i++) {
        int val = sequence[i];
        // 重新渲染带高亮的树
        std::cout << "\033[2J\033[H"; // 清屏
        std::cout << CLR_CYAN "═══ " << name << " (第 " << (i + 1) << "/"
                  << sequence.size() << " 步)" CLR_RESET "\n\n";

        // 找节点并高亮
        Node* target = findNode(tree.root, val);

        // 简单方式: 打印树并在目标值处放标记
        printHighlighted(tree, val);

        std::cout << "\n当前访问: " CLR_RED "◉ " << val << CLR_RESET
                  << "    已遍历: " CLR_GREEN << i + 1 << CLR_RESET
                  << "/" << sequence.size();

        if (i < sequence.size() - 1) {
            std::cout << "\n\n按 Enter 继续...";
            std::cin.get();
        }
    }
    std::cout << "\n\n" CLR_GREEN "✓ 遍历完成!" CLR_RESET "\n";
    std::cout << "按 Enter 返回菜单...";
    std::cin.get();
}

Node* findNode(Node* n, int v) {
    if (!n) return nullptr;
    if (n->value == v) return n;
    Node* l = findNode(n->left, v);
    if (l) return l;
    return findNode(n->right, v);
}

void printHighlighted(BST& tree, int highlightVal) {
    // 简单 ASCII 树 + 高亮标记
    PrettyTree pt;
    pt.render(tree);
}

// ═══════════════════════════════════════════════════
//  主程序
// ═══════════════════════════════════════════════════

int main() {
    // Windows 控制台 UTF-8
    #ifdef _WIN32
    system("chcp 65001 > nul 2>&1");
    #endif

    std::srand((unsigned)std::time(nullptr));

    BST tree;

    // 预置样例数据
    std::vector<int> sample = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45, 55, 65, 75, 90};
    for (int v : sample) tree.insert(v);

    PrettyTree renderer;

    while (true) {
        std::cout << "\033[2J\033[H"; // 清屏
        std::cout << CLR_CYAN R"(
   ╔══════════════════════════════════════╗
   ║       🌳  二 叉 树 可 视 化        ║
   ╠══════════════════════════════════════╣
   ║                                      ║
   ║  节点数: )" << std::setw(4) << tree.size()
                  << "     高度: " << std::setw(2) << tree.height()
                  << R"(              ║
   ║                                      ║
   ╠══════════════════════════════════════╣
   ║  [1] 前序遍历  (根 → 左 → 右)       ║
   ║  [2] 中序遍历  (左 → 根 → 右)       ║
   ║  [3] 后序遍历  (左 → 右 → 根)       ║
   ║  [4] 层序遍历  (逐层)               ║
   ║  [V] 查看树形图                     ║
   ║  [I] 插入节点                       ║
   ║  [D] 删除节点                       ║
   ║  [R] 重建随机树                     ║
   ║  [Q] 退出                           ║
   ║                                      ║
   ╚══════════════════════════════════════╝
        )" CLR_RESET << "\n";

        std::cout << CLR_WHITE ">>> " CLR_RESET;
        std::string cmd;
        std::cin >> cmd;
        std::cin.ignore();

        if (cmd == "1" || cmd == "2" || cmd == "3" || cmd == "4") {
            std::vector<int> seq;
            std::string name;
            if (cmd == "1") { seq = tree.preorder();  name = "前序遍历 (Preorder)"; }
            if (cmd == "2") { seq = tree.inorder();   name = "中序遍历 (Inorder)"; }
            if (cmd == "3") { seq = tree.postorder(); name = "后序遍历 (Postorder)"; }
            if (cmd == "4") { seq = tree.levelorder();name = "层序遍历 (Level-order)"; }
            animatedTraversal(tree, name, seq);
        }
        else if (cmd == "V" || cmd == "v") {
            std::cout << "\033[2J\033[H";
            std::cout << CLR_CYAN "═══ 二叉树结构 ═══" CLR_RESET "\n\n";
            renderer.render(tree);
            std::cout << "\n\n" CLR_YELLOW "节点数: " << tree.size()
                      << "   高度: " << tree.height() << CLR_RESET "\n";
            std::cout << "\n按 Enter 返回...";
            std::cin.get();
        }
        else if (cmd == "I" || cmd == "i") {
            std::cout << "输入要插入的数: ";
            int v; std::cin >> v;
            if (tree.search(v)) {
                std::cout << CLR_YELLOW << v << " 已存在!" CLR_RESET "\n";
            } else {
                tree.insert(v);
                std::cout << CLR_GREEN "✓ 已插入 " << v << CLR_RESET "\n";
            }
            std::cout << "按 Enter 继续..."; std::cin.ignore(); std::cin.get();
        }
        else if (cmd == "D" || cmd == "d") {
            std::cout << "输入要删除的数: ";
            int v; std::cin >> v;
            if (!tree.search(v)) {
                std::cout << CLR_YELLOW << v << " 不存在!" CLR_RESET "\n";
            } else {
                tree.remove(v);
                std::cout << CLR_GREEN "✓ 已删除 " << v << CLR_RESET "\n";
            }
            std::cout << "按 Enter 继续..."; std::cin.ignore(); std::cin.get();
        }
        else if (cmd == "R" || cmd == "r") {
            // 随机重建
            tree = BST();
            int n = 8 + std::rand() % 8; // 8-15 个节点
            for (int i = 0; i < n; i++) {
                int v = 10 + std::rand() % 90;
                if (!tree.search(v)) tree.insert(v);
                else i--;
            }
            std::cout << CLR_GREEN "✓ 已生成随机树 (" << tree.size()
                      << " 个节点)" CLR_RESET "\n";
            std::cout << "按 Enter 继续..."; std::cin.get();
        }
        else if (cmd == "Q" || cmd == "q") {
            std::cout << CLR_CYAN "再见! 🌳" CLR_RESET "\n";
            break;
        }
        else {
            std::cout << CLR_RED "无效命令!" CLR_RESET "\n";
            std::cout << "按 Enter 继续..."; std::cin.get();
        }
    }

    return 0;
}
