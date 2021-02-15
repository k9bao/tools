#include <fstream>
#include <iostream>

using namespace std;

void usage() {
    cout << "para is error." << endl;
    cout << "example: a.out in out pwd" << endl;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        usage();
        return -1;
    }
    string file = argv[1];
    ifstream in(file, ios::in | ios::in | ios::binary);
    if (in.is_open() == false) {
        cout << file << " is can not open.[read]" << endl;
        return -1;
    }

    string outFile = argv[2];
    ofstream out(outFile, ios::out | ios::binary | ios::ate);
    if (out.is_open() == false) {
        cout << outFile << " is can not open.[write]" << endl;
        return -1;
    }

    long long pw;
    if (strlen(argv[3]) > 0) {
        pw = std::atoll(argv[3]);
    } else {
        usage();
        return -1;
    }

    bool encode = true;
    unsigned long long total = 0;

    const int size = 4096;
    char buffer[size];
    string head = "lahFv9y&RCluQl%r8bNzh#FiCml7!%tq";
    auto count = in.read(buffer, head.size()).gcount();
    if (count == head.size() && memcmp(buffer, head.c_str(), head.size()) != 0) {
        for (size_t i = 0; i < count; i++) {
            if (((total + i) % pw) != 0) {
                buffer[i] = ~buffer[i];
            }
        }
        total += count;
        out.write(head.c_str(), head.size());
        out.write(buffer, count);
        cout << "file is will encode" << endl;
    } else {
        cout << "file is will decode" << endl;
        encode = false;
    }

    while (true) {
        if (in.good() == false) {
            break;
        }
        auto count = in.read(buffer, size).gcount();
        for (size_t i = 0; i < count; i++) {
            if (((total + i) % pw) != 0) {
                buffer[i] = ~buffer[i];
            }
        }
        total += count;

        if (count > 0) {
            out.write(buffer, count);
        }
    }
    if (in.eof() && out.good()) {
        cout << "process ok" << endl;
        return -1;
    } else {
        cout << "process fail" << endl;
        return -1;
    }

    in.close();
    out.close();
    return 0;
}