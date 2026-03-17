#include "UINode.h"


void Hex::UI::UINode::addChild(const std::shared_ptr<Hex::UI::UINode> &child) {
    children.emplace_back(child);

    // Suggestion from claude, return shared_ptr created from a weak_ptr stored in the parent instance.
    child->parent = shared_from_this();
}

// returns removed child, nullptr if none were removed.
std::shared_ptr<Hex::UI::UINode> Hex::UI::UINode::removeChild(const std::shared_ptr<Hex::UI::UINode> &child) {
    for (int i = 0; i < children.size(); ++i) {
        if (child == children.at(i)) {
            // Swap child with last slot
            children.at(i) = children.back();
            children.pop_back();
            child->parent = nullptr;
            return child;
        }
    }

    return nullptr;
}

// returns removed child, nullptr if none were removed.
std::shared_ptr<Hex::UI::UINode> Hex::UI::UINode::removeChild(int index) {
    if (children.size() > index) {
        std::shared_ptr<Hex::UI::UINode> removedChild = children.at(index);

        children.at(index) = children.back();
        children.pop_back();

        return removedChild;
    }

    return nullptr;
}

Hex::vec2<int> Hex::UI::UINode::getPosition() const {
    if (parent == nullptr) {
        return position;
    }
    return position + parent->getPosition(); 
}

// Also updates children.
void Hex::UI::UINode::update() {
    for (std::shared_ptr<Hex::UI::UINode> &child : children) {
        child->update();
    }
}
