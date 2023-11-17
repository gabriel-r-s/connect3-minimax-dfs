#include <cstdio>
#include "board.hpp"

void test_many(Board state, bool turn, unsigned depth) {
    for (int col = 0; col < 4; col++) {
        Board copy = state;
        copy.play(col, turn);
        Result result = copy.result();
        print_board(state);
        if (depth == 0 || result != Result_NotDone) {
            return;
        }
        pall(copy, !turn, depth - 1);
    }
}

void mock_play() {
    Board state;
    bool turn = false;
    unsigned col;
    for (;;) {
        print_board(state);
        printf("0123\n");
        if (scanf("%d", &col) != 1) {
            break;
        }
        printf("\e[1;1H\e[2J");
        state.play(col, turn);
        turn = !turn;
    }
}

void test_individual() {
    Board state;
    state.col0 = 0;
    state.len0 = 0;

    state.col1 = 1;
    state.len1 = 1;

    state.col2 = 1;
    state.len2 = 1;

    state.col3 = 0;
    state.len3 = 0;

    state.play(3, true);
    print_board(state);
}

void print_board(Board board) {
    uint8_t col[] = { board.col0, board.col1, board.col2, board.col3 };
    uint8_t len[] = { board.len0, board.len1, board.len2, board.len3 };

    for (int i = 3; i >= 0; i--) {
        for (int j = 0; j < 4; j++) {
            if (i >= len[j]) {
                printf(".");
            } else {
                printf("%c", (col[j] & (1 << i)) != 0 ? 'X' : 'O');
            }
        }
        if (i == 0) {
            printf("\t");
        } else {
            printf("\n");
        }
    }

    switch (board.result()) {
    case Result_CpuWin:
        printf("CPU Wins\n\n");
        break;
    case Result_CpuLoss:
        printf("Player Wins\n\n");
        break;
    case Result_Draw:
        printf("Draw!\n\n");
        break;
    case Result_NotDone:
        printf("\n\n");
        break;
    }
}

int main() {
    // pall(Board(), false, 4);
    mock_play();
    // test_individual();
}

