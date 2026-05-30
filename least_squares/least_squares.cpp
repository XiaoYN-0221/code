#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// 计算一组数据的离差平方和: SS = Σxi² - (Σxi)²/n
double ssq(const vector<double> &a, int l, int r) {
    double s1 = 0, s2 = 0;
    int n = r - l;
    if (n == 0) return 0;
    for (int i = l; i < r; i++) {
        s1 += a[i] * a[i];
        s2 += a[i];
    }
    return s1 - s2 * s2 / n;
}

int main() {
    cout << "请输入数字串（空格或逗号分隔均可）：" << endl;
    string line;
    getline(cin, line);

    // 解析输入，支持空格、英文逗号、中文逗号分隔
    // 中文逗号 ，UTF-8 编码为 0xEF 0xBC 0x8C，此处按字节替换
    for (size_t i = 0; i < line.size(); i++) {
        if (line[i] == ',') {
            line[i] = ' ';
        } else if (i + 2 < line.size() &&
                   (unsigned char)line[i] == 0xEF &&
                   (unsigned char)line[i+1] == 0xBC &&
                   (unsigned char)line[i+2] == 0x8C) {
            line[i] = ' ';
            line[i+1] = ' ';
            line[i+2] = ' ';
        }
    }

    stringstream ss(line);
    vector<double> nums;
    double x;
    while (ss >> x) nums.push_back(x);

    int n = nums.size();
    if (n < 2) {
        cerr << "至少需要 2 个数字才能分组。" << endl;
        return 1;
    }

    // 排序以保证有序分组的正确性
    sort(nums.begin(), nums.end());

    // 前缀和 & 前缀平方和，加速计算
    vector<double> pref1(n + 1, 0), pref2(n + 1, 0);
    for (int i = 0; i < n; i++) {
        pref1[i + 1] = pref1[i] + nums[i];
        pref2[i + 1] = pref2[i] + nums[i] * nums[i];
    }

    auto ssq_fast = [&](int l, int r) -> double {
        int k = r - l;
        if (k == 0) return 0;
        double s1 = pref2[r] - pref2[l];
        double s2 = pref1[r] - pref1[l];
        return s1 - s2 * s2 / k;
    };

    double best = 1e18;
    int best_k = -1;

    for (int k = 1; k < n; k++) {
        double total = ssq_fast(0, k) + ssq_fast(k, n);
        if (total < best) {
            best = total;
            best_k = k;
        }
    }

    // 输出结果
    cout << fixed << setprecision(4);
    cout << "\n===== 结果 =====\n";

    cout << "{ ";
    for (int i = 0; i < best_k; i++) {
        if (i > 0) cout << ", ";
        cout << nums[i];
    }
    cout << " }" << endl;

    cout << "{ ";
    for (int i = best_k; i < n; i++) {
        if (i > best_k) cout << ", ";
        cout << nums[i];
    }
    cout << " }" << endl;

    cout << "最小离差平方和 = " << best << endl;

    return 0;
}
