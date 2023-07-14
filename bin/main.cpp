#include "../lib/mp3_ID3v2.cpp"

#include <iostream>
#include <string>


int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "There's nothing was given as a path to a mp3 ID3v2.4 file";
        return 1;
    }

    std::string str1 = argv[1];
    mp3_ID3v2 mp3(str1);

    std::vector<std::string> s_v = mp3.get_info();
    for (const auto& i : s_v) {
        std::cout << i << '\n';
    }
}