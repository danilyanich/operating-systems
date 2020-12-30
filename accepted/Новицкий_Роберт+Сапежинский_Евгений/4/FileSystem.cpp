#include "FileSystem.h"
#include <iostream>
#include <cstring>
#include "File.h"


void FileSystem::run()
{
    std::string input;
    std::cout << "FS >>>\n";
    while (std::cin >> input)
    {
        if (input == "create")
        {
            std::cin >> input;
            current->addFile(input);
        } else if (input == "createdir")
        {
            std::cin >> input;
            current->addDir(input);
        } else if (input == "del")
        {
            std::cin >> input;
            try
            {
                current->delFile(current->findFile(input));
            } catch (std::exception &)
            {
                std::cout << "NO SUCH FILE\n";
            }
        } else if (input == "deldir")
        {
            std::cin >> input;
            try
            {
                current->delDir(current->findDirectory(input));
            } catch (std::exception &)
            {
                std::cout << "NO SUCH DIRECTORY\n";
            }
        } else if (input == "copy")
        {
            try
            {
                std::cin >> input;
                auto check = input;
                int index = current->findFile(input)->index;
                std::cin >> input;
                if (input == check)
                {
                    std::cout << "FILE WITH THIS NAME EXISTS\n";
                } else
                {
                    current->addFile(input);
                    auto nf = current->findFile(input);
                    nf->index = index;
                    Table::getChunk(nf->index)->copy++;
                }
            }
            catch (std::exception &)
            {
                std::cout << "NO SUCH FILE";
            }
        } else if (input == "move")
        {
            std::cin >> input;
            try
            {
                auto dir = current->findDirectory(input);
                std::cin >> input;
                auto file = current->findFile(input);
                dir->moveFile(file);
                current->delFile(file);
            }
            catch (std::exception &)
            {
                std::cout << "NO SUCH CONSUMABLES";
            }
        } else if (input == "write")
        {
            try
            {
                std::cin >> input;
                auto file = current->findFile(input);
                std::cin >> input;
                int size = input.length();
                char buf[size];
                std::strcpy(buf, input.c_str());
                file->write(buf);
            }
            catch (std::exception &)
            {
                std::cout << "ERROR\n";
            }

        } else if (input == "read")
        {
            try
            {
                std::cin >> input;
                std::cout << current->findFile(input)->read() << "\n";
            }
            catch (std::exception &)
            {
                std::cout << "ERROR\n";
            }
        } else if (input == "cd")
        {
            std::cin >> input;
            if (input == root->name)
            {
                current = root;
            } else
            {
                try
                {
                    current = current->findDirectory(input);
                }
                catch (std::exception &)
                {
                    std::cout << "NO SUCH DIRECTORY\n";
                }
            }
        } else if (input == "ls")
        {
            current->print();
        } else
        {
            std::cout << "Incorrect Command\n";
        }
        std::cout << "\nFS >>>\n";
    }
}

FileSystem::FileSystem()
{
    root = std::make_shared<Directory>(Directory("root"));
    current = root;
}
