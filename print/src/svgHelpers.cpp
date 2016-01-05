// svgHelpers.cpp

#include "svgHelpers.h"
#include "HorizontalLayout.h"

#include <iomanip>
using std::setprecision;
using std::fixed;

#include <string>
using std::to_string;

#include <cmath>

void svgRect(ostream& os, string svgClass, double x, double y, double radius, double width, double height)
{
    os << "    <rect class=\"" << svgClass << "\" x=\"" << (int)x << "\" y=\"" << (int)y << "\" rx=\"" << (int)radius << "\" ry=\"" << (int)radius << "\" width=\"" << (int)width << "\" height=\"" << (int)height << "\"/>\n";
}

void svgCircle(ostream& os, string svgClass, double cx, double cy, double radius)
{
    os << "    <ellipse class = \"" << svgClass << "\" cx = \"" << (int)cx << "\" cy = \"" << (int)cy << "\" rx = \"" << (int)radius << "\" ry = \"" << (int)radius << "\" />\n";
}

void svgText(ostream& os, string text, string svgClass, double cx, double y, bool vertical)
{
    os << "    <text class=\"" << svgClass << "\" x=\"" << cx << "\" y=\"" << y << "\" text-anchor=\"middle\" dy=\".4em\"";
    if (vertical)
    {
        os << " transform=\"rotate(-90, " << cx << ", " << y << ")\"";
    }
    os << ">" << text << "</text>\n";
}

void svgText(ostream& os, double text, int precision, string svgClass, double cx, double y)
{
    os << "    <text class=\"" << svgClass << "\" x=\"" << cx << "\" y=\"" << y << "\" text-anchor=\"middle\" dy=\".4em\"" << ">" << fixed << setprecision(precision) << text << "</text>\n";
}

void svgUse(ostream& os, string href, double x, double y)
{
    os << "    <use xlink:href=\"#" << href << "\" x=\"" << x << "\" y=\"" << y << "\" />\n";
}

void svgDots(ostream& os, double cx, double cy)
{
    svgCircle(os, "Dots", cx-8, cy, 2);
    svgCircle(os, "Dots", cx, cy, 2);
    svgCircle(os, "Dots", cx+8, cy, 2);
}

void svgEdge(ostream & os, Point from, Point to, double edgeFlattness)
{
    double xDist = to.x - from.x;
    double yDist = to.y - from.y;

    if (xDist == 0)
    {
        os << "    <path class=\"Edge\" d=\"M " << from.x << " " << from.y << " L " << to.x << " " << to.y << "\"/>\n";
    }

    else
    {
        const double drop = yDist*edgeFlattness / 2.0;
        double qFrac = drop / xDist;
        if (qFrac < 0) qFrac = -qFrac;
        if (qFrac > 0.25) qFrac = 0.25;
        double slopeFrac = 1 - 2 * qFrac;

        const double slopeDy = yDist - 2.0 * drop;

        os << "    <path class=\"Edge\" d=\"M " << from.x << " " << from.y << " q 0 " << drop << " " << xDist*qFrac << " " << drop+slopeDy*qFrac << " l " << xDist*slopeFrac << " " << slopeDy*slopeFrac << " q " << xDist*qFrac << " " << slopeDy*qFrac << " " << xDist*qFrac << " " << drop+slopeDy*qFrac << "\"/>\n";
    }
}

int GetPrecision(double value, int maxChars)
{
    int precision = 0;
    if(value >= 1)
    {
        precision = maxChars - 1 -(int)ceil(log10(value));   // save one char for "."
    }
    else if(value < 1 && value >= 0)
    {
        precision = maxChars-2;   // save two characters for "0."
    }
    else if(value < 0 && value > -1)
    {
        precision = maxChars - 3;   // save three chars for "-0."
    }
    else
    {
        precision = maxChars - 2 - (int)ceil(log10(-value));   // save two chars for "-" and "."
    }
    if(precision < 0)
    {
        precision = 0;
    }

    return precision;
}

string svgDefineElement(ostream& os, void* uid, ElementStyleArgs styleArgs)
{
    string typeName = "Element" + to_string((uint64)uid);
    os << "<defs>\n<g id = \"" << typeName << "\">\n";
    svgCircle(os, "Connector", styleArgs.width/2.0, 0, styleArgs.connectorRadius);
    svgCircle(os, "Connector", styleArgs.width/2.0, styleArgs.height, styleArgs.connectorRadius);
    svgRect(os, "Element", 0, 0, styleArgs.cornerRadius, styleArgs.width, styleArgs.height);
    os << "</g>\n</defs>\n\n";
    return typeName;
}
