
#include "elog.h"

//ELog err;

ELog::ELog(): std::ostringstream(std::ios_base::in)
{
    std::cout << "Log: Created" << std::endl;
    //setf(std::ios::unitbuf);
    type = TYPE_NONE;
}

ELog::~ELog()
{
    flush();
}

std::ostream& ELog::flush()
{
    if(type == TYPE_COUT)
    {
        std::cout << str();
    }
    else if(type == TYPE_CERR)
    {
        std::cerr << str();
    }
    else if(type == TYPE_FILE)
    {
        f << str();
    }

    // Clean:
    std::string tmp;
    str(tmp);

    std::cout << "Log: Flush" << std::endl;

    return *this;
}


void ELog::clean()
{
    if(f.is_open())f.close();

}

void ELog::use_console()
{
    clean();
    type = TYPE_COUT;
    std::cout << "Log: Using console" << std::endl;
}

void ELog::use_file(std::string name)
{
    clean();
    f.open(name.c_str(), std::ios_base::out);
    if(f) type = TYPE_FILE;

}

void ELog::use_none()
{
    clean();
    type = TYPE_NONE;
}
