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

struct VelocityCSVRow {
    std::string name;
    int mass_number;
    float x;
    float y;
    float z;

    VelocityCSVRow(std::string name, int number, Eigen::Vector3f value)
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

struct ForceCSVRow {
    std::string name;
    int mass_number;
    int density;
    float k;
    float x;
    float F;

    ForceCSVRow(std::string name, int number, int density,
                Eigen::Vector3f value)
        : name(std::move(name)), mass_number(number), density(density),
          k(value.x()), x(value.y()), F(value.z()) {}

    [[nodiscard]] static std::string Headers() {
        return "name,mass_number,density,k,x,F\n";
    }

    [[nodiscard]] std::string to_string() const {
        return name + "," + std::to_string(mass_number) + "," +
               std::to_string(density) + "," + std::to_string(k) + "," +
               std::to_string(x) + "," + std::to_string(F) + "\n";
    }
};

class Stats : public QObject {
    Q_OBJECT

  public:
    std::shared_ptr<Supervisor> supervisor;

    explicit Stats(std::shared_ptr<Supervisor> supervisor)
        : supervisor(std::move(supervisor)) {
        auto dt = QDateTime::currentDateTime();
        dt.setTimeSpec(Qt::UTC);

        if (!std::filesystem::exists("../../stats")) {
            std::filesystem::create_directory("../../stats");
        }

        velocity_filename = std::string("../../stats/velocity-stats-") +
                            GetCurrentTimestamp() + std::string(".csv");

        force_filename = std::string("../../stats/force-stats-") +
                         GetCurrentTimestamp() + std::string(".csv");
    }

  public slots:
    void DropReading();

  private:
    std::string velocity_filename;
    std::string force_filename;

    void CompressForceCSVData(
        const std::unordered_map<
            std::string, std::unordered_map<int, Eigen::Vector3f>>& values,
        std::vector<ForceCSVRow>& output);

    static void CompressVelocityCSVData(
        const std::unordered_map<
            std::string, std::unordered_map<int, Eigen::Vector3f>>& values,
        std::vector<VelocityCSVRow>& output);

    template <typename T>
    static void WriteCSVData(const std::vector<T>& rows,
                             const std::string& filename);

    static bool FileExists(const std::string& name);

    static std::string GetCurrentTimestamp();
};