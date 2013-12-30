#include "stdafx.h"
#include "CLogger.h"

using namespace std;

CLogger::CLogger()
{
    m_File = fopen("Artillery.log", "w");
    if(!m_File)
        error("Failed to open log file\n");
}

CLogger::~CLogger()
{
    if(m_File)
        fclose(m_File);
}

void CLogger::info(const char *pszFormat, ...)
{
    va_list ArgList;

    fprintf(stdout, "INFO: ");
    va_start(ArgList, pszFormat);
    vfprintf(stdout, pszFormat, ArgList);
    va_end(ArgList);

    if(m_File)
    {
        fprintf(m_File, "INFO: ");
        va_start(ArgList, pszFormat);
        vfprintf(m_File, pszFormat, ArgList);
        va_end(ArgList);
#ifdef DEBUG
        fflush(m_File);
#endif
    }
}

void CLogger::warning(const char *pszFormat, ...)
{
    va_list ArgList;

    fprintf(stderr, "WARNING: ");
    va_start(ArgList, pszFormat);
    vfprintf(stderr, pszFormat, ArgList);
    va_end(ArgList);

    if(m_File)
    {
        fprintf(m_File, "WARNING: ");
        va_start(ArgList, pszFormat);
        vfprintf(m_File, pszFormat, ArgList);
        va_end(ArgList);
#ifdef DEBUG
        fflush(m_File);
#endif
    }
}

void CLogger::error(const char *pszFormat, ...)
{
    va_list ArgList;

    fprintf(stderr, "ERROR: ");
    va_start(ArgList, pszFormat);
    vfprintf(stderr, pszFormat, ArgList);
    va_end(ArgList);

    if(m_File)
    {
        fprintf(m_File, "ERROR: ");
        va_start(ArgList, pszFormat);
        vfprintf(m_File, pszFormat, ArgList);
        va_end(ArgList);
#ifdef DEBUG
        fflush(m_File);
#endif
    }
}
