#include <fstream>

#include <lattice/stats.h>

void Stats::DropReading() {
    const auto mass_forces = supervisor->SampleMassForces();
    const auto mass_velocities = supervisor->SampleMassVelocities();

    auto force_csv_rows = std::vector<ForceCSVRow>{};
    CompressForceCSVData(mass_forces, force_csv_rows);

    auto velocity_csv_rows = std::vector<VelocityCSVRow>{};
    CompressVelocityCSVData(mass_velocities, velocity_csv_rows);

    WriteCSVData(velocity_csv_rows, velocity_filename);
    WriteCSVData(force_csv_rows, force_filename);
}

template <typename T>
void Stats::WriteCSVData(const std::vector<T>& rows,
                         const std::string& filename) {
    std::fstream file_ptr;

    if (!FileExists(filename)) {
        file_ptr.open(filename, std::fstream::in | std::fstream::out |
                                    std::fstream::trunc);

        // If the file doesn't exist, add the headers to it.
        file_ptr << rows[0].Headers();
    } else {
        // Otherwise, open in append mode.
        file_ptr.open(filename,
                      std::fstream::in | std::fstream::out | std::fstream::app);
    }

    for (const auto& row : rows) {
        file_ptr << row.to_string();
    }

    file_ptr.close();
}

bool Stats::FileExists(const std::string& filename) {
    return std::filesystem::exists(filename);
}

std::string Stats::GetCurrentTimestamp() {
    return QDateTime::currentDateTime()
        .toString(Qt::ISODate)
        .toUtf8()
        .constData();
}

void Stats::CompressForceCSVData(
    const std::unordered_map<std::string,
                             std::unordered_map<int, Eigen::Vector3f>>& values,
    std::vector<ForceCSVRow>& output) {

    const int density = supervisor->density;

    for (const auto& [mss_name, forces_by_mass] : values) {
        for (const auto& [mass_number, force] : forces_by_mass) {
            output.emplace_back(mss_name, mass_number, density, force);
        }
    }
}

void Stats::CompressVelocityCSVData(
    const std::unordered_map<std::string,
                             std::unordered_map<int, Eigen::Vector3f>>& values,
    std::vector<VelocityCSVRow>& output) {
    for (const auto& [mss_name, velocities_by_mass] : values) {
        for (const auto& [mass_number, velocity] : velocities_by_mass) {
            output.emplace_back(mss_name, mass_number, velocity);
        }
    }
}
