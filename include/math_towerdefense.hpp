#pragma once
#include <cmath>
#include <iostream>
#include <string>

//=================================================================================
//                          Class

//Class
class vector2D
{
private:
public:
    float x;
    float y;

    vector2D()
    {
        x = 0;
        y = 0;
    }

    vector2D(float nX, float nY)
    {
        x = nX;
        y = nY;
    }

    vector2D(float xy)
    {
        x = xy;
        y = xy;
    }

    vector2D(const vector2D& veccopy)
    {
        x = veccopy.x;
        y = veccopy.y;
    }

    vector2D(const vector2D& vecA , const vector2D& vecB )
    {
        x = vecB.x - vecA.x;
        y = vecB.y - vecA.y;
    }

    ~vector2D()
    {

    }

    float X() const
    {
        return x;
    }

    float Y() const
    {
        return y;
    }

    float& X()
    {
        return x;
    }

    float& Y()
    {
        return y;
    }

    float dot_product(const vector2D& vec) const
    {
        float dot = vec.x * x + vec.y * y;
        return dot;
    }

    float VectorProduct(vector2D v1, vector2D v2)
    {
        return (v1.x * v2.y) - (v1.y * v2.x);
    }

    float cross_product(const vector2D& vec) const
    {
        float cross = x * vec.y + vec.x * y;

        return cross;
    }

    float length() const
    {
        return sqrt(x*x + y*y);
    }

    float DistanceCalcul(const vector2D& vec2) const
    {
        return sqrt( (vec2.x - x) * (vec2.x - x) + (vec2.y - y) * (vec2.y - y));
    }

    vector2D add(const vector2D& vec) 
    {
        vector2D res;
        res.x = x + vec.x;
        res.y = y + vec.y;
        return res;
    }

    void scale(float mult)
    {
        x *= mult;
        y *= mult;
    }

    vector2D scale(float mult) const
    {
        vector2D res;
        res.x = x*mult;
        res.y = y*mult;
        return res;
    }

    vector2D normalize() const
    {
        float size = this->length();
        vector2D res;
        res.x = x / size;
        res.y = y / size;
        return res;
    }

    void normalize() 
    {
        float size = this->length();
        x /= size;
        y /= size;
    }

    //Normal vector
    vector2D GetNormal()
    {
        vector2D normal = {  - y  , x};
        //Get the size to normalize the vector
        float size_normal = length();
        normal.x /= size_normal;
        normal.y /= size_normal;

        return normal;
    } 



//------------------------------OPERATOR--------------------------------------

//OPERATOR AFFICHAGE

    friend std::ostream& operator <<(std::ostream& os, const vector2D& vec)
    {
        return os << vec.x << "," << vec.y;
    }

    friend std::istream& operator >>(std::istream& is , vector2D& vec)
    {

        is >> vec.x ;

        std::string coma;
        is >> coma;

        if(coma ==  ",")
        {
            is >> vec.y;
        }
        else 
        {
            vec.x = 0;
            vec.y = 0;
        }

        return is;

    }

    //  Ajoutez les valeurs du vecteur a un string

    friend std::string operator+(std::string str, vector2D vec)
    {
        return str + std::to_string(vec.x) + " , " +  std::to_string(vec.y);
    }

    friend std::string operator+= (std::string& str, const vector2D& vec)
    {
        str += std::to_string(vec.x) + " , " + std::to_string(vec.y);
        return str;
    }

//OPERATOR COPY

    vector2D operator= (const vector2D& copy)
    {
        x = copy.x;
        y = copy.y;
        return *this;
    }

// OPERATOR CALCUL

    friend vector2D operator+= (vector2D& str, const vector2D& vec)
    {
        str.x += vec.x;
        str.y += vec.y;
        return str;
    }

    friend vector2D operator-= (vector2D& str, const vector2D& vec)
    {
        str.x -= vec.x;
        str.y -= vec.y;
        return str;
    }

    friend vector2D operator+ (const vector2D& str, const vector2D& vec)
    {
        vector2D res;
        res.x = str.x + vec.x;
        res.y = str.y + vec.y;
        return res;
    }

    friend vector2D operator- (const vector2D& str, const vector2D& vec)
    {
        vector2D res;
        res.x = str.x - vec.x;
        res.y = str.y - vec.y;
        return res;
    }

    friend vector2D operator* (const vector2D& str, const vector2D& vec)
    {
        vector2D res;
        res.x = str.x * vec.x;
        res.y = str.y * vec.y;
        return res;
    }

    friend vector2D operator/ (const vector2D& str, const vector2D& vec)
    {
        vector2D res;
        res.x = str.x / vec.x;
        res.y = str.y / vec.y;
        return res;
    }

    friend vector2D& operator*= (vector2D& str, const vector2D& vec)
    {
        str.x *= vec.x;
        str.y *= vec.y;
        return str;
    }

    friend vector2D& operator/= (vector2D& str, const vector2D& vec)
    {

        str.x /= vec.x;
        str.y /= vec.y;
        return str;
    }

    vector2D& operator++()
    {
        x *= (this->length() + 1) / this->length();
        y *= (this->length() + 1) / this->length();
        return *this;
    }

    vector2D operator++(int)
    {
        vector2D old = *this; 
        operator++();  
        return old;    
    }

    vector2D& operator--()
    {
        x *= (this->length() - 1) / this->length();
        y *= (this->length() - 1) / this->length();
        return *this;
    }

    vector2D operator--(int)
    {
        vector2D old = *this; 
        operator--();  
        return old;    
    }

    friend vector2D operator- ( const vector2D& vec)
    {
        vector2D res;
        res.x = -vec.x;
        res.y = -vec.y;
        return res;
    }

//  OPERATOR BOOL
    friend bool operator==(const vector2D& comp , const vector2D& comp2)
    {
        if(comp2.x == comp.x && comp2.y == comp.y)
        {
            return 1;
        }else return 0;
    }

    friend bool operator!=(const vector2D& comp , const vector2D& comp2)
    {
        return !(comp == comp2);
    }

    friend bool operator<=(const vector2D& comp , const vector2D& comp2)
    {
        if(comp.x <= comp2.x && comp.y <= comp2.y)
        {
            return 1;
        }else return 0;
    }

    friend bool operator<(const vector2D& comp , const vector2D& comp2)
    {
        if(comp.x < comp2.x && comp.y < comp2.y)
        {
            return 1;
        }else return 0;
    }

    friend bool operator>=(const vector2D& comp , const vector2D& comp2)
    {
        if(comp.x >= comp2.x && comp.y >= comp2.y)
        {
            return 1;
        }else return 0;
    }

    friend bool operator>(const vector2D& comp , const vector2D& comp2)
    {
        return !(comp<comp2);
    }


};

// Struct
struct m_Rectangle
{
    float x; // top left x position of rectangle
    float y; // top left y position of rectangle
    float width;
    float height;

    m_Rectangle()
    {
        x = 0;
        y = 0;
        width = 0;
        height = 0;
    }

    m_Rectangle(float nx , float ny , float nwidth, float nheight)
    {
        x = nx;
        y = ny;
        width = nwidth;
        height = nheight;
    }

    friend bool operator!=(const m_Rectangle& comp , const m_Rectangle& comp2)
    {
        bool fcompPos =  ((comp.x == comp.y) && (comp.y == comp2.y));
        bool fcompSize = ((comp.width == comp2.width) && (comp.height == comp2.height));

        if(fcompPos && fcompSize)
            return false;
        else 
            return true;
    }
} ;

//=================================================================================
//                          Fonction Auxiliaire                                     

// Check if enemy in range in range (return yes or no)
inline bool VectorCompareCircle(vector2D center, vector2D target, float range)
{
    vector2D dir(center, target);
    float dirMagnitude = dir.length();
    return dirMagnitude <= range ;

}

//Check if point is in rectangle
inline bool InRec(float mousePosX, float mousePosY, int x, int y, float width, float height)
{
    return 
        (mousePosX >= x) &&
        (mousePosX <= x + width) &&
        (mousePosY >= y) &&
        (mousePosY <= y + height)
        ;
}

inline double my_remap(double value, double inputStart, double inputEnd, double outputStart, double outputEnd)
{
    return (value - inputStart)/(inputEnd - inputStart)*(outputEnd - outputStart) + outputStart;
}

