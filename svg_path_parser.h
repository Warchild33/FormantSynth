#ifndef SVG_PATH_PARSER_H
#define SVG_PATH_PARSER_H

#include <QPainterPath>
#include <QString>

bool parsePathDataFast(const QString &dataStr, QPainterPath &path);

#endif // SVG_PATH_PARSER_H
