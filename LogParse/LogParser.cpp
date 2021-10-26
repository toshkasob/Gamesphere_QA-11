// LogParser.cpp: определяет точку входа для консольного приложения.
//
#include <iostream>
#include <ctime>
#include <string>
#include "boost\filesystem.hpp"
//#include "boost\program_options.hpp"

#include "ParseInputArgs.h"
#include "ParseFile.h"


//int _tmain(int argc, _TCHAR* argv[])
//{
//    return 0;
//}

int main (int argc, char *argv[]) {
	//Инициализация таймера
	time_t start_program;
	time(&start_program);
	clock_t t_start = clock();
	time_t T1,T2,T_start, T_end;
	time(&T_start);
	clock_t t;
	t = clock();

    std::string path ;
    std::string filename ;
    std::string exe_name = argv[0];
    // Разбор аргументов запуска
    for (int i_arg = 1; i_arg < argc; i_arg++)
    {
        switch (argv[i_arg][0])
        {
        case '-':
            switch (argv[i_arg][1]) 
            {
            case '-':
                if (strcmp("dir", &argv[i_arg][2]) == NULL && argc >= i_arg + 1)
                {
                    path = argv[++i_arg];
                } 
                else if (strcmp("file", &argv[i_arg][2]) == NULL && argc >= i_arg + 1)
                {
                    filename = argv[++i_arg];
                } 
                else
                {
                    printf("invalid options =(\n");
                    showInputArgs(argv[0]);
                }

                break;
            case 'f':
                filename = argv[++i_arg];
                break;
            default:
                printf("invalid single options =(\n");
                showInputArgs(argv[0]);
                break;
            }
            break;
        default:
            showInputArgs(argv[0]);
            break;
        }
    }

    if (!filename.size() && !path.size()) {
        showInputArgs(argv[0]);
    } 
    else if (!filename.size() && boost::filesystem::is_directory(path))
    {
        boost::filesystem::directory_iterator iter_dir(path);
        boost::filesystem::directory_iterator end;
        for (; iter_dir != end; ++iter_dir) {
            if (!boost::filesystem::is_regular_file(*iter_dir))
                continue;
            filename = boost::filesystem::canonical(iter_dir->path()).string();
            parseFile(filename);
        }

    }
    else if (!path.size() && boost::filesystem::is_regular_file(filename))
    {
        parseFile(filename);
    }
    else if (path.size())
    {
        printf("%s has a problem", path);
        showInputArgs(argv[0]);
        system("pause");
    }
    else if (filename.size())
    {
        printf("%s has a problem", filename);
        showInputArgs(argv[0]);
        system("pause");
    }



}
