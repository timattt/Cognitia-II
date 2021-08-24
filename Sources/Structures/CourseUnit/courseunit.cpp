#include "courseunit.h"

// Constants
//-----------------------------------------------
#define UNITSEPARATOR "|^|"
#define FIELDSSEPARATOR "!!"
#define VECTORSEPARATOR "??"
#define COURSE_UNIT_FILE_EXTENSION ".courseunit"
//-----------------------------------------------

courseUnit::courseUnit(QObject* pobj): QObject(pobj), income(), outcome(), embedded_units(), linked_units(){

}

courseUnit::courseUnit(size_t width, size_t height, size_t x, size_t y, QObject *pobj): QObject(pobj),
                                                                                        height(height),
                                                                                        x(x), y(y),
                                                                                        width(width),
                                                                                        income(),
                                                                                        outcome(),
                                                                                        embedded_units(),
                                                                                        linked_units()
{

}

void courseUnit::saveCourseUnit(QFile *dest){
     qDebug() << "Saving courseUnit" << objectName();

     QFileInfo in = QFileInfo(*dest);
     QDir dr = in.dir();

     if (!dr.exists()) {
         dr.mkpath(dr.absolutePath());
     }

     if (dest->open(QIODevice::WriteOnly)) {
         QTextStream stream(dest);

         stream << objectName() << FIELDSSEPARATOR;
         stream << width << FIELDSSEPARATOR;
         stream << height << FIELDSSEPARATOR;
         stream << x << FIELDSSEPARATOR;
         stream << y << FIELDSSEPARATOR;

         for(int i = 0; i < income.size(); ++i){
             stream << income[i].first << ":" << income[i].second << VECTORSEPARATOR;
         }
         stream << FIELDSSEPARATOR;

         for(int i = 0; i < outcome.size(); ++i){
             stream << outcome[i].first << ":" << outcome[i].second << VECTORSEPARATOR;
         }
         stream << FIELDSSEPARATOR;

         stream << description << FIELDSSEPARATOR;

         for(int i = 0; i < linked_units.size(); ++i){
             stream << linked_units[i] << VECTORSEPARATOR;
         }

         stream << UNITSEPARATOR;

         for (int i = 0; i < embedded_units.size(); ++i) {
             QString courseUnitFileName = embedded_units[i]->objectName() + COURSE_UNIT_FILE_EXTENSION;

             QFile courseUnitFile = QFile(dr.filePath(courseUnitFileName));

             try {
                 embedded_units[i]->saveCourseUnit(&courseUnitFile);
             } catch (QString err) {
                 throw QString("Saving [" + embedded_units[i] -> objectName() + "] with error: " + err);
             }

             stream << courseUnitFileName << UNITSEPARATOR;
         }

         dest->close();
     } else {
         throw QString("Can't open course file [" + dest->fileName() + "]");
     }

}


void courseUnit::loadCourseUnit(QFile *res){
    if (!res->exists()) {
        throw QString("courseUnit file [" + res->fileName() + "] not exists");
    }

    QFileInfo info = QFileInfo(*res);

    qDebug() << "Loading courseUnit from" << info.fileName();

    QString data = nullptr;


    if (res->open(QIODevice::ReadOnly)) {
        QTextStream stream(res);
        data = stream.readAll();
        res->close();
    } else {
        throw QString("Can't open courseUnit file [" + res->fileName() + "]");
    }

    QStringList unit_data = data.split(UNITSEPARATOR, Qt::SkipEmptyParts);


    if (unit_data.empty()) {
        throw QString("Empty courseUnit [" + res->fileName() + "]");
    }

    QStringList fields_data = unit_data[0].split(FIELDSSEPARATOR, Qt::SkipEmptyParts);
    this->setObjectName(fields_data[0]);

    width = fields_data[1].toULong();
    height = fields_data[2].toULong();
    x = fields_data[3].toULong();
    y = fields_data[4].toULong();

    QStringList vector_data = fields_data[5].split(VECTORSEPARATOR, Qt::SkipEmptyParts);
    for(int i = 0; i < vector_data.size(); ++i){
        QStringList pairs = vector_data[i].split(":");
        income.push_back(std::make_pair(pairs[0], pairs[1].toULongLong()));
    }

    vector_data = fields_data[6].split(VECTORSEPARATOR, Qt::SkipEmptyParts);
    for(int i = 0; i < vector_data.size(); ++i){
        QStringList pairs = vector_data[i].split(":");
        outcome.push_back(std::make_pair(pairs[0], pairs[1].toULongLong()));
    }

    description = fields_data[7];

    vector_data = fields_data[8].split(VECTORSEPARATOR, Qt::SkipEmptyParts);
    for(int i = 0; i < vector_data.size(); ++i){
        linked_units.push_back(vector_data[i]);
    }


    embedded_units.reserve(unit_data.size() - 1);

    QDir dr = info.dir();

    for (int i = 1; i < unit_data.size(); i++) {
        QString courseUnitFileName = unit_data[i];
        QFile courseUnitFile = dr.filePath(courseUnitFileName);
        courseUnit *unit = new courseUnit(this);
        try {
            unit->loadCourseUnit(&courseUnitFile);
        } catch (QString err) {
            throw QString("Error while loading courseUnit [" + courseUnitFileName + "] with error: " + err);
        }

        embedded_units.push_back(unit);
    }
}


void courseUnit::setSize(size_t new_width, size_t new_height){
    width = new_width;
    height = new_height;
}

std::pair<size_t, size_t> courseUnit::getSize() const{
    return std::make_pair(width, height);
}

void courseUnit::setCoords(size_t new_x, size_t new_y){
    x = new_x;
    y = new_y;
}

std::pair<size_t, size_t> courseUnit::getCoords() const{
    return std::make_pair(x, y);
}


void courseUnit::addIncome(const std::pair<QString, size_t>& skill){
    income.push_back(skill);
}

void courseUnit::addOutcome(const std::pair<QString, size_t>& skill){
    outcome.push_back(skill);
}

const QVector<std::pair<QString, size_t>>& courseUnit::getIncome() const{
    return income;
}

const QVector<std::pair<QString, size_t>>& courseUnit::getOutcome() const{
    return outcome;
}

void courseUnit::setDescription(const QString& desc){
    description = desc;
}

const QString& courseUnit::getDescription() const{
    return description;
}

const QVector<QString>& courseUnit::getConnections() const{
    return linked_units;
}

const QVector<courseUnit*>& courseUnit::getEmbedded() const{
    return embedded_units;
}


void courseUnit::addConnection(courseUnit* unit){
    linked_units.push_back(unit -> objectName());
}

void courseUnit::addConnection(const QString& objname){
    linked_units.push_back(objname);
}


void courseUnit::addEmbedded(courseUnit* unit){
    embedded_units.push_back(unit);
}










