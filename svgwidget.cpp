#include <QPainter>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include "svgwidget.h"

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
    //setPathStyleDOM("path4232",Qt::yellow, 0.5);
    return true;
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
    return svg_rend.load(domDocument.toByteArray());
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
void SvgWidget::setPathStyleDOM(QString id, QColor fill_color, float fill_opacity)
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
