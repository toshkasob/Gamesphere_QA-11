﻿#include <string>
#include <fstream>
#include <ctime>
#include "ParseFile.h"

void 
updateProgressCheckingFile(const int n_prevLine, const int n_curLine) 
{
    if (n_curLine == 0) {
        printf("Check Line %d", n_curLine);
        return;
    }
    //Смещение каретки
    int numb_backspaces = std::to_string(n_prevLine).length();
    for (int n_back = 0; n_back < numb_backspaces; n_back++)
        printf("\b");
    printf("%d", n_curLine); //! Перезапись в stdout номера строки, в которой ищем
    return;
}

void
parseFile(const std::string& filename,
          const std::string& substr,
          const std::string& outpath
         )
{
    FILE *file4read = fopen(filename.c_str(), "r");
    std::fstream logfile(file4read);
    if (!logfile.is_open()) {
        printf("Sorry, File %s can't open and parse =(\n", filename.c_str());
        return ;
    }
    std::string outFile4parsedLines = outpath + "/./parsedLines.log";
    FILE * file4write = fopen(outFile4parsedLines.c_str(), "a");
    std::ofstream parsedLines(file4write);
    if (!parsedLines.is_open()) {
        printf("Sorry, File %s can't open for write parsedLines =(\n", outFile4parsedLines.c_str());
    }
    time_t cur_time ;
    time(&cur_time);
    std::string time4parsedLines = ctime(&cur_time);
    time4parsedLines.pop_back();
    fprintf(file4write, "\n[%s] Start parsing %s\n", time4parsedLines.c_str(), filename.c_str());
    printf("\n[%s] Start parsing %s\n", time4parsedLines.c_str(), filename.c_str());
    unsigned int nLine = 0;
    updateProgressCheckingFile(nLine, 0);
    while (!logfile.eof()) {
        std::string cur_line;
        std::getline(logfile, cur_line);    nLine++;    //! чтение строки
        updateProgressCheckingFile(nLine-1, nLine);
        std::size_t pos_substr = cur_line.find(substr);
        while (pos_substr >= 0 && pos_substr <= cur_line.size()) {
            //printf("[line %d, pos %d] %s\n", nLine, pos_substr, cur_line.c_str());
            fprintf(file4write, "[line %d, pos %d] %s\n", nLine, pos_substr, cur_line.c_str());
            pos_substr = cur_line.find(substr, pos_substr + 1);
        }
    }
    time(&cur_time);
    time4parsedLines = ctime(&cur_time);
    time4parsedLines.pop_back();
    fprintf(file4write, "[%s] End parsing %s\n", time4parsedLines.c_str(), filename.c_str());
    printf("\n[%s] End parsing %s\n", time4parsedLines.c_str(), filename.c_str());

    fclose(file4write);
    file4write = NULL;
    fclose(file4read);
    file4read = NULL;
    //pass
}

