#ifndef CEXCEPTION_H
#define CEXCEPTION_H

#define DEFINE_EXCEPTION_CLASS(name) class name: public CBaseException\
{\
    public:\
        name(const char *pszFormat, ...)\
        {\
            va_list ArgList;\
            va_start(ArgList, pszFormat);\
            vsnprintf(m_szReason, sizeof(m_szReason), pszFormat, ArgList);\
            va_end(ArgList);\
        }\
}

class CBaseException: public std::exception
{
    public:
        CBaseException()
        {
            m_szReason[0] = '\0';
        }
        
        const char *what() const throw() // noexcept(true)
        {
            return m_szReason;
        }
        
    protected:
        char m_szReason[255];
};

DEFINE_EXCEPTION_CLASS(CFileNotFoundException);
DEFINE_EXCEPTION_CLASS(CWriteFailedException);
DEFINE_EXCEPTION_CLASS(CInvalidParameterException);

#endif // CEXCEPTION_H
