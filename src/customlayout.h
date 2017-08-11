#ifndef CUSTOMLAYOUT_H
#define CUSTOMLAYOUT_H

#include "main.h"


/**
 * @brief Structure holding the config of a custom layout
 */
struct CustomLayout {
public:
    short rows = 4;
    short cols = 6;
    short maxRows = 2;
    short maxCols = 2;
    short minRows = 1;
    short minCols = 1;

    bool perFolder = false;

    bool mainEnabled = true;
    short mainRows = 2;
    short mainCols = 4;
    UM::ALIGN mainPos = UM::ALIGN_CENTER_CENTER;

    bool borderEnabled = true;
    short borderWidth = 10;
    QRgb borderColor = Qt::black;

    bool shadowEnabled = false;
    short shadowWidth = 10;
    QRgb shadowColor = Qt::black;

    bool variationEnabled = false;
    double angleVariation = 0.25;
    double sizeVariation = 0.25;
    double posVariation = 0.25;

    CustomLayout() {}

    CustomLayout(const QDomElement* _dom)
    {
        rows = _dom->attribute("rows").toShort();
        cols = _dom->attribute("cols").toShort();
        minRows = _dom->attribute("minRows").toShort();
        minCols = _dom->attribute("minCols").toShort();
        maxRows = _dom->attribute("maxRows").toShort();
        maxCols = _dom->attribute("maxCols").toShort();
        perFolder = QVariant(_dom->attribute("perFolder")).toBool();

        // attributes (before 2.2.1)
        if (_dom->hasAttribute("mainEnabled"))
        {
            mainEnabled = (bool) _dom->attribute("mainEnabled").toInt();
            mainRows = _dom->attribute("mainRows").toShort();
            mainCols = _dom->attribute("mainCols").toShort();
            mainPos = static_cast<UM::ALIGN>(_dom->attribute("mainPos").toInt());
            borderEnabled = (bool) _dom->attribute("borderEnabled").toInt();
            borderWidth = _dom->attribute("borderWidth").toShort();
            borderColor = _dom->attribute("borderColor").toUInt();
        }

        // inner nodes (since 2.2.1)
        QDomNode node = _dom->firstChild();

        if (!node.isNull())
        {
            borderEnabled = false;
            mainEnabled = false;
            shadowEnabled = false;
            variationEnabled = false;
        }

        while (!node.isNull())
        {
            if (node.isElement())
            {
                 QDomElement element = node.toElement();

                 if (element.tagName() == "main")
                 {
                     mainEnabled = true;
                     mainRows = element.attribute("rows").toShort();
                     mainCols = element.attribute("cols").toShort();
                     mainPos = static_cast<UM::ALIGN>(element.attribute("pos").toInt());
                 }
                 else if (element.tagName() == "border")
                 {
                     borderEnabled = true;
                     borderWidth = element.attribute("width").toShort();
                     borderColor = QColor(element.attribute("color")).rgb();
                 }
                 else if (element.tagName() == "shadow")
                 {
                     shadowEnabled = true;
                     shadowWidth = element.attribute("width").toShort();
                     shadowColor = QColor(element.attribute("color")).rgb();
                 }
                 else if (element.tagName() == "variation")
                 {
                     variationEnabled = true;
                     angleVariation = element.attribute("angle").toDouble();
                     sizeVariation = element.attribute("size").toDouble();
                     posVariation = element.attribute("pos").toDouble();
                 }
            }

            node = node.nextSibling();
        }
    }

    void writeXml(QXmlStreamWriter* _writer) const
    {
        _writer->writeStartElement("customLayout");

        _writer->writeAttribute("rows", QString::number(rows));
        _writer->writeAttribute("cols", QString::number(cols));
        _writer->writeAttribute("minRows", QString::number(minRows));
        _writer->writeAttribute("minCols", QString::number(minCols));
        _writer->writeAttribute("maxRows", QString::number(maxRows));
        _writer->writeAttribute("maxCols", QString::number(maxCols));
        _writer->writeAttribute("perFolder", QVariant(perFolder).toString());

        if (mainEnabled)
        {
            _writer->writeStartElement("main");
            _writer->writeAttribute("rows", QString::number(mainRows));
            _writer->writeAttribute("cols", QString::number(mainCols));
            _writer->writeAttribute("pos", QString::number(mainPos));
            _writer->writeEndElement();
        }

        if (borderEnabled)
        {
            _writer->writeStartElement("border");
            _writer->writeAttribute("width", QString::number(borderWidth));
            _writer->writeAttribute("color", QColor(borderColor).name());
            _writer->writeEndElement();
        }

        if (shadowEnabled)
        {
            _writer->writeStartElement("shadow");
            _writer->writeAttribute("width", QString::number(shadowWidth));
            _writer->writeAttribute("color", QColor(shadowColor).name());
            _writer->writeEndElement();
        }

        if (variationEnabled)
        {
            _writer->writeStartElement("variation");
            _writer->writeAttribute("angle", QString::number(angleVariation));
            _writer->writeAttribute("size", QString::number(sizeVariation));
            _writer->writeAttribute("pos", QString::number(posVariation));
            _writer->writeEndElement();
        }

        _writer->writeEndElement();
    }
};

#endif // CUSTOMLAYOUT_H

