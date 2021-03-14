#include <fstream>
#include <sys/stat.h>

#include <lattice/stats.h>

void Stats::DropReading() {
    const auto mass_velocities = supervisor->SampleMassForces();
    const auto mass_forces = supervisor->SampleMassVelocities();

    const auto velocity_csv_rows = Compress(mass_velocities);
    const auto force_csv_rows = Compress(mass_forces);

    WriteCSVData(velocity_csv_rows, velocity_filename);
    WriteCSVData(force_csv_rows, force_filename);
}

void Stats::WriteCSVData(const std::vector<StatsCSVRow>& rows,
                         const std::string& filename) {
    std::fstream file_ptr;

    if (!FileExists(filename)) {
        file_ptr.open(filename, std::fstream::in | std::fstream::out |
                                    std::fstream::trunc);

        // If the file doesn't exist, add the headers to it.
        file_ptr << StatsCSVRow::Headers();
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
    return QDateTime::currentDateTime().toString(Qt::ISODate).toUtf8().constData();
}

std::vector<StatsCSVRow> Stats::Compress(
    const std::unordered_map<
        std::string, std::unordered_map<int, Eigen::Vector3f>>& values) {
    std::vector<StatsCSVRow> output;

    for (const auto& [mss_name, velocities_by_mass] : values) {
        for (const auto& [mass_number, velocity] : velocities_by_mass) {
            output.emplace_back(mss_name, mass_number, velocity);
        }
    }

    return output;
}