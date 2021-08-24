#ifndef COURSEUNIT_H
#define COURSEUNIT_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QFile>
#include <QDir>
#include <QFileInfo>

class courseUnit : public QObject
{
    Q_OBJECT

private:

    // Fields
    //------------------------------------------
    size_t width = 50;
    size_t height = 50;
    size_t x = 0;
    size_t y = 0;
    QVector<std::pair<QString, size_t>> income;
    QVector<std::pair<QString, size_t>> outcome;
    QString description = "";
    QVector<QString> linked_units;
    QVector<courseUnit*> embedded_units;
    //------------------------------------------

public:
    // Constructor
    //------------------------------------
    courseUnit(QObject* pobj = nullptr);
    courseUnit(size_t width, size_t height, size_t x, size_t y, QObject* pobj = nullptr);
    //------------------------------------------------------------------------------------
    courseUnit(const courseUnit&) = default;
    courseUnit( courseUnit&&) = default;
   // ~courseUnit();


    // Methods
    //---------------------------------------------------------------------------
    void loadCourseUnit(QFile *res);
    void saveCourseUnit(QFile *dest);

    void setSize(size_t width, size_t height);
    std::pair<size_t, size_t> getSize() const;   //first - width, second - height

    void setCoords(size_t x, size_t y);
    std::pair<size_t, size_t> getCoords() const;

    void addIncome(const std::pair<QString, size_t>&);
    void addOutcome(const std::pair<QString, size_t>&);

    const QVector<std::pair<QString, size_t>>& getIncome() const;
    const QVector<std::pair<QString, size_t>>& getOutcome() const;

    void setDescription(const QString&);
    const QString& getDescription() const;

    void addConnection(courseUnit*);
    void addConnection(const QString& name);
    const QVector<QString>& getConnections() const;

    void addEmbedded(courseUnit*);
    const QVector<courseUnit*>& getEmbedded() const;

    QString print();
    //----------------------------------------------------------------------------

};

#endif // COURSEUNIT_H
