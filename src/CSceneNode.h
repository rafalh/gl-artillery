#ifndef CSCENENODE_H
#define CSCENENODE_H

class CSceneNode
{
    public:
        CSceneNode();
        virtual ~CSceneNode();
        virtual void render() = 0;
        
        virtual bool isTransparent() const
        {
            return false;
        }
        
    private:
        std::vector<CSceneNode*> m_Children;
};

#endif // CSCENENODE_H
