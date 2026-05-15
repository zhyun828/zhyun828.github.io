#include <bits/stdc++.h>
using namespace std;

int main() {
    int R, C;
    cout << "Enter rows and columns: ";
    cin >> R >> C;

    vector<vector<int>> grid(R, vector<int>(C));

    cout << "Enter matrix values:\n";
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            cin >> grid[i][j];
        }
    }

    const long long INF = (1LL << 60);
    vector<vector<long long>> dist(R, vector<long long>(C, INF));

    using State = tuple<long long, int, int>;
    priority_queue<State, vector<State>, greater<State>> pq;

    // 起点
    dist[0][0] = grid[0][0];
    pq.push({dist[0][0], 0, 0});

    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    while (!pq.empty()) {
        auto [d, r, c] = pq.top();
        pq.pop();

        if (d != dist[r][c]) continue;

        // ⭐ 关键：到达最后一列任意一行就结束
        if (c == C - 1) {
            cout << "Minimum path sum to last column = " << d << endl;
            return 0;
        }

        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k];
            int nc = c + dc[k];

            if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;

            long long nd = d + grid[nr][nc];

            if (nd < dist[nr][nc]) {
                dist[nr][nc] = nd;
                pq.push({nd, nr, nc});
            }
        }
    }

    cout << "No path found.\n";
    return 0;
}
