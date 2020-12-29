#ifndef FILE_SYSTEM_FILESYSTEM_H
#define FILE_SYSTEM_FILESYSTEM_H


#include <memory>
#include <utility>
#include "PhysicalMemory.h"
#include "Directory.h"
#include "File.h"

class FileSystem {
public:
    explicit FileSystem(PhysicalMemory memory) : memory(std::move(memory)) {
        Directory rootDir("/");
        root = std::make_shared<Directory>(rootDir);
        current_dir = root;
    }

    std::shared_ptr<Directory> CreateDir(const std::string &name) {
        std::shared_ptr<Directory> new_dir = std::make_shared<Directory>(Directory(name));
        current_dir->Add(new_dir);
        return new_dir;
    }

    std::shared_ptr<Directory> GetDir(const std::string &name) const {
        auto entity = current_dir->GetEntity(name);
        auto directory = std::dynamic_pointer_cast<Directory>(entity);
        return directory;
    }

    void ChangeDirectory(const std::string &name) {
        auto dir = GetDir(name);
        current_dir = dir;
    }

    void DeleteDir(const std::string &name) {
        current_dir->Delete(name);
    }

    void CreateFile(const std::string &name, std::string &&body) {
        auto file = std::make_shared<File>(File(name));
        file->SetDeleteFunction([&](const File &file) {
            auto[from, to]=file.GetMemoryRange();
            memory.Delete(from, to);
        });
        auto[start_idx, last_index] =memory.Add(std::move(body));
        file->SetIndexRange(start_idx, last_index);
        current_dir->Add(file);
    }

    void CopyFileToDir(const std::string &file_name, const std::string &dir_name) {
        auto file = ReadFromFile(file_name);
        CreateFile(file->GetName(), std::string(file->GetBody()));
    }

    void MoveFileToDir(const std::string &file_name, const std::string &dir_name) {
        auto file = ReadFromFile(file_name);
        CreateFile(file->GetName(), std::string(file->GetBody()));
        DeleteFile(file->GetName());
    }

    std::shared_ptr<File> ReadFromFile(const std::string &name) {
        std::shared_ptr<FileSystemEntity> entity;
        try {
            entity = current_dir->GetEntity(name);
        } catch (const std::exception &ex) {
            throw std::invalid_argument("not found");
        }
        auto file = std::dynamic_pointer_cast<File>(entity);
        auto[from, to] = file->GetMemoryRange();
        std::string_view body = memory.Read(from, to);
        file->SetBody(body);
        return file;
    }

    void WriteToFile(const std::string &name, std::string &&body) {
        auto entity = current_dir->GetEntity(name);
        auto file = std::dynamic_pointer_cast<File>(entity);
        auto[from, to] = file->GetMemoryRange();
        memory.Write(from, to, std::move(body));
    }

    void DeleteFile(const std::string &name) {
        current_dir->Delete(name);
    }

    std::string Dump(std::ostream &os = std::cout) const {
        os << std::string(0, '\t') << root->GetName() << "\n";
        dump(os, root, 1);
    }

    void dump(std::ostream &os, const std::shared_ptr<Directory> dir, int level) const {
        for (const auto &it:*dir) {
            os << std::string(level, '\t') << it->GetName() << "\n";
            if (it->GetType() == FileSystemEntities::FILE) {
                continue;
            }
            auto newDir = std::dynamic_pointer_cast<Directory>(it);
            dump(os, newDir, level + 1);
        }
    }


private:
    std::shared_ptr<Directory> root;
    std::shared_ptr<Directory> current_dir;
    PhysicalMemory memory;
};

#endif //FILE_SYSTEM_FILESYSTEM_H
