#include "segyreader.h"

SegYReader::SegYReader() {}

SegYReader::SegYReader(const QString& filePath) : filePath(filePath) {}

bool SegYReader::readFile() {
    QFile file(filePath);
    if (QFileInfo(file).suffix() != "sgy") {
        qDebug() << file.fileName() << " not segy";
        return false;
    }
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Could not open file: " << filePath.toStdString() << std::endl;
        return false;
    }

    file.seek(3600); // Skip the textual file header (3600 bytes)

    readTraceHeader(file);

    return true;
}

void SegYReader::readPathWithSegy(QDir path)
{
    for (QString i: path.entryList()) {
        setFilePath(path.path()+"/"+i);
        readFile();
    }
}

void SegYReader::setFilePath(const QString &newFilePath)
{
    filePath = newFilePath;
}

void SegYReader::readTraceHeader(QFile& file) {
    // Assuming each trace header is 240 bytes long
    QByteArray traceHeaderData = file.read(240);

    if (traceHeaderData.size() < 240) {
        return; // Not enough data
    }

    // Use QByteArray to interpret the bytes
    const uchar* data = reinterpret_cast<const uchar*>(traceHeaderData.data());

    // Extracting FFID from bytes 9-12 (4 bytes, UI4)
    // uint32_t ffid = (data[12] << 24) | (data[13] << 16) | (data[14] << 8) | data[15];
    uint32_t ffid = data[9] << 16 | data[10] << 8 | data[11];

    uint16_t hours = data[161];
    uint16_t minutes = data[163];
    uint16_t seconds = data[165];

    uint32_t microseconds = (data[233] << 16) | (data[234]) << 8 | data[235];
    ffids.push_back(ffid);
    times.push_back(QTime(hours, minutes, seconds, microseconds/1000));
    pairs.push_back(Pair(ffid, QTime(hours, minutes, seconds, microseconds/1000)));

    // qDebug() << ffid << hours << minutes << seconds << microseconds;
    file.close();
}