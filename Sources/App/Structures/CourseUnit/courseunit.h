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
#define COURSE_UNIT_FILE_EXTENSION ".CourseUnit"
//-----------------------------------------------

class CourseUnit : public QObject
{
    Q_OBJECT

private:

    // Fields
    //------------------------------------------
    size_t width = 50;
    size_t height = 50;
    size_t x = 0;
    size_t y = 0;
    int colour;
    QVector<std::pair<QString, size_t>> income;
    QVector<std::pair<QString, size_t>> outcome;
    QString description = "";
    QVector<QString> linked_units;
    QVector<CourseUnit*> embedded_units;
    QString lastFilePath;
    size_t field_width = 100;
    size_t field_height = 80;
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


    void setCoords(size_t x, size_t y);
    std::pair<size_t, size_t> getCoords() const;

    void setColour(int colour_);
    int  getColour();

    void addIncome(const std::pair<QString, size_t>&);
    void addOutcome(const std::pair<QString, size_t>&);

    const QVector<std::pair<QString, size_t>>& getIncome() const;
    const QVector<std::pair<QString, size_t>>& getOutcome() const;

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
