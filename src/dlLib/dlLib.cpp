#include "dlLib/dlLib.hpp"
#include <string.h>

namespace LLib{
    access_pair is_dir(std::string downloads, const char* filename){
        downloads = downloads + std::string("/") + filename;
        struct stat file_stat;
        stat(downloads.c_str(), &file_stat);

        access_pair ret;
        ret.is_safe = !S_ISDIR(file_stat.st_mode) && !S_ISLNK(file_stat.st_mode);
        ret.mtime = file_stat.st_mtime;
        ret.path = downloads;

        return ret;
    }

    std::multimap<time_t,std::string> parse_downloads(bool files_only){
        //
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
        access_pair pair;
        while(true){
            dw_dirent = readdir(dw_DIR);
            if(!dw_dirent) break;

            if(strncmp(".", dw_dirent->d_name, 2) == 0) continue;
            if(strncmp("..", dw_dirent->d_name, 3) == 0) continue;

            pair = is_dir(downloads.str(), dw_dirent->d_name);
            if(!pair.is_safe && files_only) continue;
            files.insert(std::make_pair(pair.mtime, pair.path));
        }

        closedir(dw_DIR);

        if(files.size() == 0){
            fprintf(stderr, "Downloads directory is empty. Exiting\n");
            exit(1);
        }
        return files;
    }

};
