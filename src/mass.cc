#include <lattice/mass.h>
#include <lattice/obj_loader.h>
#include <lattice/spring.h>

void Mass::Initialize() {
    if (!LoadObjFile("../../sim_specs/cube.obj", original_positions)) {
        exit(EXIT_FAILURE);
    }

    vertices = std::vector<Eigen::Vector3f>(original_positions.size(),
                                            Eigen::Vector3f::Zero());

    rest_position = position;
    ComputeVertexPoints();

    for (auto i = 0u; i < vertices.size(); ++i) {
        colors.push_back(kColor);
    }
}

void Mass::Update(float dt) {
    if (fixed) {
        force = Eigen::Vector3f::Zero();
        velocity = Eigen::Vector3f::Zero();
        return;
    }

    CalculateMassForces(dt);
}

void Mass::ComputeVertexPoints() {

    // Build our vertices and "scoot" them to their starting positions.
    for (auto i = 0u; i < original_positions.size(); ++i) {
        const auto vertex = original_positions[i];
        vertices[i] = Eigen::Vector3f(vertex.x() + position.x(),
                                      vertex.y() + position.y(),
                                      vertex.z() + position.z());
    }
}

void Mass::Translate(const Eigen::Vector3f& translation_vector) {
    for (auto& vertex : vertices) {
        Eigen::Matrix4f translation_matrix;
        translation_matrix.setIdentity();
        translation_matrix.block<3, 1>(0, 3) = translation_vector;

        Eigen::Vector4f transform =
            translation_matrix *
            Eigen::Vector4f(vertex.x(), vertex.y(), vertex.z(), 1.f);

        vertex = Eigen::Vector3f(transform.x(), transform.y(), transform.z());
    }
}

void Mass::CalculateMassForces(float dt) {
    const auto old_pos = position;

    position += velocity * dt;

    velocity += (force / weight) * dt;

    auto d_position = position - old_pos;
    Translate(Eigen::Vector3f(d_position.x(), d_position.y(), d_position.z()));
}
