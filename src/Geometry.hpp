#pragma once

namespace Geometry{
    class GObject;
    class GPoint;
    class GLine;
    class GBox;

    typedef struct tagGType{
        typedef enum tagen{
            POINT,
            LINE,
            BOX
        }en;
    }GType;

    class GObject
    {
    protected:
        GObject(GType::en type){ m_type = type;}
        GType::en m_type;
    public:
        ~GObject(){};
        const GType::en& Type(){ return m_type; }
    private:
        GObject();
    };

    class GBoundary{
    protected:
        GBoundary(){};
        float left;
        float right;
        float bottom;
        float top;
    public:
        virtual ~GBoundary(){};
        float const& Left() const{return left;}
        float const& Right() const{return right;}
        float const& Bottom() const{return bottom;}
        float const& Top() const{return top;}
        float const Width() const{return right - left;}
        float const Height() const{return top - bottom;}
    };
    class GPoint : public GObject
    {
    public:
        float X;
        float Y;
        GPoint(float x_, float y_) : GObject(GType::POINT), X( x_ ), Y( y_ ) {
        }
        GPoint() : GObject(GType::POINT), X( 0 ), Y( 0 ) {}
        ~GPoint(){};
    };
    class GLine : public GObject
    {
    public:
        GPoint p0;
        GPoint p1;
        GLine(const GPoint &p0_, const GPoint &p1_) : GObject(GType::LINE), p0( p0_ ), p1( p1_ ) {
        }
        ~GLine(){};
    private:
        GLine();
    };
    class GBox : public GObject
    {
    public:
        GPoint botLeft;
        GPoint topRight;
    public:
        GBox(float left, float bot, float right, float top)
            : GObject(GType::BOX), botLeft(GPoint(left, bot)), topRight(GPoint(right, top))
        {
        };
        GBox(const GPoint &botL, const GPoint &topR)
            : GObject(GType::BOX), botLeft(botL), topRight(topR)
        {
        };
        ~GBox(){};
    private:
        GBox();
    };
}