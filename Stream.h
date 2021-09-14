//
// Created by pe200012 on 2021/09/14.
//

#ifndef JSONLIB_STREAM_H
#define JSONLIB_STREAM_H

#include <fstream>
#include <memory>
#include "Token.h"
using namespace std;

template <typename T, typename U>
struct Stream {
    virtual T get() const = 0;
    T peek() const;
    virtual bool exhausted() const = 0;
    virtual U next() = 0;
};

template<typename T, typename U>
T Stream<T, U>::peek() const {
    return nullptr;
}

struct FileStream : public Stream<char, FileStream> {
    explicit FileStream(shared_ptr<ifstream> input) : Stream(), file(std::move(input)) {
        x = static_cast<char>(file->peek());
        if (!file->eof()) {
            char c[1];
            file->read(c, 1);
        }
    }
    char get() const override {
        return x;
    }
    char peek() const {
        return static_cast<char>(file->peek());
    }
    bool exhausted() const override {
        return file->eof();
    }
    FileStream next() override {
        return FileStream(file);
    }
private:
    shared_ptr<ifstream> file;
    char x;
};



#endif //JSONLIB_STREAM_H
