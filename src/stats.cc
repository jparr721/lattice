#include <fstream>

#include <lattice/stats.h>

void Stats::DropReading() {
    const auto mass_forces = supervisor->SampleMassForces();
    const auto mass_velocities = supervisor->SampleMassVelocities();

    auto velocity_csv_rows = std::vector<VelocityCSVRow>{};
    auto force_csv_rows = std::vector<ForceCSVRow>{};
    Compress(mass_velocities, velocity_csv_rows);
    Compress(mass_forces, force_csv_rows);

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

std::string Stats::GetCurrentDate() {
    return QDateTime::currentDateTime()
        .toString(Qt::ISODate)
        .toUtf8()
        .constData();
}

template <typename T>
void Stats::Compress(
    const std::unordered_map<std::string,
                             std::unordered_map<int, Eigen::Vector3f>>& values,
    std::vector<T>& output) {
    for (const auto& [mss_name, velocities_by_mass] : values) {
        for (const auto& [mass_number, force] : velocities_by_mass) {
            output.emplace_back(mss_name, mass_number, force);
        }
    }
}