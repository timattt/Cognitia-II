#ifndef CourseUnit_H
#define CourseUnit_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QFile>
#include <QDir>
#include <QFileInfo>

// Constants
//-----------------------------------------------
#define UNITSEPARATOR "|~^|"
#define FIELDSSEPARATOR "!(!"
#define VECTORSEPARATOR "?(?"
#include "../fileExtensions.h"
//-----------------------------------------------

class CourseUnit : public QObject
{
    Q_OBJECT

private:

    // Fields
    //------------------------------------------
    size_t width;
    size_t height;
    double x;
    double y;
    int colour;
    QMap<QString, size_t> income;
    QMap<QString, size_t> outcome;
    QString description;
    QVector<QString> linked_units;
    QVector<CourseUnit*> embedded_units;
    QString lastFilePath;
    size_t field_width;
    size_t field_height;
    //------------------------------------------

public:
    // Constructor
    //------------------------------------
    CourseUnit(QObject* pobj = nullptr);
    CourseUnit(size_t width, size_t height, size_t x, size_t y, QObject* pobj = nullptr);
    //------------------------------------------------------------------------------------
    CourseUnit(const CourseUnit&) = default;
    CourseUnit( CourseUnit&&) = default;
   // ~CourseUnit();


    // Methods
    //---------------------------------------------------------------------------
    void loadCourseUnit(QFile *res);
    void saveCourseUnit(QFile *dest);

    void setSize(size_t width, size_t height);
    std::pair<size_t, size_t> getSize() const;   //first - width, second - height

    void setFieldSize(size_t width, size_t height);
    std::pair<size_t, size_t> getFieldSize() const; //first - width, second - height


    void setCoords(double x, double y);
    std::pair<double, double> getCoords() const;

    void setColour(int colour_);
    int  getColour();

    void addIncome(const QString& skill, size_t lvl);
    void addOutcome(const QString& skill, size_t lvl);

    const QMap<QString, size_t>& getIncome() const;
    const QMap<QString, size_t>& getOutcome() const;

    size_t getInSkillLevel(QString sk);
    size_t getOutSkillLevel(QString sk);

    bool containsInSkill(QString name);
    bool containsOutSkill(QString name);

    void setDescription(const QString&);
    const QString& getDescription() const;

    void addConnection(CourseUnit*);
    void addConnection(const QString& name);
    const QVector<QString>& getConnections() const;

    void addEmbedded(CourseUnit*);
    const QVector<CourseUnit*>& getEmbedded() const;

    QString print();
    //----------------------------------------------------------------------------
	QString getLastFilePath() const;
	void setLastFilePath(QString lastFilePath);
};

#endif // CourseUnit_H
