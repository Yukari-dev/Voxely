#pragma once
#include "Mesh.hpp"
#include "Primitives.hpp"
#include <memory>

enum class MeshType { Cube };

class MeshFactory {
public:
    MeshFactory(VkDevice device, VkPhysicalDevice physicalDevice)
        : m_Device(device), m_PhysicalDevice(physicalDevice) {}

    std::unique_ptr<Mesh> Create(MeshType type) {
        switch(type) {
            case MeshType::Cube:
                return std::make_unique<Mesh>(
                    m_Device, m_PhysicalDevice,
                    Primitives::CubeVertices(),
                    Primitives::CubeIndices()
                );
        }
        throw std::runtime_error("Unknown mesh type");
    }

private:
    VkDevice         m_Device;
    VkPhysicalDevice m_PhysicalDevice;
};