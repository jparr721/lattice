#pragma once

#include <lattice/supervisor.h>

#include <filesystem>
#include <string>
#include <unordered_map>
#include <utility>
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

    StatsCSVRow(std::string name, int number, Eigen::Vector3f value)
        : name(std::move(name)), mass_number(number), x(value.x()),
          y(value.y()), z(value.z()) {}

    [[nodiscard]] static std::string Headers() {
        return "name,mass_number,x,y,z\n";
    }

    [[nodiscard]] std::string to_string() const {
        return name + "," + std::to_string(mass_number) + "," +
               std::to_string(x) + "," + std::to_string(y) + "," +
               std::to_string(z) + "\n";
    }
};

class Stats : public QObject {
    Q_OBJECT

  public:
    explicit Stats(std::shared_ptr<Supervisor>  supervisor) : supervisor(std::move(supervisor)) {
        auto dt = QDateTime::currentDateTime();
        dt.setTimeSpec(Qt::UTC);

        if (!std::filesystem::exists("../../stats")) {
            std::filesystem::create_directory("../../stats");
        }

        velocity_filename = std::string("../../stats/velocity-stats-") + GetCurrentDate() +
                            std::string(".csv");

        force_filename = std::string("../../stats/force-stats-") + GetCurrentDate() +
                         std::string(".csv");
    }

  public slots:
    void DropReading();

  private:
    std::string velocity_filename;
    std::string force_filename;

    std::shared_ptr<Supervisor> supervisor;

    static void WriteCSVData(const std::vector<StatsCSVRow>& rows,
                             const std::string& filename);

    static bool FileExists(const std::string& name);

    static std::string GetCurrentDate();

    static std::vector<StatsCSVRow>
    Compress(const std::unordered_map<
             std::string, std::unordered_map<int, Eigen::Vector3f>>& values);
};