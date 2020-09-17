#pragma once

#include <algorithm>
#include <graph/labeled_graph.hpp>
#include <group/group.hpp>
#include <random>
#include <string>

struct VanKampenDiagram {
  VanKampenDiagram(std::string word, const LabeledGraph &diagram)
      : word(std::move(word)), diagram(diagram) {}
  std::string word;
  LabeledGraph diagram;
};

class RandomVanKampenGenerator {
public:
  RandomVanKampenGenerator(const Group &group) : group_(group) {}

  VanKampenDiagram GenerateRandomDiagram(size_t steps = 5) {
    std::uniform_int_distribution<size_t> relation_distribution(
        0, group_.RelationCount() - 1);

    auto word = group_.GetRelationByIndex(relation_distribution(generator_));

    LabeledGraph diagram(word.size());
    std::vector<size_t> word_graph_indices;
    for (int index = 0; index < word.size(); ++index) {
      std::string label = group_.GetVariableNameByIndex(word[index]);

      if (word[index] > 0) {
        diagram.AddEdge(index, (index + 1) % word.size(), label);
      } else {
        diagram.AddEdge((index + 1) % word.size(), index, label);
      }
      word_graph_indices.push_back(index);
    }

    for (size_t step = 0; step < steps; ++step) {
      auto next_relation =
          group_.GetRelationByIndex(relation_distribution(generator_));

      // to avoid multiple edges
      while (next_relation.size() < 3) {
        next_relation.insert(next_relation.end(), next_relation.begin(),
                             next_relation.end());
      }

      std::uniform_int_distribution<size_t> element_distribution(
          0, word.size() - 1);
      size_t random_word_index = element_distribution(generator_);
      size_t random_relation_index =
          element_distribution(generator_) % (next_relation.size() - 2) + 1;

      auto it = std::find(next_relation.begin() + random_relation_index,
                          next_relation.end() - 1, word[random_word_index]);
      if (it == next_relation.end()) {
        next_relation = ReverseRelation(next_relation);
        it = std::find(next_relation.begin() + random_relation_index,
                       next_relation.end(), word[random_word_index]);
        if (it == next_relation.end()) {
          --step;
          continue;
        }
      }

      size_t equality_len = 1;
      for (; it + equality_len < next_relation.end() &&
             random_word_index + equality_len < word.size();
           ++equality_len) {
        if (*(it + equality_len) !=
            word[random_word_index + equality_len - 1]) {
          break;
        }
      }

      std::vector<int> new_path;
      std::copy(it + equality_len, next_relation.end(),
                std::back_inserter(new_path));
      std::copy(next_relation.begin(), it, std::back_inserter(new_path));
      new_path = ReverseRelation(new_path);

      word.erase(word.begin() + random_word_index,
                 word.begin() + random_word_index + equality_len);
      word.insert(word.begin() + random_word_index, new_path.begin(),
                  new_path.end());

      std::vector<size_t> new_indices;
      for (int index = diagram.Size();
           index < diagram.Size() + new_path.size() - 1; ++index) {
        new_indices.push_back(index);
      }

      word_graph_indices.erase(
          word_graph_indices.begin() + random_word_index + 1,
          word_graph_indices.begin() + random_word_index + equality_len);
      word_graph_indices.insert(word_graph_indices.begin() + random_word_index +
                                    1,
                                new_indices.begin(), new_indices.end());

      diagram.Resize(diagram.Size() + new_path.size() - 1);
      for (int i = 0; i < new_path.size(); ++i) {
        std::string label =
            group_.GetVariableNameByIndex(word[random_word_index + i]);
        if (word[random_word_index + i] > 0) {
          diagram.AddEdge(word_graph_indices[random_word_index + i],
                          word_graph_indices[random_word_index + i + 1], label);
        } else {
          diagram.AddEdge(word_graph_indices[random_word_index + i + 1],
                          word_graph_indices[random_word_index + i], label);
        }
      }
    }

    std::string word_str;
    for (const auto &index : word) {
      word_str.append(group_.GetVariableNameByIndex(index));
      if (index < 0) {
        word_str.push_back('-');
      }
      word_str.push_back(' ');
    }

    return VanKampenDiagram(word_str, diagram);
  }

private:
  std::vector<int> ReverseRelation(const std::vector<int> &relation) {
    std::vector<int> reversed(relation.size());
    for (int i = 0; i < relation.size(); ++i) {
      reversed[i] = -relation[relation.size() - i - 1];
    }
    return reversed;
  }

  const Group &group_;
  std::mt19937 generator_{static_cast<size_t>(time(0))};
};