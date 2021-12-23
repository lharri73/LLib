#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <dirent.h>
#include <map>
#include <utility>
#include <string>
#include <vector>
#include <unistd.h>
#include <sstream>

namespace LLib{
    typedef struct access_pair{
        bool is_safe;
        time_t mtime;
        std::string path;
    } access_pair;

    access_pair is_dir(std::string downloads, const char* filename);

    // return a map of every file in the downloads folder, sorted on time
    std::multimap<time_t,std::string> parse_downloads(bool files_only=true);
};
