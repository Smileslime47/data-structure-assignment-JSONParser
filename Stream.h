//
// Created by pe200012 on 2021/09/14.
//

#ifndef JSONLIB_STREAM_H
#define JSONLIB_STREAM_H

#include <fstream>
#include <memory>
#include "Token.h"
using namespace std;

// 惰性流接口
template <typename T, typename U>
struct Stream {
    virtual T get() const = 0;  // 获取当前元素
    T peek() const; // 获取下一个元素
    virtual bool exhausted() const noexcept = 0; // 是否已经结束
    virtual U next() = 0; // 进入下一状态
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
    bool exhausted() const noexcept override {
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
