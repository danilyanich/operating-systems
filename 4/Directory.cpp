//
// Created by Anton Schur on 12/12/2020.
//

#include <iostream>
#include "Directory.h"
#include "Table.h"

std::shared_ptr<Directory> & Directory::findDirectory(const std::string &dirName)
{
    for (auto& dir : children)
    {
        if (dirName == dir->name)
        {
            return dir;
        }
    }
    throw std::exception();
}

std::shared_ptr<File> & Directory::findFile(const std::string &fileName)
{
    for (auto& file : files)
    {
        if (fileName == file->name)
        {
            return file;
        }
    }
    throw std::exception();
}

void Directory::addFile(const std::string& file)
{
    files.push_back(std::make_shared<File>(File(file)));
}

void Directory::delFile(std::shared_ptr<File> &file)
{
    if (Table::getChunk(file->index)->copy == 1)
    {
        Table::delChunk(file->index);
        for (auto start = files.begin(); start != files.end(); start++)
        {
            if ((*start)->name == file->name)
            {
                files.erase(start);
                break;
            }
        }
    }
    else
    {
        Table::getChunk(file->index)->copy--;
        for (auto start = files.begin(); start != files.end(); start++)
        {
            if ((*start)->name == file->name)
            {
                files.erase(start);
                break;
            }
        }
    }
}

void Directory::addDir(const std::string& dir)
{
    children.push_back(std::make_shared<Directory>(Directory(dir)));
}

void Directory::delDir(std::shared_ptr<Directory>& dir)
{
    for (auto file : dir->files)
    {
        delFile(file);
    }
    for (auto direc : dir->children)
    {
        delDir(direc);
    }
    for (auto start = children.begin(); start != children.end(); start++)
    {
        if ((*start)->name == dir->name)
        {
            children.erase(start);
            break;
        }
    }
}

void Directory::print()
{
    std::cout << "FILES:\n";
    for (const auto &file : files)
    {
        std::cout << file->name << "\n";
    }
    std::cout << "DIRECTORIES:\n";
    for (const auto &dir : children)
    {
        std::cout << dir->name << "\n";
    }
}

void Directory::moveFile(std::shared_ptr<File> &file)
{
    files.push_back(file);
    Table::getChunk(file->index)->copy++;
}
