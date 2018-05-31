#include <QPainter>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include "svgwidget.h"
#include "svg_path_parser.h"


SvgWidget::SvgWidget(QWidget *parent):
QWidget(parent)
{
}

//----сделать замены в SVG
//запоминает строки в которых уже делались замены
void SvgWidget::doReplace()
{
   QMap<QString, QString>::iterator it;

   for(it=replaceMap.begin(); it!=replaceMap.end(); it++)
   {
       if( knownStringIndexs[it.key()].size() == 0 )
       {
           int sub_index = 0;
           while(sub_index!=-1)
           {
              sub_index = svg.indexOf(it.key(), sub_index);
              if( sub_index!=-1 )
                 knownStringIndexs[it.key()].push_back(sub_index);
              svg.replace(sub_index,it.key().size(), it.value());
           }

       }
       else
       {
           QList<int>& replace_index_list = knownStringIndexs[it.key()];
           QList<int>::iterator it2;
           for(it2 = replace_index_list.begin(); it2!=replace_index_list.end(); it2++)
              svg.replace((*it2),it.key().size(), it.value());
       }
   }
}

//загружает DOM документ из SVG файла
bool SvgWidget::LoadDom(QString fn)
{
    QString errorStr;
    int errorLine;
    int errorColumn;

    QFile f(fn);
    if( !f.open( QFile::ReadOnly ) )
    {
        qDebug() << "Cannot open file " + fn;
        return false;
    }

    QTextStream ts(&f);
    QString xml;

    xml = ts.readAll();

    if (!domDocument.setContent(xml, true, &errorStr, &errorLine, &errorColumn))
    {
        qDebug() << "Parse error at line" + QString::number(errorLine) + "column " +  QString::number(errorColumn) + errorStr;
        return false;
    }
    QDomNode root = domDocument.documentElement();
    TraverseXmlNode(root);
    setViewBox(root);
    return true;
}

// задает viewbox
void SvgWidget::setViewBox(const QDomNode& root)
{
    QStringList parts = root.toElement().attribute("viewBox").split(" ");
    viewbox = QRectF(parts[0].toDouble(), parts[1].toDouble(),
                     parts[2].toDouble(), parts[3].toDouble());

}

// рекурсивный проход по дереву xml
// с сохранением элементов с аттрибутом id в таблицу
void SvgWidget::TraverseXmlNode(const QDomNode& node)
{
    QDomNode domNode = node.firstChild();
    QDomElement domElement;
    while(!(domNode.isNull()))
    {
        if(domNode.isElement())
        {
            domElement = domNode.toElement();
            if(!(domElement.isNull()))
            {
                //qDebug() << __FUNCTION__ << "" << domElement.tagName() << domElement.text();
                //qDebug() << domElement.attribute("id");
                elementByID[domElement.attribute("id")] = domElement;
                QString name = domNode.nodeName();
                elementByName[name] = domElement;
            }

        }
        TraverseXmlNode(domNode);
        domNode = domNode.nextSibling();
    }
}

//сохраняет DOM документ в SVG файл
//TODO: удалить сгенеренные аттрибуты xmlns
bool SvgWidget::SaveDom(QString fn)
{
    QFile f(fn);
    if( !f.open( QFile::WriteOnly ) )
    {
        qDebug() << "Cannot open file " + fn;
        return false;
    }
    QTextStream ts(&f);
    domDocument.save(ts, 0);
    return true;
}

//загружает SVG рендер из DOM документа
bool SvgWidget::LoadRenderDOM()
{
    return svg_rend.load(domDocument.toByteArray(-1));
}

// загружает SVG рендер
bool SvgWidget::LoadRender()
{
    return svg_rend.load(svg.toUtf8());
}

QString changeAttr(QString attr_string, QString attr_key, QString attr_val)
{
    QStringList params_list = attr_string.split(";");
    for(auto i = params_list.begin(); i!= params_list.end(); i++ )
    {
        QStringList key_value = (*i).split(":");
        if( attr_key == key_value[0] )
            (*i) = key_value[0] + ":" + attr_val;
    }
    return params_list.join(";");
}
//;fill:#ffffff;fill-opacity:1;

//находит path с заданным id и выставляет аттрибуты заливки
void SvgWidget::setFill(QString id, QColor fill_color, float fill_opacity)
{
    QDomElement path_node;
    QMap<QString, QDomElement>::iterator it;
    it = elementByID.find(id);
    if( it!=elementByID.end() )
    {
       path_node = it.value();
       QString style = path_node.attribute("style");
       style = changeAttr(style, "fill", fill_color.name());
       style = changeAttr(style, "fill-opacity", QString::number(fill_opacity));
       path_node.setAttribute("style",style);
       //qDebug() << "change style " << path_node.attribute("style") << "\n";
       //qDebug() << val;
    }
}

void SvgWidget::setStroke(QString id, QColor stroke_color)
{
    QDomElement path_node;
    QMap<QString, QDomElement>::iterator it;
    it = elementByID.find(id);
    if( it!=elementByID.end() )
    {
       path_node = it.value();
       QString style = path_node.attribute("style");
       style = changeAttr(style, "stroke", stroke_color.name());
       path_node.setAttribute("style",style);
    }
}

QRectF SvgWidget::getEllipse(QString id)
{
    QDomElement path_node;
    QMap<QString, QDomElement>::iterator it;
    QRectF rect;
    it = elementByID.find(id);
    if( it!=elementByID.end() )
    {
       path_node = it.value();
       double cx = path_node.attribute("cx").toDouble();
       double cy = path_node.attribute("cy").toDouble();
       double rx = path_node.attribute("rx").toDouble();
       double ry = path_node.attribute("ry").toDouble();
       rect.setTop(cy-ry/2);
       rect.setLeft(cx-rx/2);
       rect.setRight(cx+rx/2);
       rect.setBottom(cy+ry/2);
    }
    return rect;
}


void SvgWidget::setText(QString group_id, QString text)
{
    QDomElement group_node;
    QMap<QString, QDomElement>::iterator it;
    it = elementByID.find(group_id);
    if( it!=elementByID.end() )
    {
       group_node = it.value();
       QDomNode e = group_node.firstChild();
       fprintf(stderr,"%s", e.nodeName().toStdString().c_str());
       if(e.nodeName()=="rect")
           e = e.nextSibling();
       if(e.nodeName() == "text")
       {
           QDomNode parent = e;
           e = e.firstChild();
           fprintf(stderr,"%s", e.nodeName().toStdString().c_str());
           e.setNodeValue(text);
           QDomText newNode = domDocument.createTextNode(text);
           parent.replaceChild(newNode,e);

       }

    }
}

QPointF SvgWidget::findPathCenter(QString id)
{
    QDomElement node;
    QMap<QString, QDomElement>::iterator it;
    it = elementByID.find(id);
    if( it!=elementByID.end() )
    {
       node = it.value();
       QString d = node.attribute("d");
       QPainterPath path;
       parsePathDataFast(d, path);
       return path.boundingRect().center();
    }
}

QPainterPath SvgWidget::getPath(QString id)
{
    QDomElement node;
    QPainterPath path;
    QMap<QString, QDomElement>::iterator it;
    it = elementByID.find(id);
    if( it!=elementByID.end() )
    {
       node = it.value();
       QString d = node.attribute("d");
       parsePathDataFast(d, path);
    }
    return path;
}

QRectF SvgWidget::getRect(QString id)
{
    QDomElement node;
    QPainterPath path;
    QMap<QString, QDomElement>::iterator it;
    QRectF rect;
    it = elementByID.find(id);
    if( it!=elementByID.end() )
    {
       node = it.value();
       double x = node.attribute("x").toDouble();
       double y = node.attribute("y").toDouble();
       double w = node.attribute("width").toDouble();
       double h = node.attribute("height").toDouble();
       rect.setRect(x,y,w,h);
    }
    return rect;
}

QPointF  SvgWidget::mapToViewbox(QPoint p, QRectF viewbox)
{
    QPointF pt(p.x(), p.y());
    float sx = (float)viewbox.width() / (float)geometry().width();
    float sy = (float)viewbox.height() / (float)geometry().height();
    QTransform t;
    t = t.scale(sx, sy);
    pt = pt * t;
    return pt;
}

QRectF  SvgWidget::mapToViewbox(QRectF p, QRectF viewbox)
{
    float sx = (float)viewbox.width() / (float)geometry().width();
    float sy = (float)viewbox.height() / (float)geometry().height();
    QTransform t;
    t = t.scale(1./sx, 1./sy);
    QPolygonF p2(p);
    p2 = t.map(p2);
    p  = p2.boundingRect();
    return p;
}


void SvgWidget::rotateNode(QString id, QPointF center, float degree)
{
    QDomElement node;
    QMap<QString, QDomElement>::iterator it;
    it = elementByID.find(id);
    if( it!=elementByID.end() )
    {
       node = it.value();
       QTransform t, ti;
       ti.translate(-center.x(), -center.y());
       t.translate(center.x(), center.y());
       QTransform R;
       R = R.rotate(degree);
       t = (ti * R) * t;
       QString matrixs = QString("matrix(%1,%2,%3,%4,%5,%6)")
               .arg(t.m11()).arg(t.m21()).arg(t.m12())
               .arg(t.m22()).arg(t.m13()).arg(t.m23());

       //node.setAttribute("transform",matrixs);
       matrixs = QString("rotate(%1 %2 %3)").
               arg(degree).
               arg(center.x()).
               arg(center.y());
       node.setAttribute("transform",matrixs);
    }
}

void SvgWidget::rotateNode(QString id, float degree)
{
    QDomElement node;
    QMap<QString, QDomElement>::iterator it;
    it = elementByID.find(id);
    if( it!=elementByID.end() )
    {
       node = it.value();
       QString transform = node.attribute("transform");
       QRegExp rx("matrix(\\S+)");
       if(rx.indexIn(transform)!=-1)
       {
          double m11, m12, m13;
          double m21, m22, m23;
          //t.setMatrix();
          QString trimmed = rx.cap(1).mid(1,rx.cap(1).size()-2);
          QStringList reals = trimmed.split(",");
          m11 = reals[0].toDouble();
          m12 = reals[1].toDouble();
          m13 = reals[2].toDouble();
          m21 = reals[3].toDouble();
          m22 = reals[4].toDouble();
          m23 = reals[5].toDouble();
          QTransform t;
          t.setMatrix(m11,m12,m13,m21,m22,m23,0,0,1);
          QTransform ti = t.inverted();
          QTransform R;
          R = R.rotate(45);
          t = (ti * R) * t;
          QString matrixs = QString("matrix(%1,%2,%3,%4,%5,%6)")
                  .arg(t.m11()).arg(t.m12()).arg(t.m13()).arg(t.m21()).arg(t.m22()).arg(t.m23());
          node.setAttribute("transform",matrixs);

       }


       //node.setAttribute("transform",QString("rotate(%1)").
       //                                   arg(QString::number(degree)));

    }

}


// загрузка SVG
void SvgWidget::SvgLoad(QString svgPath)
{
    id_list.clear();
    svg = "";
    knownStringIndexs.clear();
    if( !svg_rend.load(svgPath) )
        qDebug() << "cannot load "  << svgPath ;
    QFile f(svgPath);
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "cannot open file " << svgPath;
        return;
    }
    QTextStream ts(&f);
    ts.setCodec("UTF-8");
    while( !ts.atEnd() )
    {
        QString line = ts.readLine();
        svg+=line;
        QRegExp rx("\\bid=\"([^\"]+)\\b");
        if(rx.indexIn(line)!=-1)
        {
           QString id = rx.cap(1);
           id_list.push_back( id );
        }

    }
    svg_original = svg;
}

// загрузить оригинальный SVG (не измененный)
void SvgWidget::LoadOriginal()
{
    svg = svg_original;
}

// обновить описание SVG
void SvgWidget::SvgSave(QString svgFileM)
{
    QFile f(svgFileM);
    if (!f.open(QFile::WriteOnly | QFile::Text | QFile::Truncate)) {
        qDebug() << "cannot open file for writing file " << svgFileM;
        return;
    }
    QTextStream ts(&f);
    ts.setCodec("UTF-8");
    ts << svg;
    f.close();
}

void SvgWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    svg_rend.render(&painter);
}
