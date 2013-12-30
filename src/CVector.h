#ifndef CVECTOR_H
#define CVECTOR_H

class CVector
{
	public:
		CVector(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f)
		{
			m_Coords[0] = x;
			m_Coords[1] = y;
			m_Coords[2] = z;
			m_Coords[3] = w;
		}

		operator const float*() const
		{
			return m_Coords;
		}
		
		float x() const
		{
		    return m_Coords[0];
		}
		
		float y() const
		{
		    return m_Coords[1];
		}
		
		float z() const
		{
		    return m_Coords[2];
		}

		CVector operator+(const CVector &Vec) const
		{
			return CVector(
			    m_Coords[0] + Vec.m_Coords[0],
			    m_Coords[1] + Vec.m_Coords[1],
			    m_Coords[2] + Vec.m_Coords[2],
			    m_Coords[3] + Vec.m_Coords[3]);
		}
		
		CVector &operator+=(const CVector &Vec)
		{
            m_Coords[0] += Vec.m_Coords[0];
            m_Coords[1] += Vec.m_Coords[1];
            m_Coords[2] += Vec.m_Coords[2];
            m_Coords[3] += Vec.m_Coords[3];
            return *this;
		}

		CVector operator-(const CVector &Vec) const
		{
			return CVector(
			    m_Coords[0] - Vec.m_Coords[0],
			    m_Coords[1] - Vec.m_Coords[1],
			    m_Coords[2] - Vec.m_Coords[2],
			    m_Coords[3] - Vec.m_Coords[3]);
		}
		
		CVector &operator-=(const CVector &Vec)
		{
            m_Coords[0] -= Vec.m_Coords[0];
            m_Coords[1] -= Vec.m_Coords[1];
            m_Coords[2] -= Vec.m_Coords[2];
            m_Coords[3] -= Vec.m_Coords[3];
            return *this;
		}
		
		CVector operator-() const
		{
			return CVector(
			    -m_Coords[0],
			    -m_Coords[1],
			    -m_Coords[2],
			    -m_Coords[3]);
		}
		
		CVector operator*(float a) const
		{
			return CVector(
			    m_Coords[0] * a,
			    m_Coords[1] * a,
			    m_Coords[2] * a,
			    m_Coords[3] * a);
		}
		
		CVector &operator*=(float a)
		{
            m_Coords[0] *= a;
            m_Coords[1] *= a;
            m_Coords[2] *= a;
            m_Coords[3] *= a;
            return *this;
		}
		
		CVector operator/(float a) const
		{
			return CVector(
			    m_Coords[0] / a,
			    m_Coords[1] / a,
			    m_Coords[2] / a,
			    m_Coords[3] / a);
		}
		
		CVector &operator/=(float a)
		{
            m_Coords[0] /= a;
            m_Coords[1] /= a;
            m_Coords[2] /= a;
            m_Coords[3] /= a;
            return *this;
		}
		
		bool operator==(const CVector &Vec)
		{
            return
                m_Coords[0] == Vec.m_Coords[0] &&
                m_Coords[1] == Vec.m_Coords[1] &&
                m_Coords[2] == Vec.m_Coords[2] && 
                m_Coords[3] == Vec.m_Coords[3];
		}
		
		bool operator!=(const CVector &Vec)
		{
            return !(*this == Vec);
		}

		CVector cross(const CVector &Vec) const
		{
		    return CVector(
			    m_Coords[1] * Vec.m_Coords[2] - m_Coords[2] * Vec.m_Coords[1],
			    m_Coords[2] * Vec.m_Coords[0] - m_Coords[0] * Vec.m_Coords[2],
			    m_Coords[0] * Vec.m_Coords[1] - m_Coords[1] * Vec.m_Coords[0],
			    0.0f);
		}

		float dot(const CVector &Vec) const
		{
			return
			    m_Coords[0] * Vec.m_Coords[0] +
			    m_Coords[1] * Vec.m_Coords[1] +
			    m_Coords[2] * Vec.m_Coords[2] +
			    m_Coords[3] * Vec.m_Coords[3];
		}

		float length() const
		{
			return sqrtf(dot(*this));
		}
		
		float length2() const
		{
			return dot(*this);
		}

		CVector normalize() const
		{
			float fLen = length();
			if(fLen == 0.0f)
			    return *this;
			return (*this / fLen);
		}
		
		void setLength(float fLen)
		{
		    float fOldLen = length();
		    if(fOldLen > 0.0f)
		        *this *= fLen / fOldLen;
		}

	private:
		float m_Coords[4];
};

#endif // CVECTOR_H
