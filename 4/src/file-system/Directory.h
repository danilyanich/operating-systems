//
// Created by illfate on 11/26/20.
//

#ifndef FILE_SYSTEM_DIRECTORY_H
#define FILE_SYSTEM_DIRECTORY_H


#include <memory>
#include <set>
#include <functional>
#include <map>
#include "FileSystemEntity.h"
#include "File.h"

class Directory : public FileSystemEntity {
public:
    explicit Directory(std::string name) : name(std::move(name)) {}

    std::string GetName() const override {
        return name;
    }

    using entities = std::set<std::shared_ptr<FileSystemEntity>>;

    void Add(const std::shared_ptr<FileSystemEntity> &entity) {
        contains.insert(entity);
        nameToEntity[entity->GetName()] = entity;
    }

    entities::iterator begin()  {
        return contains.begin();
    }

    entities::iterator end()  {
        return contains.end();
    }

    FileSystemEntities GetType() const override {
        return FileSystemEntities::DIRECTORY;
    }


    void Delete(std::shared_ptr<FileSystemEntity> &entity) {
        entity->Delete();
        contains.erase(entity);
        nameToEntity.erase(entity->GetName());
    }

    void Delete(const std::string &name) {
        auto entity = GetEntity(name);
        Delete(entity);
    }

    std::shared_ptr<FileSystemEntity> GetEntity(const std::string &name) const {
        auto entity = nameToEntity.at(name);
        return entity;
    }

    void Delete() {
        for (auto &e:contains) {
            e->Delete();
        }
        contains.clear();
    }

private:
    std::set<std::shared_ptr<FileSystemEntity>> contains;
    std::map<std::string, std::shared_ptr<FileSystemEntity>> nameToEntity;
    std::string name;
};


#endif //FILE_SYSTEM_DIRECTORY_H
