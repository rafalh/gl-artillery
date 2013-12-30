#ifndef CLOGGER_H
#define CLOGGER_H

class CLogger
{
    public:
        static CLogger &getInstance()
        {
            static CLogger Inst;
            return Inst;
        }
        
        void info(const char *fmt, ...);
        void warning(const char *fmt, ...);
        void error(const char *fmt, ...);
    
    private:
        CLogger(); // constructor is hidden
        CLogger(const CLogger &Logger); // copy constructor is hidden
		~CLogger(); // destructor is hidden
		CLogger &operator=(const CLogger &Logger); // assign operator is hidden
		
		FILE *m_File;
};

#endif // CLOGGER_H
