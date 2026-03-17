#pragma once

#include <vector>
#include <string>
#include <memory>

#include "vec2.h"
#include "AnimationWindow.h"

namespace Hex {
namespace UI {

class UINode : public std::enable_shared_from_this<UINode> {
protected:
    vec2<int> position = {0, 0};
    std::string name;

    std::shared_ptr<UINode> parent = nullptr; // nullptr means no parent.
    std::vector<std::shared_ptr<UINode>> children;

    std::shared_ptr<TDT4102::AnimationWindow> windowPtr;

public:
    void addChild(const std::shared_ptr<UINode> &child);
    std::shared_ptr<UINode> removeChild(const std::shared_ptr<UINode> &child);
    std::shared_ptr<UINode> removeChild(int index);

    std::vector<std::shared_ptr<UINode>> getChildren() const { return children; }
    std::shared_ptr<UINode> getParent() const { return parent; }
    vec2<int> getPosition() const;
    std::string getName() const { return name; }

    // Also updates children.
    virtual void update();
    virtual void draw() {};

    // Constructor
    UINode() = default;
    UINode(std::string name): name{name} {}

    virtual ~UINode() {}
};

} // namespace UI
} // namespace Hex