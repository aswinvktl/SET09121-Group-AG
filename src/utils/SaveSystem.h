#pragma once
#include <fstream>

struct SaveSystem {
    static void save(bool muted,
                     bool level1Complete,
                     bool level2Complete,
                     bool level3Complete) {
        std::ofstream file("save.dat");
        if (!file) return;

        file << (muted ? 1 : 0) << "\n";
        file << (level1Complete ? 1 : 0) << "\n";
        file << (level2Complete ? 1 : 0) << "\n";
        file << (level3Complete ? 1 : 0) << "\n";
    }

    static void load(bool& muted,
                     bool& level1Complete,
                     bool& level2Complete,
                     bool& level3Complete) {
        std::ifstream file("save.dat");
        if (!file) return;

        int m = 0, c1 = 0, c2 = 0, c3 = 0;
        file >> m >> c1 >> c2 >> c3;

        muted = (m != 0);
        level1Complete = (c1 != 0);
        level2Complete = (c2 != 0);
        level3Complete = (c3 != 0);
    }
};
