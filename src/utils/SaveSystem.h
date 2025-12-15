#pragma once
#include <fstream>

struct SaveSystem {
    static void save(bool muted, bool level1Complete, bool level2Complete) {
        std::ofstream f("save.dat");
        if (!f) return;
        f << (muted ? 1 : 0) << "\n";
        f << (level1Complete ? 1 : 0) << "\n";
        f << (level2Complete ? 1 : 0) << "\n";
    }

    static void load(bool& muted, bool& level1Complete, bool& level2Complete) {
        std::ifstream f("save.dat");
        if (!f) return;

        int m = 0, c1 = 0, c2 = 0;
        f >> m >> c1 >> c2;

        muted = (m != 0);
        level1Complete = (c1 != 0);
        level2Complete = (c2 != 0);
    }
};
