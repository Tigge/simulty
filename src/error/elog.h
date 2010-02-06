
#ifndef ELOG_H
#define ELOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class ELog : public std::ostringstream {

    private:

    int type;
    std::ofstream f;

    void clean();

    protected:
    
    std::ostream& flush();

    public:

    static const int TYPE_NONE = 1;
    static const int TYPE_COUT = 1;
    static const int TYPE_CERR = 2;
    static const int TYPE_FILE = 3;

    ELog();
    virtual ~ELog();

    void use_console();
    void use_file(std::string name);
    void use_none();



};

//extern ELog err;

#endif
