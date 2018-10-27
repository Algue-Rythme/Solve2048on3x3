#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

#define EASY

#ifdef EASY
#define MAXELEM 9
#else
#define MAXELEM 10
#endif

const int64_t base = MAXELEM + 1;
const int64_t bases[9] = {
    1,
    base,
    base*base,
    base*base*base,
    base*base*base*base,
    base*base*base*base*base,
    base*base*base*base*base*base,
    base*base*base*base*base*base*base,
    base*base*base*base*base*base*base*base
};

struct Board {
    Board():code(0),score(0.f){
        data[0][0] = 0;
        data[0][1] = 0;
        data[0][2] = 0;
        data[1][0] = 0;
        data[1][1] = 0;
        data[1][2] = 0;
        data[2][0] = 0;
        data[2][1] = 0;
        data[2][2] = 0;
    }
    int64_t code;
    float score;
    int8_t data[3][3];
    int64_t compute_enc() {
        code = 0;
        code += data[0][0] * bases[0];
        code += data[0][1] * bases[1];
        code += data[0][2] * bases[2];
        code += data[1][0] * bases[3];
        code += data[1][1] * bases[4];
        code += data[1][2] * bases[5];
        code += data[2][0] * bases[6];
        code += data[2][1] * bases[7];
        code += data[2][2] * bases[8];
        return code;
    }
};

Board move_left(Board const& refboard) {
    Board ret = refboard;
    for (int i = 0; i < 3; ++i) {
        if (ret.data[i][0] == 0) {
            ret.data[i][0] = ret.data[i][1];
            ret.data[i][1] = 0;
        }
        if (ret.data[i][1] == 0) {
            ret.data[i][1] = ret.data[i][2];
            ret.data[i][2] = 0;
        }
    }
    for (int i = 0; i < 3; ++i) {
        if (ret.data[i][0] == ret.data[i][1] && ret.data[i][0] != 0) {
            ret.data[i][0] += 1;
            if (MAXELEM == ret.data[i][0])
                ret.score = 1.f;
            ret.data[i][1] = 0;
        }
        if (ret.data[i][1] == ret.data[i][2] && ret.data[i][1] != 0) {
            ret.data[i][1] += 1;
            if (MAXELEM == ret.data[i][1])
                ret.score = 1.f;
            ret.data[i][2] = 0;
        }
    }
    for (int i = 0; i < 3; ++i) {
        if (ret.data[i][0] == 0) {
            ret.data[i][0] = ret.data[i][1];
            ret.data[i][1] = 0;
        }
        if (ret.data[i][1] == 0) {
            ret.data[i][1] = ret.data[i][2];
            ret.data[i][2] = 0;
        }
    }
    return ret;
}

Board rot(Board const& b) {
    Board ret;
    ret.score = b.score;
    ret.data[0][0] = b.data[2][0];
    ret.data[0][1] = b.data[1][0];
    ret.data[0][2] = b.data[0][0];
    ret.data[1][0] = b.data[2][1];
    ret.data[1][1] = b.data[1][1];
    ret.data[1][2] = b.data[0][1];
    ret.data[2][0] = b.data[2][2];
    ret.data[2][1] = b.data[1][2];
    ret.data[2][2] = b.data[0][2];
    return ret;
}

Board sym(Board const& b) {
    Board ret;
    ret.score = b.score;
    ret.data[0][0] = b.data[2][0];
    ret.data[0][1] = b.data[2][1];
    ret.data[0][2] = b.data[2][2];
    ret.data[1][0] = b.data[1][0];
    ret.data[1][1] = b.data[1][1];
    ret.data[1][2] = b.data[1][2];
    ret.data[2][0] = b.data[0][0];
    ret.data[2][1] = b.data[0][1];
    ret.data[2][2] = b.data[0][2];
    return ret;
}

Board diagsym(Board const& b) {
    Board ret;
    ret.score = b.score;
    ret.data[0][0] = b.data[0][0];
    ret.data[0][1] = b.data[1][0];
    ret.data[0][2] = b.data[2][0];
    ret.data[1][0] = b.data[0][1];
    ret.data[1][1] = b.data[1][1];
    ret.data[1][2] = b.data[2][1];
    ret.data[2][0] = b.data[0][2];
    ret.data[2][1] = b.data[1][2];
    ret.data[2][2] = b.data[2][2];
    return ret;
}

Board proj(Board const& refboard) {
    Board bestboard = refboard;
    Board diagnat = diagsym(refboard);
    if (diagnat.compute_enc() < bestboard.code) {
        bestboard = diagnat;
    }
    Board symh = sym(refboard);
    if (symh.compute_enc() < bestboard.code) {
        bestboard = symh;
    }
    Board rot90 = rot(refboard);
    if (rot90.compute_enc() < bestboard.code) {
        bestboard = rot90;
    }
    Board antidiag = diagsym(rot90);
    if (antidiag.compute_enc() < bestboard.code) {
        bestboard = antidiag;
    }
    Board symv = sym(rot90);
    if (symv.compute_enc() < bestboard.code) {
        bestboard = symv;
    }
    Board rot180 = rot(rot90);
    if (rot180.compute_enc() < bestboard.code) {
        bestboard = rot180;
    }
    Board rot270 = rot(rot180);
    if (rot270.compute_enc() < bestboard.code) {
        bestboard = rot270;
    }
    return bestboard;
}

vector<Board> add_tiles(Board const& board) {
    vector<Board> tiles;
    for (int i =0 ; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board.data[i][j] == 0) {
                Board other2 = board;
                other2.data[i][j] = 1;
                other2.compute_enc();
                other2 = proj(other2);
                tiles.push_back(other2);

                #ifndef EASY
                Board other4 = board;
                other4.data[i][j] = 2;
                other4.compute_enc();
                other4 = proj(other4);
                tiles.push_back(other4);
                #endif
            }
        }
    }
    return tiles;
}

vector<vector<Board>> get_neighbours(Board refboard) {
    Board goleft = move_left(refboard);
    goleft.compute_enc();

    refboard = rot(refboard);
    Board godown = move_left(refboard);
    godown.compute_enc();

    refboard = rot(refboard);
    Board goright = move_left(refboard);
    goright.compute_enc();

    refboard = rot(refboard);
    Board goup = move_left(refboard);
    goup.compute_enc();

    vector<vector<Board>> neighbours(4);
    neighbours[0] = add_tiles(goleft);
    neighbours[1] = add_tiles(godown);
    neighbours[2] = add_tiles(goright);
    neighbours[3] = add_tiles(goup);
    return neighbours;
}

struct Node {
    Node()=default;
    Node(float v, char policy):v(v),policy(policy){}
    float v;
    char policy;
};

unordered_map<int, Node> nodes;
char moves [] = "LDRU";

float dfs(Board const& board) {
    auto it = nodes.find(board.code);
    if (it != nodes.end()) {
        return it->second.v;
    }
    float v = 0.f;
    char policy = '#';
    if (board.score == 0) {
        auto neighbours = get_neighbours(board);
        for (int i = 0; i < (int)neighbours.size(); ++i) {
            float thisdir = 0.f;
            for(auto const& neighbour : neighbours[i]) {
                thisdir += dfs(neighbour);
            }
            thisdir /= neighbours[i].size();
            if (thisdir > v) {
                v = thisdir;
                policy = moves[i];
            }
        }
    }
    v += board.score;
    nodes.insert({board.code, Node(v,policy)});
    return v;
}

void print(Board const& b) {
    cout << b.code << " " << b.score << "\n";
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cout.width(6);
            if (b.data[i][j] != 0)
                cout << right << (1<<b.data[i][j]);
            else
                cout << right << '.' << "|";
        }
        cout << "\n";
    }
}

void play() {
    Board b;
    int64_t code = 0;
    print(b);
    while (nodes[code].policy != '#') {
        cout << "plays " << nodes[code].policy << "\n";
        Board rot90 = rot(b);
        Board rot180 = rot(rot90);
        Board rot270 = rot(rot180);
        switch (nodes[code].policy) {
            case 'L':
            b = move_left(b);
            break ;

            case 'D':
            b = move_left(rot90);
            b = rot(rot(rot(b)));
            break ;

            case 'R':
            b = move_left(rot180);
            b = rot(rot(b));
            break ;

            case 'U':
            b = move_left(rot270);
            b = rot(b);
            break ;
        }
        auto pot = add_tiles(b);
        b = pot[0];
        code = proj(b).compute_enc();
        print(b);
        char garb;
        cin >> garb;
    }
}

int main() {
    Board start;
    cout << "computing..." << endl;
    dfs(start);
    cout << "index max of a board : "
         << max_element(begin(nodes),end(nodes),[](auto const& a, auto const& b){ return a.first < b.first;})->first << "\n";
    cout << "number of node before dead end or reaching 512 : "
         << nodes.size() << "\n";
    cout << "probability of reaching 512 : " << nodes[0].v << "\n";
    //play();
}
