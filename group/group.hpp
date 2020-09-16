#pragma once

#include <string>
#include <vector>

class Group {
public:
    Group(const std::vector<std::string> &variables, const std::vector<std::vector<int>> &relations)
        : index_to_name_(variables), relations_(relations) {
    }

    size_t VariableCount() const {
        return index_to_name_.size();
    }

    std::string GetVariableNameByIndex(int index) const {
        if (index > 0) {
            return index_to_name_[index - 1];
        }
        return index_to_name_[-index - 1];
    }

    size_t RelationCount() const {
        return relations_.size();
    }

    std::vector<int> GetRelationByIndex(size_t index) const {
        return relations_[index];
    }

private:
    std::vector<std::string> index_to_name_;
    std::vector<std::vector<int>> relations_;
};