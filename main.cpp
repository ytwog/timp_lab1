//19081999 - 7,3,1,1
#include "text.h"
using namespace mLab;
int main(int argc, char* argv[]) {
    setlocale(LC_ALL, 0);
    char *out_stream;
    if(argc < 2 || argc > 3) {
        std::cout << "Incorrect command line! \nUsage: lab.exe [input.txt] [output.txt]" << std::endl;
        exit(1);
    }
    if(argc == 2) {
        out_stream = new char[11]{'o', 'u', 't', 'p', 'u', 't', '.', 't', 'x', 't', '\0'};
    } else if(argc == 3) {
        out_stream = argv[2];
    }
    std::ifstream ifstr(argv[1]);
    std::ofstream ofstr(out_stream);
    std::cout << "Program started"<< std::endl;
    char *s = new char[255];
    _mContainer container;
    container.read_from_file(&ifstr);
    return 0;
}
