#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;

void usage() {
    cout << "para is error." << endl;
    cout << "example: a.out in out pwd" << endl;
}

std::chrono::steady_clock::time_point now() {
    return std::chrono::steady_clock::now();
}

long long subSecond(std::chrono::steady_clock::time_point before) {
    return std::chrono::duration_cast<std::chrono::seconds>(now() - before).count();
}

int main(int argc, char **argv) {
    if (argc != 4) {
        usage();
        return -1;
    }
    string file = argv[1];
    long long totalSize = 0;
    ifstream in(file, ios::in | ios::in | ios::binary);
    if (in.is_open() == false) {
        cout << file << " is can not open.[read]" << endl;
        return -1;
    } else {
        in.seekg(0, ios_base::end);
        totalSize = in.tellg();
        in.seekg(0, ios_base::beg);
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
    unsigned long long curSize = 0;

    const int size = 4096;
    char buffer[size];
    string head = "lahFv9y&RCluQl%r8bNzh#FiCml7!%tq";
    auto count = in.read(buffer, head.size()).gcount();
    if (count == head.size() && memcmp(buffer, head.c_str(), head.size()) != 0) {
        for (size_t i = 0; i < count; i++) {
            if (((curSize + i) % pw) != 0) {
                buffer[i] = ~buffer[i];
            }
        }
        curSize += count;
        out.write(head.c_str(), head.size());
        out.write(buffer, count);
        cout << "file is will encode" << endl;
    } else {
        cout << "file is will decode" << endl;
        encode = false;
    }

    auto curTime = now();
    while (true) {
        if (in.good() == false) {
            break;
        }
        auto count = in.read(buffer, size).gcount();
        for (size_t i = 0; i < count; i++) {
            if (((curSize + i) % pw) != 0) {
                buffer[i] = ~buffer[i];
            }
        }
        curSize += count;

        if (count > 0) {
            out.write(buffer, count);
        }
        if (subSecond(curTime) >= 10) {
            curTime = now();
            cout << "process is " << (double)curSize / totalSize << endl;
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