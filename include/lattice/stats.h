#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <Eigen/Dense>
#include <QDateTime>
#include <QObject>

struct StatsCSVRow {
    std::string name;
    int mass_number;
    float x;
    float y;
    float z;

    StatsCSVRow(const std::string& name, int number, Eigen::Vector3f value)
        : name(name), mass_number(number), x(value.x()), y(value.y()),
          z(value.z()) {}

    std::string to_string() const {
        return name + "," + std::to_string(mass_number) + "," +
               std::to_string(x) + "," + std::to_string(y) + "," +
               std::to_string(z) + "\n";
    }
};

class Stats : public QObject {
    Q_OBJECT

  public:
    Stats() {
        auto dt = QDateTime::currentDateTime();
        dt.setTimeSpec(Qt::UTC);
        velocity_filename = std::string("velocity-stats-") +
                            QDateTime::currentDateTime()
                                .toString(Qt::ISODate)
                                .toUtf8()
                                .constData() +
                            std::string(".csv");

        force_filename = std::string("force-stats-") +
                         QDateTime::currentDateTime()
                             .toString(Qt::ISODate)
                             .toUtf8()
                             .constData() +
                         std::string(".csv");
    }

  public slots:
    void
    DropReading(std::unordered_map<std::string,
                                   std::unordered_map<int, Eigen::Vector3f>>
                    current_mass_velocities,
                std::unordered_map<std::string,
                                   std::unordered_map<int, Eigen::Vector3f>>
                    current_mass_forces);

  private:
    std::string velocity_filename;
    std::string force_filename;

    void WriteCSVData(const std::vector<StatsCSVRow>& rows,
                      const std::string& filename);

    bool FileExists(const std::string& name);

    std::vector<StatsCSVRow>
    Compress(const std::unordered_map<
             std::string, std::unordered_map<int, Eigen::Vector3f>>& values);
};