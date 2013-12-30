#ifndef CFPSCOUNTER_H
#define CFPSCOUNTER_H

class CFpsCounter
{
    public:
        class IObserver
        {
            public:
                virtual void onFpsChange() = 0;
        };
        
        void addObserver(IObserver *pObserver)
        {
            m_Observers.push_back(pObserver);
        }
        
        float getFps() const
        {
            return m_CurrentFps;
        }
        
        float getMinFps() const
        {
            return m_MinFps;
        }
        
        float getMaxFps() const
        {
            return m_MaxFps;
        }
        
        CFpsCounter();
        virtual ~CFpsCounter() {}
        void onFrame();
    
    private:
        std::vector<IObserver*> m_Observers;
        float m_CurrentFps, m_MinFps, m_MaxFps;
        double m_LastUpdate;
        unsigned m_Counter;
        bool m_FirstFrame;
};

#endif // CFPSCOUNTER_H
