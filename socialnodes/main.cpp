#include "analyze.h"

int main() {
    Analyzer analyzer;
    analyzer.init_from_file();
    analyzer.run();
    return 0;
}
