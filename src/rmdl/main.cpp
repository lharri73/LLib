#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <sstream>
#include <dirent.h>
#include <map>
#include <utility>
#include <string.h>
#include <vector>

typedef struct acces_pair{
    bool is_safe;
    time_t mtime;
    std::string path;
} acces_pair;

acces_pair is_dir(std::string downloads, const char* filename){
    downloads = downloads + std::string("/") + filename;
    struct stat file_stat;
    stat(downloads.c_str(), &file_stat);

    acces_pair ret;
    ret.is_safe = !S_ISDIR(file_stat.st_mode) && !S_ISLNK(file_stat.st_mode);
    ret.mtime = file_stat.st_mtime;
    ret.path = downloads;

    return ret;
}

int main(int argc, char** argv){

    if(argc != 1){
        fprintf(stderr, "usage: %s [destination]\n", argv[0]);
        fprintf(stderr, "Moves the most recently downloaded file to the provided destination -or- current location.");
        exit(2);
    }

    // get the password entry
    // is this safe? ¯\_(ツ)_/¯
    // This function just *sounds* dangerous
    struct passwd *pw = getpwuid(getuid());

    const char *homedir = pw->pw_dir;
    std::stringstream downloads;
    downloads << homedir;
    downloads << "/Downloads";

    DIR* dw_DIR = opendir(downloads.str().c_str());
    if(!dw_DIR){
        perror("Unable to open Downloads directory: ");
        exit(-1);
    }

    // get the inode of the downloads directory
    int dw_fd = dirfd(dw_DIR);
    struct stat file_stat;  
    fstat (dw_fd, &file_stat);
    ino_t dw_inode = file_stat.st_ino;

    std::multimap<time_t,std::string> files;

    struct dirent *dw_dirent;
    acces_pair pair;
    while(true){
        dw_dirent = readdir(dw_DIR);
        if(!dw_dirent) break;

        if(strncmp(".", dw_dirent->d_name, 2) == 0) continue;
        if(strncmp("..", dw_dirent->d_name, 3) == 0) continue;

        pair = is_dir(downloads.str(), dw_dirent->d_name);
        if(!pair.is_safe) continue;
        files.insert(std::make_pair(pair.mtime, pair.path));

    }

    closedir(dw_DIR);

    if(files.size() == 0){
        fprintf(stderr, "No available files to remove in downloads directory. Exiting\n");
        exit(1);
    }
    std::multimap<time_t,std::string>::const_reverse_iterator it = files.crbegin(); // the first element in the reversed map (last element in the non-reversed map)

    std::vector<char*> new_argv;
    
    // did someone say memory leak?
    char* tmp = (char*)malloc(sizeof(char) * it->second.size());
    memcpy(tmp, it->second.c_str(), it->second.size()+1);

    new_argv.push_back("rm");
    new_argv.push_back(tmp);
    new_argv.push_back(nullptr);

    printf("rm %s \n", tmp);
    execvp("rm", new_argv.data());
    perror("rm did not execute:");

    return 0;
}
