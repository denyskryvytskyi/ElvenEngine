#pragma once

namespace elv {
class Cubemap {
public:
    virtual ~Cubemap() = default;

    virtual void Bind() = 0;
};
} // namespace elv
