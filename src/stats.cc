#include <fstream>
#include <sys/stat.h>

#include <lattice/stats.h>

void Stats::DropReading(
    std::unordered_map<std::string, std::unordered_map<int, Eigen::Vector3f>>
        current_mass_velocities,
    std::unordered_map<std::string, std::unordered_map<int, Eigen::Vector3f>>
        current_mass_forces) {
    const auto velocity_csv_rows = Compress(current_mass_velocities);
    const auto force_csv_rows = Compress(current_mass_forces);

    WriteCSVData(velocity_csv_rows, velocity_filename);
    WriteCSVData(force_csv_rows, force_filename);
}

void Stats::WriteCSVData(const std::vector<StatsCSVRow>& rows,
                         const std::string& filename) {
    std::fstream file_ptr;

    if (!FileExists(filename)) {
        file_ptr.open(filename, std::fstream::in | std::fstream::out |
                                    std::fstream::trunc);
    } else {
        file_ptr.open(filename,
                      std::fstream::in | std::fstream::out | std::fstream::app);
    }

    for (const auto row : rows) {
        file_ptr << row.to_string();
    }

    file_ptr.close();
}

bool Stats::FileExists(const std::string& filename) {
    struct stat buf;
    return (stat(filename.c_str(), &buf) == 0);
}

std::vector<StatsCSVRow> Stats::Compress(
    const std::unordered_map<
        std::string, std::unordered_map<int, Eigen::Vector3f>>& values) {
    std::vector<StatsCSVRow> output;

    for (const auto& [mss_name, velocities_by_mass] : values) {
        for (const auto& [mass_number, velocity] : velocities_by_mass) {
            output.push_back(StatsCSVRow(mss_name, mass_number, velocity));
        }
    }

    return output;
}