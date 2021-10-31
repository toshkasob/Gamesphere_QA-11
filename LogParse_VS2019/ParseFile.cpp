#include <string>
#include <fstream>

#include "ParseFile.h"


void
parseFile(const std::string& filename, const std::string& substr)
{
    FILE *file4read = fopen(filename.c_str(), "r");
    std::fstream logfile(file4read);
    if (!logfile.is_open()) {
        printf("Sorry, File %s can't open and parse =(\n", filename.c_str());
        return ;
    }
    unsigned int nLine = 0;
    while (!logfile.eof()) {
        std::string cur_line;
        std::getline(logfile, cur_line);    nLine++;
        std::size_t pos_substr = cur_line.find(substr);
        while (pos_substr >= 0 && pos_substr <= cur_line.size()) {
            printf("[line %d, pos %d] %s\n", nLine, pos_substr, cur_line.c_str());
            pos_substr = cur_line.find(substr, pos_substr + 1);
        
    }
    
    //pass
}

