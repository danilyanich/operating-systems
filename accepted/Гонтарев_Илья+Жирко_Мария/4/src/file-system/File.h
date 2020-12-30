#include <utility>

//
// Created by illfate on 11/26/20.
//

#ifndef FILE_SYSTEM_FILE_H
#define FILE_SYSTEM_FILE_H


class File : public FileSystemEntity {
public:
    File(const std::string &name, std::string_view body) {
        header.name = name;
        this->body.data = body;
    }

    File(const std::string &name) {
        header.name = name;
    }

    void SetDeleteFunction(std::function<void(const File &file)> callback) {
        this->deleteCallback = std::move(callback);
    }

    void SetBody(std::string_view body) {
        this->body.data = body;
    }

    std::string_view GetBody() const {
        return this->body.data;
    }

    FileSystemEntities GetType() const override {
        return FileSystemEntities::FILE;
    }

    std::string GetName() const override {
        return header.name;
    }

    void SetIndexRange(size_t first_block_idx, size_t to_block_idx) {
        this->first_block_idx = first_block_idx;
        this->to_block_idx = to_block_idx;
    }

    std::pair<size_t, size_t> GetMemoryRange() const {
        return {first_block_idx, to_block_idx};
    }

    void Delete() override {
        deleteCallback(*this);
    }

private:
    struct Header {
        std::string name;
    };

    struct Body {
        std::string_view data;
    };

    Header header;
    Body body;
    size_t first_block_idx{};
    size_t to_block_idx{};
    std::function<void(const File &file)> deleteCallback;
};

#endif //FILE_SYSTEM_FILE_H
