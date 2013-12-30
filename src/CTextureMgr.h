#ifndef CTEXTUREMGR_H
#define CTEXTUREMGR_H


class CTextureMgr
{
    public:
        static CTextureMgr &getInstance()
        {
            static CTextureMgr Instance;
            return Instance;
        }
        
        void clear();
        GLuint get(const std::string &strName);
        GLuint getWhite();
        
    private:
        std::unordered_map<std::string, GLuint> m_Map;
        
        CTextureMgr() {}
        void add(const std::string &strName, GLuint Texture);
        
        ~CTextureMgr()
        {
            clear();
        }
};

#endif // CTEXTUREMGR_H
