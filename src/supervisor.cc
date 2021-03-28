#include <lattice/supervisor.h>

Supervisor::Supervisor(const MSSConfig& config) : density(config.Density()) {
    Initialize(config);
}

Supervisor::Supervisor(const MSSConfig& config,
                       const SupervisorParameters& params)
    : density(config.Density()) {
    Initialize(config);
    SetMassWeight(params.mass);
    SetSpringConstant(params.k);
    SetSpringDampingConstant(params.damping);
    SetSpringRestLength(params.rest_length);
    SetTimeStep(params.time_step);
}

void Supervisor::RecompileVertexBuffer() {
    shapes.clear();
    for (const auto& simulation : simulations) {
        for (const auto& shape : simulation->Shapes()) {
            shapes.push_back(shape);
        }
    }
}

void Supervisor::Update() {
    for (const auto& simulation : simulations) {
        simulation->Update(time_step_size);
    }

    RecompileVertexBuffer();
}

void Supervisor::Reset() {
    for (const auto& simulation : simulations) {
        simulation->Reset();
    }

    RecompileVertexBuffer();
}

void Supervisor::SetMassWeight(float value) {
    for (const auto& simulation : simulations) {
        simulation->SetMassWeight(value);
    }
}

void Supervisor::SetSpringConstant(float value) {
    for (const auto& simulation : simulations) {
        simulation->SetSpringConstant(value);
    }
}

void Supervisor::SetSpringDampingConstant(float value) {
    for (const auto& simulation : simulations) {
        simulation->SetSpringDampingConstant(value);
    }
}

void Supervisor::SetSpringRestLength(float value) {
    for (const auto& simulation : simulations) {
        simulation->SetSpringRestLength(value);
    }
}

std::unordered_map<std::string, std::unordered_map<int, Eigen::Vector3f>>
Supervisor::SampleMassVelocities() {
    for (const auto& simulation : simulations) {
        const auto velocities = simulation->GetMassVelocities();
        current_mass_velocities[simulation->name] = velocities;
    }

    return current_mass_velocities;
}

std::unordered_map<std::string, std::unordered_map<int, Eigen::Vector3f>>
Supervisor::SampleMassForces() {
    for (const auto& simulation : simulations) {
        const auto forces = simulation->GetMassForces();
        current_mass_forces[simulation->name] = forces;
    }

    return current_mass_forces;
}
void Supervisor::Initialize(const MSSConfig& config) {
    simulations.push_back(std::make_shared<MassSpringSystem>(config));

    for (const auto& simulation : simulations) {
        for (const auto& color : simulation->Colors()) {
            colors.push_back(color);
        }
    }

    shapes =
        std::vector<Eigen::Vector3f>(colors.size(), Eigen::Vector3f::Zero());
    RecompileVertexBuffer();
}

float Supervisor::GetMassWeight() const {
    return simulations[0]->GetMassWeight();
}

float Supervisor::GetSpringConstant() const {
    return simulations[0]->GetSpringConstant();
}

float Supervisor::GetSpringRestLength() const {
    return simulations[0]->GetSpringRestLength();
}

float Supervisor::GetSpringDampingConstant() const {
    return simulations[0]->GetSpringDampingConstant();
}

float Supervisor::GetTimeStep() const { return time_step_size; }

SupervisorParameters Supervisor::CurrentParameters() const {
    return SupervisorParameters{
        .mass = GetMassWeight(),
        .k = GetSpringConstant(),
        .damping = GetSpringDampingConstant(),
        .rest_length = GetSpringRestLength(),
        .time_step = GetTimeStep(),
    };
}

float Supervisor::ComputeNextK() {
    return kSineIntervalCoefficient * std::sin(2 * kSineInterval[++sine_step]) +
           kSineIntervalCoefficient;
}
void Supervisor::ReInitialize(const MSSConfig& config,
                              const SupervisorParameters& params) {

    simulations.clear();
    shapes.clear();
    colors.clear();

    Initialize(config);
    SetMassWeight(params.mass);
    SetSpringConstant(params.k);
    SetSpringDampingConstant(params.damping);
    SetSpringRestLength(params.rest_length);
    SetTimeStep(params.time_step);
}
