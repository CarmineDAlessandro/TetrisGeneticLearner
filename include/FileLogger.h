#ifndef FILELOGGER_H
#define FILELOGGER_H

#include <Individual.h>

#include <stdio.h>
#include <math.h>


class FileLogger
{
    public:
        FileLogger(char* fileName);
        void log (Individual** population, int populationSize, int generation);

    protected:

    private:
        FILE* fp;
        char* fileName;
};

#endif // FILELOGGER_H
