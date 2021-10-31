#include <iostream>

void showInputArgs(char* exe_name) 
{
    printf("%s \n", exe_name);
    printf("This console application supports 3 types of arguments: \n");
    printf("\t -f \"filepath\" \n");
    printf("\t --file \"filepath\" \n");
    printf("\t --dir \"path\" \n");
    printf("and 2 types of options: \n");
    printf("\t --substr=\"substr4parse\" \n");
    printf("\t --outpath=\"path4parsedLines.log\" \n");

    printf("[e.g.]: %s -f \"log.log\" --substr=\"Error\" --outpath=\"./\"\n", exe_name);

    printf(" \n \t***%s will be exit =)***\n", exe_name);
    printf(" \n \t***You can run me again =)***\n");
    system("pause");
    exit(0);
}


//void parseInputArgs(const int argc, const char *argv[]) {
//    for (int i_arg = 1; i_arg < argc; i_arg++)
//    {
//        switch (argv[i_arg][0])
//        {
//        case '-':
//            switch (argv[i_arg][1]) 
//            {
//            case '-':
//                parseLongArg(argv[i_arg]);
//            }
//
//        default:
//            break;
//        }
//    }
//}
