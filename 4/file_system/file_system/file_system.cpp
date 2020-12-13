
#include "file_system.h"

Folder* file_system::find_folder(const string& name) {
    vector<string> find_folder_path;
    string find_folder_name = name.substr(name.find_last_of('/') + 1);

    if (this->root->name == find_folder_name)
        return this->root;

    find_folder_path.emplace(find_folder_path.begin(), find_folder_name);
    string curr_folder_path = name.substr(0, name.find_last_of('/'));
    curr_folder_path = curr_folder_path.substr(curr_folder_path.find_first_of('/') + 1);

    while (curr_folder_path.find_last_of('/') != string::npos) {
        find_folder_path.emplace(find_folder_path.begin(),
            curr_folder_path.substr(curr_folder_path.find_last_of('/') + 1));
        curr_folder_path = curr_folder_path.substr(0, curr_folder_path.find_last_of('/'));
    }

    Folder* curr_folder = this->root;
    while (!find_folder_path.empty()) {
        for (auto folder : curr_folder->folders) {
            if (folder->name == find_folder_path[0]) {
                curr_folder = folder;
                find_folder_path.erase(find_folder_path.begin());
            }
        }
        //return nullptr;
    }
    return find_folder(find_folder_name, curr_folder);
}

Folder* file_system::find_folder(const string& name, Folder* curr_folder) {
    if (curr_folder->name == name)
        return curr_folder;
    for (auto folder : curr_folder->folders) {
        if (folder->name == name)
            return folder;
    }
    return nullptr;
}

File* file_system::find_file(const string& name, Folder* curr_folder) {
    auto pos = name.find('.');
    string fileName = name.substr(0, pos);
    string ext = name.substr(pos + 1);
    for (auto file : curr_folder->files) {
        if (file->name == fileName && file->extension == ext)
            return file;
    }
    return nullptr;
}

int file_system::create_folder(User* user, string& folder_name) {
    if ((int)(user->chmode[0]) < 2)
        return (int)code_error::INCORRECT_ACCESS_RIGHTS;
    auto new_folder = new Folder(folder_name,
        vector<Folder*>(),
        vector<File*>(),
        user->chmode,
        this->currFolder->prev_folder + '/' + this->currFolder->name);
    this->currFolder->folders.push_back(new_folder);

    cout << "User: " << user->name << " successfully created folder " << to_string(*new_folder) << endl;
    return (int)code_error::OK;
}

int file_system::delete_folder(User* user, Folder* folder_to_delete) {
    if ((int)(user->chmode[0]) < 2)
        return (int)code_error::INCORRECT_ACCESS_RIGHTS;

    string answer;
    cout << "Are you sure?(y/n): ";
    getline(cin, answer);
    if (answer != "y") return (int)code_error::STOP;

    folder_to_delete->folders.clear();
    folder_to_delete->files.clear();
    Folder* prev_folder = find_folder(folder_to_delete->prev_folder);
    for (size_t i = 0; i < prev_folder->folders.size(); ++i) {
        if (prev_folder->folders[i]->name == folder_to_delete->name) {
            prev_folder->folders.erase(prev_folder->folders.begin() + i);
        }
    }
    cout << "User: " << user->name << " successfully deleted folder " << folder_to_delete->name << endl;
    delete folder_to_delete;
    return (int)code_error::OK;
}

int file_system::copy_folder(User* user, Folder* folder_to_copy, Folder* destination_folder) {
    if ((int)(user->chmode[0]) < 2)
        return (int)code_error::INCORRECT_ACCESS_RIGHTS;
    auto new_folder = new Folder(
        "",
        vector<Folder*>(),
        vector<File*>(),
        "",
        ""
    );
    new_folder->name = folder_to_copy->name;
    new_folder->prev_folder = destination_folder->prev_folder + '/' + destination_folder->name;
    new_folder->attr = folder_to_copy->attr;
    copy(folder_to_copy->folders.begin(), folder_to_copy->folders.end(), new_folder->folders.begin());
    copy(folder_to_copy->files.begin(), folder_to_copy->files.end(), new_folder->files.begin());

    destination_folder->folders.push_back(new_folder);

    cout << "User: " << user->name << " successfully copied folder " << to_string(*folder_to_copy) << " to "
        << to_string(*new_folder)
        << endl;
    return (int)code_error::OK;
}

int file_system::move_folder(User* user, Folder* folder_to_move, Folder* destination_folder) {
    if ((int)(user->chmode[0]) < 2)
        return (int)code_error::INCORRECT_ACCESS_RIGHTS;

    Folder* prev_folder = find_folder(folder_to_move->prev_folder);
    for (size_t i = 0; i < prev_folder->folders.size(); ++i) {
        if (prev_folder->folders[i]->name == folder_to_move->name) {
            prev_folder->folders.erase(prev_folder->folders.begin() + i);
        }
    }
 
    folder_to_move->prev_folder = destination_folder->prev_folder + '/' + destination_folder->name;
    destination_folder->folders.push_back(folder_to_move);

    cout << "User: " << user->name << " successfully moved folder " << to_string(*prev_folder)
        << " to " << to_string(*destination_folder) << " New destination is: " << to_string(*folder_to_move) << endl;
    return (int)code_error::OK;
}

int file_system::create_file(User* user, string& file) {
    if ((int)(user->chmode[0]) < 2)
        return (int)code_error::INCORRECT_ACCESS_RIGHTS;
    string::size_type pos = string::npos;
    pos = file.find('.');
    auto new_file = new File(
        file.substr(0, pos),
        file.substr(pos + 1),
        currentDateTime(),
        0,
        (int)this->blocks.size(),
        0,
        this->currFolder->prev_folder + '/' + this->currFolder->name,
        user->chmode
    );
    this->currFolder->files.push_back(new_file);

    cout << "User: " << user->name << " successfully created file " << print_file_path(new_file) << endl;
    return 0;
}

int file_system::delete_file(User* user, File* file_to_delete) {
    if ((int)(user->chmode[0]) < 2)
        return (int)code_error::INCORRECT_ACCESS_RIGHTS;

    string answer;
    cout << "Are you sure?(y/n): ";
    getline(cin, answer);
    if (answer != "y") return (int)code_error::STOP;

   
    Folder* folder = find_folder(file_to_delete->file_folder_path);
    for (size_t i = 0; i < folder->files.size(); ++i) {
        if (folder->files[i]->name == file_to_delete->name) {
            folder->files.erase(folder->files.begin() + i);
        }
    }
   
    for (int i = file_to_delete->first_block; i < file_to_delete->first_block + file_to_delete->offset + 1; ++i) {
        blocks[i].memory = "";
    }

    cout << "User: " << user->name << " successfully deleted file " << print_file_path(file_to_delete) << endl;
    delete file_to_delete;
    return (int)code_error::OK;
}

int file_system::find_block(int start_from, int size_of_memory) {
    int free_space = 0;
    int block_pos = blocks.size();
    for (int i = start_from; i < (int)blocks.size(); ++i) {
        if (free_space >= size_of_memory) {
            return block_pos;
        }
        if (blocks[i].memory.empty()) {
            if (block_pos > i) block_pos = i;
            free_space += SIZE_OF_BLOCK;
        }
    }
   
    return (int)code_error::NEW_BLOCK;
}

int file_system::copy_file(User* user, File* file_to_copy, Folder* destination_folder) {
    if ((int)(user->chmode[0]) < 2)
        return (int)code_error::INCORRECT_ACCESS_RIGHTS;
   
    auto new_file = new File(
        file_to_copy->name,
        file_to_copy->extension,
        currentDateTime(),
        file_to_copy->size,
        (int)blocks.size(),
        (int)file_to_copy->offset,
        destination_folder->prev_folder + '/' + destination_folder->name,
        destination_folder->attr
    );
    
    for (int i = file_to_copy->first_block; i < file_to_copy->first_block + file_to_copy->offset + 1; ++i) {
        blocks.push_back(blocks[i]);
    }
    destination_folder->files.push_back(new_file);

    cout << "User: " << user->name << " successfully copied file " << print_file_path(file_to_copy) << " to "
        << print_file_path(new_file)
        << endl;
    return (int)code_error::OK;
}

int file_system::move_file(User* user, File* file_to_move, Folder* destination_folder) {
    if ((int)(user->chmode[0]) < 2)
        return (int)code_error::INCORRECT_ACCESS_RIGHTS;

    
    Folder* folder = this->currFolder;
    for (size_t i = 0; i < folder->files.size(); ++i) {
        if (folder->files[i]->name == file_to_move->name) {
            folder->files.erase(folder->files.begin() + i);
        }
    }

    file_to_move->file_folder_path = destination_folder->prev_folder + '/' + destination_folder->name;
    file_to_move->attr = destination_folder->attr;
    file_to_move->date = currentDateTime();
    destination_folder->files.push_back(file_to_move);

    cout << "User: " << user->name << " successfully moved file " << folder << '/' << file_to_move->name << " to "
        << print_file_path(file_to_move)
        << endl;
    return (int)code_error::OK;
}

void file_system::write_to_blocks(const string& data, int count_of_blocks, int first_block) {

    if (count_of_blocks == 0) {
        block new_block{ data, SIZE_OF_BLOCK };
        blocks.emplace(blocks.begin() + first_block, new_block);
        return;
    }
    vector<block> new_blocks;
    string tail = data.substr(count_of_blocks * SIZE_OF_BLOCK);
    for (int i = first_block; i < first_block + count_of_blocks; ++i) {
        block new_block{ data.substr((i - first_block) * SIZE_OF_BLOCK, SIZE_OF_BLOCK), SIZE_OF_BLOCK };
        new_blocks.push_back(new_block);
    }
    block new_block{ tail, SIZE_OF_BLOCK };
    new_blocks.push_back(new_block);
    for (int i = 0; i < (int)new_blocks.size(); ++i) {
        blocks.emplace(blocks.begin() + first_block + i, new_blocks[i]);
    }
}

int file_system::write_file(User* user, File* file, string& new_data) {
    string data;
    for (int i = file->first_block; i < (int)blocks.size(); ++i) {
        data += blocks[i].memory;
        blocks[i].memory = "";
    }
    data += new_data;
    file->size = (int)data.size();
    file->first_block = find_block(0, (int)file->size);
    file->offset = (file->size % SIZE_OF_BLOCK) ? (file->size / SIZE_OF_BLOCK) : file->size / SIZE_OF_BLOCK + 1;
    if (file->first_block == -1)
        file->first_block = blocks.size();
    write_to_blocks(data, (int)file->offset, file->first_block);
    return (int)code_error::OK;
}

string file_system::read_file(User* user, File* file) {
    string res = "";
    for (int i = file->first_block; i < file->first_block + file->offset + 1; ++i) {
        res += blocks[i].memory;
    }
    return res;
}


void file_system::print_file_system() {
    auto currFol_name = this->currFolder->prev_folder + '/' + this->currFolder->name;
    vector<string> curr_folder_name;
    string name = this->currFolder->name;
    while (!this->currFolder->prev_folder.empty()) {
        curr_folder_name.insert(curr_folder_name.begin(), this->currFolder->name);
        this->currFolder = find_folder(this->currFolder->prev_folder);
    }
    
    curr_folder_name.insert(curr_folder_name.begin(), this->currFolder->name);
    int level = 0;
    vector<int> places;
    while (curr_folder_name[0] != name) {
        for (int j = 0; j < level; ++j) {
            cout << "-";
        }
        cout << this->currFolder->name << endl;
        ++level;
        for (int i = 0; i < (int)this->currFolder->folders.size(); ++i) {
            if (this->currFolder->folders[i]->name == curr_folder_name[1]) {
                places.insert(places.begin(), i + 1);
                this->currFolder = find_folder(curr_folder_name[1], this->currFolder);
                curr_folder_name.erase(curr_folder_name.begin());
                break;
            }
            for (int j = 0; j < level; ++j) {
                cout << "-";
            }
            cout << this->currFolder->folders[i]->name << endl;
        }
    }
    print_folder(this->currFolder, level);
    if (!this->currFolder->prev_folder.empty())
        this->currFolder = find_folder(this->currFolder->prev_folder);
    while (!this->currFolder->prev_folder.empty()) {
        if (places[0] == this->currFolder->folders.size()) {
            places.erase(places.begin());
            continue;
        }
        for (int i = places[0]; i < (int)this->currFolder->folders.size(); ++i) {
            for (int j = 0; j < level; ++j) {
                cout << "-";
            }
            cout << this->currFolder->folders[i]->name << endl;
        }
        this->currFolder = find_folder(this->currFolder->prev_folder);
        --level;
    }
    int i = (!places.empty()) ? places[0] : (int)this->root->folders.size();
    if (i != this->root->folders.size()) {
        for (; i < (int)this->root->folders.size(); ++i) {
            for (int j = 0; j < level; ++j) {
                cout << "-";
            }
            cout << this->root->folders[i]->name << endl;
        }
    }
    this->currFolder = find_folder(currFol_name);
}

void file_system::print_folder(Folder* folder, int level) {
    cout << "*";
    for (int j = 0; j < level; ++j) {
        cout << "-";
    }
    cout << this->currFolder->name << endl;
    ++level;
    for (auto& i : folder->folders) {
        for (int j = 0; j < level; ++j) {
            cout << "-";
        }
        cout << i->name << endl;
    }
    for (auto& file : folder->files) {
        for (int j = 0; j < level; ++j) {
            cout << "-";
        }
        cout << file->name + '.' + file->extension << endl;
    }

}

void file_system::dump_dir(User* user, Folder* curr_folder) {
    for (auto folder : curr_folder->folders) {
        auto folderName = folder->prev_folder + '/' + folder->name;
        folderName = folderName.substr(folderName.find_first_of('/') + 1);
        fs::create_directory(folderName);
    }
    for (auto file : curr_folder->files) {
        auto folderName = file->file_folder_path;
        folderName = folderName.substr(folderName.find_first_of('/') + 1);
        ofstream fout(folderName + "/" + file->name + "." + file->extension);
        fout << read_file(user, file);
        fout.close();
    }
}

void file_system::dump(User* user) {
    fs::remove_all("root");
    fs::create_directory("root");
    for (auto folder : this->root->folders) {
        auto folderName = folder->prev_folder + '/' + folder->name;
        folderName = folderName.substr(folderName.find_first_of('/') + 1);
        fs::create_directory(folderName);
    }
    for (auto folder : this->root->folders) {
        dump_dir(user, folder);
    }
    for (auto file : this->root->files) {
        ofstream fout("root/" + file->name + "." + file->extension);
        fout << read_file(user, file);
        fout.close();
    }
}


Folder::Folder(const string& basicString, vector<Folder*> vec, vector<File*> vec1,
    const string& basicString1, const string& prev_folder) {
    this->name = basicString;
    this->folders = std::move(vec);
    this->files = std::move(vec1);
    this->attr = basicString1;
    this->prev_folder = prev_folder;
}

File::File(const string& name, const string& extension, const string& date, unsigned int size, int first_block,
    int offset,
    const string& file_folder_path, const string& attr) {
    this->name = name;
    this->extension = extension;
    this->date = date;
    this->size = size;
    this->first_block = first_block;
    this->offset = offset;
    this->file_folder_path = file_folder_path;
    this->attr = attr;
}
