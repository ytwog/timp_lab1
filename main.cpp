//19081999 - 7,3,1,1
#include "text.h"
using namespace mLab;

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, 0);
    char *out_stream = nullptr;
    if(argc < 2 || argc > 3) return print_err(8);
    if(argc == 2) {
        out_stream = new char[11]{'o', 'u', 't', 'p', 'u', 't', '.', 't', 'x', 't', '\0'};
    } else if(argc == 3) {
        out_stream = argv[2];
    }
    std::ifstream ifstr(argv[1]);
    std::ofstream ofstr(out_stream);
    if(ifstr.bad() || ifstr.fail() || ofstr.bad() || ofstr.fail()) {
        return print_err(7);
    }
    std::cout << "Program started"<< std::endl;
    _mContainer container;
    print_err(text::read_from_file(&ifstr, &container));
    text::write_to_file(&ofstr, &container);
    container.multi_method(ofstr);
    std::cout << "Program finished"<< std::endl;
    return 0;
}
